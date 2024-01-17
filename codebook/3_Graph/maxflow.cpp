ll adj[505][505];
int parent[505];
bool vis[505];

bool check() {
  memset(vis, false, sizeof(vis));
  queue<int> q;
  q.push(1);
  while (!q.empty()) {
    int now = q.front();
    q.pop();
    for (int nxt = 1; nxt <= n; nxt++) {
      if (adj[now][nxt] && !vis[nxt]) {
        vis[nxt] = 1;
        parent[nxt] = now;
        q.push(nxt);
      }
    }
  }
  return vis[n];
}

void solve() {
  cin >> n >> m;
  for (int i = 0; i < m; i++) {
    ll a, b, w;
    cin >> a >> b >> w;
    adj[a][b] += w;
  }
  int u, v;
  ll maxflow = 0;
  while (check()) {
    ll flow = 1e18;
    for (v = n; v != 1; v = parent[v]) {
      u = parent[v];
      flow = min(flow, adj[u][v]);
    }
    maxflow += flow;
    for (int v = n; v != 1; v = parent[v]) {
      u = parent[v];
      adj[u][v] -= flow;
      adj[v][u] += flow;
    }
  }
  cout << maxflow << endl;
}