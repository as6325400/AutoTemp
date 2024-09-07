// 矩陣乘法 (A * B) % mod

template <typename T>
vector<vector<T>> matrix_mult(const vector<vector<T>>& A, const vector<vector<T>>& B, T mod) {
  int m = A.size();
  int n = A[0].size(); 
  int p = B[0].size();
  assert(A[0].size() == B.size());
  vector<vector<T>> result(m, vector<T>(p, 0));
  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < p; ++j) {
      for (int k = 0; k < n; ++k) {
        result[i][j] = (result[i][j] + A[i][k] * B[k][j]) % mod;
      }
    }
  }
  return result;
}