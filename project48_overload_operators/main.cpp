#include <iostream>


/**
 * Оператори у C++
 *
 * Уявимо вираз
 * int x { 2 };
 * int y { 3 };
 * std::cout << x + y << '\n';
 * Це буде operator+(x, y)
 *
 * !!! Тобто оператор у C++ - це функція яка має вхідні параметри і яку можна перевантажити (overload) !!!
 *
 * Майже всі оператори у C++ можна перевантажити (overlord).
 * Не можна лише conditional (?:), sizeof, scope (::), member selector (.), pointer member selector (.*),
 * typeid, та casting operators
 *
 * Існує правило для overload - хоча б один параметр має бути user-defined type.
 * (Типи std:: бібліотеки вважаються user-defined type, тому ми можемо зробити overload для
 * operator+(double, std::string), але краще так не робити, щоб майбутні версії C++ не зламали наш код)
 *
 * Неможливо змінити кількість операндів, які підтримує оператор. Тобто якщо + підтримує два операнди, ми
 * не можемо зробити унарний + який перетворить string на number як у JS
 *
 * Також неможливо змінити пріоритетність оператор, тобто bitwise оператор ^ не можна перетворити на
 * оператор ступеня, бо арифметичні операції мають вищий пріоритет. Тобто 4+3^2 буде (4+3)^2=7^2 відповідно 7^2=49.
 * І щоб це працювало треба буде весь час обгортати оператор ^ у дужки 4+(3^2)
 *
 * Best practice
 * Оператори, які не змінюють свої операнди (наприклад, арифметичні оператори), вони повинні повертати
 * результати за значенням (by value).
 * Тому треба використовувати overload операторів нв власних типах даних (зазвичай класах)
 *
 *
 * Існує три різних види overload operator:
 *  1) the member function way
 *  2) the friend function way
 *  3) the normal function way
 *
 */

// Метод перевантаження (overload) оператора через friend function
class Cents {
private:
    int m_cents {};

public:
    Cents(int cents) : m_cents{ cents } { }

    friend int operator+(const Cents& c1, const Cents& c2);

    /**
     * Цікава особливість overload це те що послідовність типів має значення.
     * Припустимо Cents(4) + 6 викличе operator+(Cents, int), а 6 + Cents(4) викличе operator+(int, Cents)
     * Тобто якщо ми хочемо додати overload на Cents + int то маємо обробити дві послідовності
     *
     * У прикладі ниже friend функція написана в середині класу, але це все одно friend функція,
     * і вона не є member function! Тобто різниці між верхнім і уим прикладом немає.
     */

    friend int operator+(const Cents& c1, int c2) {
        return c1.m_cents + c2;
    }

    friend int operator+(int c1, const Cents& c2) {
        return c1 + c2.m_cents;
    }
};

int operator+(const Cents& c1, const Cents& c2) {
    return c1.m_cents + c2.m_cents;
}


/**
 * Метод перевантаження (overload) оператора через normal function.
 * Цей метод не має доступу до приватних полів класу, але якщо наші поля public, то проблем немає
 *
 * Краще використовувати цей метод замість friend function
 */
class Cents2 {
private:
    int m_cents {};

public:
    Cents2(int cents) : m_cents{ cents } { }

    int getCents() {
        return m_cents;
    }
};

int operator+(Cents2& c1,Cents2& c2) {
    return c1.getCents() + c2.getCents();
}
int operator+(const int c1,Cents2& c2) {
    return c1 + c2.getCents();
}
int operator+(Cents2& c1, const int c2) {
    return c1.getCents() + c2;
}


// Метод перевантаження (overload) оператора через member functions
class Cents3 {
private:
    int m_cents {};

public:
    Cents3(int cents)
        : m_cents { cents } { }

    // Overload Cents + int
    int operator+(int value) const;
    int operator+(const Cents3& cents) const;

    int getCents() const { return m_cents; }
};

int Cents3::operator+(int value) const {
    return this->m_cents + value;
}
int Cents3::operator+(const Cents3& cents) const {
    return this->m_cents + cents.getCents();
}


int main() {
    // overload with friend function
    std::cout << "***************** friend function ************\n";
    Cents cents1 { 6 };
    Cents cents2 { 8 };
    int centsSum { cents1 + cents2 };
    std::cout << "I have " << centsSum << " cents.\n"; // I have 14 cents.

    int a1 {cents1 + 1}; // 7
    int b1 {1 + cents1}; // 7


    // overload with normal function
    std::cout << "***************** normal function ************\n";
    Cents2 cents3 { 6 };
    Cents2 cents4 { 8 };
    int centsSum2 { cents3 + cents4 }; // 14
    std::cout << "I have " << centsSum2 << " cents.\n"; // I have 14 cents.

    int a2 {cents3 + 1}; // 7
    int b2 {1 + cents3}; // 7


    // overload with member functions
    std::cout << "***************** member function ************\n";
    const Cents3 cents5 { 6 };
    const Cents3 cents6 { 8 };
    const int sumCents3 { cents5 + cents6 };
    std::cout << "I have " << sumCents3 << " cents.\n"; // I have 14 cents.

    int a3 {cents5 + 1}; // 7
    // int b3 {1 + cents5}; // Таке зробити з member function неможливо




    /**
     * Загальна інформація
     * Навіщо нам потрбібно аж три варіанти overload
     *
     * Не все може бути перевантажено (overloaded) через friend function
     * The assignment (=), subscript ([]), function call (()), and member selection (->) operators must be overloaded
     * as member functions, because the language requires them to be.
     *
     * Не все може бути перевантажено (overloaded) як member function
     * Не можна оператор <<, оскільки перевантажений (overloaded) оператор має бути доданий як
     * left operand. У цьому випадку left operand є об'єктом типу std::ostream
     * Це вимагає, щоб operator<< був перевантажений як звичайна функція (non-member function (бажано)) або як friend object.
     * Те саме і для >> оператора
     *
     *
     *
     * Ось перелік правил коли який варіант перевантаження (olverload) використовуавти:
     * The following rules of thumb can help you determine which form is best for a given situation:
     *  - If you’re overloading assignment (=), subscript ([]), function call (()), or member selection (->), do so as a member function.
     *  - If you’re overloading a unary operator, do so as a member function.
     *  - If you’re overloading a binary operator that does not modify its left operand (e.g. operator+), do so as a normal function (preferred) or friend function.
     *  - If you’re overloading a binary operator that modifies its left operand, but you can’t add members to the class definition of the left
     *    operand (e.g. operator<<, which has a left operand of type ostream), do so as a normal function (preferred) or friend function.
     *  - If you’re overloading a binary operator that modifies its left operand (e.g. operator+=), and you can modify the definition of the left operand,
     *    do so as a member function.
     */


  return 0;
}