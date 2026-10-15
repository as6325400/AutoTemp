#!/usr/bin/env python3
"""Run codebook correctness checks and reproducible baseline comparisons.

  python3 tests/run.py --list
  python3 tests/run.py --suite km --bench --json /tmp/codebook-results.json
  python3 tests/run.py --bench-only --baseline REVISION

Suite manifests live in tests/suites. No binaries or baseline copies are kept
in the repository. Python 3, Git, and a C++17 compiler are required.
"""

import argparse
from datetime import datetime, timezone
import hashlib
import json
import math
import os
from pathlib import Path
import platform
import shlex
import signal
import subprocess
import sys
import tempfile
import time


ROOT = Path(__file__).resolve().parents[1]
SUITES = ROOT / "tests/suites"
COMMON_FLAGS = ["-std=c++17", "-Wall", "-Wextra", "-Wpedantic"]
RELEASE_FLAGS = [*COMMON_FLAGS, "-O2"]
SANITIZER_FLAGS = [*COMMON_FLAGS, "-O1", "-g", "-fsanitize=address,undefined",
                   "-fno-omit-frame-pointer", "-fno-sanitize-recover=all"]
TIMEOUTS = {"compile": 180, "check": 120, "assertion": 10, "benchmark": 600}


def capture(command, timeout=30):
    return subprocess.run(command, cwd=ROOT, check=True, capture_output=True,
                          text=True, timeout=timeout).stdout.strip()


def cpu_model():
    cpuinfo = Path("/proc/cpuinfo")
    if cpuinfo.exists():
        for line in cpuinfo.read_text().splitlines():
            if line.startswith("model name"):
                return line.split(":", 1)[1].strip()
    return platform.processor() or "unknown"


def metadata(compiler):
    return {
        "started_at": datetime.now(timezone.utc).isoformat(),
        "compiler_command": compiler,
        "compiler": capture([*compiler, "--version"]).splitlines()[0],
        "cpu": cpu_model(),
        "platform": platform.platform(),
        "python": platform.python_version(),
        "release_flags": RELEASE_FLAGS,
        "sanitizer_flags": SANITIZER_FLAGS,
        "timeouts_seconds": TIMEOUTS,
        "current_commit": capture(["git", "rev-parse", "HEAD"]),
        "dirty": bool(capture(["git", "status", "--porcelain"])),
        "harness_sha256": {
            path: hashlib.sha256((ROOT / path).read_bytes()).hexdigest()
            for path in ("tests/run.py", "tests/support/test.hpp")
        },
    }


def repository_file(value):
    path = (ROOT / value).resolve()
    if ROOT not in path.parents or not path.is_file():
        raise ValueError(f"Expected a file inside the repository: {value}")
    return path


def load_suites():
    suites = {}
    for path in sorted(SUITES.glob("*.json")):
        spec = json.loads(path.read_text())
        name = spec["name"]
        if not isinstance(name, str) or not name or name in suites:
            raise ValueError(f"Invalid or duplicate suite name in {path}")
        repository_file(spec["test"])
        repository_file(spec["candidate"])
        if not spec["baseline"]["revision"] or not spec["baseline"]["path"]:
            raise ValueError(f"Missing baseline in {path}")
        variants = spec["variants"]
        names = [variant["name"] for variant in variants]
        if not names or len(names) != len(set(names)):
            raise ValueError(f"Missing or duplicate variants in {path}")
        for variant in variants:
            if not isinstance(variant["name"], str) or not variant["name"]:
                raise ValueError(f"Invalid variant name in {path}")
            if not isinstance(variant["defines"], list) or not all(
                    isinstance(value, str) and value for value in variant["defines"]):
                raise ValueError(f"Invalid variant defines in {path}")
        assertions = spec.get("assertions", [])
        if not isinstance(assertions, list) or not all(
                isinstance(value, str) and value for value in assertions):
            raise ValueError(f"Invalid assertion cases in {path}")
        spec["assertions"] = assertions
        spec["_manifest_path"] = str(path.relative_to(ROOT))
        suites[name] = spec
    if not suites:
        raise ValueError(f"No suite manifests found in {SUITES}")
    return suites


def decoded(output):
    # TimeoutExpired may carry bytes even when subprocess uses text=True.
    if isinstance(output, bytes):
        return output.decode(errors="replace")
    return output or ""


def run_step(command, kind, expected=0, env=None, quiet=False):
    started = time.monotonic()
    result = {"command": command, "expected_returncode": expected}
    try:
        process = subprocess.run(command, cwd=ROOT, capture_output=True, text=True,
                                 timeout=TIMEOUTS[kind], env=env)
        result.update(returncode=process.returncode, stdout=process.stdout,
                      stderr=process.stderr,
                      status="passed" if process.returncode == expected else "failed")
    except subprocess.TimeoutExpired as error:
        result.update(status="failed", returncode=None, error="timeout",
                      stdout=decoded(error.stdout), stderr=decoded(error.stderr))
    except OSError as error:
        result.update(status="failed", returncode=None, error=str(error),
                      stdout="", stderr="")
    result["elapsed_seconds"] = round(time.monotonic() - started, 6)
    if not quiet or result["status"] == "failed":
        for stream in ("stdout", "stderr"):
            if result[stream]:
                print(result[stream], end="" if result[stream].endswith("\n") else "\n",
                      file=sys.stderr if stream == "stderr" else sys.stdout, flush=True)
    if result["status"] == "failed":
        reason = result.get("error", f"exit {result['returncode']}, expected {expected}")
        print(f"FAIL {kind}: {reason}\n  {shlex.join(command)}", file=sys.stderr, flush=True)
    return result


def benchmark_records(step):
    records = []
    for line in step["stdout"].splitlines():
        if not line.lstrip().startswith("{"):
            continue
        record = json.loads(line)
        if record.get("type") != "benchmark":
            continue
        if not isinstance(record.get("name"), str):
            raise ValueError("Benchmark name must be a string")
        for key in ("baseline_ms", "candidate_ms", "speedup", "rounds", "batches"):
            value = record.get(key)
            if not isinstance(value, (int, float)) or not math.isfinite(value) or value <= 0:
                raise ValueError(f"Invalid benchmark field: {key}")
        if not isinstance(record["rounds"], int) or not isinstance(record["batches"], int):
            raise ValueError("Benchmark rounds and batches must be integers")
        for key in ("baseline_samples_ms", "candidate_samples_ms"):
            samples = record.get(key)
            if not isinstance(samples, list) or len(samples) != record["rounds"] or not all(
                    isinstance(value, (int, float)) and math.isfinite(value) and value > 0
                    for value in samples):
                raise ValueError(f"Invalid benchmark samples: {key}")
        records.append(record)
    if not records:
        raise ValueError("The benchmark produced no JSONL benchmark records")
    return records


def run_variant(spec, variant, baseline_header, directory, args, compiler):
    result = {"name": variant["name"], "defines": variant["defines"], "status": "passed"}
    # Use numeric temporary names; suite/variant names never become filesystem paths.
    binary = directory / "release"
    includes = [f'-DBASELINE_HEADER="{baseline_header}"',
                f'-DCANDIDATE_HEADER="{repository_file(spec["candidate"])}"']
    defines = [f"-D{value}" for value in variant["defines"]]
    source = str(repository_file(spec["test"]))
    command = [*compiler, *RELEASE_FLAGS, *includes, *defines, source, "-o", str(binary)]
    result["compile"] = run_step(command, "compile")
    if result["compile"]["status"] == "failed":
        result["status"] = "failed"
        return result
    if not args.bench_only:
        result["check"] = run_step([str(binary), "--check"], "check")
        result["assertions"] = {}
        for case in spec["assertions"]:
            result["assertions"][case] = run_step(
                [str(binary), f"--bad-{case}"], "assertion", expected=-signal.SIGABRT, quiet=True)
        if result["assertions"] and all(
                item["status"] == "passed" for item in result["assertions"].values()):
            print(f"PASS {spec['name']}/{variant['name']} assertions={len(spec['assertions'])}", flush=True)
    if args.bench or args.bench_only:
        result["benchmark"] = run_step([str(binary), "--bench"], "benchmark")
        try:
            result["benchmark"]["records"] = benchmark_records(result["benchmark"])
        except (ValueError, TypeError) as error:
            result["benchmark"].update(status="failed", parse_error=str(error))
            print(f"FAIL benchmark output: {error}", file=sys.stderr, flush=True)
    if not args.bench_only and not args.no_sanitize:
        sanitized = directory / "sanitized"
        result["sanitize"] = {"status": "passed"}
        sanitizer = result["sanitize"]
        sanitizer["compile"] = run_step(
            [*compiler, *SANITIZER_FLAGS, *includes, *defines, source, "-o", str(sanitized)], "compile")
        if sanitizer["compile"]["status"] == "passed":
            env = {**os.environ, "ASAN_OPTIONS": "detect_leaks=1:abort_on_error=1",
                   "UBSAN_OPTIONS": "halt_on_error=1:print_stacktrace=1"}
            sanitizer["check"] = run_step([str(sanitized), "--check"], "check", env=env)
        if any(value["status"] == "failed" for value in sanitizer.values() if isinstance(value, dict)):
            sanitizer["status"] = "failed"
        else:
            print(f"PASS {spec['name']}/{variant['name']} ASan/UBSan", flush=True)
    steps = [value for value in result.values() if isinstance(value, dict) and "status" in value]
    steps.extend(result.get("assertions", {}).values())
    if any(step["status"] == "failed" for step in steps):
        result["status"] = "failed"
    print(f"{result['status'].upper()} {spec['name']}/{variant['name']}", flush=True)
    return result


def run_suite(spec, args, compiler, result):
    revision = args.baseline or spec["baseline"]["revision"]
    commit = capture(["git", "rev-parse", "--verify", f"{revision}^{{commit}}"])
    baseline = subprocess.run(["git", "show", f"{commit}:{spec['baseline']['path']}"],
                              cwd=ROOT, capture_output=True, check=True, timeout=30).stdout
    candidate = repository_file(spec["candidate"])
    result.update(
        manifest={"path": spec["_manifest_path"],
                  "sha256": hashlib.sha256(repository_file(spec["_manifest_path"]).read_bytes()).hexdigest()},
        baseline={"requested_revision": revision, "revision": commit,
                  "path": spec["baseline"]["path"], "sha256": hashlib.sha256(baseline).hexdigest()},
        candidate={"path": spec["candidate"], "sha256": hashlib.sha256(candidate.read_bytes()).hexdigest()},
        test={"path": spec["test"], "sha256": hashlib.sha256(repository_file(spec["test"]).read_bytes()).hexdigest()},
        variants=[],
    )
    print(f"\nSuite: {spec['name']} | baseline: {commit} | candidate: {spec['candidate']}", flush=True)
    with tempfile.TemporaryDirectory(prefix="codebook-tests-") as temporary:
        directory = Path(temporary)
        header = directory / "baseline.hpp"
        header.write_bytes(baseline)
        for index, variant in enumerate(spec["variants"]):
            variant_dir = directory / str(index)
            variant_dir.mkdir()
            result["variants"].append(run_variant(spec, variant, header, variant_dir, args, compiler))
    result["status"] = "passed" if all(v["status"] == "passed" for v in result["variants"]) else "failed"


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--list", action="store_true", help="list available suites without compiling")
    parser.add_argument("--suite", action="append", nargs="+", metavar="NAME", help="select one or more suites; repeatable")
    parser.add_argument("--baseline", metavar="REF", help="override every selected suite's pinned baseline revision")
    parser.add_argument("--compiler", default=os.environ.get("CXX", "g++"))
    parser.add_argument("--bench", action="store_true", help="also compare release performance")
    parser.add_argument("--bench-only", action="store_true", help="only run release benchmarks")
    parser.add_argument("--no-sanitize", action="store_true", help="skip ASan/UBSan checks")
    parser.add_argument("--json", type=Path, metavar="PATH", help="write machine-readable results, including failures")
    args = parser.parse_args()
    report = {"schema_version": 1, "status": "failed", "suites": []}
    exit_code = 1
    try:
        # Invalid-input cases intentionally abort; prevent core files in the checkout.
        import resource
        resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
        suites = load_suites()
        selected = list(dict.fromkeys(name for group in args.suite for name in group)) if args.suite else list(suites)
        unknown = set(selected) - suites.keys()
        if unknown:
            raise ValueError(f"Unknown suites: {', '.join(sorted(unknown))}")
        if args.list:
            for name in selected:
                spec = suites[name]
                print(f"{name}: {spec['candidate']} ({', '.join(v['name'] for v in spec['variants'])})")
            report["available_suites"] = selected
            report["status"] = "passed"
            return 0
        compiler = shlex.split(args.compiler)
        if not compiler:
            raise ValueError("The compiler command cannot be empty")
        report["metadata"] = metadata(compiler)
        report["options"] = {"suites": selected, "bench": args.bench or args.bench_only,
                             "bench_only": args.bench_only, "sanitize": not (args.no_sanitize or args.bench_only),
                             "baseline_override": args.baseline}
        print(f"Compiler: {report['metadata']['compiler']}\nCPU: {report['metadata']['cpu']}\n"
              f"Release flags: {' '.join(RELEASE_FLAGS)}", flush=True)
        for name in selected:
            result = {"name": name, "status": "failed"}
            report["suites"].append(result)
            try:
                run_suite(suites[name], args, compiler, result)
            except (OSError, ValueError, subprocess.SubprocessError) as error:
                result["error"] = str(error)
                print(f"FAIL {name}: {error}", file=sys.stderr, flush=True)
        if all(suite["status"] == "passed" for suite in report["suites"]):
            report["status"] = "passed"
            exit_code = 0
    except (OSError, ValueError, KeyError, TypeError, subprocess.SubprocessError) as error:
        report["error"] = str(error)
        print(f"FAIL runner: {error}", file=sys.stderr, flush=True)
    except KeyboardInterrupt:
        report["error"] = "interrupted"
        exit_code = 130
    finally:
        report["finished_at"] = datetime.now(timezone.utc).isoformat()
        if args.json:
            args.json.parent.mkdir(parents=True, exist_ok=True)
            args.json.write_text(json.dumps(report, indent=2, ensure_ascii=False) + "\n")
            print(f"Results: {args.json}", flush=True)
    return exit_code


if __name__ == "__main__":
    sys.exit(main())
