#include <iostream>
#include <iomanip>

/**
 * Форматування
 * Існує два способи змінити параметри форматування: прапорці (flags) та маніпулятори (manipulators).
 * Прапорці (flags) можна розглядати як логічні змінні, які можна вмикати та вимикати.
 * Маніпулятори (manipulators) – це об'єкти, розміщені в потоці, які впливають на спосіб введення та виведення даних.
 *
 * Для std::cout задати flag можна за допомогою .setf()
 *
 */

void rest_flags_and_manipulators() {
    std::cout.flags(std::ios_base::fmtflags(0)); // скидаємо всі попередні флаги
    std::cout << std::defaultfloat; // скидаємо всі попередні маніпулятори
}

int main() {

    /**
    * За замовченням std::cout не ставить знак "+" для позитивних чисел, але якщо
    * встановити flag std::ios::showpos то він буде
    */

    std::cout.setf(std::ios::showpos); // turn on the std::ios::showpos flag
    std::cout << 27 << '\n'; // +27


    // Можна за один раз встановити два флага за допомогою Bitwise OR (|) operator
    std::cout.setf(std::ios::showpos | std::ios::uppercase); // turn on the std::ios::showpos and std::ios::uppercase flag
    std::cout << 1234567.89f << '\n'; // +1.23457E+06


    // Щоб вимкнути флаг треба викликати .unsetf()
    std::cout.setf(std::ios::showpos); // turn on the std::ios::showpos flag
    std::cout << 27 << '\n'; // +27
    std::cout.unsetf(std::ios::showpos); // turn off the std::ios::showpos flag
    std::cout << 28 << '\n'; // 28



    std::cout << "--------------------------------------------\n";
    /**
    * Є ще одна особливість використання setf(), про яку варто згадати. Багато прапорців належать до груп,
    * які називаються групами форматування. Група форматування — це група прапорців, які виконують подібні
    * (іноді взаємовиключні) опції форматування. Наприклад, група форматування з назвою «basefield» містить
    * прапорці «oct», «dec» та «hex», які контролюють основу цілих значень. За замовчуванням встановлено
    *
    * Спробуємо увімкнути hex нотацію і вивести число 27
    */

    std::cout.setf(std::ios::hex); // try to turn on hex output
    std::cout << 27 << '\n'; // 27

    /**
    * Це не спрацювало! Причина в тому, що setf() лише вмикає прапорці — він недостатньо розумний,
    * щоб вимикати взаємовиключні прапорці. Тому нас спершу треба вимкнути dec і тоді увімкнути hex
    */

    std::cout.unsetf(std::ios::dec); // turn off decimal output
    std::cout.setf(std::ios::hex); // turn on hexadecimal output
    std::cout << 27 << '\n'; // 1B

    /**
    * Другий спосіб полягає у використанні іншої форми функції setf(), яка приймає два параметри:
    * перший параметр — це прапорець, який потрібно встановити, а другий — група форматування, до якої він належить.
    * При використанні цієї форми setf() усі прапорці, що належать до цієї групи, вимикаються, а вмикається
    * лише той прапорець, який було передано.
    */

    // Turn on std::ios::hex as the only std::ios::basefield flag
    std::cout.setf(std::ios::hex, std::ios::basefield);
    std::cout << 27 << '\n'; // 1B


    // Manipulators
    std::cout << "---------------------------MANIPULATORS------------------------\n";
    /**
    * Використання setf() та unsetf() зазвичай є незручним, тому C++ пропонує інший спосіб зміни
    * параметрів форматування: маніпулятори. Перевага маніпуляторів полягає в тому, що вони достатньо «розумні»,
    * щоб самостійно вмикати та вимикати відповідні прапорці.
    *
    * Зазвичай на практиці маніпулятори застосовують набагато частіше ніж flag
    */

    std::cout << std::hex << 27 << '\n'; // 1B (це 27 у hex)
    std::cout << 28 << '\n'; // 1C (все ще застосований hex - це 27 у hex)
    std::cout << std::dec << 29 << '\n'; // 29 (повертаємося назад у decimal)

    std::cout << "----------------------------------------\n";
    // Перелік корисних прапорців (flags) та маніпуляторів (manipulators)
    /**
    * Ось перелік корисних прапорців (flags) та маніпуляторів
    * Прапорці (flags) живуть у класі std::ios
    * Маніпулятори живуть у namespace std
    *
    * Flag
    * std::ios::boolalpha	If set, booleans print “true” or “false”. If not set, booleans print 0 or 1
    * std::ios::showpos	If set, prefix positive numbers with a +
    *
    * std::ios::uppercase	If set, uses upper case letters
    *
    * group std::ios::basefield 	class std::ios::dec	Prints values in decimal (default)
    * group std::ios::basefield	    class std::ios::hex	Prints values in hexadecimal
    * group std::ios::basefield	    class std::ios::oct	Prints values in octal
    * group std::ios::basefield	(none)	Prints values according to leading characters of value
    *
    * group std::ios::floatfield	    class std::ios::fixed	Uses decimal notation for floating-point numbers
    * group std::ios::floatfield	    class std::ios::scientific	Uses scientific notation for floating-point numbers
    * group std::ios::floatfield	    (none)	Uses fixed for numbers with few digits, scientific otherwise
    * group std::ios::floatfield	    class std::ios::showpoint	Always show a decimal point and trailing 0’s for floating-point values
    *
    *
    * Manipulators
    * std::boolalpha	Booleans print “true” or “false”
    * std::noboolalpha	Booleans print 0 or 1 (default)
    *
    * std::showpos	Prefixes positive numbers with a +
    *
    * std::noshowpos	Doesn’t prefix positive numbers with a +
    * std::uppercase	Uses upper case letters
    * std::nouppercase	Uses lower case letters
    *
    * std::dec	Prints values in decimal
    * std::hex	Prints values in hexadecimal
    * std::oct	Prints values in octal
    *
    * std::fixed	Use decimal notation for values
    * std::scientific	Use scientific notation for values
    * std::showpoint	Show a decimal point and trailing 0’s for floating-point values
    * std::noshowpoint	Don’t show a decimal point and trailing 0’s for floating-point values
    * std::setprecision(int)	Sets the precision of floating-point numbers (defined in the iomanip header)
    *
    *
    * також є звичайні функції які змінюють число
    * std::ios_base::precision()	Returns the current precision of floating-point numbers
    * std::ios_base::precision(int)	Sets the precision of floating-point numbers and returns old precision
    */

    // true/false
    rest_flags_and_manipulators();
    std::cout << "display true/false\n";

    std::cout << true << ' ' << false << '\n'; // 1 0

    std::cout.setf(std::ios::boolalpha);
    std::cout << true << ' ' << false << '\n'; // true false

    std::cout << std::noboolalpha << true << ' ' << false << '\n'; // 1 0
    std::cout << std::boolalpha << true << ' ' << false << '\n'; // true false

    rest_flags_and_manipulators();
    std::cout << "\nsign +\n";
    // sign "+"
    std::cout << 5 << '\n'; // 5

    std::cout.setf(std::ios::showpos);
    std::cout << 5 << '\n'; // +5

    std::cout << std::noshowpos << 5 << '\n'; // 5
    std::cout << std::showpos << 5 << '\n'; // +5

    // uppercase
    rest_flags_and_manipulators();
    std::cout << "\nUppercase\n";
    std::cout << 12345678.9 << '\n'; // 1.23457e+07

    std::cout.setf(std::ios::uppercase);
    std::cout << 12345678.9 << '\n'; // +1.23457E+07

    std::cout << std::nouppercase << 12345678.9 << '\n'; // +1.23457e+07
    std::cout << std::uppercase << 12345678.9 << '\n'; // +1.23457E+07

    // Нотації hex, dec, oct
    rest_flags_and_manipulators();
    std::cout << "\nNotation dec, oct, hex\n";
    std::cout << 27 << '\n'; // 27

    std::cout.setf(std::ios::dec, std::ios::basefield);
    std::cout << 27 << '\n'; // 27

    std::cout.setf(std::ios::oct, std::ios::basefield);
    std::cout << 27 << '\n'; // 33

    std::cout.setf(std::ios::hex, std::ios::basefield);
    std::cout << 27 << '\n'; // 1B

    std::cout << std::dec << 27 << '\n'; // 27
    std::cout << std::oct << 27 << '\n'; // 33
    std::cout << std::hex << 27 << '\n'; // 1B



    /**
     * За допомогою маніпуляторів (або прапорців) можна змінити точність та формат відображення чисел з плаваючою комою.
     * Існує кілька варіантів форматування, які поєднуються дещо складним чином, тому ми розглянемо це детальніше.
     *
     *
     */
    rest_flags_and_manipulators();
    std::cout << "\nPrecision, notation, and decimal points";
    std::cout << std::fixed << '\n';
    std::cout << std::setprecision(3) << 123.456 << '\n'; // 123.456
    std::cout << std::setprecision(4) << 123.456 << '\n'; // 123.4560
    std::cout << std::setprecision(5) << 123.456 << '\n'; // 123.45600
    std::cout << std::setprecision(6) << 123.456 << '\n'; // 123.456000
    std::cout << std::setprecision(7) << 123.456 << '\n'; // 123.4560000
    std::cout << std::setprecision(3) << 0.123456 << '\n'; // 0.123
    std::cout << std::setprecision(4) << 0.123456 << '\n'; // 0.1234

    std::cout << std::scientific << '\n';
    std::cout << std::setprecision(3) << 123.456 << '\n'; // 1.235e+02
    std::cout << std::setprecision(4) << 123.456 << '\n'; // 1.2346e+02
    std::cout << std::setprecision(5) << 123.456 << '\n'; // 1.23456e+02
    std::cout << std::setprecision(6) << 123.456 << '\n'; // 1.234560e+02
    std::cout << std::setprecision(7) << 123.456 << '\n'; // 1.2345600e+02

    rest_flags_and_manipulators();
    std::cout << '\n';
    // Якщо не вказано не std::fixed ні std::scientific то std::setprecision визначає скільки чисел показати
    std::cout << std::setprecision(3) << 123.456 << '\n'; // 123
    std::cout << std::setprecision(4) << 123.456 << '\n'; // 123.5 (відбулося округлення .456 перетворилося на .5. Частин .446 перетворилася б на .4)
    std::cout << std::setprecision(5) << 123.456 << '\n'; // 123.46 (відбулося округлення .56 перетворилося на .6)
    std::cout << std::setprecision(6) << 123.456 << '\n'; // 123.456
    std::cout << std::setprecision(7) << 123.456 << '\n'; // 123.456

    // Використовуючи маніпулятор або прапорець showpoint, ви можете змусити потік записувати десяткову кому та завершальні нулі.
    std::cout << std::showpoint << '\n';
    std::cout << std::setprecision(3) << 123.456 << '\n'; // 123.
    std::cout << std::setprecision(4) << 123.456 << '\n'; // 123.5
    std::cout << std::setprecision(5) << 123.456 << '\n'; // 123.45
    std::cout << std::setprecision(6) << 123.456 << '\n'; // 123.456
    std::cout << std::setprecision(7) << 123.456 << '\n'; // 123.4560



    /**
     * Додатково є цікавий спосіб заповнити якимось знаком певну маску
     * Нехай є банківська картка і лише останні 4 цифри мають бути видимими, а решта *
     *
     * std::internal	Left-justifies the sign of the number, and right-justifies the value
     * std::left	Left-justifies the sign and value
     * std::right	Right-justifies the sign and value
     * std::setfill(char)	Sets the parameter as the fill character (defined in the iomanip header)
     * std::setw(int)	Sets the field width for input and output to the parameter (defined in the iomanip header)
     */
    rest_flags_and_manipulators();
    std::cout << "fill specific char\n";
    std::cout.fill('*'); // вказуємо зо заповнювач буде *
    std::cout << -12345 << '\n'; // -12345 - print default value with no field width
    std::cout << std::setw(10) << -12345 << '\n'; // ****-12345 - print default with field width
    std::cout << std::setw(10) << std::left << -12345 << '\n'; // -12345**** - print left justified
    std::cout << std::setw(10) << std::right << -12345 << '\n'; // ****-12345 - print right justified
    std::cout << std::setw(10) << std::internal << -12345 << '\n'; // -****12345 - print internally justified


    return  0;
}