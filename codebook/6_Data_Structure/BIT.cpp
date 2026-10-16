#define lowbit(x) ((x) & -(x))
// 1-based，bit 開 n + 1 格；modify 的 idx 須 >= 1。
void modify(vector<int>& bit, int idx, int val) {
  for (; idx < (int)bit.size(); idx += lowbit(idx))
    bit[idx] += val;
}

int query(const vector<int>& bit, int idx) {
  int ans = 0;
  for (; idx > 0; idx -= lowbit(idx)) ans += bit[idx];
  return ans;
}

// the first i s.t. a[1]+...+a[i] >= k
// 須 a[i] >= 0、k >= 1；若總和不足，回傳 n + 1。
int findK(const vector<int>& bit, int k) {
  int n = (int)bit.size() - 1, idx = 0;
  for (int step = n ? (1LL << __lg(n)) : 0;
       step; step >>= 1) {
    int next = idx + step;
    if (next <= n && bit[next] < k)
      idx = next, k -= bit[next];
  }
  return idx + 1;
}

//O(n)建bit
// a 也是 1-based，第 0 格不使用。
vector<int> buildBIT(const vector<int>& a) {
  auto bit = a;
  for (int i = 1; i < (int)bit.size(); ++i) {
    int j = i + lowbit(i);
    if (j < (int)bit.size()) bit[j] += bit[i];
  }
  return bit;
}
