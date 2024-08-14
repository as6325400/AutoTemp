template<typename T>
class point{
    public:
    T x;
    T y;
    point(){}
    point(T x, T y){
        this -> x = x;
        this -> y = y;
    }
    point<T>& operator+(const point<T> &a);
    point<T>& operator-(const point<T> &a);
    point<T>& operator/(const point<T> &a);
    point<T>& operator/(T a);
    point<T>& operator*(const T &a);
    bool operator<(const point<T> &a);
};

template<typename T>
point<T>& point<T>::operator+(const point<T> &a){
    auto temp = new point<T>;
    temp -> x = a.x + this -> x;
    temp -> y = a.y + this -> y;
    return *temp;
}

template<typename T>
point<T>& point<T>::operator-(const point<T> &a){
    auto temp = new point<T>;
    temp -> x = this -> x - a.x;
    temp -> y = this -> y - a.y;
    return *temp;
}

template<typename T>
point<T>& point<T>::operator/(const point<T> &a){
    auto temp = new point<T>;
    temp -> x = this -> x / a.x;
    temp -> y = this -> y / a.y;
    return *temp;
}

template<typename T>
point<T>& point<T>::operator/(T a){
    auto temp = new point<T>;
    temp -> x = this -> x / a;
    temp -> y = this -> y / a;
    return *temp;
}

template<typename T>
point<T>& point<T>::operator*(const T &a){
    auto temp = new point<T>;
    temp -> x = this -> x * a;
    temp -> y = this -> y * a;
    return *temp;
}

template<typename T>
bool point<T>::operator<(const point<T> &a){
    if(this -> x != a.x) return this -> x < a.x;
    return this -> y < a.y;
}