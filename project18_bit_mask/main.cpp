#include <bitset>
#include <cstdint>
#include <iostream>


/**
 * Bit mask потрібна для того щоб застосувати зміни тільки до певних бітів,
 * уявимо ми хочемо застосувати зміни інверсії тільки для парних бітів.
 * bits 0b00000000
 * mask 0b10101010
 * bits = bits ^ mask = 10101010
 *
 * Все що у масці позначено як 0 перетвориться на 0, все що позначено як 1 повернеться без змін - це
 * основна логіка масок
 */
int main() {
    constexpr std::uint8_t mask0{ 0b00000001 }; // represents bit 0
    constexpr std::uint8_t mask1{ 0b00000010 }; // represents bit 1
    constexpr std::uint8_t mask2{ 0b00000110 }; // represents bit 1 and 2
    constexpr std::uint8_t mask3{ 0x08 }; // hex for 0000 1000
    constexpr std::uint8_t mask4{ 0b10101010 };


    std::bitset<8> flags{ 0b00000101 };

    // Setting bits
    std::cout << "---------RESTING BITS---------" << "\n";
    // 1. Поставити 1 усюди де у mask теж стоять 1
    flags |= mask1;  // mask1 = 0b00000010
    std::cout << flags << "\n"; // 0b00000111



    // Resetting bits
    std::cout << "---------RESTING BITS---------" << "\n";
    // 1. зробити всі біти 0
    std::bitset<8> bits1 {0b01100011};
    bits1 &= ~bits1;
    std::cout << bits1 << "\n"; // 00000000

    std::uint8_t bits2 {0b01100011};
    bits2 &= ~bits2;
    std::cout << bits2 << "\n"; // Нічого не вивело, бо результат 0b01100011 тобто для uint - це просто 0, а оскільки uint8_t сприймається як char то 0 код в ASCII - NULL

    // 2. зробити біти 0 тільки які у масці позначені як 1
    flags = 0b01111111;
    flags &= ~mask2; // ~mask2 = 0b11111001
    std::cout << flags << "\n";  // 0b01111001




    // Flipping a bit
    // 1. Інвертувати у flags біт, які у mask дорівнюють 1
    std::cout << "---------INVERT BITS---------" << "\n";
    flags = 0b00000000;
    flags ^= mask4; // mask1 = 0b10101010
    std::cout << flags << "\n"; // 0b10101010

    flags = 0b00111100;
    flags ^= mask4;  // mask1 = 0b10101010
    std::cout << flags << "\n"; // 0b10010110



    // Цікавий приклад як можна це використати для запуси інформації
    std::cout << "---------INTERESTING CASE---------" << "\n";
    [[maybe_unused]] constexpr std::uint8_t isHungry   { 1 << 0 }; // 0000 0001
    [[maybe_unused]] constexpr std::uint8_t isSad      { 1 << 1 }; // 0000 0010
    [[maybe_unused]] constexpr std::uint8_t isMad      { 1 << 2 }; // 0000 0100
    [[maybe_unused]] constexpr std::uint8_t isHappy    { 1 << 3 }; // 0000 1000
    [[maybe_unused]] constexpr std::uint8_t isLaughing { 1 << 4 }; // 0001 0000
    [[maybe_unused]] constexpr std::uint8_t isAsleep   { 1 << 5 }; // 0010 0000
    [[maybe_unused]] constexpr std::uint8_t isDead     { 1 << 6 }; // 0100 0000
    [[maybe_unused]] constexpr std::uint8_t isCrying   { 1 << 7 }; // 1000 0000

    std::uint8_t me{}; // all flags/options turned off to start
    me |= (isHappy | isLaughing); // I am happy and laughing
    me &= ~isLaughing; // I am no longer laughing

    std::cout << std::boolalpha; // print true or false instead of 1 or 0
    std::cout << "I am happy? " << static_cast<bool>(me & isHappy) << '\n'; // true
    std::cout << "I am laughing? " << static_cast<bool>(me & isLaughing) << '\n'; // false


    // Цікавий приклад з кльорами, як отримати значення red, greeb, blue, alpha
    std::cout << "---------PARSE HEX COLORS---------" << "\n";

    constexpr std::uint32_t redBits{ 0xFF000000 };
    constexpr std::uint32_t greenBits{ 0x00FF0000 };
    constexpr std::uint32_t blueBits{ 0x0000FF00 };
    constexpr std::uint32_t alphaBits{ 0x000000FF };

    std::cout << "Enter a 32-bit RGBA color value in hexadecimal (e.g. FF00FFAC): ";
    std::uint32_t pixel{};
    std::cin >> std::hex >> pixel; // std::hex allows us to read in a hex value

    // use Bitwise AND to isolate the pixels for our given color,
    // then right shift the value into the lower 8 bits
    const std::uint8_t red{ static_cast<std::uint8_t>((pixel & redBits) >> 24) }; // Робимо right shift на 24 біти, бо red займає перші 8 біт, а решта 24 нам не потрібні, тож приберемо їх
    const std::uint8_t green{ static_cast<std::uint8_t>((pixel & greenBits) >> 16) };
    const std::uint8_t blue{ static_cast<std::uint8_t>((pixel & blueBits) >> 8) };
    const std::uint8_t alpha{ static_cast<std::uint8_t>(pixel & alphaBits) };

    std::cout << "Your color contains:\n";
    // std::cout << std::hex; // print the following values in hex

    // reminder: std::uint8_t will likely print as a char
    // we static_cast to int to ensure it prints as an integer
    std::cout << static_cast<int>(red)   << " red\n"; // 255
    std::cout << static_cast<int>(green) << " green\n"; // 0
    std::cout << static_cast<int>(blue)  << " blue\n"; // 255
    std::cout << static_cast<int>(alpha) << " alpha\n"; // 172



    return 0;
}