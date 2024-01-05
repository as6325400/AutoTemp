class tarjan{
    int time = 1;
    stack<int> s;
    vector<int> dfn;
    vector<int> low;
    vector<bool> in_stack;
    vector<vector<int>> ans;
    void dfs(int node, vector<vector<int>> &graph){
      in_stack[node] = true;
      s.push(node);
      dfn[node] = low[node] = time++;
      for(auto &j : graph[node]){
        if(dfn[j] == 0){
          dfs(j, graph);
          // 看看往下有沒有辦法回到更上面的點
          low[node] = min(low[node], low[j]); 
        }
        else if(in_stack[j]){
          low[node] = min(low[node], low[j]);
        }
      }
      vector<int> t; // 儲存這個強連通分量
      if(dfn[node] == low[node]){
        while(s.top() != node){
          t.push_back(s.top());
          in_stack[s.top()] = false;
          s.pop();
        }
        t.push_back(s.top());
        in_stack[s.top()] = false;
        s.pop();
      }
      if(!t.empty()) ans.push_back(t);
    }
  public:
    vector<vector<int>> scc(vector<vector<int>> &graph){
      int num = graph.size();
      dfn.resize(num, 0);
      low.resize(num, 0);
      in_stack.resize(num, false);
      for(int i = 1; i < num; i++){
        if(dfn[i] == 0) dfs(i, graph);
      }
      return ans;
    }
};
