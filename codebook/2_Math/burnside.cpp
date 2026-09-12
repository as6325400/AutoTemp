#define int long long

// Burnside 引理：軌道數 = (1/|G|) Σ_{g∈G} |Fix(g)|
// 著色時 |Fix(g)| = k^(g 的循環個數)
// 項鍊 = 只算旋轉（循環群 C_n）
// 手鍊 = 旋轉 + 翻轉（二面體群 D_n）
// fast_pow 去前面抄（快速冪），phi 去前面抄（歐拉函數）
// mod 需為質數（用費馬小定理求逆元）

// 項鍊：n 顆珠子、k 色，本質不同數量
// = (1/n) Σ_{d|n} φ(d) * k^(n/d)
int necklace(int n, int k, int mod) {
  int sum = 0;
  for (int d = 1; d * d <= n; ++d)
    if (n % d == 0) {
      sum = (sum + phi(d) % mod *
                       fast_pow(k, n / d, mod)) % mod;
      if (d * d != n)
        sum = (sum + phi(n / d) % mod *
                         fast_pow(k, d, mod)) % mod;
    }
  return sum * fast_pow(n, mod - 2, mod) % mod;
}

// 手鍊：旋轉 + 翻轉
// = (1/2n) [ Σ_{d|n} φ(d) k^(n/d) + R ]
//   n 奇: R = n * k^((n+1)/2)
//   n 偶: R = (n/2) * k^(n/2) + (n/2) * k^(n/2+1)
int bracelet(int n, int k, int mod) {
  int rot = necklace(n, k, mod) * n % mod;  // 旋轉部分總和
  int refl;
  if (n & 1)
    refl = n % mod *
           fast_pow(k, (n + 1) / 2, mod) % mod;
  else
    refl = (n / 2) % mod *
           (fast_pow(k, n / 2, mod) +
            fast_pow(k, n / 2 + 1, mod)) % mod;
  return (rot + refl) % mod *
         fast_pow(2 * n, mod - 2, mod) % mod;
}
