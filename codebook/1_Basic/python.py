# Python 3；區塊結尾用冒號、縮排 4 格，不用大括號
import sys
# 調高遞迴上限；很深的 DFS 宜改迭代
sys.setrecursionlimit(20000)
input = sys.stdin.readline  # 保留換行；EOF 回傳 ""

n, m = map(int, input().split())  # 同一行兩個整數
a = list(map(int, input().split()))  # 一行整數
a = list(map(float, input().split()))  # 一行浮點數
g = [list(map(int, input().split())) for _ in range(n)]
s = input().rstrip("\r\n")  # 字串；保留頭尾空白
for line in sys.stdin:  # 一直讀到 EOF
    a = list(map(int, line.split()))
# 多筆：先讀 t = int(input())，再 for _ in range(t):
print(*a)  # 空白分開；print(x, end="") 不換行
print("\n".join(map(str, a)))  # 每行一個
print(f"{3.14159:.3f}")  # 小數點後 3 位

# list / 迴圈 / 排序（索引從 0 開始）
# range(l, r) 不含 r；倒序 range(n-1, -1, -1)
for i, x in enumerate(a): print(i, x)  # 索引、值
a.append(5); x = a.pop()  # push_back / 取出尾端
b = a[1:3]; c = a[::-1]  # 切片不含右端；反轉副本
a.sort()  # 原地排序，回傳 None；勿寫 a = a.sort()
b = sorted(a, reverse=True)  # 新陣列，遞減排序
d = {}; d[x] = d.get(x, 0) + 1  # dict 計數
seen = set(a); print(x in seen)  # 去重、判斷存在

# 複製 / reference：list 可修改，int / str 不可修改
a = [1, 2]; b = a  # 共用同一物件，不會複製
# 此時 b[0] = 9 會改到 a；b = [9] 則只是換綁定
b = a[:]  # 或 a.copy()；只複製外層，內層仍共用
g = [[0] * m for _ in range(n)]  # 勿用 [[0]*m]*n
h = [row[:] for row in g]  # 二維數字陣列逐列複製
from copy import deepcopy
h = deepcopy(g)  # 更深的巢狀結構也獨立複製
def change(a):  # 傳入 list 也共用物件
    a.append(1)  # 會改外部 list；改寫 a = [] 則不會

# 值比較用 ==；is 是同一物件（None 判斷用 is None）
# / 是浮點除法；// 向下取整：-7 // 3 == -3
# -7 % 3 == 2；次方用 **，^ 是 XOR
# map 是一次性迭代器；要索引或重複使用先包 list

# Fraction(分子, 分母)：精確分數，可直接加減乘除
from fractions import Fraction
frac = Fraction(3, 4)
numerator = frac.numerator  # 取出分子
denominator = frac.denominator  # 取出分母
print(frac + Fraction(1, 6))  # 11/12
# 精確的 1/10 用 Fraction(1, 10)，不要 Fraction(0.1)
