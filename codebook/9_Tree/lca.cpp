// n 為點數，graph 由子節點往父節點建有向邊
// graph 要 resize

int n, q;
int fa[20][200001];
int dep[200001];

vector<vector<int>> graph;

void dfs(int now, int lst){
  fa[0][now] = lst;
  for(int &i:graph[now]){
    dep[i] = dep[now] + 1;
    dfs(i, now);
  }
}

void build_lca(int root){
  dep[root] = 1;
  dfs(root, root);
  for(int i = 1; i < 18; i++){
    for(int j = 1; j < n + 1; j++){
      fa[i][j] = fa[i - 1][fa[i - 1][j]];
    }
  }
}

int lca(int a, int b){
  // 預設a比b淺
  if(dep[a] > dep[b]) return lca(b, a);
  // 讓a和b跳到同一個地方
  int step = dep[b] - dep[a];
  for (int i = 0; i < 18; i++)
  {
    if(step >> i & 1){
      b = fa[i][b];
    }
  }
  if(a == b) return a;
  for(int i = 17; i >= 0; i--){
    if(fa[i][a] != fa[i][b]){
      a = fa[i][a];
      b = fa[i][b];
    }
  }
  return fa[0][a];
}
