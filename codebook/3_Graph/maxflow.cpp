#define int long long

// dicnic Algorithm Time: O(V^2E) 實際上會快一點
// 記得在 main 裡面 resize graph
// 最小割，找到最少條的邊切除，使得從 src 到 end 的 maxflow 為 0
// 枚舉所有邊 i -> j，src 可以到達 i 但無法到達 j ，那這條邊為最小割裡的邊之一
// 無向圖最大流：修改 add_edge，反向邊建為 capacity
    // 使用時只要 add_edge 一次

class edge{
  public:
    int next;
    int capacity;
    int rev;
    bool is_rev;
    edge(int _n, int _c, int _r, int _ir) : next(_n), capacity(_c), rev(_r), is_rev(_ir){};
};

vector<vector<edge>> graph;
vector<int> level, iter;

void add_edge(int a, int b, int capacity){
  graph[a].push_back(edge(b, capacity, graph[b].size(), false));
  graph[b].push_back(edge(a, 0, graph[a].size() - 1, true));
}

void bfs(int start) {
  fill(level.begin(), level.end(), -1);
  queue<int> q;
  level[start] = 0;
  q.push(start);
  while (!q.empty()) {
    int v = q.front();
    q.pop();
    for (auto& e : graph[v]) {
      if (e.capacity > 0 && level[e.next] < 0) {
        level[e.next] = level[v] + 1;
        q.push(e.next);
      }
    }
  }
}

int dfs(int v, int end, int flow) {
  if (v == end) return flow;
  for (int &i = iter[v]; i < graph[v].size(); i++) {
    edge &e = graph[v][i];
    if (e.capacity > 0 && level[v] < level[e.next]) {
      int d = dfs(e.next, end, min(flow, e.capacity));
      if (d > 0) {
        e.capacity -= d;
        graph[e.next][e.rev].capacity += d;
        return d;
      }
    }
  }
  return 0;
}

int maxflow(int start, int end) {
  int flow = 0;
  level.resize(graph.size() + 1);
  while (true) {
    bfs(start);
    if (level[end] < 0) return flow;
    iter.assign(graph.size() + 1, 0);
    int f;
    while ((f = dfs(start, end, 1e9)) > 0) {
      flow += f;
    }
  }
}
