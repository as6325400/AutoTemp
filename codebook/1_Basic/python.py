## Input
# p q 都是整數，中間以空白分開輸入
p, q = map(int, input().split())

# 輸入很多個用空白隔開的數字，轉成 float 放進陣列，s 是 input 字串
arr = list(map(float, s.split()))


# 分數用法 Fraction(被除數, 除數)
from fractions import Fraction

arr = [Fraction(0), Fraction(1, 6), Fraction(1, 2), Fraction(5, 12), Fraction(0), Fraction(-1, 12), Fraction(0)]

def fx(x):
    x = Fraction(x)
    ans = Fraction(0)
    for i in range(1, 7):
        ans += arr[i] * x ** (7 - i)
    return ans