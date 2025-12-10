#include <iostream>

/**
 * Friend function - це функція яка описана у класі, але її реалізація лежить поза класом і що важливо,
 * вона має доступ до усіх приватних змінних класу (див. функцію print).
 * Технічно можна зробити friend функцію і в середині класу (див. функцію print_v2)
 *
 * Важливо розуміти
 * ClassName::functionName — це створення методу класу (member function).
 * Friend (дружня функція) — це створення звичайної, вільної функції, яка не належить класу, але має права доступу.
 *
 * Як я розумію основна причина існування friend function - Проблема "Лівого операнда" (Математична причина)
 *
 * Уявіть, що ви робите клас BigInt (великі числа). Якщо ви зробите додавання методом
 * класу (ваш варіант з ClassName::operator+), то лівим операндом завжди має бути ваш об'єкт
 * BigInt a(100);
 * BigInt b = a + 50; // Працює: викликається a.operator+(50)
 * BigInt c = 50 + a; // ПОМИЛКА! Число 50 (int) не має методу .operator+(BigInt)
 *
 * Щоб вираз 50 + a працював, потрібна зовнішня функція (не член класу), яка приймає
 * два аргументи: operator+(int, BigInt). Оскільки це зовнішня функція, вона не бачить private полів BigInt.
 * Рішення: Зробити цю зовнішню функцію friend
 */

class Accumulator {
private:
    int m_value { 0 };

public:
    void add(int value) { m_value += value; }

    void test();

    // Here is the friend declaration that makes non-member function void print(const Accumulator& accumulator) a friend of Accumulator
    friend void print(const Accumulator& accumulator);

    // Friend functions defined inside a class are non-member functions
    friend void print_v2(const Accumulator& accumulator) {
        // Because print() is a friend of Accumulator
        // it can access the private members of Accumulator
        std::cout << accumulator.m_value;
    }
};

void print(const Accumulator& accumulator) {
    // Because print() is a friend of Accumulator
    // it can access the private members of Accumulator
    std::cout << accumulator.m_value;
}


/**
 * Приклад для вирішення проблеми лівого операнда
 */
class Number {
private:
    int value;

public:
    Number(int v) : value(v) {}

    int getValue() const { return value; }

    // 1. МЕТОД КЛАСУ (Member function)
    // Працює, коли об'єкт стоїть ЗЛІВА: (n1 + 10)
    // Цей метод викликається як: n1.operator+(10)
    Number operator+(int rightSide) {
        return Number(this->value + rightSide);
    }

    // 2. ДРУЖНЯ ФУНКЦІЯ (Friend function)
    // Працює, коли об'єкт стоїть СПРАВА: (10 + n1)
    // Це звичайна функція, не метод! Вона має доступ до приватного 'value'.
    friend Number operator+(int leftSide, const Number& rightSide);
};

// Реалізація дружньої функції (ззовні класу)
Number operator+(int leftSide, const Number& rightSide) {
    // Ми маємо прямий доступ до rightSide.value, бо ми friend
    return Number(leftSide + rightSide.value);
}

int main() {
    Accumulator acc{};
    acc.add(5); // add 5 to the accumulator

    print(acc); // call the print() non-member function


    //.....................................
    Number n1(50);

    // Сценарій А: Об'єкт зліва
    // Працює завдяки методу класу
    Number res1 = n1 + 10;
    std::cout << "n1 + 10 = " << res1.getValue() << std::endl;

    // Сценарій Б: Об'єкт справа (Проблема лівого операнда)
    // Працює ТІЛЬКИ завдяки friend-функції
    // Якби friend-функції не було, компілятор видав би помилку,
    // бо у числа '20' (тип int) немає методу .operator+(Number)
    Number res2 = 20 + n1;
    std::cout << "20 + n1 = " << res2.getValue() << std::endl;

    return 0;
}