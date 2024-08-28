// 主函數記得放 srand(time(nullptr))
// prime 檢測以及快速冪, gcd 等請從前面抄

// 輸入一個數字 p，隨機回傳一個 非 1 非 p 的因數，若 p 是質數會無窮迴圈
#define rg register int
inline int rho(int p) {  
  int x, y, z, c, g;
  rg i, j;                
  while (1) {              
    y = x = rand() % p;  
    z = 1;
    c = rand() % p;               
    i = 0, j = 1;                    
    while (++i) {                    
      x = (ksc(x, x, p) + c) % p;  
      z = ksc(z, abs(y - x), p);  
      if (x == y || !z) break; 
      if (!(i % 127) || i == j) { 
        g = gcd(z, p);
        if (g > 1) return g;
        if (i == j) y = x, j <<= 1;  
      }
    }
  }
}

// 回傳隨機一個質因數，若 input 為質數，則直接回傳
int prho(int p){
  if(prime(p)) return p;
  int m = rho(p);
  if(prime(m)) return m;
  return prho(p / m);
}

// 回傳將 n 質因數分解的結果，由小到大排序
vector<int> prime_factorization(int n){
  vector<int> ans;
  while(n != 1){
    int m = prho(n);
    ans.push_back(m);
    n /= m;
  }
  sort(ans.begin(), ans.end());
  return ans;
}