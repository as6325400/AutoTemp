class Sparse_Table{
  // 0-base
  // 要改成找最大把min換成max就好
  private:
  public:
    int spt[500005][22][2];
    Sparse_Table(vector<int> &ar){
      int n = ar.size();
      for (int i = 0; i < n; i++){
          spt[i][0][0] = ar[i];
          // spt[i][0][1] = ar[i];
      }
      for (int j = 1; (1 << j) <= n; j++) {
        for (int i = 0; (i + (1 << j) - 1) < n; i++) {
          spt[i][j][0] = min(spt[i + (1 << (j - 1))][j - 1][0], spt[i][j - 1][0]);
          // spt[i][j][1] = max(spt[i + (1 << (j - 1))][j - 1][1], spt[i][j - 1][1]);
        }
      }
    }
    int query_min(int l, int r)
    {
      if(l>r) return INT_MAX;
      int j = (int)__lg(r - l + 1);
      ///j = 31 - __builtin_clz(r - l+1);
      return min(spt[l][j][0], spt[r - (1 << j) + 1][j][0]);
    }
    int query_max(int l, int r)
    {
      if(l>r) return INT_MAX;
      int j = (int)__lg(r - l + 1);
      ///j = 31 - __builtin_clz(r - l+1);
      return max(spt[l][j][1], spt[r - (1 << j) + 1][j][1]);
    }
};