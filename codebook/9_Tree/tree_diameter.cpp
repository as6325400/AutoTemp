int dis[200005];
int c;

void dfs(int now, int fa, vector<vector<int>> &graph){
  for(auto i: graph[now]){
    if(i != fa){
      dis[i] = dis[now] + 1;
      if(dis[i] > dis[c]) c = i;
      dfs(i, now, graph);
    }
  }
}

signed main()
{
  int n;
  cin >> n;
  vector<vector<int>> graph(n + 1);
  for(int i = 0; i < n - 1; i++){
    int a, b;
    cin >> a >> b;
    graph[a].push_back(b);
    graph[b].push_back(a);
  }
  dfs(1, 0, graph);
  dis[c] = 0;
  dfs(c, 0, graph);
  cout << dis[c];
}
