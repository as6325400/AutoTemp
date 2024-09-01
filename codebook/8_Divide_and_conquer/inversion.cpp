int merge(vector<pair<int, int>>& v, int l, int mid, int r) {
  vector<pair<int, int>> temp(r - l + 1);
  int i = l, j = mid + 1, k = 0, inv_count = 0;
  while (i <= mid && j <= r) {
      if (v[i].second <= v[j].second) {
          temp[k++] = v[i++];
      } else {
          temp[k++] = v[j++];
          inv_count += (mid - i + 1); 
      }
  }
  while (i <= mid) temp[k++] = v[i++];
  while (j <= r) temp[k++] = v[j++];
  for (int i = l; i <= r; i++) {
    v[i] = temp[i - l];
  }
  return inv_count;
}
 
int mergeSort(vector<pair<int, int>>& v, int l, int r) {
  int count = 0;
  if (l < r) {
    int mid = l + (r - l) / 2;
    count += mergeSort(v, l, mid);
    count += mergeSort(v, mid + 1, r);
    count += merge(v, l, mid, r);
  }
  return count;
}

signed main()
{
  int n;
  cin >> n;
  vector<pair<int, int>> arr(n);
  for(int i = 0; i < n; i++){
    arr[i].first = i;
    cin >> arr[i].second;
  }
  cout << mergeSort(arr, 0, n - 1) << '\n';
}