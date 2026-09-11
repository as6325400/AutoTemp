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
    int origin;
    int capacity;
    int rev;
    bool is_rev;
    edge(int _n, int _c, int _r, int _ir) : next(_n), origin(_c), capacity(_c), rev(_r), is_rev(_ir){};
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
    while ((f = dfs(start, end, LLONG_MAX)) > 0) {
      flow += f;
    }
  }
}

// 回朔 max_flow 的 path

class flow_path{ 
  public: vector<int> path; 
  int flow; 
};

vector<flow_path> get_flow_paths(int start, int end) {
    vector<flow_path> result;
    if (start == end) return result;
    vector<int> ptr(graph.size(), 0);
    vector<int> pos(graph.size(), -1);
    vector<int> path{start}, edges;
    pos[start] = 0;
    // 原始邊目前剩下多少尚未分解的流量
    auto used = [&](int u, int i) {
        return graph[u][i].origin - graph[u][i].capacity;
    };
    // 消耗這條邊的 f 單位流量
    auto take = [&](int u, int i, int f) {
        edge &e = graph[u][i];
        e.capacity += f;
        graph[e.next][e.rev].capacity -= f;
    };
    // 保留 path[0..last]
    auto trim = [&](int last) {
        while ((int)path.size() > last + 1) {
            pos[path.back()] = -1;
            path.pop_back();
        }
        edges.resize(last);
    };
    while (true) {
        int v = path.back();
        if (v == end) {
            int f = LLONG_MAX;
            for (int j = 0; j < (int)edges.size(); j++)
                f = min(f, used(path[j], edges[j]));
            result.push_back({path, f});
            // 找到第一條流量歸零的邊
            int cut = edges.size();
            for (int j = 0; j < (int)edges.size(); j++) {
                take(path[j], edges[j], f);
                if (used(path[j], edges[j]) == 0)
                    cut = min(cut, j);
            }
            // 保留仍有流量的前綴，不必從起點重走
            trim(cut);
            continue;
        }
        int &i = ptr[v];
        while (i < (int)graph[v].size() &&
               (graph[v][i].is_rev || used(v, i) <= 0)) {
            i++;
        }
        // 流量守恆：走進中間點後，一定有正流量出邊
        if (i == (int)graph[v].size()) {
            assert(v == start);
            break;
        }
        int u = graph[v][i].next;
        if (pos[u] == -1) {
            edges.push_back(i);
            pos[u] = path.size();
            path.push_back(u);
        } else {
            // 發現環：path[k] -> ... -> v -> path[k]
            int k = pos[u];
            int f = used(v, i);
            for (int j = k; j < (int)edges.size(); j++)
                f = min(f, used(path[j], edges[j]));
            // 消去環流，不影響 s -> t 的總流量
            for (int j = k; j < (int)edges.size(); j++)
                take(path[j], edges[j], f);
            take(v, i, f);
            trim(k);
        }
    }
    return result;
}