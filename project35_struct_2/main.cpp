#include <iostream>


struct Employee {
    int id {};
    int age {};
    double wage {};
};

struct Company {
    int numberOfEmployees {};
    Employee CEO {}; // Employee is a struct within the Company struct
};


// Можна об'єднати у таку структуру
struct GlobalCompany {
    // accessed via Company::Employee
    struct Employee {
        int id{};
        int age{};
        double wage{};
    };

    int numberOfEmployees{};
    Employee CEO{}; // Employee is a struct within the Company struct
};


// getName() returns the user-entered string as a temporary std::string
// This temporary std::string will be destroyed at the end of the full expression
// containing the function call.
std::string getName() {
    std::cout << "Enter a name: ";
    std::string name{};
    std::cin >> name;
    return name;
}

int main() {

    Company myCompany{ 7, { 1, 32, 55000.0 } }; // Nested initialization list to initialize Employee
    std::cout << myCompany.CEO.wage << '\n'; // 55000 - print the CEO's wage

    GlobalCompany globalCompany{ 7, { 1, 32, 55000.0 } }; // Nested initialization list to initialize Employee
    std::cout << globalCompany.CEO.wage << '\n'; // 55000 - print the CEO's wage

    globalCompany.CEO.wage++;
    ++globalCompany.CEO.wage;
    std::cout << globalCompany.CEO.wage << '\n'; // 55002 - print the CEO's wage


    /**
     * Хорошою практикою буде гарантування того що struct або class є власником даних,
     * тобто отримує не reference/pointer, а самі дані.
     * Саме тому варто використовувати std::string, а не std::string_view
     *
     */

    struct Owner {
        std::string name{}; // std::string is an owner
    };

    struct Viewer {
        std::string_view name {}; // std::string_view is a viewer
    };

    Owner o { getName() };  // The return value of getName() is destroyed just after initialization
    std::cout << "The owners name is " << o.name << '\n';  // ok

    Viewer v { getName() }; // The return value of getName() is destroyed just after initialization
    std::cout << "The viewers name is " << v.name << '\n'; // undefined behavior




    // Size of struct
    struct Foo {
        short a {};
        int b {};
        double c {};
    };

    std::cout << "The size of short is " << sizeof(short) << " bytes\n"; // 2
    std::cout << "The size of int is " << sizeof(int) << " bytes\n"; // 4
    std::cout << "The size of double is " << sizeof(double) << " bytes\n"; // 8

    std::cout << "The size of Foo is " << sizeof(Foo) << " bytes\n"; // 16

    /**
     * short + int + double це 14 bytes, але розмір Foo 16 bytes!
     * Виявляється, ми можемо лише сказати, що розмір структури буде принаймні таким же великим, як розмір усіх змінних,
     * що вона містить. Але він може бути і більшим! З міркувань продуктивності компілятор іноді додає пробіли
     * в структури (це називається заповненням (padding)). Це робиться для того, щоб процесор швидше обррбив дані,
     * бо сучасні процесори швидше оброблюються дані коли вони кратні їх розрядності.
     *
     * Наприклад, на 32-бітній машині структура даних, що містить 16-бітове значення, за яким йде 32-бітове значення,
     * може мати 16-бітне доповнення між 16-бітним та 32-бітним значеннями, щоб вирівняти 32-бітове значення на 32-бітній межі.
     *
     * Хоча вирівнювання залежить від архітектури та компілятора, сучасні компілятори C++ (навіть на 32-бітних системах)
     * зазвичай вирівнюють int по 4 байти, а double по 8 байтів (якщо це можливо для даної архітектури)
     */



    struct Foo1 {
        short a{}; // will have 2 bytes of padding after a
        int b{};
        short c{}; // will have 2 bytes of padding after c
    };

    struct Foo2 {
        int b{};
        short a{};
        short c{};
    };
    std::cout << sizeof(Foo1) << '\n'; // prints 12
    std::cout << sizeof(Foo2) << '\n'; // prints 8

    /**
     * Зверніть увагу, що Foo1 та Foo2 мають однакові члени, єдина відмінність полягає в порядку оголошення.
     * Проте Foo1 на 50% більший через додане доповнення.
     */

    return 0;
}