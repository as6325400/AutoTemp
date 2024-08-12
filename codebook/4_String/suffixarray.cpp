vector<int> SuffixArray(const string& s) {
  int n = s.size();
  vector<int> suffixArray(n), rank(n), tempRank(n);
  for (int i = 0; i < n; ++i) {
    suffixArray[i] = i;
    rank[i] = s[i];
  }
  for (int k = 1; k < n; k <<= 1) {
    auto compare = [&](int i, int j) {
      if (rank[i] != rank[j])
        return rank[i] < rank[j];
      int ri = (i + k < n) ? rank[i + k] : -1;
      int rj = (j + k < n) ? rank[j + k] : -1;
      return ri < rj;
    };
    sort(suffixArray.begin(), suffixArray.end(), compare);
    tempRank[suffixArray[0]] = 0;
    for (int i = 1; i < n; ++i) {
      tempRank[suffixArray[i]] = tempRank[suffixArray[i - 1]] + compare(suffixArray[i - 1], suffixArray[i]);
    }
    rank = tempRank;
  }
  return suffixArray;
}