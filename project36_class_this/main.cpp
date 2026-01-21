#include <iostream>

/**
 * Коли ми викликаємо якийсь метод на класі і він оперує з внутрішніми даними то під капотом він
 * використовує this. This - це pointer який посилається на адресу поточного об'єкту.
 * void print() const { std::cout << m_id; }       // implicit use of this
 * void print() const { std::cout << this->m_id; } // explicit use of this
 *
 * Розглянемо приклад
 * simple.setID(2);
 * Хоча виклик функції setID(2) виглядає так, ніби він має тільки один аргумент, насправді він має два!
 * Під час компіляції компілятор переписує вираз simple.setID(2); наступним чином:
 * Simple::setID(&simple, 2); // note that simple has been changed from an object prefix to a function argument!
 *
 * Тож рядок
 * void setID(int id) { m_id = id; }
 * компілятор перепише
 * static void setID(Simple* const this, int id) { this->m_id = id; }
 */



/**
 * Чому ми повертаємо *this, а не просто this, ми ж як тип повернення вказали Сalc&
 * Посилання (&) у C++ може бути прив'язане тільки до самого об'єкта (l-value), а не до його адреси.
 * Тому нам потрібно перетворити адресу (this) назад в об'єкт за допомогою оператора розіменування (*).
 */
class Calc {
private:
    int m_value{};

public:
    Calc& add(int value) { m_value += value; return *this; }
    Calc& sub(int value) { m_value -= value; return *this; }
    Calc& mult(int value) { m_value *= value; return *this; }

    int getValue() const { return m_value; }

    // return to default state
    void reset() { *this = {}; }
};

int main() {

    // повертати this як reference може бути корисним для таких випадків
    Calc calc{};
    calc.add(5).sub(3).mult(4); // method chaining

    std::cout << calc.getValue() << '\n';


    /**
     * Resetting a class back to default state
     * If your class has a default constructor, you may be interested in providing a way to return
     * an existing object back to its default state.
     *
     * void reset() {
     *      *this = {}; // value initialize a new object and overwrite the implicit object
     * }
     *
     * Функція створює новий об'єкт (використовуючи конструктор за замовчуванням),
     * а потім присвоює цей новий об'єкт поточному неявному об'єкту
     */

    return 0;
}