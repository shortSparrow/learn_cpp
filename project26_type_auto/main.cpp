#include <iostream>

/**
 * auto - це спеціальне слово для автоматичного визначення типу
 *
 *
 * Може бути корисно у випадку коли ми змінили тип значення яке повертає
 * функція з int на double. Тобто у вас відбулося підвищення. Жодних проблем виникнути
 * не повинно, але ми маємо поміняти усі місця де функція застосовується.
 * А якби було auto то міняти не довелося б.
 *
 * int someFunc() {
 *  return 1;
 * }
 *
 * double someFunc() {
 *  return 1.0;
 * }
 *
 * int a = someFunc() - треба міняти на double
 */


// Тип буде автоматично int
auto add(const int a, const int b) {
    return a + b;
}

// Приклад коли типи аргументів не важливі
void addAndPrint(auto x, auto y) {
    std::cout << x + y << '\n';
}


/**
 *  Маємо помилку, оскільки функція повинна завжди повертати лише один тип.
 *  Навісь якщо у нас є автоматичні приведення від меншого типу до більшого,
 *  все одно повертатися має один тип
 */
auto someFunc(const bool x) {
    if (x) {
        return 5;
    } else {
        return 6.7;
    }
}


int main() {

    auto a {5}; // автоматично буде int, оскільки ми передали числовий літерал int 5
    auto b {5.2}; // автоматично буде double, оскільки ми передали числовий літерал double

    auto sum = add(1,3); // автоматично буде int

    addAndPrint(2, 3);
    return 0;
}

/**
 * Цікавою комбінацією є поєднання auto і явного вказання того, що поверне функція
 */

auto myFunc() -> int {
    return  1;
}

// Переваги використання auto + тип повернення

// 1. Простіше читати
std::common_type_t<int, double> compare(int, double);         // harder to read (where is the name of the function in this mess?)
auto compare(int, double) -> std::common_type_t<int, double>; // easier to read (we don't have to read the return type unless we care)

// 2. Можна вирівняти і легко читати назви функції не залежно від складності і довжини типу що повертається
auto subtract(int x, int y) -> int;
auto divide(double x, double y) -> double;
auto printSomething() -> void;
auto generateSubstring(const std::string &s, int start, int len) -> std::string;


// 3. Працює у випадку коли тип повернення залежить від типу вхідних параметрів
std::common_type_t<decltype(x), decltype(y)> add(int x, double y);         // Compile error: Компілятор ще не знає які будуть типу x та y
auto add(int x, double y) -> std::common_type_t<decltype(x), decltype(y)>; // ok