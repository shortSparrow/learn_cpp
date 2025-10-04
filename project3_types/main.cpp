#include <iomanip>
#include <iostream>
#include <string>
/**
 * Більш детальний опис типів даних можна дістати тут https://devdocs.io/cpp/language/types
 *
 * ---------------------------------- STRINGS ---------------------------------------
 * char - 8 bits (1 byte) for one character (type for UTF-8 character representation).
 *        unsigned char - приймає значення від 0 до 255
 *                        Використовується для зберігання цілих чисел від 0 до 255.
 *                        Його основне призначення — робота з байтами даних, а не з символами
 *        signed char - приймає значення від -128 до 127
 *                      Використовується для зберігання малих цілих чисел від -128 до 127.
 *                      Цей тип може бути корисним для оптимізації пам'яті, коли тобі потрібно
 *                      зберегти невеликі від'ємні або додатні значення, і ти знаєш, що вони не вийдуть
 *                      за межі цього діапазону. Наприклад, для зберігання температури, невеликих зсувів,
 *                      або інших числових значень, які не потребують більшого типу, як int
 *
 *        Звичайний char може бути або signed char, або unsigned char, залежно від компілятора.
 *        Це одна з особливостей C++, яка може створювати проблеми з портативністю,
 *        якщо ти покладаєшся на його знаковий чи беззнаковий характер.
 *
 *
 * string - послідовність char (Наприклад є символ для запису якого треба кілька char
 *          Символ 你 вимагає аж 3 байти, тому він записується комбінацією трьох char що є string
 *
 * char16_t - 16 bits (2 byte) for one character (type for UTF-16 character representation)
 *            Доданий спеціально для роботи з UTF-16
 *
 * char32_t - 32 bits (4 byte) for one character (type for UTF-32 character representation)
 *            Доданий спеціально для роботи з UTF-32
 *
 * wchar_t - 32 bits для Linux і багатьох не Windows систем і 16 bits у Windows
 *           (type for wide character representation (see wide strings)).
 *           Його розмір залежить від реалізації компілятора
 *
 *
 * ---------------------------------- NUMBERS ---------------------------------------
 * singed - це числові значення які можуть бути як позитивними, так і негативними.
 * Типи такі як (like int, short, long, long long) signed за замовчуванням, тож це слово
 * можна не писати. Для них виділяється 1 bit пам'яті під знак
 *
 * signed short (int) - не менше 16 bits (це означає що максимальне число, яке можна сюди записати - це 2^15 - 1)
 * signed int - не менше 16 bits (це означає що максимальне число, яке можна сюди записати - це 2^15 - 1).
 *              На сучасних 64-бітних системах має 32 bits
 * signed long int - не менше 32 bits (це означає що максимальне число, яке можна сюди записати - це 2^31 - 1)
 *                   На сучасних 64-бітних системах Windows має 32 bits, а на Unix-подібних систем (Linux) 64 bits
 * signed long long int - не менше  64 bits (це означає що максимальне число, яке можна сюди записати - це 2^63 - 1)
 *                        Він використовується, коли потрібно працювати з дуже великими числами, які не поміщаються в long.
 *
 *
 *
 * unsigned - це числові значення які можуть бути лише позитивними і 0
 * Слово unsigned необхідно писати на відміну від signed
 *
 * unsigned short - 16 bits (це означає що максимальне число, яке можна сюди записати - це 2^16 - 1)
 * unsigned int - 16 bits (це означає що максимальне число, яке можна сюди записати - це 2^16 - 1)
 *              На сучасних 64-бітних системах має 32 bits
 * unsigned long int - 32 bits (це означає що максимальне число, яке можна сюди записати - це 2^32 - 1)
 *                   На сучасних 64-бітних системах Windows має 32 bits, а на Unix-подібних систем (Linux) 64 bits
 * unsigned long long int - 64 bits (це означає що максимальне число, яке можна сюди записати - це 2^63 - 1)
 *                        Він використовується, коли потрібно працювати з дуже великими числами, які не поміщаються в long.
 *
 *
 * Floating point type
 * float - 4 byte (може зберігати до 7 цифр)  ( від 1.2*10^-38 до 3.4*10^38 )
 * double - 8 byte (може зберігати до 15 цифр) ( від 2.2*10^-308 до 1.8*10^308 )
 * long double - 8, 12, or 16 bytes (може зберігати до 15 цифр) ( від 3.3*10^-4932 до 1.2*10^4932 )
 *
 * ---------------------------------- BOOLEAN ---------------------------------------
 * bool - 8 bits (true або false)
 *        Варто зауважити 0 - false, будь яке не 0 значення - це true
 *
 *
 * Присвоєння змінних через '=' не має помилки при запуску якщо дані перевищують пам'ять яка
 * зарезервована під змінну, до прикладу якщо я у short покладу 1000000000 так
 * short a = 1000000000 то я отримую лише warning на етапі компіляції, і компілятор автоматично
 * підніме розмір short a до більшого a щоб вмістити дані. Але тип лишається short
 * Однак якщо я напишу так
 * short a {1000000000} то на етапі компіляції я отримую помилку що пам'яті недостатньо
 *
 *
 *
 */
int main() {
    // ---------------------------------- STRINGS ---------------------------------------
    // std::cout << "Size of char: " << sizeof(char) << std::endl; // Size of char: 1
    // std::cout << "Size of char: " << sizeof(signed char) << std::endl; // Size of char: 1
    // std::cout << "Size of char: " << sizeof(unsigned char) << std::endl; // Size of char: 1
    // std::cout << "Size of char: " << sizeof(char16_t) << std::endl; // Size of char: 2
    // std::cout << "Size of char: " << sizeof(char32_t) << std::endl; // Size of char: 4
    // std::cout << "Size of char: " << sizeof(wchar_t) << std::endl; // Size of char: 2
    //
    //
    // char a0 = 'A'; // English A
    // // char I = 'Я'; // Українська Я - займає більше пам'яті ніж може вмістити char (видає помилку). Вона не входить в ASCII
    // std::cout << "Size of A char: " << sizeof(a0) << std::endl; // Вивело 1 Тобто 1 byte (8 bit)
    //
    // unsigned char a1 = 'A';  // English A
    // std::cout << "Size of A unsigned char: " << sizeof(a1) << std::endl; // Вивело 1 Тобто 1 byte (8 bit)
    // signed char a2 = 'A';  // English A
    // std::cout << "Size of A signed char: "  << sizeof(a2) << std::endl; // Вивело 1 Тобто 1 byte (8 bit)
    //
    //
    // std::string I = "Я";
    // std::cout << "Size of Ukrainian I string: " << sizeof("Я") << std::endl; // Вивело 3 Тобто 3 byte (32 bit)
    // std::cout  << "Size of Ukrainian I string: " << sizeof(I) << std::endl; // Вивело 32 Тобто 32 byte
    // видасть 32 (на 64-бітній системі), тому що це розмір самого об'єкта std::string, а не вмісту рядка
    //
    //
    // char16_t a3  = 'A';
    // std::cout  << "Size of A char16_t: " << sizeof(a3) << std::endl; // Вивело 2, Тобто 2 byte (16 bit)
    // char16_t I1  = 'Я';
    // std::cout  << "Size of Ukrainian I char16_t: " << sizeof(I1) << std::endl; // Вивело 2, Тобто 2 byte (16 bit). Помилки вже немає при компіляції
    //
    // char32_t a4  = 'A';
    // std::cout  << "Size of A char32_t: " << sizeof(a4) << std::endl; // Вивело 4, Тобто 4 byte (64 bit)
    //
    // wchar_t a5  = L'A';
    // std::cout  << "Size of A wchar_t: " << sizeof(a5) << std::endl; // Вивело 2, Тобто 2 byte (16 bit)
    // wchar_t I2  = L'Я';
    // std::cout  << "Size of Ukrainian I wchar_t: " << sizeof(I2) << std::endl; // Вивело 2, Тобто 2 byte (16 bit)


    // ---------------------------------- INT NUMBERS ---------------------------------------
    // Finding the size of different data types
    // std::cout << "Size of int: " << sizeof(int) << std::endl; // Size of int: 4
    // std::cout << "Size of unsigned int: " << sizeof(unsigned int) << std::endl; // Size of int: 4
    // std::cout << "Size of float: " << sizeof(float) << std::endl; // Size of float: 4
    // std::cout << "Size of double: " << sizeof(double) << std::endl; // Size of double: 8
    // std::cout << "Size of long: " << sizeof(long); // Size of long: 4
    //
    //
    // short a = 1;
    // signed short a1 = 1;
    // short int a2 = 1;
    // signed short int a3 = 1;
    // // Це все однакові записи
    // std::cout << "Size of a: " << sizeof(a) << std::endl; // Size of a: 2
    // std::cout << "Size of a1: " << sizeof(a1) << std::endl; // Size of a1: 2
    // std::cout << "Size of a2: " << sizeof(a2) << std::endl; // Size of a2: 2
    // std::cout << "Size of a3: " << sizeof(a3) << std::endl; // Size of a3: 2
    //
    // short a4 = 131072; // 131072 - це 17 bit. На етапі компіляції видається warning.
    // //  warning: overflow in conversion from 'int' to 'short int' changes value from '131072' to '0' [-Woverflow]
    // //  short a4 = 131072; // 131072 - це 17 bit.
    // std::cout << "Size of short b: " << sizeof(a4) << std::endl; // Size of a4: 4.
    // // Хитрість в, тому що компілятор бачить, що a4 перевищує short, тому він його
    // // підвищує. І саме тому він починає займати 4 byte (32 bit), отже тепер це long
    // std::cout << "Ty a4: " << typeid(a4).name() << std::endl;
    //
    //
    // int b = 1;
    // std::cout << "Size of int b: " << sizeof(b) << std::endl; // Size of int b: 4
    //
    //
    // long с {100'000'000'000}; // error: narrowing conversion of '100000000000' from 'long long int' to 'long int' [-Wnarrowing]
    // При такому присвоєнні отримую помилку при компіляції, натомість при = отримую лише warning
    // long с = 100'000'000'000; // warning: overflow in conversion from 'long long int' to 'long int' changes value from '100000000000'
    // std::cout << "Size of long c: " << sizeof(с) << std::endl; // Size of long: 4
    //
    //
    // long long d = 100'000'000'000; // ' допомагає краще читати число, це щось по типу _ у JS і Python
    // std::cout << "Size of long long d: " << sizeof(d) << std::endl; // Size of long long d: 8
    //
    //
    // short a1 = -65000; // warning бо перевищили ліміт для short (бо знак - вимагає додаткового біту)
    // //  warning: overflow in conversion from 'int' to 'short int' changes value from '-65000' to '536' [-Woverflow]
    // unsigned short a2 = 65000;
    // std::cout << "Size of signed short a1: " << sizeof(a1) << std::endl; // Size of signed short a1: 2
    // std::cout << "Size of unsigned short a2: " << sizeof(a2) << std::endl; // Size of signed short a2: 2


    // short a {1000};
    // short b {30000};
    // short sum = a * b;
    // std::cout << "Result of sum: " << sum << std::endl; // -15488
    // // Ця помилка обчислення виникає, бо отриманий результат занадто великий, щоб міститися у short
    // // Важливе розуміння що коли два short множаться то вони автоматично приводяться до більшого типу (int)
    // // щоб уникнути помилки перевантаження

    // short a {1000};
    // short b {30000};
    // // short sum = {a * b}; // Помилка Cannot initialize local variable 'sum' of type short with int: narrowing conversion
    // int sum = {a * b}; //
    // std::cout << "Result of sum: " << sum << std::endl; // Result of sum: 30000000

    // int a {1000000000};
    // int b {300000000};
    // int sum = {a * b}; //
    // std::cout << "Result of sum: " << sum << std::endl; // Result of sum: 413007872
    // // Тож тут треба вже писати long long sum, щоб не було помилки

    // // Можна також описати типи через константи U/L/LL
    // std::cout << "Unsigned integer: " << 12U << std::endl;
    // std::cout << "Long integer: " << 12L << std::endl;
    // std::cout << "Long Long integer: " << 12LL << std::endl;


    // int num {5};
    // std::cout << num << std::endl; // 5
    // num = -num;
    // std::cout << num << std::endl; // -5, тому що - перед числом робить його негативним

    // int num2 {-5};
    // std::cout << num2 << std::endl; // -5
    // num2 = +num2;
    // std::cout << num2 << std::endl; // -5. З оператором + ТАК НЕ ПРАЦЮЄ


    // ---------------------------------- FLOAT NUMBERS ---------------------------------------

    // float a {1.0};
    // std::cout << "Size of float a: " << sizeof(a) << std::endl; // Size of float a: 4
    // float a1 {1.23456789};
    // float b1 {1234.56789};
    // std::cout << "Value a1 is: " << a1 << std::endl; // Value a1 is 1.23457
    // std::cout << "Value b1 is: " << b1 << std::endl; // Value a1 is 1234.57
    //
    //
    // float d1 {1.2345678910f}; // f в кінці автоматично робить число float
    // double d2 {1.2345678910}; // f немає тому це double
    // std::cout << std::setprecision(17); // show 17 digits of precision
    // std::cout << d1 <<'\n'; // 1.2345678806304932
    // std::cout << d2 <<'\n'; // 1.234567891
    // std::cout << 3.33333333333333333333333333333333333333f <<'\n'; // f suffix means float 3.3333332538604736
    // std::cout << 3.33333333333333333333333333333333333333 << '\n'; // no suffix means double 3.3333333333333335
    // // Тут наочно видно різницю в точності між float і double


    /**
     * float дає неточність, з цим все зрозуміло. Але double теж цікаво округлює
     * 1.23456789 закінчується на 9 і замість 9 ми отримали 899999999, а числа до того збережені правильно
     * 1.2345678901 закінчується на 1 і щамість 1 ми отримали 0999999, а числа до того збережені правильно
     * Тож треба бути обережним з останнім знаком, бо навіть якщо тим даних може його вмастити його може
     * неправильно записати (лише якщо останній)
     */
    // std::cout << std::setprecision(17);
    // std::cout << 1.23456789f <<'\n'; // 1.2345678806304932
    // std::cout << 1.23456789 << '\n'; // 1.2345678899999999
    //
    // std::cout << 1.2345678901f <<'\n'; // 1.2345678806304932
    // std::cout << 1.2345678901 << '\n'; // 1.2345678900999999

    // // Додавання double
    // std::cout << std::setprecision(17);
    // double d2{ 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 }; // should equal 1.0
    // std::cout << d2 << '\n'; // 0.99999999999999989


    // // Можна також описати типи через константи F/L для float/double
    // std::cout << "Float: " << 12.F << std::endl;
    // std::cout << "Float: " << 12.3F << std::endl;
    // std::cout << "Double Long: " << 12.L << std::endl;

    // ---------------------------------- BOOLEAN ---------------------------------------
    // // На внутрішньому рівні, bool перетворюються на інші числові типи, true стає 1, а false стає 0.
    // bool a = {true == 1};
    // bool b = {true == 23}; // true перетворюється на 1 і маємо 1 == 23 => false
    // std::cout << a << '\n'; // 1
    // std::cout << b << '\n'; // 0
    // std::cout << true << '\n'; // 1
    // std::cout << false << '\n'; // 0
    //
    // if (23) {
    //     std::cout << "23 is true" << '\n'; // 23 is true
    // }
    //
    // // A перетворюється на свій числовий код у ASCII - 65, далі це сприймається як true
    // if ('A') {
    //     std::cout << "A is true" << '\n'; // A is true
    // }

    // ---------------------------------- EXTRA ---------------------------------------

    // double zero { 0.0 };
    //
    // double posinf { 5.0 / zero }; // ifn (positive infinity)
    // std::cout << posinf << '\n';
    //
    // double neginf { -5.0 / zero }; // -ifn (negative infinity)
    // std::cout << neginf << '\n';
    //
    // double z1 { 0.0 / posinf }; // 0
    // std::cout << z1 << '\n';
    //
    // double z2 { -0.0 / posinf }; // -0
    // std::cout << z2 << '\n';
    //
    // double nan { zero / zero }; // nan (not a number (mathematically invalid))
    // std::cout << nan << '\n';



    return 0;
}
