#include <iostream>
#include <cstring> // for std::strlen

/**
 * Особливістю C-style string є:
 *  1. Це C-style масив char. І він так само як і C-style array розпадається (decay),
 *     якщо передати його у функцію, а не робить копію, і відповідно втрачає інформацію про
 *     свою довжину.
 *  2. Має в кінці null terminator (як раз через decay це і треба. По ньому можна дізнатися, що рядок закінчився)
 *  3. Живе протягом усього часу програми, на відміну від інших змінних
 *
 *  Коли ми пишемо cout << "" або return "some string" (якщо не зазачено тип, або ти char), то це C-style string
 */

void print(char ptr[]) {
    std::cout << ptr << '\n'; // output string
}

// Будемо виводити посимвольно
void printCString(const char str[]){
    // While we haven't encountered a null terminator
    while (*str != '\0') {
        // print the current character
        std::cout << *str;

        // and use pointer arithmetic to move str to the next character
        ++str;
    }
    std::cout << '\n';
}

void printCStringBackwards(const char str[]) {
    size_t length {std::strlen(str)};
    const char* ptr {str + static_cast<int>(length) - 1}; // str - посилання на pointer першого char, length - кількість char, а -1 це ми віднімаємо null pointer

    while (ptr >= str) {
        std::cout << *ptr;
        ptr--;
    }
    std::cout << '\n';
}

int main() {
    std::cout << "This is C-style string" << '\n';

    const char my_str[] {"Hello world"}; // C-style strings - це по суті масив char

    char str1[8]{};                    // an array of 8 char, indices 0 through 7
    const char str2[]{ "string" };     // an array of 7 char, indices 0 through 6
    std::cout << sizeof(str2) << '\n'; // 7 - тому що 6 char і 1 null terminator, щоб знати де закінчується рядок (по суті масив char)


    char str[]{ "string" };
    std::cout << str << '\n'; // outputs string
    print(str); // outputs string
    printCString(str); // string
    printCStringBackwards(str); // gnirts

    // Так само як і C-style array можна визначити, але не можна записати нове знасення змінній
    char str3[]{ "string" };
    // str3 = "rope";        // error


    /**
     * Because C-style strings are C-style arrays, you can use std::size()
     * (or in C++20, std::ssize()) to get the length of the string as an array.
     *
     * Але таке працює тільки до моменту decay, бо після нього інформація про length втрачається,
     * для цього випадку можна викорситаи функцію std::strlen(), вона працює для pointer і C-style string до decay
     *
     */
    std::cout << "********************** get size ******************" << '\n';

    char str4[255]{ "string" }; // 6 characters + null terminator
    std::cout << "length = " << std::size(str4) << '\n'; // prints length = 255

    char *ptr { str4 };
    // std::cout << "length = " << std::size(ptr) << '\n'; // compile error
    std::cout << "length = " << std::strlen(ptr) << '\n';   // prints length = 6

    std::cout << "************************** cout char ************************** \n";

    /**
     * З виводом через cout є мені нюанси. У коді нижче я виводжу char c та c2, різниця тільки в тому,
     * що c - не C-style array, а один char, і коли cout його бачить то думає що це C-style string, яка є
     * масмвом char і закінчується '\0' (null terminator), тож виводить все від pointer на який вказує
     * масив (перший елемент масиву), аж до поки не зустріне null terminator. От тільки в варіанти з c
     * null termanitor немає, бо нагадую ще ращ, що це не масив, а один char. Тож буде виводитьтися всяке сміття,
     * яке лижить в пам'яті після pointer аж до поки не зустріне null pointer. В моємо випадку "сміття" це
     * рядок "string" який лежить після змінної c.
     *
     * Якщо ж ми все таки хочемо вивести адерсу c то можна скористатися  static_cast<const void*>(ptr)
     * так ми приведемо насильно до типу const void*
     */
    char c{ 'Q' };
    char c2[]{ 'Q' };
    std::cout << &c << '\n'; // Qstring
    std::cout << &c2 << '\n'; // 0x629edffd1d

    char* ptr_c2 {&c};
    std::cout << static_cast<const void*>(ptr_c2) << '\n'; // 0x42567ffa8f



    return 0;
}