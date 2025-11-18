#include <iostream>

/**
 * З static варто бути обережним, бо за звичайних умов це гарантує,
 * що змінна буде жити до кінця програми, тож ми очікуємо що
 * змінна id буде доступна протягом часу роботи усієї програми і збільшуватися
 * на 1 при кожному запуску.
 *
 * Але так буде тільки для одного типу.
 * Коли ми запускаємо функцію з параметром int то id буде зберігатися для цього типу,
 * але коли запустимо з типом double то створиться нове id (бо технічно функція то інша)
 *
 *
 */
template <typename T>
void printIDAndValue(T value) {
    static int id{ 0 };
    std::cout << ++id << ") " << value << '\n';
}


// Template з різними аргументами. Оскільки ми не знаємо занчення якого
// типу будуть більше T чи U, тому нехай повертається auto
template <typename T, typename U>
auto max(T x, U y) {
    return (x < y) ? y: x;
}

/**
 * Тут ми вказуємо спільний ти для U і T
 * Спільний тип — це такий тип, до якого всі вхідні типи T, U можуть бути неявно (implicit)
 * конвертовані без втрати інформації (або з мінімальною очікуваною втратою, як при
 * арифметичних підвищеннях)
 *
 * Наприклад для int і double спільним буде double
 */
template <typename T, typename U>
auto min(T x, U y) -> std::common_type_t<T, U> {
    return (x < y) ? x: y;
}


/**
 * Примітка для auto для C++20
 * Коли функція повертає тип auto то компілятор автоматично стоврює template,
 * і кожний параметр автоматично стає auto
 *
 * auto super_max(auto x, auto y) {
 *   return (x < y) ? y : x;
 * }
 *
 * // Це те, що створить компілятор автоматично
 * template <typename T, typename U>
 * auto super_max(T x, U y) {
 *   return (x < y) ? y : x;
 * }
 *
 */


int main() {
    printIDAndValue(12); // 1) 12
    printIDAndValue(13); // 2) 13

    printIDAndValue(14.5); // 1) 14.5


    std::cout << max(2, 3.5) << '\n'; // resolves to max<int, double>
    std::cout << min(1.4, 5) << '\n'; // resolves to max<double, int>

    return 0;
}