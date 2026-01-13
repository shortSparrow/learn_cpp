#include <iostream>
#include <cstdarg> // needed to use ellipsis

/**
 * Це старий спосіб працювати з ellipsis, нам треба допомога va_list
 *
 * Зверніть увагу, що параметр ellipsis не має імені! Натомість ми отримуємо доступ до значень в ellipsis
 * через спеціальний тип, відомий як va_list. Концептуально корисно уявляти va_list як pointer, що вказує на масив ellipsis.
 * Спочатку ми оголошуємо va_list, який для простоти назвали «list».
 *
 *
 * Наступне, що нам потрібно зробити, це вказати списоку на наші параметри ellipsis.
 * Ми робимо це, викликаючи va_start(). va_start() приймає два параметри: сам va_list і ім'я останнього параметра,
 * що не є ellipsis, у функції. Після виклику va_start() va_list вказує на перший параметр в ellipsis.
 *
 * Щоб отримати значення параметра, на який в даний момент вказує va_list, ми використовуємо va_arg().
 * va_arg() також приймає два параметри: сам va_list і тип параметра, до якого ми намагаємося отримати доступ.
 * Зверніть увагу, що va_arg() також переміщує va_list до наступного параметра в ellipsis!
 */
 double findAverage(int count, ...) {
    int sum{ 0 };

    // We access the ellipsis through a va_list, so let's declare one
    std::va_list list;

    // We initialize the va_list using va_start.  The first argument is
    // the list to initialize.  The second argument is the last non-ellipsis
    // parameter.
    va_start(list, count);

    // Loop through all the ellipsis values
    for (int arg{ 0 }; arg < count; ++arg){
        // We use va_arg to get values out of our ellipsis
        // The first argument is the va_list we're using
        // The second argument is the type of the value
        sum += va_arg(list, int);
    }

    // Cleanup the va_list when we're done.
    va_end(list);

    return static_cast<double>(sum) / count;
}

// Сучасний підхід - Fold Expressions (C++17)
template<typename... Args>
double average(Args... args) {
     // sizeof...(args) повертає кількість аргументів під час компіляції
     if (sizeof...(args) == 0) return 0;

     // (args + ...) — це "правобічне згортання"
     // Воно розгортається в (arg1 + (arg2 + (arg3 + ...)))
     return (args + ...) / sizeof...(args);
 }


/**
 * Ellipsis - це три крапки ... Це спосіб передати у функцію будь-яку кількість аргументів.
 *
 * Зверніть увагу, що функції, які використовують три крапки, повинні мати принаймні один параметр,
 * що не є трьома крапками (У findAverage - це int count)
 *
 * Хоча це не зовсім точно, концептуально корисно уявляти трикрапку як масив, який містить будь-які
 * додаткові параметри, окрім тих, що містяться в списку_аргументів.
 *
 * Але з ellipsis є дві проблеми - це тип даних і кількість аргументів.
 *
 * ellipsis (три крапки) відкидає тип змінної!
 * Отже, єдиний спосіб отримати значуще значення з ellipsis (три крапки) — це вручну вказати va_arg(),
 * який тип має наступний параметр. Саме це і робить другий параметр va_arg().
 * Якщо фактичний тип параметра не збігається з очікуваним типом параметра, зазвичай трапляються неприємності.
 *
 *
 * ellipsis не містить інформації скільки аргументів передали, якщо візьмемо замало якийсь пропустимо,
 * якщо візьмемо забагато, то отримаємо якесь значення зі смітника (grabadge)
 *
 *
 * Загалом через проблеми з типами і не зручним способом використання краще уникати ellipsis
 */
int main() {

    /**
     * Наочно видно проблему з типами, бо у середині findAverage ми обробляємо кожен аргумент як
     * int, але 1.0 - це double тож всі біти якими він записаний буде спийтняо як біти int числа
     * і різницю між реальним double числом і тим як int його інтерпретує може бути колосальною.
     *
     */
    std::cout << findAverage(6, 1, 2, 3, 4, 5, 6) << '\n'; // 3.5
    std::cout << findAverage(6, 1.0, 2, 3, 4, 5, 6) << '\n'; // 3.33333
    std::cout << findAverage(6, 1.3, 2, 3, 4, 5, 6) << '\n'; // -1.43166e+08


     std::cout << average(6, 1, 2, 3, 4, 5, 6) << '\n'; // 3.5
     std::cout << average(6, 1.0, 2, 3, 4, 5, 6) << '\n'; // 3.85714

    return 0;
}