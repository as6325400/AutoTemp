// vec[i] = {m_i, x_i}，求最小非負 x 使得 x ≡ x_i (mod m_i) 對所有 i 同時成立；無解回 -1
// 注意 overflow
int CRT(vector<pair<int, int>> &v)
{
  int m = v[0].first, x = (v[0].second % m + m) % m;
  for (int i = 1; i < (int)v.size(); ++i)
  {
    int mi = v[i].first, xi = (v[i].second % mi + mi) % mi;
    int g = gcd(m, mi), d = xi - x;
    if (d % g) return -1;
    int m1 = m / g, m2 = mi / g;
    auto ab = ext_gcd((int)m1, (int)m2);
    int inv = ((int)ab.first % m2 + m2) % m2;
    int k = ((d / g) % m2 + m2) % m2;
    k = (k * inv) % m2;
    x = (x + m * k) % (m * m2);
    m *= m2;
    x = (x + m) % m;
  }
  return x;
}