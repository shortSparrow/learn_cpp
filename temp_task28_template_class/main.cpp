#include <iostream>

/**
 * Тут реалізовано три задачі. Суть кожної з них - це написати клас який буде виконувати код,
 * що написано у main
 *
 */

template <typename T>
class Pair1 {
private:
    T m_a {};
    T m_b {};

public:
    Pair1(T a, T b): m_a(a), m_b(b) { }

    T first() const {
        return m_a;
    }

    T second() const {
        return m_b;
    }
};


template <typename T, typename S>
class Pair {
private:
    T m_x;
    S m_y;

public:
    Pair(const T& x, const S& y): m_x{x}, m_y{y} {}

    T& first() { return m_x; }
    S& second() { return m_y; }
    const T& first() const { return m_x; }
    const S& second() const { return m_y; }
};

template <typename T>
class StringValuePair : public Pair<std::string, T> {
public:
    StringValuePair(std::string a, T b): Pair<std::string, T>{a,b} {}
};


int main() {
    std::cout << "---------------------TASK 1---------------------\n";
    Pair1<int> p1 { 5, 8 };
    std::cout << "Pair: " << p1.first() << ' ' << p1.second() << '\n';

    const Pair1<double> p2 { 2.3, 4.5 };
    std::cout << "Pair: " << p2.first() << ' ' << p2.second() << '\n';


    std::cout << "---------------------TASK 2---------------------\n";
    Pair<int, double> p3 { 5, 6.7 };
    std::cout << "Pair: " << p3.first() << ' ' << p3.second() << '\n';

    const Pair<double, int> p4 { 2.3, 4 };
    std::cout << "Pair: " << p4.first() << ' ' << p4.second() << '\n';


    std::cout << "---------------------TASK 3---------------------\n";
    StringValuePair<int> svp { "Hello", 5 };
    std::cout << "Pair: " << svp.first() << ' ' << svp.second() << '\n';

    return 0;
}