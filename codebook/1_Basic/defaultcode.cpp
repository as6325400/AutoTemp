#include <bits/stdc++.h>
#define int long long
#define endl '\n' // 如果是互動題要把這個註解掉
#define de(x) cout << #x << '=' << x << ", "
#define dd cout << '\n';
// #pragma GCC target("popcnt")
// #pragma GCC optimize("O3")
using namespace std;
int tt = 1;

void pre() {
  cout.tie(nullptr); // 輸出加速
  cin >> tt; // 多筆輸入
}

void solve() {}

signed main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
#ifdef LOCAL
  // g++ -DLOCAL -std=c++17 <filename> && ./a.out
  freopen("input.txt", "r", stdin);
  // freopen("output.txt", "w", stdout);
#endif // LOCAL
  pre();
  while (tt--) { solve(); }
  return 0;
}
