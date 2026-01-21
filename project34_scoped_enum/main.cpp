#include <iostream>

/**
 * Scoped enums майже такі саме, як і unscoped, різниця лише у формі запису,
 * наявності скопа і не відсутності implicit приведення значень до int
 *
 */


enum class Color {
    red, // red is considered part of Color's scope region
    blue,
};

enum class Fruit {
    red, // не буде name collision
    banana,
    apple,
};


enum UnscopedColor1 {
    purple,
};
enum UnscopedColor2 {
    yellow,
};

enum class Pet {
    cat, // assigned 0
    dog, // assigned 1
    pig, // assigned 2
    whale, // assigned 3
};


/**
 * Для enum теж можна використовувати using і це може бути дуже корисним
 * коли треба одномоментно отримати доступ до багатьох елементів одного enum
 */
constexpr std::string_view getPet(const Pet pet) {
    using enum Pet; // bring all Color enumerators into current scope (C++20)
    // We can now access the enumerators of Color without using a Color:: prefix

    switch (pet) {
        case cat: return "cat"; // note: black instead of Color::black
        case dog:   return "dog";
        case pig:  return "pig";
        case whale:  return "whale";
        default:    return "???";
    }
}

int main() {

    // Color color { Color::red };
    // Fruit color2 { Fruit::red };
    // Fruit fruit { Fruit::banana };
    //
    // if (color == fruit) // compile error: the compiler doesn't know how to compare different types Color and Fruit
    //     std::cout << "color and fruit are equal\n";
    // else
    //     std::cout << "color and fruit are not equal\n";
    //
    // if (color == Color::red) // this Color to Color comparison is okay
    //     std::cout << "The shirt is red!\n";

    // if (color == 0) // compile error
    //     std::cout << "The shirt is red!\n";


    // Scoped enum не приводяться неявно до int на відміну від unscoped enum
    // std::cout << (UnscopedColor1::purple == UnscopedColor2::yellow) << '\n'; // 1 - true бо буде 0 == 0
    // std::cout << (Color::red == Fruit::red) << '\n'; // error
    // std::cout << Color::red << '\n'; // буде помилка бо немає неявного приведення до int
    // std::cout << static_cast<int>(Color::red) << '\n'; // 0 - так буде працювати, бо приводимо до int через static_cast

    // Можна присвоїти enum на основі введених даних
    std::cout << "Enter a pet (0=cat, 1=dog, 2=pig, 3=whale): ";
    int input;
    std::cin >> input;

    const Pet my_pet {static_cast<Pet>(input)};
    const Pet my_pet2 {input};

    std::cout << getPet(my_pet) << '\n'; // for input 1 - dog


    return 0;
}