using ll = long long;
struct Sum {
  static constexpr ll id = 0;
  static ll merge(ll a, ll b) { return a + b; }
  static ll add(ll a, ll x, ll len) {
    return a + x * len;
  }
};
struct Max {
  static constexpr ll id = LLONG_MIN;
  static ll merge(ll a, ll b) { return max(a, b); }
  static ll add(ll a, ll x, ll) { return a + x; }
};
struct Min {
  static constexpr ll id = LLONG_MAX;
  static ll merge(ll a, ll b) { return min(a, b); }
  static ll add(ll a, ll x, ll) { return a + x; }
};

// 閉區間，初始全 0；區間長度及運算須在 ll 範圍內
// 每次 O(log C)，Q 次更新空間 O(Q log C)
// C = R-L+1；query 不開點；使用 C++17
template<class Op> struct DynamicSegTree {
  struct Node {
    int32_t lc = 0, rc = 0;
    ll val = 0, lazy = 0;
  };
  ll L, R;
  vector<Node> t = {{}, {}}; // 0 空節點，1 根

  DynamicSegTree(ll l, ll r) : L(l), R(r) {
    assert(l <= r && (__int128)r-l+1 <= LLONG_MAX);
  }
  void range_add(ll l, ll r, ll x) {
    check(l, r);
    if (x) update(1, L, R, l, r, x, false);
  }
  void modify(ll i, ll x) {
    check(i, i);
    update(1, L, R, i, i, x, true);
  }
  ll query(ll l, ll r) const {
    check(l, r);
    return query(1, L, R, l, r, 0);
  }
private:
  void check(ll l, ll r) const {
    assert(L <= l && l <= r && r <= R);
  }
  int32_t make() {
    t.push_back({});
    return t.size() - 1;
  }
  void apply(int32_t p, ll l, ll r, ll x) {
    t[p].val = Op::add(t[p].val, x, r-l+1);
    t[p].lazy += x;
  }
  void push(int32_t p, ll l, ll r) {
    ll x = t[p].lazy, m = l + (r-l)/2;
    if (!x || l == r) return;
    if (!t[p].lc) t[p].lc = make();
    if (!t[p].rc) t[p].rc = make();
    apply(t[p].lc, l, m, x);
    apply(t[p].rc, m+1, r, x);
    t[p].lazy = 0;
  }
  int32_t update(int32_t p, ll l, ll r,
                 ll a, ll b, ll x, bool set) {
    if (!p) p = make();
    if (a <= l && r <= b) {
      if (set) { t[p].val = x; t[p].lazy = 0; }
      else apply(p, l, r, x);
      return p;
    }
    push(p, l, r);
    ll m = l + (r-l)/2;
    if (a <= m)
      t[p].lc = update(t[p].lc, l, m, a, b, x, set);
    if (b > m)
      t[p].rc = update(t[p].rc, m+1, r, a, b, x, set);
    t[p].val = Op::merge(t[t[p].lc].val,
                         t[t[p].rc].val);
    return p;
  }
  ll query(int32_t p, ll l, ll r,
           ll a, ll b, ll carry) const {
    if (r < a || b < l) return Op::id;
    if (!p)
      return Op::add(0, carry, min(r,b)-max(l,a)+1);
    if (a <= l && r <= b)
      return Op::add(t[p].val, carry, r-l+1);
    ll m = l + (r-l)/2;
    carry += t[p].lazy;
    return Op::merge(
      query(t[p].lc, l, m, a, b, carry),
      query(t[p].rc, m+1, r, a, b, carry));
  }
};

// DynamicSegTree<Max> seg(1, 1000000000); // Sum/Min
// seg.range_add(2, 4, 3); seg.modify(3, 5);
// seg.query(1, 5); // 5
