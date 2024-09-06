vector<int> prime_table(int n){
  vector<int> table(n + 1, 0);
  for(int i = 1; i <= n; i++){
    for(int j = i; j <= n; j += i){
      table[j]++;
    }
  }
  return table;
}