#include <complex>
#include <iomanip>
#include <iostream>

int main() {

    /**
     * Піднесення у ступінь
     * Може бути проблема, бо ітандартна функція std::pow приймає double і повертає double
     * і можуть бути помилки при округленні (але це не точно, треба уточнити)
     *
     */

    const double result {std::pow(2.0, 5.0)};

    // Встановлює режим виводу з фіксованою крапкою
    std::cout << std::fixed;
    std::cout << result << std::endl; // 32.000000



    return 0;
}