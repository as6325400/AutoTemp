// fast_pow 去前面抄，需要處裡防暴乘法 
// 記得 #define int long long 也要放
// long long 範圍內測試過答案正確
// time: O(logn)

inline bool mr(int x, int p) {
  if (fast_pow(x, p - 1, p) != 1) return 0; 
  int y = p - 1, z;
  while (!(y & 1)) { 
      y >>= 1;
      z = fast_pow(x, y, p);                    
      if (z != 1 && z != p - 1) return 0;  
      if (z == p - 1) return 1;  
  }
  return 1;
}

inline bool prime(int x) {
  if (x < 2) return 0;
  if (x == 2 || x == 3 || x == 5 || x == 7 || x == 43) return 1;
  // 如果把 2 到 37 前 12 個質數都檢查一遍 可以保證 2^78 皆可用
  return mr(2, x) && mr(3, x) && mr(5, x) && mr(7, x) && mr(43, x);
}
