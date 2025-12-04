#include <iostream>

class Fraction {
private:
    int m_numerator{ 0 };
    int m_denominator{ 1 };

public:
    // Default constructor
    Fraction(int numerator=0, int denominator=1)
        : m_numerator{numerator}, m_denominator{denominator}
    {
    }

    // Delete the copy constructor so no copies can be made. Заборонить використоувати constructor copy
    // Fraction(const Fraction& fraction) = delete;

    void print() const {
        std::cout << "Fraction(" << m_numerator << ", " << m_denominator << ")\n";
    }
};

class FractionWithConstructorCopy {
private:
    int m_numerator{ 0 };
    int m_denominator{ 1 };

public:
    // Default constructor
    FractionWithConstructorCopy(int numerator=0, int denominator=1)
        : m_numerator{numerator}, m_denominator{denominator}
    {
    }

    FractionWithConstructorCopy(const FractionWithConstructorCopy& item):
        m_numerator {item.m_numerator},
        m_denominator {item.m_denominator}
    {
        std::cout << "Copy constructor called\n"; // just to prove it works
    }

    void print() const {
        std::cout << "Fraction(" << m_numerator << ", " << m_denominator << ")\n";
    }
};

int main() {
    Fraction f { 5, 3 };  // Fraction(5, 3)
    Fraction fCopy { f }; // Fraction(5, 3)

    /**
     * Fraction f { 5, 3 } працює як і очікується
     * Fraction fCopy { f } працює неочіквано, саме тут і відбувається виклик копії конструктора
     * Конструктор копіювання (copy constructor) — це конструктор, який використовується для ініціалізації об'єкта
     * існуючим об'єктом того ж типу. Після виконання конструктора копіювання новостворений об'єкт має бути копією об'єкта,
     * переданого як ініціалізатор.
     *
     * Якщо ви не надасте конструктор копіювання для своїх класів, C++ створить для вас публічний неявний
     * конструктор копіювання. У наведеному вище прикладі оператор Fraction fCopy { f }; викликає неявний конструктор
     * копіювання для ініціалізації fCopy за допомогою f.
     *
     * Коли система робить copy constructor за мене то вона ініціалізує зміни у новому класі тими ж значеннями що лежать
     * у змінних в оригінальному класі, тобто в нашому випадку numerator ініціалузовано значеням 5, а denominator - 3.
     *
     * Краще використовувати implicit copy construcotr, тобто коли C++ робить його за нас ніє писати самому!
     */

    f.print();
    fCopy.print();



    FractionWithConstructorCopy f2 { 7, 1 };  // Calls Fraction(int, int) constructor
    FractionWithConstructorCopy f2Copy { f2 }; // Calls Fraction(const Fraction&) copy constructor

    f2.print();
    f2Copy.print();

    return 0;
}