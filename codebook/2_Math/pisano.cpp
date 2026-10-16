// fib(x) % m = fib(x + kn) % m 當 k >= 1，求 n
// n 為費式數列 % m 會重複的週期
// pisano_period(m) <= 6m
// 通常這都要本地跑

#define int long long

// m >= 1，6m 須放得進 long long；O(週期)。
int pisano_period(int m) {
  if (m == 1) return 1;
  int pre = 0, cur = 1;
  for (int i = 1;; ++i) {
    int next = (pre + cur) % m;
    pre = cur, cur = next;
    if (pre == 0 && cur == 1) return i;
  }
}
