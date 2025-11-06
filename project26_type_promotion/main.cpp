#include <iomanip>
#include <iostream>


/**
 *
 * Уявімо що моя функція приймає параметр double, а якщо я хочу передати
 * float мені треба писати нову функцію?
 *
 * Насправді ні, бо відбудеться підвищення типу (type promotion)
 * type promotion розділяється на дві категорії:
 *      1) floating point promotions
 *      2) integral promotions
 *
 * floating point promotions - перетворить float на double, це логічно, бо double
 * займає більше пам'яті, тож якщо перетворити float на double, то нічого не втратиться.
 *
 * integral promotions - перетворює цілі числові типи/char/bool на int
 * З integral promotions все дещо складніше, бо є sign short/int/... і unsigned short/int/...,
 * також є wchar_t/char8_t/... і інші більш специфічні типи
 *
 * Тож враховуючи що int займає не менше 4 byte то integral promotions можливе для типів:
 * bool, char, signed char, unsigned char, signed short, and unsigned short
 *
 */
void printDouble(double d){
    std::cout << d << '\n';
}

void printInt(int x) {
    std::cout << x << '\n';
}

int main() {
    printDouble(5.0); // no conversion necessary (double by default)
    printDouble(4.0f); // numeric promotion of float to double


    printInt(2); // 2

    short s{ 3 }; // there is no short literal suffix, so we'll use a variable for this one
    printInt(s); // 3 - numeric promotion of short to int

    printInt('a'); // 97 - numeric promotion of char to int
    printInt(true); // 1 - numeric promotion of bool to int



    /**
     * Numeric Conversion
     * Це ми розглянули type promotions, але C++ надає нам ще один механізм, який називається numeric conversion.
     *
     * Існує п'ять основних типів числових перетворень (numeric conversion)
     *  1. Перетворення одного integral type на інший integral type (не включаючи integral promotions):
     *      short s = 3; // convert int to short
     *      long l = 3; // convert int to long
     *      char ch = s; // convert short to char
     *      unsigned int u = 3; // convert int to unsigned int
     *
     *  2. Перетворення floating point type на інший floating point type (не включаючи floating point promotions):
     *      float f = 3.0; // convert double to float
     *      long double ld = 3.0; // convert double to long double
     *
     *  3. Перетворення floating point type на будь-який integral type:
     *      int i = 3.5; // convert double to int
     *
     *  4. Перетворення integral type на будь-який інший floating point type:
     *      double d = 3; // convert int to double
     *
     *  5. Перетворення integral type чи floating point type на bool:
     *      bool b1 = 3; // convert int to bool
     *      bool b2 = 3.0; // convert double to bool
     *
     *
     * Тут присвоєння відбувається через "=" оскільки якщо робити "{}" то можемо отримати помилку через
     * перевірку типів на початку ініціалізації значень
     *
     */

    // Безпечні числові перетворення (conversion). Жодні дані не втрачаються
    int n = static_cast<int>(static_cast<long>(3)); // convert int 3 to long and back
    std::cout << n << '\n';                         // prints 3

    char c = static_cast<char>(static_cast<double>('c')); // convert 'c' to double and back
    std::cout << c << '\n';                               // prints 'c'


    // Переосмислені перетворення (conversion). Дані не втрачаються, але отримані дані можуть мати інше значення
    int n1 { 5 };
    unsigned int u1 { n1 }; // okay: will be converted to unsigned int 5 (value preserved)

    int n2 { -5 };
    unsigned int u2 { n2 }; // bad: singed int перетворюється на unsigned int і відповідно біт який відповідав за "-" стане значенням - 4294967291
    std::cout << "Convert signed to unsigned: " << u2 << '\n'; // 4294967291

    // Таке значення може повернути назад
    int u = static_cast<int>(n2); // convert '-5' to unsigned and back
    std::cout << u << '\n'; // prints -5


    // Перетворення (conversion) з втратами – це небезпечні числові перетворення, під час яких дані можуть бути втрачені.
    int i = 3.0; // okay: will be converted to int value 3 (value preserved)
    int j = 3.5; // data lost: will be converted to int value 3 (fractional value 0.5 lost)

    float f = 1.2;        // okay: will be converted to float value 1.2 (value preserved)
    float g = 1.23456789; // data lost: will be converted to float 1.23457 (precision lost)


    // Додаткові приклади коли дані можуть бути втрачені, бо тими мають різі розміри, але якщо самі значення вписуються у діапазон, то втрат не буде
    // Перетворення цілого числа на число з плаваючою комою зазвичай працює, якщо значення поміщається в діапазон типу з плаваючою комою.
    int r{ 10 };
    float t = r;
    std::cout << t << '\n'; // 10

    // Перетворення числа з плаваючою комою на ціле число працює, якщо значення поміщається в діапазон цілого числа, але будь-які дробові значення втрачаються.
    int y = 3.5;
    std::cout << y << '\n'; // 3

    int o = 12345678912.23456;
    std::cout << o << '\n'; // -2147483648 - бо уже не влазить

    return 0;
}
