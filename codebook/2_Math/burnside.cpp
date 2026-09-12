#define int long long

// Burnside 引理：軌道數 = (1/|G|) Σ_{g∈G} |Fix(g)|
// 著色時 |Fix(g)| = k^(g 的循環個數)
// 項鍊 = 只算旋轉（循環群 C_n）
// 手鍊 = 旋轉 + 翻轉（二面體群 D_n）
// 以下 mod 需為質數（用費馬小定理求逆元）

// 歐拉函數 φ(n)
int phi(int n) {
  int r = n;
  for (int p = 2; p * p <= n; ++p)
    if (n % p == 0) {
      r -= r / p;
      while (n % p == 0) n /= p;
    }
  if (n > 1) r -= r / n;
  return r;
}

int modpow(int a, int e, int mod) {
  int r = 1;
  while (e) {
    if (e & 1) r = r * a % mod;
    a = a * a % mod;
    e >>= 1;
  }
  return r;
}

// 項鍊：n 顆珠子、k 色，本質不同數量
// = (1/n) Σ_{d|n} φ(d) * k^(n/d)
int necklace(int n, int k, int mod) {
  int sum = 0;
  for (int d = 1; d * d <= n; ++d)
    if (n % d == 0) {
      sum = (sum + phi(d) % mod *
                       modpow(k, n / d, mod)) % mod;
      if (d * d != n)
        sum = (sum + phi(n / d) % mod *
                         modpow(k, d, mod)) % mod;
    }
  return sum * modpow(n, mod - 2, mod) % mod;
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
           modpow(k, (n + 1) / 2, mod) % mod;
  else
    refl = (n / 2) % mod *
           (modpow(k, n / 2, mod) +
            modpow(k, n / 2 + 1, mod)) % mod;
  return (rot + refl) % mod *
         modpow(2 * n, mod - 2, mod) % mod;
}
