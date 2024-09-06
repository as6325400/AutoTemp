int LCS(string t1, string t2) {
  if(t1.size() < t2.size()) swap(t1, t2);
  int len = t1.size();
  vector<vector<int>> dp(2, vector<int>(len + 1, 0));
  for(int j = 1; j <= t2.size(); j++){
    for(int i = 1; i <= len; i++){
      if(t2[j - 1] == t1[i - 1]) dp[j % 2][i] = dp[(j + 1) % 2][i - 1] + 1;
      else dp[j % 2][i] = max(dp[(j + 1) % 2][i], dp[j % 2][i - 1]);
    }
  }
  return dp[t2.size() % 2][t1.size()];
}