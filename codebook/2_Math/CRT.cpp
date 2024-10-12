int exgcd(int a, int b, int &x, int &y) {
  if (!b) {
    x = 1, y = 0;
    return a;
  }
  int g = exgcd(b, a % b, y, x);
  y -= a / b * x;
  return g;
}

int inv(int x, int m) {
  int a, b;
  exgcd(x, m, a, b);
  a %= m;
  if (a < 0) a += m;
  return a;
}

// 求解 x = r1 % m1 = r2 % m2 = r3 % m3...
// a[i] = {{remain, mod}, ...}
// notice: 如出現極限測資(1e18)，需開__int128
int CRT(vector<pair<int, int>> &a) {
  int s = 1, ret = 0;
  for (auto &[r, m] : a) s *= m;
  for (auto &[r, m] : a) {
    int t = s / m;
    ret += r * t % s * inv(t, m) % s;
    if (ret >= s) ret -= s;
  }
  return ret;
}