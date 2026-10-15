#include "../../support/test.hpp"
using namespace std;
using cbtest::require;
using cbtest::compare_speed;

// 以一般 int 與競賽常用的 int 巨集各跑一遍。
#ifdef TEST_LONG_LONG_INT
#define int long long
#endif
namespace original {
#include BASELINE_HEADER
}
#include CANDIDATE_HEADER

using Matrix = vector<vector<ll>>;
using Graph = vector<vector<int>>;
static mt19937_64 rng(20261016);
static size_t matrix_cases = 0, graph_cases = 0;

void check_assignment(const Matrix& a, ll cost, const vector<int>& ans) {
  require(ans.size() == a.size(), "answer length");
  if (a.empty()) {
    require(cost == 0, "empty matrix cost");
    return;
  }
  vector<bool> used(a[0].size());
  ll sum = 0;
  for (int i = 0; i < (int)a.size(); ++i) {
    require(ans[i] >= 0 && ans[i] < (int)a[0].size(), "answer column");
    require(!used[ans[i]], "answer reuses a column");
    used[ans[i]] = true;
    sum += a[i][ans[i]];
  }
  require(sum == cost, "answer cost");
}

ll brute_assignment(const Matrix& a, int row = 0, unsigned used = 0) {
  if (row == (int)a.size()) return 0;
  ll best = numeric_limits<ll>::max();
  for (int j = 0; j < (int)a[row].size(); ++j)
    if (!(used & (1u << j)))
      best = min(best, a[row][j] + brute_assignment(a, row + 1, used | (1u << j)));
  return best;
}

void check_matrix(const Matrix& a) {
  vector<int> ans(3, 99), old_ans(3, 99);
  ll cost = km(a, ans), old_cost = original::km(a, old_ans);
  require(cost == brute_assignment(a), "weighted optimum");
  require(cost == old_cost, "weighted baseline mismatch");
  check_assignment(a, cost, ans);
  check_assignment(a, old_cost, old_ans);
  ++matrix_cases;
}

// 獨立建立無向鄰接矩陣，每筆鄰接表紀錄都視為一條邊。
vector<vector<bool>> adjacency(const Graph& graph) {
  vector<vector<bool>> edge(graph.size(), vector<bool>(graph.size()));
  for (int u = 0; u < (int)graph.size(); ++u)
    for (int v : graph[u]) edge[u][v] = edge[v][u] = true;
  return edge;
}

int reference_matching(const vector<vector<bool>>& edge, const vector<int>& color) {
  int n = color.size(), matches = 0;
  vector<int> mate(n, -1);
  function<bool(int, vector<bool>&)> augment = [&](int u, vector<bool>& seen) {
    for (int v = 0; v < n; ++v) {
      if (!edge[u][v] || seen[v]) continue;
      seen[v] = true;
      if (mate[v] == -1 || augment(mate[v], seen)) {
        mate[v] = u;
        return true;
      }
    }
    return false;
  };
  for (int u = 0; u < n; ++u) if (color[u] == 0) {
    vector<bool> seen(n);
    matches += augment(u, seen);
  }
  return matches;
}

int brute_independent_set(const vector<vector<bool>>& edge) {
  int n = edge.size(), best = 0;
  vector<unsigned> neighbors(n);
  for (int u = 0; u < n; ++u)
    for (int v = 0; v < n; ++v)
      if (edge[u][v]) neighbors[u] |= 1u << v;
  for (unsigned mask = 0; mask < (1u << n); ++mask) {
    bool independent = true;
    for (int u = 0; u < n && independent; ++u)
      if ((mask >> u & 1u) && (neighbors[u] & mask)) independent = false;
    if (independent) best = max(best, (int)__builtin_popcount(mask));
  }
  return best;
}

void check_graph(const Graph& graph, const vector<int>& color) {
  auto edge = adjacency(graph);
  int zeros = count(color.begin(), color.end(), 0);
  int row_color = zeros <= (int)color.size() - zeros ? 0 : 1;
  vector<int> rows, columns;
  for (int u = 0; u < (int)color.size(); ++u)
    (color[u] == row_color ? rows : columns).push_back(u);
  Matrix expected(rows.size(), vector<ll>(columns.size()));
  for (int i = 0; i < (int)rows.size(); ++i)
    for (int j = 0; j < (int)columns.size(); ++j)
      expected[i][j] = edge[rows[i]][columns[j]] ? -1 : 0;
  Matrix a = km_matrix(graph, color);
  require(a == expected, "matrix conversion / stable vertex ordering");
  vector<int> ans;
  ll cost = km(a, ans);
  check_assignment(a, cost, ans);
  require(-cost == reference_matching(edge, color), "graph maximum matching");
  require((ll)color.size() + cost == brute_independent_set(edge), "graph independent set");
  ++graph_cases;
}

// 0：雙向；1/2：單向；3：混合方向；4：混合且重複。
Graph store_edges(int n, const vector<pair<int, int>>& edges, int mode) {
  Graph graph(n);
  for (auto [u, v] : edges) {
    if (mode >= 3 && (rng() & 1)) swap(u, v);
    if (mode != 2) graph[u].push_back(v);
    if (mode == 0 || mode == 2) graph[v].push_back(u);
    if (mode == 4) {
      graph[u].push_back(v);
      graph[u].push_back(v);
    }
  }
  return graph;
}

void correctness() {
  rng.seed(20261016);
  check_matrix({});
  check_matrix({{-1000000000000LL, 1000000000000LL}});
  check_matrix({{0, 0}, {0, 0}});
  check_matrix({{-10, -2, 5}, {-11, 7, -1}});
  for (int n = 1; n <= 5; ++n)
    for (int m = n; m <= 7; ++m)
      for (int trial = 0; trial < 35; ++trial) {
        Matrix a(n, vector<ll>(m));
        for (auto& row : a) for (ll& x : row)
          x = trial % 7 == 0 ? 0 : (ll)(rng() % 81) - 40;
        if (trial % 11 == 0)
          for (auto& row : a) for (ll& x : row) x *= 1000000000LL;
        check_matrix(a);
      }

  // 枚舉至 3 x 3 的所有圖，涵蓋存邊方式與顏色翻轉。
  for (int left = 0; left <= 3; ++left)
    for (int right = 0; right <= 3; ++right) {
      vector<int> color(left + right, 1);
      fill(color.begin(), color.begin() + left, 0);
      for (unsigned mask = 0; mask < (1u << (left * right)); ++mask) {
        vector<pair<int, int>> edges;
        for (int u = 0; u < left; ++u)
          for (int v = 0; v < right; ++v)
            if (mask >> (u * right + v) & 1u) edges.emplace_back(u, left + v);
        for (int mode = 0; mode < 5; ++mode) {
          auto graph = store_edges(left + right, edges, mode);
          check_graph(graph, color);
          vector<int> flipped = color;
          for (int& x : flipped) x ^= 1;
          check_graph(graph, flipped);
        }
      }
    }
  for (int trial = 0; trial < 600; ++trial) {
    int n = rng() % 13;
    vector<int> color(n);
    for (int& x : color) x = rng() & 1;
    vector<pair<int, int>> edges;
    int density = trial % 5 == 0 ? 100 : trial % 101;
    for (int u = 0; u < n; ++u)
      for (int v = u + 1; v < n; ++v)
        if (color[u] != color[v] && (int)(rng() % 100) < density)
          edges.emplace_back(u, v);
    check_graph(store_edges(n, edges, trial % 5), color);
  }
  cout << "PASS index_bits=" << sizeof(int) * 8 << " weighted=" << matrix_cases
    << " graph=" << graph_cases << " seed=20261016\n";
}

// 原本的 map 轉換，限雙向鄰接表；用於比較建矩陣速度。
Matrix old_map_matrix(const Graph& graph, const vector<int>& color) {
  int n = graph.size(), cnt = accumulate(color.begin(), color.end(), (int)0);
  int nn = n - cnt, mm = cnt, flag = 0;
  if (nn > mm) swap(nn, mm), flag = 1;
  Matrix a(nn, vector<ll>(mm));
  int tmp = 0;
  map<int, int> mp;
  for (int i = 0; i < n; ++i)
    if (color[i] != flag) mp[i] = tmp++;
  int ct = -1;
  for (int i = 0; i < n; ++i) if (color[i] == flag) {
    ++ct;
    for (int node : graph[i]) a[ct][mp[node]] = -1;
  }
  return a;
}

void benchmark() {
  rng.seed(20261016);
  cout << "BENCH index_bits=" << sizeof(int) * 8
    << " seed=20261016 rounds=7 batches=3 statistic=median_ms_per_call\n";
  struct Case { const char* name; int n, m, density; };
  for (const auto& c : vector<Case>{{"km weighted 320x320", 320, 320, -1},
                  {"km weighted 160x480", 160, 480, -1},
                  {"km sparse -1/0 300x450", 300, 450, 2},
                  {"km dense -1/0 300x450", 300, 450, 70}}) {
    Matrix a(c.n, vector<ll>(c.m));
    for (auto& row : a) for (ll& x : row)
      x = c.density == -1 ? (ll)(rng() % 2000001) - 1000000
               : ((int)(rng() % 100) < c.density ? -1 : 0);
    vector<int> old_ans, new_ans;
    ll old_cost = original::km(a, old_ans), new_cost = km(a, new_ans);
    require(old_cost == new_cost, "benchmark KM result mismatch");
    check_assignment(a, old_cost, old_ans);
    check_assignment(a, new_cost, new_ans);
    compare_speed(c.name, [&] { return original::km(a, old_ans); },
               [&] { return km(a, new_ans); });
  }
  for (const auto& c : vector<Case>{{"convert sparse 400x1600", 400, 1600, 1},
                  {"convert dense 500x700", 500, 700, 45}}) {
    vector<int> color(c.n + c.m, 1);
    fill(color.begin(), color.begin() + c.n, 0);
    vector<pair<int, int>> edges;
    for (int u = 0; u < c.n; ++u)
      for (int v = 0; v < c.m; ++v)
        if ((int)(rng() % 100) < c.density) edges.emplace_back(u, c.n + v);
    Graph graph = store_edges(c.n + c.m, edges, 0);
    require(old_map_matrix(graph, color) == km_matrix(graph, color), "benchmark conversion mismatch");
    compare_speed(c.name,
      [&] { auto a = old_map_matrix(graph, color); return a.size() + a[0].size() + a[0][0]; },
      [&] { auto a = km_matrix(graph, color); return a.size() + a[0].size() + a[0][0]; });
  }
}

signed main(signed argc, char** argv) {
  try {
    string mode = argc > 1 ? argv[1] : "--check";
    if (mode == "--check") correctness();
    else if (mode == "--bench") benchmark();
    else if (mode == "--bad-shape") {
      vector<int> ans;
      km({{1}, {2}}, ans);
    }
    else throw runtime_error("unknown mode: " + mode);
  } catch (const exception& e) {
    cerr << "FAIL: " << e.what() << '\n';
    return 1;
  }
}
