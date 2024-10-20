// 直角座標轉換為極座標
const long double PI = acos(-1);
std::pair<double, double> cartesian_to_polar(double x, double y) {
  double r = sqrt(x * x + y * y);
  double theta = atan2(y, x) / PI; 
  return {r, theta};
}

int main() {
  double x = 3, y = 4;  // 直角座標
  auto result = cartesian_to_polar(x, y);
  cout << "r = " << result.first << ", theta = " << result.second << endl;
  // 5, 0.295167
  return 0;
}