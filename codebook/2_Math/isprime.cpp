// fast_pow 去前面抄，需要處裡防暴乘法
// 記得 #define int long long 也要放
// 配合 ksc 的 __uint128_t 版本，適用正 long long。
// time: O(logn)，每個底數只做一次快速冪。
// 底數來源：https://miller-rabin.appspot.com/

// p > 2 為奇數，x % p != 0。
inline bool mr(int x, int p) {
  int d = p - 1, s = 0;
  while (!(d & 1)) d >>= 1, ++s;
  int z = fast_pow(x % p, d, p);
  if (z == 1 || z == p - 1) return 1;
  for (int i = 1; i < s; ++i) {
    z = ksc(z, z, p);
    if (z == p - 1) return 1;
  }
  return 0;
}

inline bool prime(int x) {
  if (x < 2 || x % 2 == 0) return x == 2;
  // 這 7 個底數可判定小於 2^64 的整數。
  for (int a : {2LL, 325LL, 9375LL, 28178LL,
                450775LL, 9780504LL, 1795265022LL})
    if (a % x && !mr(a, x)) return 0;
  return 1;
}
