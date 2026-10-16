// 向量長度平方
template<typename T>
T square(const point<T>& a) {
  return a.x * a.x + a.y * a.y;
}

// 向量逆時針旋轉 90 度
template<typename T>
point<T> rot(const point<T>& a) { return {-a.y, a.x}; }

// 直線定義：通過兩點 a, b
template<typename T>
struct Line { point<T> a, b; };

// 兩直線交點 (假設不平行)
template<typename T>
point<T> lineIntersection(const Line<T>& a,
                          const Line<T>& b) {
  auto u = a.b - a.a, v = b.b - b.a;
  T d = cross(u, v);
  assert(d != 0);
  return a.a + u * (cross(b.a - a.a, v) / d);
}

// T 用浮點數；輸入依邊界排序的凸包，順逆時針皆可
// 可含邊界共線點；刪相鄰重複點，空/線段回傳 {0,{}}
// O(n)，回傳 {面積, 逆時針四角}，不改輸入
template<typename T>
pair<T, vector<point<T>>> minRectangleCover(
    vector<point<T>> p) {
  auto same = [](point<T> a, point<T> b) {
    return a.x == b.x && a.y == b.y;
  };
  p.erase(unique(p.begin(), p.end(), same), p.end());
  if (p.size() > 1 && same(p.front(), p.back()))
    p.pop_back();
  int n = p.size();
  if (n <= 2) return {0, {}};
  T signed_area = 0;
  for (int i = 0; i < n; ++i)
    signed_area += cross(p[i] - p[0],
                         p[(i + 1) % n] - p[0]);
  if (signed_area == 0) return {0, {}};
  if (signed_area < 0) reverse(p.begin(), p.end());
  T ans = numeric_limits<T>::max();
  vector<point<T>> corners;
  int top = 0, left = 0, right = 0;
  for (int i = 0; i < n; ++i) {
    auto e = p[(i + 1) % n] - p[i];
    auto height = [&](int k) {
      // line 到 p 圍成的四邊形面積
      return cross(e, p[k] - p[i]);
    };
    // 以當前點為原點，避免大座標的投影相減損失精度
    auto along = [&](int k) {
      return dot(e, p[k] - p[i]);
    };
    if (!i) {
      for (int k = 1; k < n; ++k) {
        if (height(k) > height(top)) top = k;
        if (along(k) < along(left)) left = k;
        if (along(k) > along(right)) right = k;
      }
    }
    while (height((top + 1) % n) >= height(top))
      top = (top + 1) % n;
    while (along((left + 1) % n) <= along(left))
      left = (left + 1) % n;
    while (along((right + 1) % n) >= along(right))
      right = (right + 1) % n;
    T lo = along(left), hi = along(right);
    T up = height(top), norm = square(e);
    T area = (hi - lo) * up / norm;
    if (area >= ans) continue;
    ans = area;
    auto at = [&](T x, T y) {
      return p[i] + (e * x + rot(e) * y) / norm;
    };
    corners = {at(lo, 0), at(hi, 0),
               at(hi, up), at(lo, up)};
  }
  return {ans, corners};
}
