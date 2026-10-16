// return the maximum of $f(x)$ in $[l, r]$
// f 在最大值區間外嚴格增／減；EPS > 0、r-l 有限。
double ternary_search(double l, double r) {
  while (r - l > EPS) {
    double m1 = l + (r - l) / 3;
    double m2 = r - (r - l) / 3;
    if (m1 == l || m2 == r) break;
    double f1 = f(m1), f2 = f(m2);
    if (f1 < f2) l = m1;
    else r = m2;
  }
  return max(f(l), f(r));
}

// return the maximum of $f(x)$ in $(l, r]$
// 整數版回傳「位置」；l < r，r-l 須放得進 int。
// f 在最大值區間外嚴格增／減；相同最大值取最右。
int ternary_search(int l, int r) {
  while (r - l > 1) {
    int mid = l + (r - l) / 2;
    if (f(mid) > f(mid + 1)) r = mid;
    else l = mid;
  }
  return r;
}
