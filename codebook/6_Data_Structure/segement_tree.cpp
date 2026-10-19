// 區間 add / assign / sum，單次 O(log n)
// 只有單點修改時，才可移除 push 與 lazy
// 最外層呼叫時，每個 id 都傳 1
// 大值搭配 #define int long long；加總須不溢位

const int N = 200000 + 9;
int a[N];
int seg[4 * N];
int lazy[4 * N];
bool has_assign[4 * N];

inline void pull(int id) {
    seg[id] = seg[id * 2] + seg[id * 2 + 1];
}

// tag：有 assign 時為指定值，否則為待加量
// assign 覆蓋舊操作，add 累加並保留 assign 標記
inline void apply(int id, int l, int r,
                  int v, bool assign = false) {
    if (assign) {
        seg[id] = v * (r - l + 1);
        lazy[id] = v;
        has_assign[id] = true;
    } else {
        seg[id] += v * (r - l + 1);
        lazy[id] += v;
    }
}

inline void push(int id, int l, int r) {
    if (l == r || (!has_assign[id] && !lazy[id])) return;
    int mid = (l + r) / 2;
    apply(id * 2, l, mid, lazy[id], has_assign[id]);
    apply(id * 2 + 1, mid + 1, r,
          lazy[id], has_assign[id]);
    lazy[id] = 0;
    has_assign[id] = false;
}

// 編號為 id 的節點，存的區間為 [l, r]
void build(int id, int l, int r) {
    lazy[id] = 0; has_assign[id] = false; // 可重複 build
    // 葉節點的值
    if (l == r) { seg[id] = a[l]; return; }
    int mid = (l + r) / 2; // 將區間切成兩半
    build(id * 2, l, mid);                  // 左子節點
    build(id * 2 + 1, mid + 1, r);          // 右子節點
    pull(id);
}

// 區間查詢：回傳 [ql, qr] 的區間和
int query(int id, int l, int r, int ql, int qr) {
    if (r < ql || qr < l) return 0;         // 交集為空
    if (ql <= l && r <= qr) return seg[id]; // 完全覆蓋
    push(id, l, r); // 下傳 lazy
    int mid = (l + r) / 2;
    return query(id * 2, l, mid, ql, qr)    // 左
         + query(id * 2 + 1, mid + 1, r, ql, qr); // 右
    // 否則，往左、右進行遞迴
}

// 共用更新：assign=true 設值，false 加值
void range_update(int id, int l, int r,
                  int ql, int qr, int x, bool assign) {
    if (r < ql || qr < l) return; // 交集為空
    // 完全覆蓋
    if (ql <= l && r <= qr) {
        apply(id, l, r, x, assign);
        return;
    }
    push(id, l, r); // 下傳 lazy 再往下走
    int mid = (l + r) / 2;
    range_update(id * 2, l, mid, ql, qr, x, assign);
    range_update(id * 2 + 1, mid + 1, r,
                 ql, qr, x, assign);
    pull(id);
}

// 區間加值：將 [ql, qr] 每個位置都加上 x
void range_add(int id, int l, int r,
               int ql, int qr, int x) {
    range_update(id, l, r, ql, qr, x, false);
}
// 區間設值：將 [ql, qr] 每個位置都改成 x（含 0）
void range_assign(int id, int l, int r,
                  int ql, int qr, int x) {
    range_update(id, l, r, ql, qr, x, true);
}
// 單點修改 (設值版)：將 a[i] 改成 x
void modify(int id, int l, int r, int i, int x) {
    range_assign(id, l, r, i, i, x);
}

// 同一棵樹交錯使用（先令 a[1..4] = {1,2,3,4}）：
// build(1, 1, 4);
// range_add(1, 1, 4, 1, 3, 2);    // {3,4,5,4}
// range_assign(1, 1, 4, 2, 4, 0); // {3,0,0,0}
// range_add(1, 1, 4, 3, 4, -1);   // {3,0,-1,-1}
// query(1, 1, 4, 1, 4);          // 區間和 = 1

// ====================================================
// 區間加值 / 設值 + 區間最小值 + 最小值出現次數
// 以下替換主模板對應部分，不要整段接在後面
// 1. 宣告：
//    int seg[4 * N];
//    -> pair<int,int> seg[4 * N];
//    lazy / has_assign 宣告不變
// 2. 葉子：
//    build 的 seg[id] = a[l];
//    -> seg[id] = {a[l], 1};
//    modify 已共用 range_assign，不用改
// 3. pull / query 的合併：抽出 merge，
//    min/cnt 邏輯只寫一次（pull 直接呼叫）：
inline pair<int, int> merge(pair<int, int> L,
                            pair<int, int> R) {
    // cnt 為 0 代表空區間，不需 INF 哨兵
    if (!L.second) return R;
    if (!R.second) return L;
    // min 較小者整包保留
    if (L.first != R.first)
        return L.first < R.first ? L : R;
    // 相等時累加個數
    return {L.first, L.second + R.second};
}
inline void pull(int id) {
    seg[id] = merge(seg[id * 2], seg[id * 2 + 1]);
}
// 4. apply：加值保留 cnt；設值讓整段都成為最小值
inline void apply(int id, int l, int r,
                  int v, bool assign = false) {
    if (assign) {
        seg[id] = {v, r - l + 1};
        lazy[id] = v;
        has_assign[id] = true;
    } else {
        seg[id].first += v;
        lazy[id] += v;
    }
}
// query 骨架不變，只有回傳型別、
// 空區間值、最後的合併改用 merge：
pair<int, int> query(int id, int l, int r,
                     int ql, int qr) {
    if (r < ql || qr < l) return {0, 0};
    if (ql <= l && r <= qr) return seg[id];
    push(id, l, r);
    int mid = (l + r) / 2;
    return merge(query(id * 2, l, mid, ql, qr),
                 query(id * 2 + 1, mid + 1, r,
                       ql, qr));
}
// 5. push / 更新函數完全不用改（build 只改葉子值）
//    沿用上方 add / assign / add 範例，最後
//    query(1, 1, 4, 1, 4) = {-1, 2}。
