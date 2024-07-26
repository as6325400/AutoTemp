#define int long long

// Edmonds-Karp Algorithm

class edge{
  public:
    int next;
    int capicity;
    int rev;
    edge(int _n, int _c, int _r) : next(_n), capicity(_c), rev(_r){};
};

vector<vector<edge>> graph;

void add_edge(int a, int b, int capacity){
  graph[a].push_back(edge(b, capacity, graph[b].size()));
  graph[b].push_back(edge(a, 0, graph[a].size() - 1));
}

int dfs(int now, int end, int flow, vector<pair<int, int>> &path, int idx){
  if(now == end) return flow;
  auto &e = graph[now][path[idx + 1].second];
  if(e.capicity > 0){
    auto ret = dfs(e.next, end, min(flow, e.capicity), path, idx + 1);
    if(ret > 0){
      e.capicity -= ret;
      graph[e.next][e.rev].capicity += ret;
      return ret;
    }
  }
  return 0;
}

vector<pair<int, int>> search_path(int start, int end){
  vector<pair<int, int>> ans;
  queue<int> q;
  vector<pair<int, int>> parent(graph.size(), {-1, -1});
  q.push(start);
  while(!q.empty()){
    int now = q.front();
    q.pop();
    for(int i = 0; i < (int)graph[now].size(); i++){
      auto &e = graph[now][i];
      if(e.capicity > 0 and parent[e.next].first == -1){
        parent[e.next] = {now, i};
        if(e.next == end) break;
        q.push(e.next);
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

int maxflow(int start, int end, int node_num){
  int ans = 0;
  while(1){
    vector<bool> visited(node_num + 1, false);
    auto tmp = search_path(start, end);
    if(tmp.size() == 0) break;
    auto flow = dfs(start, end, 1e9, tmp, 0);
    ans += flow;
  }
  return ans;
}