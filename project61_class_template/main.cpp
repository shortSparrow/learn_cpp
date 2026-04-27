#include <iostream>
#include "Array.h"

/**
 * Template у класах такий самий як і у функціях. Ми пишемо один приклад і потім зможемо використовувати
 * Array<int>, Array<double> і так далі.
 *
 * Але на відміну від функцій тут є певна особливість. Зазвичай ми пишемо оголошення класів у файлах header (.h),
 * а саму реалізацію у файлах .cpp.
 * З шаблонами так не можна (зазвичай). Оскільки шаблон — це лише «інструкція» для створення коду, компілятор
 * повинен бачити весь шаблон (разом із методами) у тому файлі, де ви його використовуєте.
 * Якщо ми винесемо методи в окремий .cpp, то отримаємо помилку лінкера (linker error).
 *
 * Рішення: писати весь код шаблонного класу прямо у файлі загаловку (.h).
 *
 */

int main() {
    const int length { 12 };
    Array<int> intArray { length };
    Array<double> doubleArray { length };

    for (int count{ 0 }; count < length; ++count) {
        intArray[count] = count;
        doubleArray[count] = count + 0.5;
    }

    for (int count{ length - 1 }; count >= 0; --count)
        std::cout << intArray[count] << '\t' << doubleArray[count] << '\n';

    return 0;
}