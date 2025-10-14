#include <cstdint>
#include <iostream>
using namespace std;

/**
 * static_cast - використовується для явного приведення типів
 *
 *
 */

int main() {
    // double num {5.3};
    // int num2 {static_cast<int>(num)}; // Тепер num2 це просто 5


    // // Приведення char до int
    // char ch{ 97 }; // 97 is ASCII code for 'a'
    // std::cout << ch << '\n'; // a, тому що 97 у ASCII це латинська літера a
    // std::cout << static_cast<int>(ch) << '\n'; // 97
    //
    // // Приведення unsigned int у signed int
    // unsigned int u1 { 5 };
    // int s1 { static_cast<int>(u1) }; // тепер це singed int, оскільки singed стоїть за замовченням у int
    //
    // // Приведення signed int у unsigned int
    // int s2 { 5 };
    // unsigned int u2 { static_cast<unsigned int>(s2) }; // перетворили у unsigned int

    // // Ось результат з переповненням
    // int s { -1 };
    // std::cout << static_cast<unsigned int>(s) << '\n'; // 4294967295
    //
    // unsigned int u { 4294967295 }; // largest 32-bit unsigned int
    // std::cout << static_cast<int>(u) << '\n'; // -1. Значення визначається реалізацією до C++20, і -1 починаючи з C++20

    // // Конвертація int8_t
    // std::int8_t my_int{65};
    // std::cout << my_int << '\n'; // A. Тому що int8_t часто опрацьовується як тип char
    // std::cout << static_cast<int>(my_int) << '\n'; // 65. Точно вказали що ми хочемо отримати int

    // У випадках, коли std::int8_t обробляється як char,
    // введення з консолі також може спричинити проблеми
    std::cout << "Enter a number between 0 and 127: ";
    std::int8_t my_int2{};
    std::cin >> my_int2; // Введемо число 35
    std::cout << "You entered: " << static_cast<int>(my_int2) << '\n'; // 51

    /**
     * Ось що відбувається. Коли std::int8_t розглядається як char, процедури введення інтерпретують
     * наше введення як послідовність символів, а не як ціле число. Тому, коли ми вводимо 35, ми насправді
     * вводимо два символи, '3' і '5'. Оскільки об'єкт char може містити тільки один символ, витягується '3'
     * (а '5' залишається у вхідному потоці для можливого витягування пізніше). Оскільки символ '3' має ASCII-код 51,
     * значення 51 зберігається в myInt, яке ми потім виводимо як int.
     */

    return 0;
}