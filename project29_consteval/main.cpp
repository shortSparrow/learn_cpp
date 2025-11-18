#include <iostream>


/**
 * Оскільки у consrexpr є великий недолік - він не гарантує що функція
 * буде запущена під час компіляції. Там є дещо складні умови з обрахування,
 * буде значення обраховане під час компіляції чи у run-time.
 *
 * consteval дозволяє нам буди впевненими, що функція буде обрахована під
 * час компіляції, якщо вона цього не може, то виникне помилка
 *
 */

consteval int greater(int x, int y) {
    return (x > y) ? x : y;
}

/**
 * std::is_constant_evaluated() повертає true, якщо  поточна функція обчислюється у константному контексті.
 * Отже, у випадках, коли компілятор повинен обчислити константний вираз під час компіляції,
 * std::is_constant_evaluated() буде true, а коли не може то false.
 *
 * Це дає нам змогу писати різні умови на випадок якщо функція обчислиться на етапі компіляції і
 * якщо вона буде обчислена у run-time.
 */

#include <type_traits> // for std::is_constant_evaluated()
int doSomething() {return 1;};
int doSomethingElse() {return 2;};
constexpr int someFunction() {
    if (std::is_constant_evaluated()) // if evaluating in constant context
        return doSomething();
    else
        return doSomethingElse();
}

int main() {

    constexpr int g { greater(5, 6) };              // ok: will evaluate at compile-time
    std::cout << g << '\n';

    std::cout << greater(5, 6) << " is greater!\n"; // ok: will evaluate at compile-time

    int x{ 5 }; // not constexpr
    std::cout << greater(x, 6) << " is greater!\n"; // error: consteval functions must evaluate at compile-time




    return 0;
}