
//build
const int N = 100000 + 9;
int a[N];//葉
int seg[4 * N];
void bulid(int id, int l, int r) { // 編號為 id 的節點，存的區間為[l, r]
    if (l == r) {
        seg[id] = a[l]; // 葉節點的值
        return;
    }
    int mid = (l + r) / 2; // 將區間切成兩半
    build(id * 2, l, mid); // 左子節點
    build(id * 2 + 1, mid + 1, r); // 右子節點
    seg[id] = seg[id * 2] + seg[id * 2 + 1]
}

//區間查詢

int query(int id, int l, int r, int ql, int qr) {
    if (r < ql || qr < l) return 0;//若目前的區間與詢問的區間的交集為空的話，return 0
    if (ql <= l && r <= qr) return seg[id];//若目前的區間是詢問的區間的子集的話，則終止，並回傳當前節點的答案
    int mid = (l + r) / 2;
    return query(id * 2, l, mid, ql, qr) //左
        + query(id * 2 + 1, mid + 1, r, ql, qr);//右
		//否則，往左、右進行遞迴
}


//單點修改

void modify(int id, int l, int r, int i, int x) {
    if (l == r) {
        seg[id] = x; // 將a[i]改成x
        //seg[id] += x; // 將a[i]加上x
        return;
    }
    int mid = (l + r) / 2;
    // 根據修改的點在哪裡，來決定要往哪個子樹進行DFS
    if (i <= mid) modify(id * 2, l, mid, i, x);//左
    else modify(id * 2 + 1, mid + 1, r, i, x);//右
    seg[id] = seg[id * 2] + seg[id * 2 + 1];
}

