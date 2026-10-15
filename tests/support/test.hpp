#pragma once
#include <bits/stdc++.h>

namespace cbtest {
inline void require(bool ok, const std::string& message) {
  if (!ok) throw std::runtime_error(message);
}

// 計時只包待測操作；測資生成、對答案放在外面。
inline volatile long long sink = 0;
template<class F> double elapsed_ms(F&& f, int batches) {
  auto start = std::chrono::steady_clock::now();
  for (int i = 0; i < batches; ++i) sink = sink ^ (long long)f();
  return std::chrono::duration<double, std::milli>(
      std::chrono::steady_clock::now() - start).count() / batches;
}

inline void json_string(const std::string& s) {
  std::cout << '"';
  for (unsigned char c : s) {
    if (c == '"' || c == '\\') std::cout << '\\' << c;
    else if (c < 32) {
      const char* hex = "0123456789abcdef";
      std::cout << "\\u00" << hex[c >> 4] << hex[c & 15];
    } else std::cout << c;
  }
  std::cout << '"';
}

inline void samples(const std::vector<double>& values) {
  std::cout << '[';
  for (size_t i = 0; i < values.size(); ++i) {
    if (i) std::cout << ',';
    std::cout << values[i];
  }
  std::cout << ']';
}

// 暖機、交替順序，取每輪平均單次時間的中位數（ms）。
// 保留每輪數據；速度受機器負載影響，不當成 CI 通過門檻。
template<class A, class B>
void compare_speed(const std::string& name, A&& old_fn,
                   B&& new_fn, int rounds = 7, int batches = 3) {
  require(rounds > 0 && (rounds & 1) && batches > 0,
          "benchmark needs positive odd rounds and positive batches");
  std::vector<double> old_times, new_times;
  sink = sink ^ (long long)old_fn();
  sink = sink ^ (long long)new_fn();
  for (int round = 0; round < rounds; ++round) {
    if (round & 1) {
      new_times.push_back(elapsed_ms(new_fn, batches));
      old_times.push_back(elapsed_ms(old_fn, batches));
    } else {
      old_times.push_back(elapsed_ms(old_fn, batches));
      new_times.push_back(elapsed_ms(new_fn, batches));
    }
  }
  auto median = [](std::vector<double> a) {
    std::sort(a.begin(), a.end());
    return a[a.size() / 2];
  };
  double old_ms = median(old_times), new_ms = median(new_times);
  std::cout << std::setprecision(10)
            << "{\"type\":\"benchmark\",\"name\":";
  json_string(name);
  std::cout << ",\"baseline_ms\":" << old_ms
            << ",\"candidate_ms\":" << new_ms
            << ",\"speedup\":" << old_ms / new_ms
            << ",\"rounds\":" << rounds
            << ",\"batches\":" << batches
            << ",\"baseline_samples_ms\":";
  samples(old_times);
  std::cout << ",\"candidate_samples_ms\":";
  samples(new_times);
  std::cout << "}\n";
}
} // namespace cbtest
