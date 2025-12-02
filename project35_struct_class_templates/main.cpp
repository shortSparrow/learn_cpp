#include <iostream>


/**
 * struct не можна overload як з функціями.
 * Припустимо у нас є Pair який має два поля з типами даних int, але потім
 * нам знадобилося така сама структура, але щоб вона також містила ті самі поля
 * типу double.
 *
 * Простий overload ми зробити не зможемо, тому єдиним шляхом буде template <typename T>
 * Це стосується class type - тобто class, struct, union
 */

// .......................ПРИКЛАД ЯК НЕ ПРАЦЮЄ..................
// struct Pair {
//     int first{};
//     int second{};
// };
//
// // compile error: erroneous redefinition of Pair
// struct Pair {
//     double first{};
//     double second{};
// };
//
// constexpr int max(Pair p) {
//     return (p.first < p.second ? p.second : p.first);
// }
//
// // compile error: overloaded function differs only by return type
// constexpr double max(Pair p) {
//     return (p.first < p.second ? p.second : p.first);
// }
//
// int main() {
//     Pair p1{ 5, 6 };
//     std::cout << max(p1) << " is larger\n";
//
//     Pair p2{ 1.2, 3.4 };
//     std::cout << max(p2) << " is larger\n";
//
//
//     return 0;
// }

// .......................ПРИКЛАД ЯК ПРАЦЮЄ..................

template <typename T>
struct Pair {
    T first {};
    T second {};
};


constexpr int max(Pair<int> p) {
    return (p.first < p.second ? p.second : p.first);
}

// okay: overloaded function differentiated by parameter type
constexpr double max(Pair<double> p) {
    return (p.first < p.second ? p.second : p.first);
}

// Тут ми не вказуємо конкретний тип, а натомість робимо шаблон і коли будемо викликати функцію то передамо тип як Pair<int>
template <typename T>
constexpr double max2(Pair<T> p) {
    return (p.first < p.second ? p.second : p.first);
}



template <typename T>
/**
 * Тут p має будь-який тип який ми передамо тому ми можемо писати у p будь-яку властивість, головне,
 * щоб коли ми передавали аргумент то у нього були ці властивості, інакше буде помилка програми.
 */
void print(T p) {
    std::cout << '[' << p.first << ", " << p.second << ']'; // will only compile if type has first and second members
}

int main() {
    Pair<int>  p1 {1,2};
    Pair<double> p2 {3.1, 4.5};

    std::cout << p1.first << ' ' << p1.second << '\n'; // 1 2
    std::cout << p2.first << ' ' << p2.second << '\n'; // 3.1 4.5

    // Тут ми передаємо будь-який тип для Pair який захочемо
    max2(Pair<float>{.1f, 0.3f});

    Pair<double> p3{ 4.5, 6 };
    print(p3); // [4.5, 6]




    return 0;
}