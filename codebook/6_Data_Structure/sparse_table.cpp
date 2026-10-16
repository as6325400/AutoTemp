class Sparse_Table {
  // 0-base
  // 要改成找最大把min換成max就好
  // 此版同時建 min/max；只需一種時可刪另一張表。
  vector<vector<int>> mn, mx;
public:
  Sparse_Table(const vector<int>& a) {
    int n = a.size(), k = n ? __lg(n) + 1 : 0;
    mn.assign(k, vector<int>(n));
    mx = mn;
    if (!n) return;
    mn[0] = mx[0] = a;
    for (int j = 1; j < k; ++j)
      for (int i = 0; i + (1 << j) <= n; ++i) {
        int r = i + (1 << (j - 1));
        mn[j][i] = min(mn[j-1][i], mn[j-1][r]);
        mx[j][i] = max(mx[j-1][i], mx[j-1][r]);
      }
  }
  // 非空詢問須滿足 0 <= l <= r < n。
  int query_min(int l, int r) const {
    if (l > r) return numeric_limits<int>::max();
    int j = __lg(r - l + 1);
    return min(mn[j][l], mn[j][r - (1 << j) + 1]);
  }
  int query_max(int l, int r) const {
    if (l > r) return numeric_limits<int>::lowest();
    int j = __lg(r - l + 1);
    return max(mx[j][l], mx[j][r - (1 << j) + 1]);
  }
};
