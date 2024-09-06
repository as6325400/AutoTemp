class dsu{
  public:
    vector<int> parent;
    dsu(int num){
      parent.resize(num);
      for(int i = 0; i < num; i++) parent[i] = i;
    }
    int find(int x){
      if(parent[x] == x) return x;
      return parent[x] = find(parent[x]);
    }
    bool same(int a, int b){
      return find(a) == find(b);
    }
    void Union(int a, int b){
      parent[find(a)] = find(b);
    }
};