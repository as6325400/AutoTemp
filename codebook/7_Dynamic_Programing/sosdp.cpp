// SOS DP：子集合和，O(k * 2^k)，額外空間 O(1)
// f：大小為 2^k（k >= 0），原地更新
// f[mask]：mask 所有子集合的原始值總和
//   包含空集合與 mask 本身；加總須不溢位
// 用法：auto f = a; sosdp(f);
//   a = {1, 2, 3, 4}，結果 f = {1, 3, 4, 10}
// 取模時將 += 改成模加法；bit 必須放外層
template<typename T>
void sosdp(vector<T>& f) {
  size_t n = f.size();
  assert(n && !(n & (n - 1)));
  for (size_t bit = 1; bit < n; bit <<= 1)
    for (size_t mask = 0; mask < n; ++mask)
      if (mask & bit) f[mask] += f[mask ^ bit];
}

// 位元操作（long long，0 <= n < 63）
// 檢查第 n 位是否為1：a & (1LL << n)
// 強制將第 n 位變成1：a |= (1LL << n)
// 強制將第 n 位變成0：a &= ~(1LL << n)
// 將第 n 位反轉(1變0, 0變1)：a ^= (1LL << n)
// 第 0 ~ n - 1位 全部都是1：(1LL << n) - 1
// 兩個集合的聯集：S = a | b
// 兩個集合的交集：S = a & b
