// 依 second 計逆序數對，並將 [l, r] 穩定排序。
// 回傳 long long；暫存陣列只配置一次，O(n log n)。
long long mergeSort(vector<pair<int, int>>& v,
    vector<pair<int, int>>& tmp, int l, int r) {
  if (l >= r) return 0;
  int mid = l + (r - l) / 2;
  long long count = mergeSort(v, tmp, l, mid)
                 + mergeSort(v, tmp, mid + 1, r);
  int i = l, j = mid + 1;
  for (int k = l; k <= r; ++k) {
    if (j > r || (i <= mid &&
                  v[i].second <= v[j].second))
      tmp[k] = v[i++];
    else tmp[k] = v[j++], count += mid - i + 1;
  }
  copy(tmp.begin() + l, tmp.begin() + r + 1,
       v.begin() + l);
  return count;
}
long long mergeSort(vector<pair<int, int>>& v,
                    int l, int r) {
  vector<pair<int, int>> tmp(v.size());
  return mergeSort(v, tmp, l, r);
}

signed main() {
  int n;
  cin >> n;
  vector<pair<int, int>> arr(n);
  for (int i = 0; i < n; ++i) {
    arr[i].first = i;
    cin >> arr[i].second;
  }
  cout << mergeSort(arr, 0, n - 1) << '\n';
}
