const long double PI = acos(-1);

// 寬 w 高 h 的四邊形，旋轉一個 pi 後在每個角度的寬高

vector<pair<double, double>> rotate(double w, double h){
  int freq = 1000; // 自己調整精度
  vector<pair<double, double>> res;
  for (int i = 0; i <= 5; ++i) {
    double theta = (PI * i) / 5;
    double nw = c * fabs(cos(theta)) + d * fabs(sin(theta));
    double nh = c * fabs(sin(theta)) + d * fabs(cos(theta));
    res.push_back({nw, nh});
  }
  return res;
}