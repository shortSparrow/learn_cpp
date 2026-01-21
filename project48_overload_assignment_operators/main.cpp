#include <cassert>
#include <iostream>


class Fraction
{
private:
    int m_numerator { 0 };
    int m_denominator { 1 };

public:
    // Default constructor
    Fraction(int numerator = 0, int denominator = 1 )
        : m_numerator { numerator }, m_denominator { denominator }
    {
        assert(denominator != 0);
    }

    // Copy constructor
    Fraction(const Fraction& copy)
        : m_numerator { copy.m_numerator }, m_denominator { copy.m_denominator } {
        // no need to check for a denominator of 0 here since copy must already be a valid Fraction
        std::cout << "Copy constructor called\n"; // just to prove it works
    }

    // Overloaded assignment
    Fraction& operator= (const Fraction& fraction) {
        std::cout << "Overloaded assignment called\n";
        // do the copy
        m_denominator = fraction.m_denominator ;
        m_numerator = fraction.m_numerator;


        // return the existing object so we can chain this operator
        return *this;
    };

    friend std::ostream& operator<<(std::ostream& out, const Fraction& f1) {
        out << f1.m_numerator << '/' << f1.m_denominator;
        return out;
    }
};


// Ми забороняємо робити копії через конструктор та присвоєння
class Fraction2 {
private:
    int m_numerator { 0 };
    int m_denominator { 1 };

public:
    // Default constructor
    Fraction2(int numerator = 0, int denominator = 1)
        : m_numerator { numerator }, m_denominator { denominator }
    {
        assert(denominator != 0);
    }

    // Copy constructor
    Fraction2(const Fraction2 &copy) = delete;

    // Overloaded assignment
    Fraction2& operator= (const Fraction2& fraction) = delete; // no copies through assignment!

    friend std::ostream& operator<<(std::ostream& out, const Fraction2& f1) {
        out << f1.m_numerator << '/' << f1.m_denominator;
        return out;
    };

};


int main() {

  /**
   * Overloading the assignment operator (Перевантаження оператора присвоєння)
   * Copy assignment operator (operator=) використовується для копіювання значення з одного об'єкта у інший вже існуючий об'єкт
   *
   * Мета конструктора копіювання та оператора присвоювання копіювання майже однакова — обидва копіюють один об'єкт в інший.
   * Однак конструктор копіювання ініціалізує нові об'єкти, тоді як оператор присвоювання замінює вміст існуючих об'єктів.
   *
   * If a new object has to be created before the copying can occur, the copy constructor is used (note: this includes passing or returning objects by value).
   * If a new object does not have to be created before the copying can occur, the assignment operator is used.
   *
   * The copy assignment operator must be overloaded as a member function
   */

    Fraction fiveThirds { 5, 3 };
    Fraction f;
    f = fiveThirds; // calls overloaded assignment
    std::cout << f << '\n'; // 5/3


    // Наш перевантажений оператор operator= повертає *this, щоб ми могли об'єднати кілька присвоєнь
    Fraction f1 { 5, 3 };
    Fraction f2 { 7, 2 };
    Fraction f3 { 9, 5 };

    f1 = f2 = f3; // Два рази викличеться Overloaded assignment called


    /**
     * З перевантаженням оператора присвоєння ( assignment = ) може бути проблема, коли присвоюємо значення саме собі
     * Fraction f1 { 5, 3 };
     * f1 = f1; // self assignment
     *
     * Конкретно проблема може виникати коли є динамічна алокація пам'яті і там треба робити перевірку чи ми не присвоюємо
     * копію, бо як ми казали раніше оператор = працює так, що має видалити старі дані і дати їм нові значення, тобто
     * зробити delete і потім проставити у nullptr і після цього записати нові значення, але якщо у нас іде присвоєння
     * самого себе то буде проблема, тож треба робити перевірку чи this не дорівнює переданому значення (чи різні посилання).
     *
     * Але це упоратий випадок тож я його тут не буду детально розглядати
     */


    // Приклад з забороною присвоєння
    Fraction2 fraction { 5, 3 };
    Fraction2 fCopy;
    fCopy = fraction; // compile error, operator= has been deleted
    std::cout << fCopy;

    /**
     * Зверніть увагу, якщо ваш клас містить const поля, компілятор визначить сам  implicit operator= як deleted.
     * Це тому що const поля не можуть бути присвоєні (assigned), тож компілятор вирішить що ваш клас повинен бути
     * не присвоюваним.
     *
     * Якщо ми хочемо щоб наш клас з const полями був присвоюваним (для всіх полів окрім const) нам треба самим
     * перевантажити operator= і написати свою логіку
     */


  return 0;
}