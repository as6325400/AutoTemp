#define int long long

// 根據費馬小定理，若 a p 互質，a^(p-2) 為 a 在 mod p 時的乘法逆元
// 上式還需 p 為質數。
// a ^ (b ^ c) % mod = fast_pow(a, fast_pow(b, c, mod - 1), mod)
// 上式需 mod 為質數且 a 與 mod 互質。
typedef unsigned long long ull;
inline int ksc(ull x, ull y, int p) {
  // O(1)快速乘（防爆long long）；p > 0，GCC/Clang。
  return (__uint128_t)x * y % p;
}

// a^b % mod；b >= 0、mod > 0，支援負底數。
inline int fast_pow(int a, int b, int mod) {
  a %= mod;
  if (a < 0) a += mod;
  int res = 1 % mod;
  for (; b; b >>= 1, a = ksc(a, a, mod))
    if (b & 1) res = ksc(res, a, mod);
  return res;
}
