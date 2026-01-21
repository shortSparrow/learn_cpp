#include <iostream>


/**
 * Overloading unary operators +, -, and !
 * Оскільки унарні оператори мають лише один операнд, то логічно, що їх overload робиться
 * у member function
 *
 * Особливості, overload унарного "-" не зламає бінарний "-" бо вони приймають різну кількість параметрів
 */

class Cents {
private:
    int m_cents {};

public:
    Cents(int cents): m_cents{cents} {}

    // Overload -Cents as a member function
    Cents operator-() const {
        return Cents(-m_cents);
    };

    int getCents() const { return m_cents; }
};


class Point {
private:
    double m_x {};
    double m_y {};
    double m_z {};

public:
    Point(double x=0.0, double y=0.0, double z=0.0):
        m_x{x}, m_y{y}, m_z{z} {
    }

    // Convert a Point into its negative equivalent
    Point operator- () const {
        return Point{-m_x, -m_y, -m_z};
    };

    // Return true if the point is set at the origin
    bool operator! () const {
        return (m_x==0.0 && m_y==0.0 && m_z==0.0);
    };

    // повертає Point з позитивними значеннями
    Point operator+() const {
        return Point{std::abs(m_x), std::abs(m_y), std::abs(m_z)};
    }

    double getX() const { return m_x; }
    double getY() const { return m_y; }
    double getZ() const { return m_z; }
};


int main() {
    const Cents nickle{ 5 };
    std::cout << "A nickle of debt is worth " << (-nickle).getCents() << " cents\n"; // A nickle of debt is worth -5 cents


    Point point{}; // use default constructor to set to (0.0, 0.0, 0.0)
    if (!point) // повертає true і цей if відпрацьовує
        std::cout << "point is set at the origin.\n";
    else
        std::cout << "point is not set at the origin.\n";


    Point point2 {-1.0, -5.0, -3.0};
    Point positivePoint {+point2};
    std::cout << positivePoint.getX() << " " << positivePoint.getY() << " " << positivePoint.getZ() << '\n'; // 1 5 3

    return 0;
}