#include <cstdint>
#include <iostream>

enum Color {
    black,
    red,
    green,
    yellow
};

enum ExtraColor {
    pink,
    // red - не можна, оскільки буде конфлікт імен з Color, треба використовувати namespace
};


enum Roles {
    dev=5,
    qa='a',
    pm=-3,
};

// Оскільки тут стоїть namespace то не буде конфлікту з enum Color
namespace Paint {
    enum AdditionalColor {
        black,
        green,
        red,
    };
}

void some_func(const Color color) {
    // Автоматично підсвітить якщо додався якийсь елемент у Color, а тут його немає
    switch (color) {
        case black:
            break;
        case red:
            break;
        case green:
            break;
    }
}

/**
 * Оскільки enum використовує як значення числа, то більшість компіляторів використовую int,
 * однак у випадках коли вам треба більші, або менші тими можна визначити їх в ручну, однак
 * не факт що всі компілятори таке підтримують, і я не знаю нахуя це може комусь бути потрібним
 *
 *
 *
 * Оскільки std::int8_t та std::uint8_t зазвичай є псевдонімами типів для типів char, використання
 * будь-якого з цих типів як бази перерахувань, найімовірніше, призведе до того, що перечислителі
 * друкуватимуть значення char, а не цілочисельні значення
 */

enum MyColors : std::int8_t {
    purple,
    dark_red,
};


enum LargeColors : long {
    white,
};


enum Pet {
    cat, // assigned 0
    dog, // assigned 1
    pig, // assigned 2
    whale, // assigned 3
};

int main() {
    Color apple {Color::black};
    Color orange {green}; // теж можна
    Paint::AdditionalColor pickle {Paint::green};

    if (Color::red) {
        // do something
        // спрацює, бо red - це якась цифра 1

        std::cout << "color black: " << Color::black << '\n'; // 0
        std::cout << "color red: " << Color::red << '\n'; // 1
        std::cout << "color green: " << Color::green << '\n'; // 2
    }

    std::cout << "Roles dev: " << Roles::dev << '\n'; // 5
    std::cout << "Roles qa: " << Roles::qa << '\n'; // 97 - ascii code of 'a'
    std::cout << "Roles pm: " << Roles::pm << '\n'; // -3


    std::cout << "Size of int Color: " << sizeof(Color::black) << '\n'; // 4 byte
    std::cout << "Size of int8_t Color: " << sizeof(MyColors::purple) << '\n'; // 1 byte
    std::cout << "Size of long Color: " << sizeof(LargeColors::white) << '\n'; // 4 byte



    // Pet pet { 2 }; // compile error: integer value 2 won't implicitly convert to a Pet
    // pet = 3;       // compile error: integer value 3 won't implicitly convert to a Pet

    Pet pet2 { static_cast<Pet>(2) }; // convert integer 2 to a Pet
    std::cout << "Pet2 " << pet2 << '\n'; // 2
    pet2 = static_cast<Pet>(3);       // our pig evolved into a whale!
    std::cout << "new Pet2 " << pet2 << '\n'; // 3


    // Тут відбувається явне приведення типів до int. Оскільки і Color::black і ExtraColor::pink мають значення 1 то буде 1==1 => true
    std::cout << "black and pink are equal " << (Color::black == ExtraColor::pink) << '\n'; // 1 - тобто true



    return 0;
}