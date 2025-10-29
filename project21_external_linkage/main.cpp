#include <iostream>

int add(int x, int y);

extern const int a;
// extern const int b;
extern int c;
extern const int d;


/**
 * If you want to define an uninitialized non-const global variable, do not use the extern keyword,
 * otherwise C++ will think you’re trying to make a forward declaration for the variable.
 *
 * extern використовується у двох випадках:
 * 1. Створити external linkage, щоб змінну можна було імпортувати до іншого файлу
 * 2. Для forward declarations змінної у файлі куди буде відбуватися імпорт
 *
 * Приклади
 * Global variable forward declarations (extern w/ no initializer):
 *   extern int g_y;                 // forward declaration for non-constant global variable
 *   extern const int g_y;           // forward declaration for const global variable
 *   extern constexpr int g_y;       // not allowed: constexpr variables can't be forward declared
 *
 * External global variable definitions (no extern)
 *   int g_x;                        // defines non-initialized external global variable (zero initialized by default)
 *   int g_x { 1 };                  // defines initialized external global variable
 *
 * External const global variable definitions (extern w/ initializer)
 *   extern const int g_x { 2 };     // defines initialized const external global variable
 *   extern constexpr int g_x { 3 }; // defines initialized constexpr external global variable
 */
int main() {

    std::cout << add(1,2) << std::endl;
    std::cout << "a: " << a << std::endl; // 1
    // std::cout << "b: " << b << std::endl; // error, тому що b не була визначена як extern у foo.cpp
    std::cout << "c: " << c << std::endl; // 3
    std::cout << "d: " << d << std::endl; // 4


    return 0;
}