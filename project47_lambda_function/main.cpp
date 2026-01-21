#include <iostream>
#include <array>
#include <algorithm>
#include <functional>

/**
 * The syntax for lambdas is one of the weirder things in C++
 * [ captureClause ] ( parameters ) -> returnType
 * {
 *  statements;
 * }
 *
 * The capture clause can be empty if no captures are needed.
 * The parameter list can be empty if no parameters are required. It can also be omitted entirely unless a return type is specified.
 * The return type is optional, and if omitted, auto will be assumed (thus using type deduction used to determine the return type)
 */



/**
 * Як передати lambda function як аргумент до іншої функції.
 * Існує 4 можливих варіанти
 * Найкраще використовувати спосіб 3 з const auto& fn
 */

// Case 1: use a `std::function` parameter
void repeat1(int repetitions, const std::function<void(int)>& fn) { // void говорить, функція fn нічого не повертає, але (int) - означає, що вона приймає один int аргумент
    for (int i{ 0 }; i < repetitions; ++i)
        fn(i);
}

// Case 2: use a function template with a type template parameter
template <typename T>
void repeat2(int repetitions, const T& fn){
    for (int i{ 0 }; i < repetitions; ++i)
        fn(i);
}

// Case 3: use the abbreviated function template syntax (C++20)
void repeat3(int repetitions, const auto& fn) {
    for (int i{ 0 }; i < repetitions; ++i)
        fn(i);
}

// Case 4: use function pointer (only for lambda with no captures)
void repeat4(int repetitions, void (*fn)(int)) {
    for (int i{ 0 }; i < repetitions; ++i)
        fn(i);
}




int main() {

    [] {}; // a lambda with an omitted return type, no captures, and omitted parameters.

    constexpr std::array<std::string_view, 4> arr{ "apple", "banana", "walnut", "lemon" };
    auto founded {
        std::find_if(arr.begin(), arr.end(), [](std::string_view str){
            return str.find("nut") != std::string_view::npos;
        })
    };
    std::cout << founded << '\n'; // 0x5aa23ff6d0
    std::cout << *founded << '\n'; // walnut


    /**
     * C++ не підтримує вкладені функції, однак це не стосується lambda function.
     *
     * Це тому, що технічно lambda function - це не функція, це особливий тип об'єктів,
     * які називаються functor. Functor – це об'єкти, що містять перевантажений operator(),
     * що робить їх викликаними як функції.
     *
     * Але який тим має lambda?
     * Коли ми пишемо lambda, компілятор генерує унікальний тип лише для цієї lambda, який нам недоступний
     * Тому ми використовуємо auto або можна ще std::function, але краще auto. Тобто якщо у нас буде
     * auto і зробимо три різні виклики, один з int, один з double і один зі std::string_view то
     * у компілятор згенерує три різні функції в залежності від переданого нами типу аргументу
     */

    std::array array {1,2,3,4,5};
    auto isEven{
        [](int i)
        {
            return (i % 2) == 0;
        }
    };

    auto result { std::all_of(array.begin(), array.end(), isEven)};

    //*********************************************************************
    // Є три способи записати lambda
    // 1 спосіб. A regular function pointer. Only works with an empty capture clause (empty []).
    double (*addNumbers1)(double, double){
        [](double a, double b) {
            return a+b;
        }
    };

    // 2 спосіб. Using std::function. The lambda could have a non-empty capture clause (discussed next lesson).
    std::function addNumbers2{ // note: pre-C++17, use std::function<double(double, double)> instead
        [](double a, double b) {
            return a + b;
        }
    };

    // 3 спосіб через змінну з типом auto (найкращий спосіб)
    auto addNumbers3 {
        [](double a, double b)
        {
            return a+b;
        }
    };

    //*********************************************************************
    // Різні способи передачі lambda function як аргумент
    auto lambda = [](int i) {
        std::cout << i << '\n';
    };

    repeat1(3, lambda);
    repeat2(3, lambda);
    repeat3(3, lambda);
    repeat4(3, lambda);



    // lambda function, як і звичайна функція повинна повертати лише один тип даних, інакше буде помилка
    auto divide{ [](int x, int y, bool intDivision) { // note: no specified return type
        if (intDivision)
            return x / y; // return type is int
        else
            return static_cast<double>(x) / y; // ERROR: return type doesn't match previous return type
    } };


    /**
     * Та сама функція, як і вище, але на відміну від return type deduction тут ми напряму вказали,
     * що повертатися буде double, тому помилки не буде, то x/y приведеться до double
     */
    auto divide2{ [](int x, int y, bool intDivision) -> double {
        if (intDivision)
            return x / y; // will do an implicit conversion of result to double
        else
            return static_cast<double>(x) / y;
    } };

    return 0;
}