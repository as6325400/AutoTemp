// O(n^2)
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

// O(nlogn)
// 這裡string 要以 1 base index 所以開頭要補個字元
// d:記住此數字的前一個數字, t:當前LIS位置, num:根據t2生成出string來找LIS長度
// N: 最大字串長度
#define N 120
int t[N*N], d[N*N], num[N*N];
map<char, vector<int>> dict; // 每個字串出現的index位置
int binarySearch(int l, int r, int v){
    int m;
    while(r>l){
        m = (l+r)/2;
        if(num[v] > num[t[m]])l = m+1;
        else if(num[v] < num[t[m]])r = m;
        else return m;
    }
    return r;
}
int LCS(string t1, string t2){
    dict.clear();
    //i = strA.length() -1 才可以逆序
    for(int i = t1.length()-1 ; i > 0 ; i--) dict[t1[i]].push_back(i) ;
    int k = 0; //生成數列的長度的最長長度
    for(int i = 1 ; i < t2.length() ; i++){ // 依據 strB 的每個字元來生成數列
        for(int j = 0 ; j < dict[t2[i]].size() ; j++) 
        //將此字元在 strA 出現的位置放入數列
            num[++k] = dict[t2[i]][j] ;
    }
    if(k==0) return 0;
    d[1] = -1 , t[1] = 1 ; //LIS init
    int len = 1, cur ; // len 由於前面已經把 LCS = 0 的機會排除，於是這裡則從 1 開始

    // 標準的 LIS 作法，不斷嘗試將 LCS 生長
    for(int i = 1 ; i <= k ; i++ ){
        if(num[i] > num[t[len]]) t[++len] = i , d[i] = t[len-1] ;
        else{
            cur = binarySearch(1,len,i);
            t[cur] = i ;
            d[i] = t[cur-1];
        }
    }
    return len ;

}