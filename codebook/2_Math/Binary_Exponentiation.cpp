// 根據費馬小定理，若 a p 互質，a^(p-2) 為 a 在 mod p 時的乘法逆元
int fast_pow(int a, int b, int mod)
{	
	// a^b % mod 
  int res = 1;
  while(b)
  {
    if(b & 1) res = (res * a) % mod;
    a = (a * a) % mod;
    b >>= 1;
  }
  return res;
}