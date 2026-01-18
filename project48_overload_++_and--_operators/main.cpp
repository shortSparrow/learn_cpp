#include <iostream>

/**
 * Overloading ++ та -- оператори
 *
 * Цей процес схожий на інші перевантаження операторів, але є нюанс, у нас є
 * x++ та ++x і так само x-- та --x, тобто prefix increment and decrement та postfix increment and decrement
 *
 * Оскільки оператори increment та decrement є унарними операторами та змінюють свої операнди,
 * їх найкраще перевантажувати як member function.
 *
 */

class Digit {
private:
    int m_digit{};
public:
    Digit(int digit=0)
        : m_digit{digit}
    {
    }

    Digit& operator++() {
        // If our number is already at 9, wrap around to 0
        if (m_digit == 9)
            m_digit = 0;
        // otherwise just increment to next number
        else
            m_digit = m_digit+1;

        return *this;
    };

    Digit& operator--() {
        // If our number is already at 0, wrap around to 9
        if (m_digit == 0)
            m_digit = 9;
        // otherwise just decrement to next number
        else
            m_digit = m_digit-1;

        return *this;
    };


    /**
     * Тепер треба перевантажити postfix operator ++ та --, однак вони ж такі самі як і prefix operator,
     * тому у C++ дай таке рішення: Компілятор дивиться чи має перевантажений оператор параметр int, якщо
     * має, то це postfix, якщо немає, то це prefix
     */
    Digit operator++(int) {
        // Створюємо незалежну копію (передали *this, а не this)
        Digit temp{*this};

        // Use prefix operator to increment this digit
        ++(*this); // Застосовуємо prefix оператор ++ який описали вище

        // повертаємо незалежну копію, яка має старе значення
        return temp;
    }

    Digit operator--(int) {
        // Створюємо незалежну копію (передали *this, а не this)
        Digit temp{*this};

        // Use prefix operator to increment this digit
        --(*this); // Застосовуємо prefix оператор -- який описали вище

        // повертаємо незалежну копію, яка має старе значення
        return temp;
    }


    friend std::ostream& operator<< (std::ostream& out, const Digit& d) {
        out << d.m_digit;
        return out;
    }
};

int main() {
    std::cout << "************************** prefix operator ********************************\n";

    Digit digit { 8 };

    std::cout << digit << '\n';   // 8
    std::cout << ++digit << '\n'; // 9
    std::cout << ++digit << '\n'; // 0
    std::cout << --digit << '\n'; // 9
    std::cout << --digit << '\n'; // 8


    std::cout << "************************** podtfix operator ********************************\n";
    /**
     * І prefix і posfix оператори роблять однакові роботу. Різниця між ними полягає у значенні, яке вони повертають.
     * Перевантажені prefix оператори повертають об'єкт після того, як його значення було збільшено або зменшено.
     * А postfix спочатку об'єкт збільшують, а потім повертають.
     *
     * Тобто postfix оператори повинні повертати стан об'єкта перед тим, як його значення буде збільшено або зменшено.
     * З іншого боку, якщо ми повернемо стан об'єкта перед тим, як збільшимо або зменшимо його значення, інкремент або декремент ніколи не буде викликано.
     * Типовий спосіб вирішення цієї проблеми полягає у використанні тимчасової змінної, яка містить значення об'єкта до його збільшення або зменшення.
     * Потім сам об'єкт може бути збільшений або зменшений. І нарешті, тимчасова змінна повертається тому хто її викликав.
     * Таким чином, виклик отримує копію об'єкта до його збільшення або зменшення, але сам об'єкт збільшується або зменшується.
     * Зверніть увагу, що це означає, що значенням повернення перевантаженого оператора має бути не посилання, оскільки ми не можемо повернути
     * посилання на локальну змінну, яка буде знищена при виході з функції. Також зверніть увагу, що це означає, що постфіксні оператори зазвичай
     * менш ефективні, ніж префіксні оператори, через додаткові накладні витрати на створення тимчасової змінної та повернення за значенням, а не за посиланням.
     *
     * Тобто підсумуємо, у postfix ми свторили нелажену копію і повернули її, потім зміниили значення у оригінала на +1 чи -1 (залежно від ++ чи --)
     * і повернули копію зі старим значенням. Але оригінал вже має нове значення
     * Ниже наведені приклади з числом 5 і як саме працює postfix оператор
     */

    // int num1 {5};
    // std::cout << ++num1 << '\n'; // 6
    //
    // int num2 {5};
    // std::cout << num2++ << '\n'; // 5


    Digit digit2 { 5 };
    std::cout << digit2 << '\n';   // 5
    std::cout << ++digit2 << '\n'; // 6 calls Digit::operator++();
    std::cout << digit2++ << '\n'; // 6 calls Digit::operator++(int);
    std::cout << digit2 << '\n';   // 7
    std::cout << --digit2 << '\n'; // 6 calls Digit::operator--();
    std::cout << digit2-- << '\n'; // 6 calls Digit::operator--(int);
    std::cout << digit2 << '\n';   // 5

  return 0;
}