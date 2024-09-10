#define int long long
#define pii pair<int, int>

// Edmonds-Karp Algorithm Time: O(VE^2) 實際上會快一點
// 一條邊的費用為 單位花費 * 流過流量
// 把原本的 BFS 換成 SPFA 而已
// 記得在 main 裡面 resize graph
// MCMF 回傳 {flow, cost}

class edge{
  public:
    int next;
    int capacity;
    int rev;
    int cost;
    bool is_rev;
    edge(int _n, int _c, int _r, int _co, int _ir) : next(_n), capacity(_c), rev(_r), cost(_co), is_rev(_ir){};
};

vector<vector<edge>> graph;

void add_edge(int a, int b, int capacity, int cost){
  graph[a].push_back(edge(b, capacity, graph[b].size(), cost, false));
  graph[b].push_back(edge(a, 0, graph[a].size() - 1, -cost, true));
}

pii dfs(int now, int end, pii data, vector<pii> &path, int idx){
  auto [flow, cost] = data;
  if(now == end) return {flow, 0};
  auto &e = graph[now][path[idx + 1].second];
  if(e.capacity > 0){
    auto [ret, nc] = dfs(e.next, end, {min(flow, e.capacity), cost + e.cost}, path, idx + 1);
    if(ret > 0){
      e.capacity -= ret;
      graph[e.next][e.rev].capacity += ret;
      return {ret, nc + ret * e.cost};
    }
  }
  return {0, 0};
}

vector<pii> search_path(int start, int end){
  int n = graph.size() + 1;
  vector<int> dis(n + 1, 1e9);
  vector<bool> vis(n + 1, false);
  vector<pii> ans; queue<int> q;
  vis[start] = true; q.push(start); dis[start] = 0;
  vector<pii> parent(graph.size(), {-1, -1});
  q.push(start);
  while(!q.empty()){
    auto node = q.front(); vis[node] = false; q.pop();
    for(int i = 0; i < graph[node].size(); i++){
      auto &e = graph[node][i];
      if(e.capacity > 0 and e.cost + dis[node] < dis[e.next]){
        dis[e.next] = e.cost + dis[node];
        parent[e.next] = {node, i};
        if(!vis[e.next]){
          q.push(e.next);
          vis[e.next] = true;   
        }
      }
    }
  }
  if(parent[end].first == -1) return ans;
  int now = end;
  while(now != start){
    auto [node, idx] = parent[now];
    ans.emplace_back(node, idx);
    now = node;
  }
  ans.emplace_back(start, -1);
  reverse(ans.begin(), ans.end());
  return ans;
}

pii MCMF(int start, int end){
  int ans = 0, cost = 0;
  while(1){
    vector<bool> visited(graph.size() + 1, false);
    auto tmp = search_path(start, end);
    if(tmp.size() == 0) break;
    auto [flow, c] = dfs(start, end, {1e9, 0}, tmp, 0);
    ans += flow;
    cost += c;
  }
  return {ans, cost};
}