int LIS(vector<int>& save) {
  vector<int> dp;
  int n = save.size();
  for (int i = 0; i < n; i++) {
    auto it = lower_bound(dp.begin(),dp.end(),save[i]);
    if(it == dp.end()) dp.push_back(save[i]);
    else *it = save[i];
  }
  return dp.size();
}