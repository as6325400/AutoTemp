// 帶權區間排程 Weighted Interval Scheduling
// dp[i] = max(dp[i-1], w[i] + dp[h[i]])
// h[i]: 最後一個結束時間 < l[i] 的任務 (用二分查找)
// 時間複雜度 O(n log n)


struct Project { long long l, r, w; };
bool cmp(const Project &a, const Project &b){ return a.r < b.r; }

long long weightedInterval(vector<Project> &p){
    int n = p.size();
    sort(p.begin(), p.end(), cmp);
    vector<long long> dp(n), endTime;
    for (auto &x : p) endTime.push_back(x.r);
    for (int i = 0; i < n; i++){
        int j = upper_bound(endTime.begin(), endTime.end(), p[i].l-1) - endTime.begin() - 1;
        dp[i] = max((i? dp[i-1] : 0), p[i].w + (j >= 0 ? dp[j] : 0));
    }
    return dp[n-1];
}