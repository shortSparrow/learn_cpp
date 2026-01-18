#include <iostream>

class Point {
private:
    double m_x{};
    double m_y{};
    double m_z{};

public:
    Point(double x=0.0, double y=0.0, double z=0.0)
      : m_x{x}, m_y{y}, m_z{z} {
    }

    double getX() const { return m_x; }
    double getY() const { return m_y; }
    double getZ() const { return m_z; }

    friend std::ostream& operator<< (std::ostream& out, const Point& point);
    friend std::istream& operator>> (std::istream& in, Point& point);

};

std::ostream& operator<<(std::ostream& out, const Point& point) {
    out << "Point(" << point.m_x << ", " << point.m_y << ", " << point.m_z << ')';
    return out; // return out треба, щоб на результаті цього виклику можна було викликати наступний оператор <<
}

std::istream& operator>>(std::istream& in, Point& point) {
    // Простий варіант, але якщо не передамо якийсь аргумент то він ініціалізується порожнім
    // in >> point.m_x >> point.m_y >> point.m_z;
    double x{};
    double y{};
    double z{};

    if (in >> x >> y >> z)      // if all extractions succeeded
        point = Point{x, y, z}; // overwrite our existing point

    return in;
}

int main() {
    /**
     * У нас є клас Point і ми хочемо мати змогу виводити його поля x,y,z у std::cout,
     * для цього можна написати метож helper або зробити overload оператора <<
     *
     * Розлянемо вираз std::cout << point
     * Які операнди у оператора << тут є:
     *  left operand - це std::cout object
     *  right operand - це екземпляр класу Point
     *
     * Варто зазначити, що std::cout це насправді об'єкт типу std::ostream
     *
     * Коли ми зробили overload operator <<, то ми повернули out (об'єкт std::stream), бо якщо ми цього не зробимо то після
     * виклику std::cout << point1 ми не зможемо додати << '\n', бо буде void << '\n', а це не матиме сенсу і викличе помилку.
     * Тож або треба не додавати другого << після std::cout << point1, або робити як ми зробили з retuen out
     */

    const Point point1 { 2.0, 3.0, 4.0 };
    std::cout << point1 << '\n'; // Point(2, 3, 4)


    Point point2 { 2.0, 3.5, 4.0 };
    Point point3 { 6.0, 7.5, 8.0 };
    std::cout << point2 << ' ' << point3 << '\n'; // Point(2, 3.5, 4) Point(6, 7.5, 8)



    /**
     * Overload >> operator
     * Все те саме, що із << оператором. Функція перевантаження приймає екземпляр класу Point,
     * в нашому випадку це породній клас, ми передамо його і він заповниться даними які введе користувач.
     *
     * Якщо хочаб один параметр який треба користувач не введе то >> не зробить ніяких змін у переданому point,
     * тож це вбереже нас від часткового внесення даних (саме таку реалізацію ми зробили на overload >>)
     */

    std::cout << "Enter a point: ";

    Point point{ };
    std::cin >> point; // Вводжу 1.0 2.0 3.0
    std::cout << "You entered: " << point << '\n'; // You entered: Point(1, 2, 3)

    return 0;
}