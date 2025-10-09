// time complexity: n * sqrt(q) * O(p)
// O(p) 為 add, remove 的時間複雜度
// 若知道 [l, r] 的答案 需要快速知道 [l - 1, r], [l + 1, r], [l, r - 1], [l, r + 1] 的答案

int n, q, k, l = 0, r = 0;

array queries = 詢問們;
type ans; //目前答案
void add(type v){/*...*/} //增加一個數字，算新答案
void remove(type v){/*...*/} //移除一個數字，算新答案

vector<tuple<int, int, int, int>> queries(q);
k = sqrt(n);
for(int i = 0; i < q; i++){
  int l, r;
  cin >> l >> r;
  queries[i] = {l / k, r, l, i};
  // 先對 l 的塊，再對 r 排序
}

sort(queries.begin(), queries.end());

add(a[0]);

for(int i = 0; i < q; i++){
  auto [_, rp, lp, id] = queries[i];
  lp--; rp--;
  while(l > lp) add(a[--l]);
  while(l < lp) remove(a[l++]); 
  while(r < rp) add(a[++r]);
  while(r > rp) remove(a[r--]);
  ans_v[id] = ans;
}
