// O(n m) 時間、O(min(n,m)) 空間，兩字串皆 0-based。
int LCS(const string& t1, const string& t2) {
  string_view a = t1, b = t2;
  if (a.size() < b.size()) swap(a, b);
  vector<int> dp(b.size() + 1);
  for (char c : a) {
    int prev = 0;
    for (int j = 1; j <= (int)b.size(); ++j) {
      int old = dp[j];
      dp[j] = c == b[j-1] ? prev + 1
                          : max(dp[j], dp[j-1]);
      prev = old;
    }
  }
  return dp.back();
}

// 匹配位置轉 LIS；同字元的位置必須逆序。
// R = 相等字元對數；O(n+m+R log(min(n,m)+1))。
// R 可達 n*m，重複字元多時建議用上面的 DP。
int LCS_sparse(const string& a, const string& b) {
  vector<vector<int>> pos(256);
  for (int i = (int)a.size() - 1; i >= 0; --i)
    pos[(unsigned char)a[i]].push_back(i);
  vector<int> tail;
  for (unsigned char c : b) for (int p : pos[c]) {
    auto it = lower_bound(tail.begin(), tail.end(), p);
    if (it == tail.end()) tail.push_back(p);
    else *it = p;
  }
  return tail.size();
}
