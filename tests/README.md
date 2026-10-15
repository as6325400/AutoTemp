# Codebook 測試與速度比較

每個模板用同一個入口跑正確性、sanitizer 與速度比較。正式模板在 `codebook/`；待審查版本在 `candidates/codebook/`，不會自動加入 PDF 或取代正式版。測試與報告一起維護，先確認正確性，再評估速度、行數及可讀性的取捨。

## 使用方式

目前支援 Linux／WSL 等 POSIX 環境，需要 Python 3、Git、支援 C++17 的 g++，不需要額外 Python 套件。assertion 測試使用 POSIX 的 abort 訊號。

```bash
python3 tests/run.py --list
python3 tests/run.py --suite km
python3 tests/run.py --suite km --bench --json /tmp/km-results.json
python3 tests/run.py --bench --json /tmp/codebook-results.json
```

預設執行所有已登錄 suite 的正確性與 ASan／UBSan；`--bench` 額外比較速度。`--bench-only` 僅跑效能案例，但各案例仍須先驗證結果。`--no-sanitize` 用於快速檢查，不能取代正式驗證。執行檔、舊版原始碼和編譯產物都在暫存目錄。

`--baseline REF` 可明確指定其他比較版本；一般情況使用各 suite 固定的完整 commit。無法取得基準時直接失敗，不會改拿目前檔案比較。

## 檔案配置

```text
tests/
  run.py                         共用執行入口
  support/test.hpp               斷言、計時與 JSON 輸出
  suites/km.json                 路徑、基準、編譯模式
  cases/graph/km_test.cpp         測資、獨立解答與效能案例
candidates/codebook/              待使用者審查的模板
reports/                         逐檔審查、差異與測量結果
```

新增模板只需自己的測試與 JSON，不必修改共用 runner。分類目錄沿用 `basic`、`math`、`graph`、`string`、`geometry`、`data_structure`、`dp`、`divide`、`tree`、`else`。

設定檔範例：

```json
{
  "name": "example",
  "test": "tests/cases/graph/example_test.cpp",
  "baseline": {
    "revision": "完整的 Git commit hash",
    "path": "codebook/3_Graph/example.cpp"
  },
  "candidate": "candidates/codebook/3_Graph/example.cpp",
  "variants": [{"name": "contest", "defines": []}],
  "assertions": []
}
```

若只測正式模板，`candidate` 直接指向 `codebook/`。不需為沒有改善價值的模板複製一份候選版。需要支援一般 `int` 和 `#define int long long` 時，參考 KM 的兩種 variant；原本就限定競賽巨集的片段只測其實際使用方式。

## C++ 測試約定

先在全域 include `support/test.hpp`，再把 `BASELINE_HEADER` 與 `CANDIDATE_HEADER` include 進各自 namespace。這兩個路徑由 runner 提供。模板缺少的 `ll`、常數、資料結構、初始化及依賴由該測試明確補上，不由 runner 猜測或改寫演算法。標準庫不能 include 在自訂 namespace 中；巨集也不受 namespace 隔離，需清理會互相影響的巨集。

每份測試提供以下命令，失敗須回傳非零：

- `--check`：固定邊界案例、可重現的隨機測資、獨立 oracle 或性質檢查。
- `--bench`：先檢查該組輸入的新舊答案，再用 `cbtest::compare_speed` 計時。
- `--bad-NAME`：只有模板明定用 assert 擋掉的無效輸入才登錄；runner 確認是 assertion abort，其他 crash 不算通過。

`cbtest::require` 不受 `NDEBUG` 影響。隨機 seed 寫在測試內並印出；`--check` 和 `--bench` 各自重設 seed，避免執行順序影響測資。錯誤案例最好印出 seed、尺寸與操作序列，方便縮小反例。

新舊一致不等於正確。至少使用暴力解、獨立演算法或可驗證的不變量之一。已知舊版錯誤的輸入要寫成回歸測試：驗證候選版的正確答案，並清楚記錄舊版的限制；不要為了讓兩者相等而排除問題。

KM 的 `km_matrix` 由呼叫者保證 `color.size() == graph.size()`、顏色為 0/1、頂點編號合法且邊的兩端異色。測試這些前提成立時的結果，不把未定義的無效輸入當作必須拒絕的功能；另驗證 `km` 本體的 `n <= m` assertion。

## 公平比較速度

- 新舊版使用同一編譯器、`-O2` 和編譯模式；sanitizer 的耗時不拿來比較。
- 固定輸入，生成資料、驗證答案、輸出文字均不計時。
- 先暖機，再交替執行新舊版本；預設 7 輪，每輪 3 次，報每次耗時的中位數。
- callback 回傳與結果相關的 checksum，避免編譯器刪除計算；若演算法改動輸入，每次要恢復相同初始狀態，並說明是否包含複製成本。
- 依實際用途選小／大、稀疏／密集、隨機／退化案例。不同複雜度不能只測一個尺寸。
- 不平行跑 benchmark；不要在其他大量編譯或測試同時進行時量測。
- JSON 保存編譯環境、來源版本、候選檔指紋及每輪結果。細小差異視為測量波動，不設固定速度門檻讓 CI 誤報。

只在兩版都正確、同樣完成指定工作的範圍比較速度。舊版遇到越界、溢位或錯誤答案，報告應標示「不適用」，不能宣稱修正版較快幾倍。

## 審查與採用

每份候選版附上行為差異、原有使用限制、測資範圍、實測時間及行數。行數同時看完整檔案和非空、非純註解行；保留必要註解與約 55 欄的競賽排版，不把很多敘述塞成一行當作改善。

使用者決定採用後，才把候選檔移入正式位置，調整 suite 的 `candidate` 路徑，重新驗證，並在同一個 commit 更新報告。測試的 baseline 不會自動前移；要改比較目的時再明確更新它。

未新增候選版的模板也列在審查清單中。純公式筆記、未完成片段與可執行模板分開記錄；沒有執行測試的項目不能寫成「測試通過」。
