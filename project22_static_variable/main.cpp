#include <iostream>

/**
 * static для локальних змінних змінює час існування.
 * За звичайних умов змінна знищується коли завершується її область видимості (функція),
 * але якщо поставити static, то змінна при першому запуску оголоситься і існуватиме аж до
 * завершення всієї програми
 *
 * Варто пам'ятати, що на глобальних змінних static має зовсім інший ефект.
 * Для глобальних змінних static робить їх локальними і забороняє експорт (extern)
 */

void incrementAndPrint(){
    int value{ 1 }; // automatic duration by default
    ++value;
    std::cout << value << '\n';
} // value is destroyed here


void incrementAndPrintStatic(){
    static int s_value{ 1 }; // static duration via static keyword.  This initializer is only executed once.
    ++s_value;
    std::cout << s_value << '\n';
}

int getInteger() {
    static bool s_isFirstCall{ true };

    if (s_isFirstCall) {
        std::cout << "Enter an first integer: ";
        s_isFirstCall = false;
    }
    else {
        std::cout << "Enter another integer: ";
    }

    int i{};
    std::cin >> i;
    return i;
}

int main()
{
    incrementAndPrint(); // 2
    incrementAndPrint(); // 2
    incrementAndPrint(); // 2

    std::cout << "--------------STATIC--------------" << "\n";
    incrementAndPrintStatic(); // 2
    incrementAndPrintStatic(); // 3
    incrementAndPrintStatic(); // 4

    std::cout << "--------------EXAMPLE--------------" << "\n";
    /**
     * static може бути дуже корисним для визначення того, чи функція запускається вперше
     */
    int a {getInteger()}; // Enter an first integer:
    int b {getInteger()}; // Enter another integer:

    return 0;
}