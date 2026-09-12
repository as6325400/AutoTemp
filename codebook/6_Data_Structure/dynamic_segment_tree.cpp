using ll = long long;
struct Sum {
  static constexpr ll id = 0;
  static ll merge(ll a, ll b) { return a + b; }
  static ll add(ll a, ll x, ll len) {
    return a + x * len;
  }
  static ll assign(ll x, ll len) { return x * len; }
};
struct Max {
  static constexpr ll id = LLONG_MIN;
  static ll merge(ll a, ll b) { return max(a, b); }
  static ll add(ll a, ll x, ll) { return a + x; }
  static ll assign(ll x, ll) { return x; }
};
struct Min {
  static constexpr ll id = LLONG_MAX;
  static ll merge(ll a, ll b) { return min(a, b); }
  static ll add(ll a, ll x, ll) { return a + x; }
  static ll assign(ll x, ll) { return x; }
};

// 閉區間，初始全 0；區間長度及運算須在 ll 範圍內
// 支援區間加值、區間賦值、單點設值、區間查詢
// 每次 O(log C)，Q 次更新空間 O(Q log C)
// C = R-L+1；query 不開點；使用 C++17
template<class Op> struct DynamicSegTree {
  struct Node {
    int32_t lc = 0, rc = 0;
    ll val = 0, lazy = 0; // lazy：要給子樹的值，set 決定覆寫或累加
    bool set = false;
  };
  ll L, R;
  vector<Node> t = {{}, {}}; // 0 空節點，1 根

  DynamicSegTree(ll l, ll r) : L(l), R(r) {
    // 用 unsigned 差值檢查長度，避免有號溢位，亦支援 32-bit 編譯器。
    assert(l <= r && (unsigned long long)r - (unsigned long long)l
                        < (unsigned long long)LLONG_MAX);
  }
  void range_add(ll l, ll r, ll x) {
    check(l, r);
    if (x) update(1, L, R, l, r, x, false);
  }
  void range_assign(ll l, ll r, ll x) {
    check(l, r);
    update(1, L, R, l, r, x, true);
  }
  void modify(ll i, ll x) {
    check(i, i);
    update(1, L, R, i, i, x, true);
  }
  ll query(ll l, ll r) const {
    check(l, r);
    return query(1, L, R, l, r, 0, false);
  }
private:
  void check(ll l, ll r) const {
    assert(L <= l && l <= r && r <= R);
  }
  int32_t make() {
    t.push_back({});
    return static_cast<int32_t>(t.size() - 1);
  }
  void apply_add(int32_t p, ll l, ll r, ll x) {
    t[p].val = Op::add(t[p].val, x, r-l+1);
    t[p].lazy += x; // set 為真時即「賦值再加」，lazy 是子樹要變成的新值
  }
  void apply_assign(int32_t p, ll l, ll r, ll x) {
    t[p].val = Op::assign(x, r-l+1);
    t[p].lazy = x;
    t[p].set = true;
  }
  void push(int32_t p, ll l, ll r) {
    ll x = t[p].lazy, m = l + (r-l)/2;
    if ((!x && !t[p].set) || l == r) return;
    if (!t[p].lc) t[p].lc = make();
    if (!t[p].rc) t[p].rc = make();
    if (t[p].set) {
      apply_assign(t[p].lc, l, m, x);
      apply_assign(t[p].rc, m+1, r, x);
      t[p].set = false;
    } else {
      apply_add(t[p].lc, l, m, x);
      apply_add(t[p].rc, m+1, r, x);
    }
    t[p].lazy = 0;
  }
  int32_t update(int32_t p, ll l, ll r,
                 ll a, ll b, ll x, bool assign) {
    if (!p) p = make();
    if (a <= l && r <= b) {
      if (assign) apply_assign(p, l, r, x);
      else apply_add(p, l, r, x);
      return p;
    }
    push(p, l, r);
    ll m = l + (r-l)/2;
    if (a <= m)
      t[p].lc = update(t[p].lc, l, m, a, b, x, assign);
    if (b > m)
      t[p].rc = update(t[p].rc, m+1, r, a, b, x, assign);
    t[p].val = Op::merge(t[t[p].lc].val,
                         t[t[p].rc].val);
    return p;
  }
  ll query(int32_t p, ll l, ll r,
           ll a, ll b, ll carry, bool carried_set) const {
    if (r < a || b < l) return Op::id;
    ll len = min(r,b)-max(l,a)+1;
    if (!p)
      return carried_set ? Op::assign(carry, len)
                         : Op::add(0, carry, len);
    if (a <= l && r <= b)
      return carried_set ? Op::assign(carry, len)
                         : Op::add(t[p].val, carry, r-l+1);
    ll m = l + (r-l)/2;
    if (!carried_set) {
      // 祖先的加值較新，加在子孫的標記值之上；
      // 已帶著較新的 assign 時，子孫較舊的標記一律忽略
      carry += t[p].lazy;
      if (t[p].set) carried_set = true;
    }
    return Op::merge(
      query(t[p].lc, l, m, a, b, carry, carried_set),
      query(t[p].rc, m+1, r, a, b, carry, carried_set));
  }
};

// DynamicSegTree<Max> seg(1, 1000000000); // Sum/Min
// seg.range_add(2, 4, 3); seg.range_assign(2, 3, 5);
// seg.modify(3, 5); seg.query(1, 5);
