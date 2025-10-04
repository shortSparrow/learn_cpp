#include <iostream>

/**
 * climint - надає інформацію про цілочисленні тими
 * cfloats - надає інформацію про float типи
 *
 * Також ці файли надають додаткові константи такі як INT_MAX, INT_MIN,...
 *
 *
 */
int main() {
    /**
     * sizeof дасть мені інформацію скільки пам'яті буде зарезервовано під змінну
     * саме на МОЄМУ комп'ютері з МОЇМ компілятором (на інших можуть бути інші значення, див. документацію)
     */
    // std::cout << "char: " << sizeof(char) << " bytes" << std::endl; // char: 1 bytes
    // std::cout << "short: " << sizeof(short) << " bytes" << std::endl; // short: 2 bytes
    // std::cout << "unsigned short: " << sizeof(unsigned short) << " bytes" << std::endl; // unsigned short: 2 bytes
    // std::cout << "int: " << sizeof(int) << " bytes" << std::endl; // int: 4 bytes
    // std::cout << "long: " << sizeof(long) << " bytes" << std::endl; // int: 4 bytes
    // std::cout << "long long: " << sizeof(long long) << " bytes" << std::endl; // int: 8 bytes
    // std::cout << "float: " << sizeof(float) << " bytes" << std::endl; // float: 4 bytes
    // std::cout << "double: " << sizeof(double) << " bytes" << std::endl; // double: 8 bytes


    // INT_MAX - вказує на те, наскільки велике число ми можемо зберегти у int на цій машині
    // std::cout << "INT_MAX: " << INT_MAX << std::endl; // 2147483647
    /**
     * sizeof(int) - 4 bytes (32 bits), отже ми можемо зберегти у ньому 2^32 = 4 294 967 296
     * Однак це unsigned int, тож ми маєо відняти 1 bit під знак, і тоді матимемо 2^31 = 2 147 483 647
     */

    // Minimum values (які мінімальні значення на цій машині з цим компілятором я можу записати у різні типи даних)
    std::cout << "==============================" << std::endl;
    std::cout << "CHAR_MIN: " << CHAR_MIN << std::endl; // -128
    std::cout << "SHRT_MIN: " << SHRT_MIN << std::endl; // -32768
    std::cout << "INT_MIN: " << INT_MIN << std::endl; // -2147483648
    std::cout << "LONG_MIN: " << LONG_MIN << std::endl; // -2147483648
    std::cout << "LLONG_MIN: " << LLONG_MIN << std::endl; // -9223372036854775808


    // Maximum values (які максимальні значення на цій машині з цим компілятором я можу записати у різні типи даних)
    std::cout << "==============================" << std::endl;
    std::cout << "CHAR_MAX: " << CHAR_MAX << std::endl; // 127
    std::cout << "SHRT_MAX: " << SHRT_MAX << std::endl; // 32767
    std::cout << "INT_MAX: " << INT_MAX << std::endl; // 2147483647
    std::cout << "LONG_MAX: " << LONG_MAX << std::endl; // 2147483647
    std::cout << "LLONG_MAX: " << LLONG_MAX << std::endl; // 9223372036854775807


    // EXTRA

    int age {27};
    double age2 {27.0};
    std::cout << "==============================" << std::endl;
    std::cout << "age is: " << sizeof(age) << " bytes" << std::endl; // age is: 4 bytes
    std::cout << "age is: " << sizeof age2 << " bytes" << std::endl; // age is: 8 bytes


    return 0;
}