// 用途：維護一組直線 y = m*x + b，支援「加入直線」與「單點查詢最大值」。
// 求最小值：對所有 (m,b) 取負後 query 再取負即可。
// 適用情境：動態 CHT、斜率優化 DP。
using ll = long long;
const ll LINF = 4e18; / 大於所有可能的直線取值

struct Line {
    ll m, b;     // y = m*x + b
    ll eval(ll x) const { return m * x + b; }
};

struct LiChao {
    vector<ll> xs;    
    vector<Line> tree;
    int n;

    LiChao(const vector<ll>& qs) {
        xs = qs;
        sort(xs.begin(), xs.end());
        xs.erase(unique(xs.begin(), xs.end()), xs.end());
        n = (int)xs.size();
        tree.assign(4 * max(1, n), Line{0, -LINF});
    }

    void add_line(Line nw, int id, int l, int r) {
        int m = (l + r) / 2;
        bool lef = nw.eval(xs[l]) > tree[id].eval(xs[l]);
        bool mid = nw.eval(xs[m]) > tree[id].eval(xs[m]);
        if (mid) swap(tree[id], nw);
        if (r - l == 1) return;
        if (lef != mid) add_line(nw, id * 2, l, m);
        else            add_line(nw, id * 2 + 1, m, r);
    }

    void add_line(ll m, ll b) {
        if (n == 0) return;
        add_line(Line{m, b}, 1, 0, n);
    }

    ll query(ll x, int id, int l, int r) const {
        ll res = tree[id].eval(x);
        if (r - l == 1) return res;
        int m = (l + r) / 2;
        if (x < xs[m]) return max(res, query(x, id * 2, l, m));
        else           return max(res, query(x, id * 2 + 1, m, r));
    }

    ll query(ll x) const {
        if (n == 0) return -LINF;
        int idx = (int)(lower_bound(xs.begin(), xs.end(), x) - xs.begin());
        assert(idx < n && xs[idx] == x);
        return query(x, 1, 0, n);
    }
};
//   vector<ll> qs = {1, 3, 5};          // 所有會 query 的點
//   LiChao lc(qs);
//   lc.add_line(2, -1);                 // y = 2x - 1
//   lc.add_line(-3, 7);                 // y = -3x + 7
//   cout << lc.query(3) << '\n';        // 兩線在 x=3 的最大值