#include <iostream>
//#include <vector>
//#include <string>
//#include <fstream>

template<class T>
const T &maxT(const T &a, const T &b) {
    return a > b ? a : b;
}
template<class T>
const T &minT(const T &a, const T &b) {
    return a < b ? a : b;
}
template<class T>
void swapT(T &a, T &b) {
    T t = a;
    a = b;
    b = t;
}
template<class T>
void absT(T &a) {
    return a > 0 ? a : -a;
}
constexpr uint64_t factorial(uint64_t n) {
    uint64_t m = 1;
    for (uint64_t i = 1; i <= n; i++) {
        m *= i;
    }
    return m;
}
constexpr bool isPrime(uint64_t n) {
    for (uint64_t i = 2; i * i <= n; i++) {
        if (!(n % i)) return true;
    }
    return false;
}

int main() {
/*
    std::string a = "123", b = "abc";
    swapT(a, b);
    std::cout << a << ' ' << b << '\n';
*/
    printf("%lu", factorial(10));
    return 0;
}