// 需要使用前一個向量模板的 point ， 需要 operator - 以及 <
// 需要前面向量模板的 cross
// 回傳逆時針凸包，保留邊界點、不重複首點；會排序輸入
// 全共線時回傳排序去重的點；cross 須不溢位
template<typename T>
vector<point<T>> getConvexHull(vector<point<T>>& pnts){
  sort(pnts.begin(), pnts.end());
  auto same = [](point<T> a, point<T> b) {
    return a.x == b.x && a.y == b.y;
  };
  pnts.erase(unique(pnts.begin(), pnts.end(), same),
             pnts.end());
  int n = pnts.size();
  if (n <= 2) return pnts;
  bool flat = true;
  for (auto p : pnts)
    flat &= cross(pnts.back() - pnts[0], p - pnts[0]) == 0;
  if (flat) return pnts;
  vector<point<T>> hull;
  for (int side = 0; side < 2; ++side) {
    int start = hull.size();
    for (auto p : pnts) {
      while ((int)hull.size() - start >= 2) {
        auto a = hull[hull.size() - 2], b = hull.back();
        // <= 0 或者 < 0 要看點有沒有在邊上
        if (cross(b - a, p - a) >= 0) break;
        hull.pop_back();
      }
      hull.push_back(p);
    }
    hull.pop_back();
    reverse(pnts.begin(), pnts.end());
  }
  return hull;
}
