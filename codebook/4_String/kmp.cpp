// 計算可重疊出現次數；空模式匹配 n + 1 個位置
vector<int> build(const string &s){
  vector<int> next(s.empty() ? 1 : 2);
  // 匹配失敗跳去哪 (最長共同前後綴)
  int length = s.size(), j = 0;
  for(int i = 1; i < length; i++){
    while(j > 0 and s[j] != s[i]){
      j = next[j];
    }
    if(s[j] == s[i]) j++;
    next.push_back(j);
  }
  return next;
}

int match(const string &a, const string &b){
  if (b.empty()) return (int)a.size() + 1;
  auto next = build(b);
  int length = a.size(), length2 = b.size();
  int j = 0, count = 0;
  for(int i = 0; i < length; i++){
    while(j > 0 and a[i] != b[j]){
      j = next[j];
    }
    if(a[i] == b[j]) j++;
    if(j == length2){
      count++;
      j = next[j];
    }
  }
  return count;
}
