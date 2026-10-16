int sign(double a) {
  // 小於 eps 回傳 0，否則正回傳 1 ，負回傳 應付浮點數誤差用
  const double eps = 1e-10;
  return fabs(a) < eps ? 0 : a > 0 ? 1 : -1;
}

// 輸入三個點求外心
// T 用浮點數；三點不可共線，eps 為叉積容許值
template<typename T>
point<T> findCircumcenter(point<T> A, point<T> B,
                         point<T> C, T eps = 1e-10) {
  auto AB = B - A, AC = C - A;
  T ab = AB.x * AB.x + AB.y * AB.y;
  T ac = AC.x * AC.x + AC.y * AC.y;
  T d = AB.x * AC.y - AB.y * AC.x;
  // 若三點接近共線
  assert(fabs(d) >= eps && d != 0);
  // 外心的座標
  return A + point<T>(AC.y * ab - AB.y * ac,
                      AB.x * ac - AC.x * ab) / (2 * d);
}

template<typename T>
pair<T, point<T>> MinCircleCover(vector<point<T>>& p) {
  // 引入前面的 len 跟 point
  // 回傳最小覆蓋圓{半徑,中心}
  // 會打亂輸入；空集合回傳 {0, (0, 0)}；期望 O(n)
  // 直接比較容差，避免與其他模板的 sign 多載衝突
  const T eps = 1e-10;
  if (p.empty()) return {0, {0, 0}};
  static mt19937 rng(712367);
  shuffle(p.begin(), p.end(), rng);
  int n = p.size();
  point<T> c = p[0];
  T r = 0;
  for (int i = 1; i < n; ++i) {
    if (len(c - p[i]) - r < eps) continue;
    // 不在圓內
    c = p[i], r = 0;
    for (int j = 0; j < i; ++j) {
      if (len(c - p[j]) - r < eps) continue;
      c = (p[i] + p[j]) / T(2);
      r = len(c - p[i]);
      for (int k = 0; k < j; ++k) {
        if (len(c - p[k]) - r < eps) continue;
        c = findCircumcenter(p[i], p[j], p[k], T(0));
        r = len(c - p[i]);
      }
    }
  }
  return {r, c};
}
