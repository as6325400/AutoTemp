template<typename T>
bool collinearity(point<T> p1, point<T> p2, point<T> p3){
    //檢查三點是否共線
    return cross(p2 - p1, p2 - p3) == 0;
}

template<typename T>
bool inLine(point<T> a, point<T> b, point<T> p){
    //檢查 p 點是否在ab線段
    return collinearity(a, b, p) && dot(a - p, b - p) <= 0;
}

template<typename T>
bool intersect(point<T> a, point<T> b, point<T> c, point<T> d){
    //ab線段跟cd線段是否相交
    return (cross(b - a, c - a) * \ 
        cross(b - a, d - a) < 0 && \
        cross(d - c, a - c) * \
        cross(d - c, b - c) < 0) \
        || inLine(a, b, c) || \
        inLine(a, b, d) || inLine(c, d, a) \
        || inLine(c, d, b);
}

template<typename T>
point<T> intersection(point<T> a, point<T> b, point<T> c, point<T> d){
    //ab線段跟cd線段相交的點
    assert(intersect(a, b, c, d));
    return a + (b - a) * cross(a - c, d - c) / cross(d - c, b - a);
}

template<typename T>
bool inPolygon(vector<point<T>> polygon, point<T> p){
    //判斷點p是否在多邊形polygon裡，vector裡的點要連續填對
    for(int i = 0; i < polygon.size(); i++)
        if(cross(p - polygon[i], \ 
            polygon[(i - 1 + polygon.size()) % \
            polygon.size()] - polygon[i]) * \
            cross(p - polygon[i], \ 
            polygon[(i + 1) % polygon.size()] - polygon[i]) > 0)
            return false;
    return true;
}

template<typename T>
T triangleArea(point<T> a, point<T> b, point<T> c){
    //三角形頂點，求面積
    return abs(cross(b - a, c - a)) / 2;
}

template<typename T,typename F,typename S>
long double triangleArea_Herons_formula(T a, F b, S c){
    //三角形頂點，求面積(給邊長)
    auto p = (a + b + c)/2;
    return sqrt(p * (p - a) * (p - b) * (p - c));
}

template<typename T>
T area(vector<point<T>> &p){
    //多邊形頂點，求面積
    T ans = 0;
    for(int i = 0; i < p.size(); i++)
        ans += cross(p[i], p[(i + 1) % p.size()]);
    return ans / 2 > 0 ? ans / 2 : -ans / 2;
}