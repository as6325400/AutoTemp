#define int long long

// 歐拉函數 φ(n)：1..n 中與 n 互質的整數個數
// φ(n) = n * Π_{p|n} (1 - 1/p)
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
