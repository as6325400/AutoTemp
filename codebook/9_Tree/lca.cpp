// n 為點數，graph 由父節點往子節點建有向邊
// graph 要 resize；1-base，n <= 200000。
// build_lca 可重建；用堆疊走訪，避免深鏈遞迴。
int n, q;
int fa[18][200001], dep[200001];
vector<vector<int>> graph;

void dfs(int root, int lst){
  fa[0][root] = lst;
  vector<int> st{root};
  while(!st.empty()){
    int u = st.back(); st.pop_back();
    for(int v: graph[u]){
      fa[0][v] = u, dep[v] = dep[u] + 1;
      st.push_back(v);
    }
  }
}

void build_lca(int root){
  dep[root] = 1;
  dfs(root, root);
  for(int i = 1; i < 18; i++)
    for(int j = 1; j <= n; j++)
      fa[i][j] = fa[i - 1][fa[i - 1][j]];
}

int lca(int a, int b){
  // 預設 a 比 b 淺，讓 a 和 b 跳到同一個地方
  if(dep[a] > dep[b]) swap(a, b);
  int step = dep[b] - dep[a];
  for(int i = 0; i < 18; i++)
    if(step >> i & 1) b = fa[i][b];
  if(a == b) return a;
  for(int i = 17; i >= 0; i--)
    if(fa[i][a] != fa[i][b])
      a = fa[i][a], b = fa[i][b];
  return fa[0][a];
}
