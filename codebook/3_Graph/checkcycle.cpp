
vector<int> G[MAXN];
bool visit[MAXN];
/* return if the connected component where u is
    contains a cycle*/
bool dfs(int u, int pre) {
    if(visit[u])    return true;
    visit[u] = true;
    for(int v : G[u])
        if(v != pre && dfs(v, u)) 
            return true;
    return false;
}

//check if a graph contains a cycle

bool checkCycle(int n) {
    for(int i = 1; i <= n; i++)
        if(!visit[i] && dfs(i, -1))
            return true;
    return false; 
}