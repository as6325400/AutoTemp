vector<int> build(string &s){
  vector<int> next = {0, 0};
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

int match(string &a, string &b){
  auto next = build(b);
  int length = a.size(), length2 = b.size(), j = 0, count = 0;
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
