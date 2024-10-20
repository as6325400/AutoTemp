// 極座標轉換為直角座標函數，theta 單位為弧度
const long double PI = acos(-1);
pair<double, double> polar_to_cartesian(double r, double theta) {
  double theta_radians = theta * PI;
  double x = r * cos(theta_radians);
  double y = r * sin(theta_radians);
  return {x, y};
}

int main() {
  double r = 5, theta = 0.5;  // 極座標
  auto result = polar_to_cartesian(r, theta);
  cout << result.first << " " << result.second << endl;
  // 0, 5
  return 0;
}
