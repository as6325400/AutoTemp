#define int long long

// 根據費馬小定理，若 a p 互質，a^(p-2) 為 a 在 mod p 時的乘法逆元
// a ^ (b ^ c) % mod = fast_pow(a, fast_pow(b, c, mod - 1), mod)
typedef unsigned long long ull;
inline int ksc(ull x, ull y, int p) {  // O(1)快速乘（防爆long long）
  return (x * y - (ull)((long double)x / p * y) * p + p) % p;
}

inline int fast_pow(int a, int b, int mod)
{	
	// a^b % mod 
  int res = 1;
  while(b)
  {
    if(b & 1) res =  ksc(res, a, mod);
    a = ksc(a, a, mod);
    b >>= 1;
  }
  return res;
}