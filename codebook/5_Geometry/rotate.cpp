const long double PI = acos(-1);
// 逆時針旋轉
// angle_red 為弧度
pair<double, double> rotate_point(double x, double y, double angle_rad) {
  angle_rad *= PI;
  double new_x = x * cos(angle_rad) - y * sin(angle_rad);
  double new_y = x * sin(angle_rad) + y * cos(angle_rad);
  return {new_x, new_y};
}

int main() {
  double x = 5, y = 0;  
  double angle = 0.5;   // 逆時針旋轉 90 度
  auto result = rotate_point(x, y, angle);
  cout << result.first << " " << result.second << endl;
  // 0, 5
  return 0;
}