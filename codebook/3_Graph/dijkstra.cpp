// 傳入圖的 pair 為 {權重, 點}, 無限大預設 1e9 是情況改
#define pii pair<int, int>
vector<int> dijkstra(vector<vector<pii>> &graph, int src){
  int n = graph.size();
  vector<int> dis(n, 1e9);
  vector<bool> vis(n, false);
  priority_queue<pii, vector<pii>, greater<pii>> pq;
  pq.push({0, src});
  dis[src] = 0;
  while(!pq.empty()){
    auto [w, node] = pq.top();
    pq.pop();
    if(vis[node]) continue;
    vis[node] = true;
    for(auto [nw, nn]:graph[node]){
      if(w + nw < dis[nn]){
        dis[nn] = w + nw;
        pq.push({dis[nn], nn});
      }
    }
  } 
  return dis;
}