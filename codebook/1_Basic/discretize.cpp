// 建表 O(n log n)，查詢 O(log n)，不修改原陣列
// 編號從 1 開始；查詢值必須已收集
struct Discretize {
  vector<int> xs;

  Discretize(const vector<int> &a) : xs(a) {
    sort(xs.begin(), xs.end());
    xs.erase(unique(xs.begin(), xs.end()), xs.end());
  }

  int operator()(int x) const {
    auto it = lower_bound(xs.begin(), xs.end(), x);
    assert(it != xs.end() && *it == x);
    return it - xs.begin() + 1;
  }

  int size() const { return xs.size(); }
};

// Discretize d({30, 10, 30});
// d(10) -> 1, d(30) -> 2
