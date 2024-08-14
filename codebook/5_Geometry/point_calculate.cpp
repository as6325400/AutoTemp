template<typename T>
T dot(const point<T> &a,const point<T> &b){
    auto temp = new T;
    *temp = a.x * b.x + a.y * b.y;
    return *temp;
}

template<typename T>
T cross(const point<T> &a,const point<T> &b){
    auto temp = new T;
    *temp = a.x * b.y - a.y * b.x;
    return *temp;
}

template<typename T>
T abs2(const point<T> &a){
    auto temp = new T;
    *temp = a.x * a.x + a.y * a.y;
    return *temp;
}