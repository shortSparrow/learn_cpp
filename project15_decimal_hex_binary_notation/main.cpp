#include <bitset>
#include <format>
#include <iostream>

int main() {

    // int num {12}; // decimal
    //
    // int num_oct {};
    // num_oct = 012; // 12 у 8-ми значній системі - це 10 у десятковій
    // std::cout << num_oct << std::endl; // 10
    //
    //
    // int num_hex {};
    // num_hex = 0xFF; // F - це 15 у decimal, а FF - 255 (Hex нотація це від 0 до 15)
    // std::cout << num_hex << std::endl; // 255
    //
    //
    // int num_binary{}; // алокує 2 байти, бо int, тож маємо 0000 0000 0000 0000 - 16 біт
    // num_binary = 0b101; // 2^2 + 2^0 = 5
    // std::cout << num_binary << std::endl; // 5
    //
    // int bin { 0b1011'0010 };  // великі числа можна розділяти знаком ', і це працює в будь якій системі числення


    // // Префікси, щоб показати число у потрібний системі числення
    // int int_num {12};
    // std::cout << std::hex << int_num << std::endl; // с
    // std::cout << int_num << '\n'; // с - тепер hexadecimal, доки не змінимо
    //
    // std::cout << std::oct << int_num << std::endl; // 14
    // std::cout << std::dec << int_num << std::endl; // 12
    // std::cout << std::dec << 0b111 << std::endl; // 7 - 2^0 + 2^1 + 2^2 = 7

    /**
     * Виведення значень у двійковому форматі трохи складніше, оскільки std::cout не має вбудованої такої можливості.
     * На щастя, стандартна бібліотека C++ містить тип під назвою std::bitset, який зробить це за нас (у заголовку <bitset>).
     */
    //
    // std::bitset<8> bin1 {0011};
    // std::cout << bin1 << std::endl; // 00001001
    // std::cout << std::dec << bin1 << std::endl; // 00001001
    //
    // std::bitset<2> bin2 {10};
    // std::cout << bin2 << std::endl; // 10
    //
    // std::bitset<8> bin3 {0xF};
    // std::cout << bin3 << std::endl; // 00001111
    //
    // std::bitset<4> bin4 {0b1010 };
    // std::cout << bin4 << std::endl; // 1010
    //
    //
    // // Формати виводу у C++20 і C++23
    // std::cout << std::format("{:b}\n", 0b1010);  // 1010 - C++20, {:b} formats the argument as binary digits
    // std::cout << std::format("{:#b}\n", 0b1010); // 0b1010 - C++20, {:#b} formats the argument as 0b-prefixed binary digits


    // Операції з бітами. Зазвичай ми оперуємо мінімумом байтом, але для bitset є виключення, тут ми можемо оперувати навіть одним бітом
    std::bitset<8> bits{ 0b0000'0001 }; // we need 8 bits, start with bit pattern 0000 0001
    bits.set(3);   // Призначити біт на позиції 3 у значення 1 (now we have 0000 1001)
    bits.set(4, true); // Призначити біт на позиції 4 у значення 1 (now we have 0001 1001)
    bits.set(4, false); // Призначити біт на позиції 4 у значення 1 (now we have 0000 1001)
    bits.flip(5);  // flip bit 4 (now we have 0010 1001)
    bits.reset(5); // set bit 4 back to 0 (now we have 0000 1101)
    std::cout << bits.test(0) << std::endl; // 1 - means true. На позиції 0 стоїть значення 1
    std::cout <<  bits.test(1) << std::endl; // 0 - means false. На позиції 1 стоїть значення 0

    return 0;
}