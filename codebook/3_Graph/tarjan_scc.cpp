class tarjan{
  // 1-base；遞迴 DFS，深鏈須注意堆疊大小。
  int timer, id;
  stack<int> s;
  vector<int> low, dfn;
  vector<char> in_stack;
  void dfs(int u, vector<vector<int>> &graph){
    in_stack[u] = true, s.push(u);
    dfn[u] = low[u] = timer++;
    for(int v: graph[u]){
      if(!dfn[v]){
        dfs(v, graph);
        // 看看往下有沒有辦法回到更上面的點
        low[u] = min(low[u], low[v]);
      }
      else if(in_stack[v]) low[u] = min(low[u], dfn[v]);
    }
    if(low[u] != dfn[u]) return;
    vector<int> t; // 儲存這個強連通分量
    int v;
    do{
      v = s.top(), s.pop();
      in_stack[v] = false, scc_id[v] = id;
      t.push_back(v);
    }while(v != u);
    ++id, ans.push_back(t);
  }
  public:
    vector<int> scc_id;
    vector<vector<int>> ans;
    // ans ans[i] 代表第 i 個強連通分量裡面包涵的點
    // scc_id[i] 代表第 i 個點屬於第幾個強連通分量
    // 編號由 1 開始，較小者先被彈出；可重複呼叫。
    vector<vector<int>> scc(vector<vector<int>> &graph){
      int n = graph.size();
      timer = id = 1, s = {}, ans.clear();
      scc_id.assign(n, -1), dfn.assign(n, 0);
      low.assign(n, 0), in_stack.assign(n, false);
      for(int i = 1; i < n; i++)
        if(!dfn[i]) dfs(i, graph);
      return ans;
    }
};
