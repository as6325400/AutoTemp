// 抄 BBuf github 的

#include <bits/stdc++.h>
using namespace std;

// 輸入須為 int128 範圍內的整數；可有空白、正負號。
// EOF 回 false；使用 getchar，勿混用關閉同步的 cin。
bool scan(__int128 &x) { // 輸入
  int ch = getchar();
  while (ch != EOF && isspace((unsigned char)ch))
    ch = getchar();
  if (ch == EOF) return false;
  bool neg = ch == '-';
  if (ch == '-' || ch == '+') ch = getchar();
  __uint128_t v = 0;
  while (ch >= '0' && ch <= '9') {
    v = v * 10 + ch - '0';
    ch = getchar();
  }
  x = neg && v ? -(__int128)(v - 1) - 1 : (__int128)v;
  return true;
}

void print(__int128 x) { // 輸出
  if (x < 0) putchar('-');
  __uint128_t v = x;
  if (x < 0) v = 0 - v;
  char s[40];
  int n = 0;
  do { s[n++] = v % 10 + '0'; v /= 10; } while (v);
  while (n) putchar(s[--n]);
}

int main() {
  __int128 a, b;
  if (!scan(a) || !scan(b)) return 0;
  // 範例：加法、乘法的結果仍須放得進 int128。
  print(a + b);
  puts("");
  print(a * b);
  return 0;
}
