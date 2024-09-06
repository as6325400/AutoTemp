// 1. 無向圖是歐拉圖：
// 非零度頂點是連通的
// 頂點的度數都是偶數

// 2. 無向圖是半歐拉圖(有路沒有環)：
// 非零度頂點是連通的
// 恰有 2 個奇度頂點

// 3. 有向圖是歐拉圖：
// 非零度頂點是強連通的
// 每個頂點的入度和出度相等

// 4. 有向圖是半歐拉圖(有路沒有環)：
// 非零度頂點是弱連通的
// 至多一個頂點的出度與入度之差為 1
// 至多一個頂點的入度與出度之差為 1
// 其他頂點的入度和出度相等
vector<set<int>> adj;
vector<int> ans;

void dfs(int x) { // Hierholzer's Algorithm
  while (!adj[x].empty()) {
    auto next = *(adj[x].begin());
    adj[x].erase(next);
    adj[next].erase(x);
    dfs(next);
  }
  ans.emplace_back(x);
}

void solve() {
  // 建立雙向邊，set用來防重邊，點數n，邊數m
  for (int i = 1; i <= n; i++)
    if (adj[i].size() & 1) return; /* impossible */
  dfs(1);
  if (ans.size() != m + 1) return; /* impossible */
  reverse(ans.begin(), ans.end()); /* then print it */
}
