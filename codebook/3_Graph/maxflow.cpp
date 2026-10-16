#define int long long

// dicnic Algorithm Time: O(V^2E) 實際上會快一點
// 記得在 main 裡面 resize graph
// 最小割，找到最少條的邊切除，使得從 src 到 end 的 maxflow 為 0
// 枚舉所有邊 i -> j，src 可以到達 i 但無法到達 j ，那這條邊為最小割裡的邊之一
// 無向圖最大流：修改 add_edge，反向邊建為 capacity
// 使用時只要 add_edge 一次
// 「最少條」限等正容量；一般為容量總和最小。
// 多測資用 graph.assign(n, {})；容量非負，總流量不可溢位。
// maxflow 回傳本次新增流量；start == end 回傳 0。
struct edge{
  int next, origin, capacity, rev;
  bool is_rev;
  edge(int n, int c, int r, bool ir)
    : next(n), origin(c), capacity(c), rev(r), is_rev(ir) {}
};
vector<vector<edge>> graph;
vector<int> level, iter;

void add_edge(int a, int b, int capacity){
  int i = graph[a].size(), j = graph[b].size();
  graph[a].emplace_back(b, capacity, j + (a == b), false);
  graph[b].emplace_back(a, 0, i, true);
}

void bfs(int start){
  fill(level.begin(), level.end(), -1);
  queue<int> q;
  level[start] = 0, q.push(start);
  while(!q.empty()){
    int v = q.front(); q.pop();
    for(auto &e: graph[v])
      if(e.capacity > 0 && level[e.next] < 0)
        level[e.next] = level[v] + 1, q.push(e.next);
  }
}

int dfs(int v, int end, int flow){
  if(v == end) return flow;
  for(int &i = iter[v]; i < (int)graph[v].size(); i++){
    auto &e = graph[v][i];
    if(e.capacity <= 0 || level[e.next] != level[v] + 1)
      continue;
    int d = dfs(e.next, end, min(flow, e.capacity));
    if(!d) continue;
    e.capacity -= d, graph[e.next][e.rev].capacity += d;
    return d;
  }
  return 0;
}

int maxflow(int start, int end){
  if(start == end) return 0;
  int flow = 0, f;
  level.resize(graph.size());
  while(true){
    bfs(start);
    if(level[end] < 0) return flow;
    iter.assign(graph.size(), 0);
    while((f = dfs(start, end, LLONG_MAX))) flow += f;
  }
}

// 回朔 max_flow 的 path
// 用法：graph.assign(n, {}); add_edge(...);
// int f = maxflow(s, t); auto paths = get_flow_paths(s, t);
// 每項 {path, flow}；會消耗同一組 s,t 已送的流量。
// 限上述有向邊模型的 maxflow 結果，不任意更換源匯。
// 分解最壞 O(VE)，保留不連通環流；Dinic DFS 有深度限制。
struct flow_path{ vector<int> path; int flow; };
vector<flow_path> get_flow_paths(int start, int end){
  vector<flow_path> result;
  if(start == end) return result;
  vector<int> ptr(graph.size()), pos(graph.size(), -1);
  vector<int> path{start}, edges;
  pos[start] = 0;
  // 原始邊目前剩下多少尚未分解的流量
  auto used = [&](int u, int i){
    return graph[u][i].origin - graph[u][i].capacity;
  };
  // 保留 path[0..last]
  auto trim = [&](int last){
    while((int)path.size() > last + 1){
      pos[path.back()] = -1; path.pop_back();
    }
    edges.resize(last);
  };
  while(true){
    int v = path.back(), k = 0;
    if(v != end){
      int &i = ptr[v];
      while(i < (int)graph[v].size() &&
            (graph[v][i].is_rev || used(v, i) <= 0)) i++;
      // 流量守恆：走進中間點後，一定有正流量出邊
      if(i == (int)graph[v].size()){
        assert(v == start); break;
      }
      int u = graph[v][i].next;
      edges.push_back(i);
      if(pos[u] == -1){
        pos[u] = path.size(); path.push_back(u);
        continue;
      }
      // 發現環：path[k] -> ... -> v -> path[k]
      k = pos[u]; // edges 已含最後一條回到入口的邊
      // 消去環流，不影響 s -> t 的總流量
    }
    // 路徑取全部邊，環只取 k 起的邊；共用扣流流程。
    int f = LLONG_MAX, cut = edges.size();
    for(int j = k; j < (int)edges.size(); j++)
      f = min(f, used(path[j], edges[j]));
    if(v == end) result.push_back({path, f});
    // 找到第一條流量歸零的邊
    for(int j = k; j < (int)edges.size(); j++){
      auto &e = graph[path[j]][edges[j]];
      // 消耗這條邊的 f 單位流量
      e.capacity += f, graph[e.next][e.rev].capacity -= f;
      if(!used(path[j], edges[j])) cut = min(cut, j);
    }
    // 保留仍有流量的前綴，不必從起點重走
    trim(v == end ? cut : k);
  }
  return result;
}
