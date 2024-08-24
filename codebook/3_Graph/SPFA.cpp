#define pii pair<int, int>
// {在 src 可到達的點中是否存在負環, 最短路徑}, arg 中 n 為點的數量
// arg 中 pair 裡的第一個值為權重, 第二個為點
pair<bool, vector<int>> SPFA(vector<vector<pii>> &graph, int n, int src){
  vector<int> dis(n + 1, 1e9);
  vector<int> cnt(n + 1, 0);
  vector<bool> vis(n + 1, false);
  queue<int> q;
  vis[src] = true; q.push(src); dis[src] = 0;
  while(!q.empty()){
    auto node = q.front(); vis[node] = false; q.pop();
    for(auto [w, nn]:graph[node]){
      if(w + dis[node] < dis[nn]){
        dis[nn] = w + dis[node];
        if(!vis[nn]){
          if(++cnt[nn] >= n) return {true, {}};
          q.push(nn);
          vis[nn] = true;          
        }
      }
    }
  }
  return {false, dis};
}