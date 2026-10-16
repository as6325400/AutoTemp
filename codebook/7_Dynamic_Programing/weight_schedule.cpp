// 帶權區間排程 Weighted Interval Scheduling
// dp[i] = max(dp[i-1], w[i] + dp[h[i]])
// h[i]: 最後一個結束時間 < l[i] 的任務 (用二分查找)
// 時間複雜度 O(n log n)
// 每個任務 l <= r；可不選任務，函式會依 r 排序 p。
struct Project { long long l, r, w; };
bool cmp(const Project& a, const Project& b) {
  return a.r < b.r;
}
long long weightedInterval(vector<Project>& p) {
  int n = p.size();
  sort(p.begin(), p.end(), cmp);
  vector<long long> dp(n + 1), endTime;
  for (auto& x : p) endTime.push_back(x.r);
  for (int i = 0; i < n; ++i) {
    int j = lower_bound(endTime.begin(),
        endTime.begin() + i, p[i].l) - endTime.begin();
    dp[i + 1] = max(dp[i], p[i].w + dp[j]);
  }
  return dp[n];
}
