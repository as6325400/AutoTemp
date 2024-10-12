/**
 * 背包問題：
 * 1. dp[i][j]: 考慮 1~i 個物品，重量為 j 時的最大價格
 * 2. dp[i][j]: 考慮 1~i 個物品，價值為 j 時的最小重量
 */

// 當重量比較輕時 O(nw)
vector<int> dp(sum + 1, 0);
for (int i = 1; i <= n; ++i) {
  for (int j = sum /* bound */; j >= weight[i]; --j) {
    if (dp[j] < dp[j - weight[i]] + price[i]) {
      dp[j] = dp[j - weight[i]] + price[i];
      backtrack[i][j] = 1;
    }
  }
}
// 當重量比較重時 O(nc)
vector<int> dp(sum + 1, 1e9 + 7);
dp[0] = 0;
for (int i = 1; i <= n; ++i) {
  for (int j = sum /* bound */; j >= price[i]; --j) {
    if (dp[j] > dp[j - price[i]] + weight[i]) {
      dp[j] = dp[j - price[i]] + weight[i];
      backtrack[i][j] = 1;
    }
  }
}

// backtrack: 找到當 bound 為 k 時，背包內有哪些東西
// 註：只找到其中一種
int l = n, r = k;
vector<int> ans;
while (l != 0 && r != 0) {
  if (backtrack[l][r]) {
    ans.push_back(l);
    r -= weight[l]; // 當用方法一時，用這行
    r -= price[l]; // 當用方法二時，用這行
  }
  l--;
}