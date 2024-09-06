// 通常用於對樹上任兩點間的路徑做加值、修改、查詢等工作
// 與線段樹相同，要修改 LCT 的功能只需更改
// pull、push、fix、query 等函數，再加上需要的懶標即可
// 範例為樹上任兩點 x, y 路徑上的權值 xor
// 和，樹上任意點單點改值
const int N = 300005;
class LinkCutTree {
private:
#define lc(x) node[x].ch[0]
#define rc(x) node[x].ch[1]
#define fa(x) node[x].fa
#define rev(x) node[x].rev
#define val(x) node[x].val
#define sum(x) node[x].sum
  struct Tree {
    int val, sum, fa, rev, ch[2];
  } node[N];
  inline void pull(int x) {
    sum(x) = val(x) ^ sum(lc(x)) ^ sum(rc(x));
  }
  inline void reverse(int x) {
    swap(lc(x), rc(x));
    rev(x) ^= 1;
  }
  inline void push(int x) {
    if (rev(x)) {
      reverse(lc(x));
      reverse(rc(x));
      rev(x) ^= 1;
    }
  }
  inline bool get(int x) { return rc(fa(x)) == x; }
  inline bool isroot(int x) {
    return (lc(fa(x)) ^ x) && (rc(fa(x)) ^ x);
  }
  inline void update(int x) {
    if (!isroot(x)) update(fa(x));
    push(x);
  }
  void rotate(int x) {
    int y = fa(x), z = fa(y), d = get(x);
    if (!isroot(y))
      node[z].ch[get(y)] = x; // 重要，不能更換順序
    fa(x) = z;
    node[fa(node[x].ch[d ^ 1]) = y].ch[d] =
      node[x].ch[d ^ 1];
    node[fa(y) = x].ch[d ^ 1] = y;
    pull(y), pull(x); // 先 y 再 x
  }
  void splay(int x) {
    update(x);
    for (int y = fa(x); !isroot(x);
         rotate(x), y = fa(x)) {
      if (!isroot(y)) rotate(get(x) == get(y) ? y : x);
    }
    pull(x);
  }
  int access(int x) {
    int p = 0;
    for (; x; x = fa(p = x)) {
      splay(x), rc(x) = p, pull(x);
    }
    return p;
  }
  inline void makeroot(int x) {
    access(x), splay(x), reverse(x);
  }
  inline int findroot(int x) {
    access(x), splay(x);
    while (lc(x)) { push(x), x = lc(x); }
    return splay(x), x;
  }
  inline void split(int x, int y) {
    makeroot(x), access(y), splay(y);
  }

public:
  inline void init(int len, int *data) {
    for (int i = 1; i <= len; ++i) {
      node[i].val = data[i];
    }
  }
  inline void link(int x, int y) { // 連邊
    makeroot(x);
    if (findroot(y) == x) return;
    fa(x) = y;
  }
  inline void cut(int x, int y) { // 斷邊
    makeroot(x);
    if (findroot(y) != x || fa(y) != x || lc(y))
      return;
    fa(y) = rc(x) = 0;
    pull(x);
  }
  inline void fix(int x, int v) { // 單點改值
    splay(x);
    val(x) = v;
  }
  // 區間查詢
  inline int query(int x, int y) {
    return split(x, y), sum(y);
  }
};

LinkCutTree LCT;
int n, a[N];

signed main() {
  int n, q, op, x, y;
  cin >> n >> q;
  for (int i = 1; i <= n; ++i) { cin >> a[i]; }
  LCT.init(n, a);
  while (q--) {
    cin >> op >> x >> y;
    if (op == 0) {
      cout << LCT.query(x, y) << endl;
    } else if (op == 1) {
      LCT.link(x, y);
    } else if (op == 2) {
      LCT.cut(x, y);
    } else {
      LCT.fix(x, y);
    }
  }
  return 0;
}
