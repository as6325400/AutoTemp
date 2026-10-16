// cross、dot 的中間結果須不溢位；浮點判斷採精確比較
template<typename T>
bool collinearity(point<T> a, point<T> b, point<T> c) {
  //檢查三點是否共線
  return cross(b - a, c - a) == 0;
}

template<typename T>
bool inLine(point<T> a, point<T> b, point<T> p) {
  //檢查 p 點是否在ab線段
  return collinearity(a, b, p) && dot(a - p, b - p) <= 0;
}

template<typename T>
bool intersect(point<T> a, point<T> b,
               point<T> c, point<T> d) {
  //ab線段跟cd線段是否相交
  auto opposite = [](T x, T y) {
    return (x < 0 && y > 0) || (x > 0 && y < 0);
  };
  return (opposite(cross(b - a, c - a),
                   cross(b - a, d - a)) &&
          opposite(cross(d - c, a - c),
                   cross(d - c, b - c))) ||
         inLine(a, b, c) || inLine(a, b, d) ||
         inLine(c, d, a) || inLine(c, d, b);
}

template<typename T>
point<T> intersection(point<T> a, point<T> b,
                       point<T> c, point<T> d) {
  //ab線段跟cd線段相交的點
  // T 用浮點數；要求唯一交點，不接受共線重疊
  assert(intersect(a, b, c, d));
  T den = cross(d - c, b - a);
  assert(den != 0);
  return a + (b - a) * (cross(a - c, d - c) / den);
}

template<typename T>
bool inPolygon(const vector<point<T>>& polygon,
               point<T> p) {
  //判斷點p是否在多邊形polygon裡，vector裡的點要連續填對
  // 簡單多邊形，含邊界、支援凹形與兩種方向
  bool inside = false;
  int n = polygon.size();
  for (int i = 0; i < n; ++i) {
    auto a = polygon[i], b = polygon[(i + 1) % n];
    if (inLine(a, b, p)) return true;
    if ((a.y > p.y) == (b.y > p.y)) continue;
    auto turn = cross(b - a, p - a);
    if ((turn > 0) == (b.y > a.y)) inside = !inside;
  }
  return inside;
}

template<typename T>
long double triangleArea(point<T> a, point<T> b,
                         point<T> c) {
  //三角形頂點，求面積
  return fabs((long double)cross(b - a, c - a)) / 2;
}

template<typename T, typename F, typename S>
long double triangleArea_Herons_formula(T a, F b, S c) {
  //三角形頂點，求面積(給邊長)
  long double x = a, y = b, z = c;
  long double p = (x + y + z) / 2;
  assert(x >= 0 && y >= 0 && z >= 0 &&
         x + y >= z && y + z >= x && z + x >= y);
  return sqrtl(max(0.L, p * (p - x) * (p - y) * (p - z)));
}

template<typename T>
long double area(const vector<point<T>>& p) {
  //多邊形頂點，求面積
  // 以首點為原點，減少大座標平移造成的浮點抵消
  long double sum = 0;
  for (int i = 1; i + 1 < (int)p.size(); ++i)
    sum += (long double)cross(p[i] - p[0],
                             p[i + 1] - p[0]);
  return fabs(sum) / 2;
}
