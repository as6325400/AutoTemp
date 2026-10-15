using ll = long long;
// KM（Kuhn-Munkres / Hungarian）
//   最小權完美匹配，O(n^2 m)
// 來源：cp-algorithms Hungarian
//   （Andrey Lopatin，O(n^3) 版）
// a：n x m 權重矩陣，0-based，需 n <= m（否則 assert）
// ans[i]：第 i 列配到的行（0-based）；回傳最小總權
// 求最大權：a 全取負呼叫，結果取負回去；負權安全。
ll km(const vector<vector<ll>>& a, vector<int>& ans) {
  int n = (int)a.size();
  ans.assign(n, -1);
  if (n == 0) return 0;
  int m = (int)a[0].size();
  assert(n <= m);
  const ll INF = (ll)4e18;
  vector<ll> u(n + 1), v(m + 1);
  // p[j] 1-based，0 = 未配
  vector<int> p(m + 1), way(m + 1);
  for (int i = 1; i <= n; ++i) {
    p[0] = i;
    int j0 = 0;
    vector<ll> minv(m + 1, INF);
    vector<char> used(m + 1);
    do {
      used[j0] = true;
      int i0 = p[j0], j1 = 0;
      ll delta = INF;
      for (int j = 1; j <= m; ++j) if (!used[j]) {
        ll cur = a[i0 - 1][j - 1] - u[i0] - v[j];
        if (cur < minv[j])
          minv[j] = cur, way[j] = j0;
        if (minv[j] < delta) delta = minv[j], j1 = j;
      }
      for (int j = 0; j <= m; ++j)
        if (used[j]) u[p[j]] += delta, v[j] -= delta;
        else minv[j] -= delta;
      j0 = j1;
    } while (p[j0] != 0);
    do {
      int j1 = way[j0];
      p[j0] = p[j1];
      j0 = j1;
    } while (j0 != 0);
  }
  for (int j = 1; j <= m; ++j)
    if (p[j] != 0) ans[p[j] - 1] = j - 1;
  return -v[0];
}

// 無權二分圖轉 KM 矩陣，O(V + E + L R)
// graph：0-based 鄰接表，單向或雙向存邊皆可
// color：已染好 0 / 1，邊須異色
// L、R 為兩側點數；小側當列，同大時用 color 0
// 列、行各依原頂點編號遞增；回傳 ll 矩陣
// 有邊 -1，沒邊 0（補位）；-km 即最大匹配數
// ans 為矩陣編號，a[i][ans[i]] == -1 才是原邊
// 用法：auto a = km_matrix(graph, color);
//       vector<int> ans;
//       ll match = -km(a, ans);
// 最大匹配 = 最小點覆蓋；最大獨立集 = V - match
vector<vector<ll>> km_matrix(
    const vector<vector<int>>& graph,
    const vector<int>& color) {
  int n = (int)graph.size(), cnt[2] = {};
  vector<int> id(n);
  for (int i = 0; i < n; ++i) {
    id[i] = cnt[color[i]]++;
  }
  int side = cnt[0] > cnt[1];
  vector<vector<ll>> a(cnt[side],
      vector<ll>(cnt[side ^ 1]));
  for (int u = 0; u < n; ++u) for (int v : graph[u]) {
    if (color[u] == side) a[id[u]][id[v]] = -1;
    else a[id[v]][id[u]] = -1;
  }
  return a;
}
