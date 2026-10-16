template<typename T>
struct point {
  T x{}, y{};
  point() = default;
  point(T x, T y) : x(x), y(y) {}
  point operator+(const point& a) const {
    return {x + a.x, y + a.y};
  }
  point operator-(const point& a) const {
    return {x - a.x, y - a.y};
  }
  point operator/(const point& a) const {
    return {x / a.x, y / a.y};
  }
  point operator/(T a) const { return {x / a, y / a}; }
  point operator*(T a) const { return {x * a, y * a}; }
  bool operator<(const point& a) const {
    return x != a.x ? x < a.x : y < a.y;
  }
  bool operator==(const point& a) const {
    return x == a.x && y == a.y;
  }
};
