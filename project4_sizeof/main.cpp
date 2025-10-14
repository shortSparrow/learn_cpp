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

    // // Minimum values (які мінімальні значення на цій машині з цим компілятором я можу записати у різні типи даних)
    // std::cout << "==============================" << std::endl;
    // std::cout << "CHAR_MIN: " << CHAR_MIN << std::endl; // -128
    // std::cout << "SHRT_MIN: " << SHRT_MIN << std::endl; // -32768
    // std::cout << "INT_MIN: " << INT_MIN << std::endl; // -2147483648
    // std::cout << "LONG_MIN: " << LONG_MIN << std::endl; // -2147483648
    // std::cout << "LLONG_MIN: " << LLONG_MIN << std::endl; // -9223372036854775808
    //
    //
    // // Maximum values (які максимальні значення на цій машині з цим компілятором я можу записати у різні типи даних)
    // std::cout << "==============================" << std::endl;
    // std::cout << "CHAR_MAX: " << CHAR_MAX << std::endl; // 127
    // std::cout << "SHRT_MAX: " << SHRT_MAX << std::endl; // 32767
    // std::cout << "INT_MAX: " << INT_MAX << std::endl; // 2147483647
    // std::cout << "LONG_MAX: " << LONG_MAX << std::endl; // 2147483647
    // std::cout << "LLONG_MAX: " << LLONG_MAX << std::endl; // 9223372036854775807
    //
    //
    // // EXTRA
    //
    // int age {27};
    // double age2 {27.0};
    // std::cout << "==============================" << std::endl;
    // std::cout << "age is: " << sizeof(age) << " bytes" << std::endl; // age is: 4 bytes
    // /**
    //  * тому що int займає 4 byte, навіть якщо число 27 займає всього 5 bit
    //  * 0  0 0 0 0
    //  * 16 8 4 2 1  => 2^0 + 2^1 + 2^3 + 2^4 = 27
    //  */
    // std::cout << "age is: " << sizeof age2 << " bytes" << std::endl; // age is: 8 bytes


    // ТИП ДАНИХ std::size_t
    std::cout << sizeof(int) << '\n'; // 4 bytes

    /**
     * Доволі передбачувано що повернуло 4, але sizeof повертає кількість байт
     * які займає переданий аргумент, але потриманий результат треба десь зберегти,
     * так от якого типу буде цей результат? Ми є маємо алокувати певну кількість байт під цей
     * тип даних, а ми не знаємо, що нам повернеться, int, short чи щось інше?
     * Отож результат sizeof буде тип даних std::size_t.
     * std::size_t — це псевдонім для беззнакового цілочисельного типу, визначеного реалізацією.
     * Він використовується в стандартній бібліотеці для представлення розміру в байтах або довжини об'єктів.
     *
     * auto s = sizeof(int); - тут s автоматично отримує тип std::size_t
     *
     * std::size_t гарантовано буде беззнаковим та матиме щонайменше 16 біт, але на більшості систем буде
     * еквівалентним ширині адреси застосунку. Тобто, для 32-бітних застосунків std::size_t зазвичай буде
     * 32-бітним беззнаковим цілим числом, а для 64-бітного застосунку std::size_t зазвичай буде 64-бітним
     * беззнаковим цілим числом.
     *
     * Оператор sizeof повинен мати можливість повертати розмір об'єкта в байтах як значення типу std::size_t.
     * Таким чином, розмір об'єкта в байтах не може бути більшим за найбільше значення, яке може містити std::size_t.
     * Ось що говорить специфікація: Побудова типу таким чином, що кількість байтів у його об'єктному представленні перевищує
     * максимальне значення, яке можна представити в типі std::size_t (17.2), є неправильною
     */

    std::cout << sizeof(std::size_t) << '\n'; // 8
    // (Ми можемо використовувати оператор sizeof (який повертає значення типу std::size_t),
    // щоб запитати розмір самого std::size_t:



    return 0;
}