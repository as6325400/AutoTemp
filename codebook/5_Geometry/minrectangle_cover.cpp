// 向量長度平方
template<typename T>
T square(const point<T>& a){
    return a.x * a.x + a.y * a.y;
}

// 向量逆時針旋轉 90 度
template<typename T>
point<T> rot(const point<T>& a){
    return point<T>(-a.y, a.x);
}

// 直線定義：通過兩點 a, b
template<typename T>
struct Line{
    point<T> a, b;
};

// 兩直線交點 (假設不平行)
template<typename T>
point<T> lineIntersection(const Line<T>& l1, const Line<T>& l2){
    T A1 = (l1.b.x - l1.a.x) * (l2.a.y - l1.a.y) - (l1.b.y - l1.a.y) * (l2.a.x - l1.a.x);
    T A2 = (l1.b.x - l1.a.x) * (l2.b.y - l1.a.y) - (l1.b.y - l1.a.y) * (l2.b.x - l1.a.x);
    return (l2.b * A1 - l2.a * A2) / (A1 - A2);
}


template<typename T>
pair<T, vector<point<T>>> minRectangleCover(vector<point<T>> p) {
    if (p.size() <= 2) return {T(0), {}};
    auto get = [&](point<T> q, Line<T> l) -> T {
        return abs(cross(l.a - l.b, l.a - q));
    }; // line 到 p 圍成的四邊形面積
    int n = (int)p.size(), j = 2, l = 1, r = 1;
    p.push_back(p.front());
    T ans = numeric_limits<T>::max();
    vector<point<T>> ps;
    for (int i = 0; i < n; i++) {
        while (get(p[j], {p[i], p[i + 1]}) <= get(p[(j + 1) % n], {p[i], p[i + 1]}))
            j = (j + 1) % n;
        while (dot(p[i + 1] - p[i], p[r] - p[i]) <= dot(p[i + 1] - p[i], p[(r + 1) % n] - p[i]))
            r = (r + 1) % n;
        if (i == 0) l = j;
        while (dot(p[i + 1] - p[i], p[l] - p[i]) >= dot(p[i + 1] - p[i], p[(l + 1) % n] - p[i]))
            l = (l + 1) % n;
        T area = get(p[j], {p[i], p[i + 1]});
        T w = dot(p[i] - p[i + 1], p[l] - p[i]) + dot(p[i + 1] - p[i], p[r] - p[i]);
        area *= w / square(p[i + 1] - p[i]);
        if (area < ans) {
            ps.clear(); ans = area;
            Line<T> l1{p[i], p[i + 1]};
            for (auto u : {p[r], p[j], p[l], p[i]}) {
                if (u == l1.b) {
                    ps.push_back(u);
                    l1 = {u, u + rot(l1.b - l1.a)};
                } else {
                    Line<T> l2 = {u, u + rot(l1.b - l1.a)};
                    point<T> res = lineIntersection(l1, l2);
                    ps.push_back(res);
                    l1 = {res, u};
                }
            }
        }
    }
    return {ans, ps};
}
