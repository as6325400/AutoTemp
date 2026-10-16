#define int long long
#define pii pair<int, int>

// 逐次最短增廣路；K 次增廣，最壞 O(KVE)
// 一條邊的費用為 單位花費 * 流過流量
// 把原本的 BFS 換成 SPFA 而已
// 記得在 main 裡面 resize graph；多測資用 assign 清空
// MCMF 回傳 {flow, cost}；會修改殘餘容量
// 無向圖：add_edge(u,v,C,W), add_edge(v,u,C,W);
// 容量非負；不可有源點可達的負環。
// 距離絕對值 < INF；容量、總費用不可溢位。
const int INF = LLONG_MAX / 4;

struct edge{
  int next, capacity, rev, cost;
  bool is_rev;
};
vector<vector<edge>> graph;

void add_edge(int a, int b, int capacity, int cost){
  int i = graph[a].size(), j = graph[b].size();
  graph[a].push_back({b, capacity, j + (a == b),
                      cost, false});
  graph[b].push_back({a, 0, i, -cost, true});
}

pii MCMF(int start, int end){
  if(start == end) return {0, 0};
  int n = graph.size(), flow = 0, cost = 0;
  vector<int> dis(n), parent(n), pe(n);
  vector<char> vis(n);
  while(true){
    fill(dis.begin(), dis.end(), INF);
    fill(vis.begin(), vis.end(), false);
    queue<int> q;
    dis[start] = 0, vis[start] = true, q.push(start);
    while(!q.empty()){
      int u = q.front(); q.pop(); vis[u] = false;
      for(int i = 0; i < (int)graph[u].size(); i++){
        auto &e = graph[u][i];
        if(e.capacity <= 0 ||
           dis[e.next] <= dis[u] + e.cost)
          continue;
        dis[e.next] = dis[u] + e.cost;
        parent[e.next] = u, pe[e.next] = i;
        if(!vis[e.next])
          vis[e.next] = true, q.push(e.next);
      }
    }
    if(dis[end] == INF) return {flow, cost};
    int add = LLONG_MAX;
    for(int v = end; v != start; v = parent[v])
      add = min(add, graph[parent[v]][pe[v]].capacity);
    for(int v = end; v != start; v = parent[v]){
      auto &e = graph[parent[v]][pe[v]];
      e.capacity -= add;
      graph[v][e.rev].capacity += add;
    }
    flow += add, cost += add * dis[end];
  }
}
