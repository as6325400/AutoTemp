const long double PI = acos(-1);

// 寬 w 高 h 的四邊形，旋轉一個 pi 後在每個角度的寬高
// 回傳 freq + 1 筆等距取樣，非連續角度的精確極值
vector<pair<double, double>> rotate(double w, double h,
                                    int freq = 5) {
  // 自己調整精度
  assert(freq > 0);
  vector<pair<double, double>> res;
  res.reserve(freq + 1);
  for (int i = 0; i <= freq; ++i) {
    double theta = PI * i / freq;
    double c = fabs(cos(theta)), s = fabs(sin(theta));
    res.push_back({w * c + h * s, w * s + h * c});
  }
  return res;
}
