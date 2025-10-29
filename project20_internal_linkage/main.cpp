#include <iostream>

int add(int x, int y);
int substruct(int x, int y);

/**
 * substruct видасть помилку, тому що у foo.cpp ми далий
 * цій функції модифікатор static, отже ця функція буде видима
 * лише у межах файлу у якому вона була створена
 *
 * Є сенс додавати static для кожної функції і змінної, яку ми не пануємо
 * використовувати поза межами файлу.
 * Для цього також можна використати unnamed namespace
 * namespace {...}
 */
int main() {
    std::cout << add(1,2) << "\n";
    std::cout << substruct(5,2) << "\n";

    return 0;
}