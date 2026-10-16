// 0-base；依大小合併 + 路徑壓縮，均攤 O(alpha(n))
// 代表點由合併順序決定，不保證是 a 或 b。
class dsu{
  public:
    vector<int> parent, siz;
    dsu(int num): parent(num), siz(num, 1){
      iota(parent.begin(), parent.end(), 0);
    }
    int find(int x){
      while(parent[x] != x){
        parent[x] = parent[parent[x]];
        x = parent[x];
      }
      return x;
    }
    bool same(int a, int b){
      return find(a) == find(b);
    }
    void Union(int a, int b){
      a = find(a), b = find(b);
      if(a == b) return;
      if(siz[a] < siz[b]) swap(a, b);
      parent[b] = a, siz[a] += siz[b];
    }
};
