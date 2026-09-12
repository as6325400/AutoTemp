using ll = long long;
// KM（Kuhn-Munkres / Hungarian）：最小權完美匹配，O(n^2 m)
// 來源：cp-algorithms Hungarian（Andrey Lopatin，O(n^3) 版）
// a：n x m 權重矩陣，0-based，需 n <= m（否則 assert）
// ans[i]：第 i 列配到的行（0-based）；回傳最小總權
// 求最大權：a 全取負呼叫，結果取負回去；負權安全。
ll km(const vector<vector<ll>>& a, vector<int>& ans) {
    int n = (int)a.size();
    ans.clear();
    if (n == 0) return 0;
    int m = (int)a[0].size();
    assert(n <= m);
    const ll INF = (ll)4e18;
    vector<ll> u(n + 1, 0), v(m + 1, 0);
    vector<int> p(m + 1, 0), way(m + 1, 0); // p[j] 1-based，0 = 未配

    for (int i = 1; i <= n; ++i) {
        p[0] = i;
        int j0 = 0;
        vector<ll> minv(m + 1, INF);
        vector<bool> used(m + 1, false);
        do {
            used[j0] = true;
            int i0 = p[j0];
            ll delta = INF;
            int j1 = 0;
            for (int j = 1; j <= m; ++j) {
                if (!used[j]) {
                    ll cur = a[i0 - 1][j - 1] - u[i0] - v[j];
                    if (cur < minv[j]) minv[j] = cur, way[j] = j0;
                    if (minv[j] < delta) delta = minv[j], j1 = j;
                }
            }
            for (int j = 0; j <= m; ++j) {
                if (used[j]) u[p[j]] += delta, v[j] -= delta;
                else minv[j] -= delta;
            }
            j0 = j1;
        } while (p[j0] != 0);
        do {
            int j1 = way[j0];
            p[j0] = p[j1];
            j0 = j1;
        } while (j0 != 0);
    }
    ans.assign(n, -1);
    for (int j = 1; j <= m; ++j)
        if (p[j] != 0) ans[p[j] - 1] = j - 1;
    return -v[0];
}