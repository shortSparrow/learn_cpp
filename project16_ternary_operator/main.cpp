#include <iomanip>
#include <iostream>

int main() {

    /**
     * Видасть помилку
     * Або якщо не видасть помилку то виведе 1, бо оператор
     * << має вищий пріоритет ніж ?, тому з початку виведеться true,
     */
    // std::cout << true ? "A" : "B" << std::endl;

    // std::cout << (true ? "A" : "B") << std::endl; // Виведе A

    /**
     * Важливо що другий і третій операнд були однаковими!
     * В нашому прикладі оператор 1 - це int, а другий - це douvle,
     * але це ок, оскільки int перейде до double. Для демонстарції
     * будемо виводити числа з знаками після коми, і побачимо, що наш
     * перший оператор виводиться як double
     */

    // // Встановлює режим виводу з фіксованою крапкою
    // std::cout << std::fixed;
    // std::cout << 1 << std::endl; // 1
    // std::cout << (true ? 1 : 2.4) << std::endl; // 1.000000
    // std::cout << (true ? -1 : 2u) << '\n';  // surprising result: -1 converted to unsigned int, result out of range

    // std::cout << (true ? "A" : 2.4) << std::endl; // видає помилку, бо типи string і double різні


    return 0;
}
