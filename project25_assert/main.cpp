#include <cassert> // for assert()
#include <cmath> // for std::sqrt
#include <iostream>


/**
 *
 * Якщо assert поверне true, то рядок ігнорується і програма виконується як і має, але
 * якщо поверне false, то ми отримаємо помилку у терміналі.
 *
 * Ця річ корисна під час розробки, це як коментар - у цю функцію не можна передавати 0,
 * але на відміну від коментаря під час дебагу у нас буде помилка тож це не можна ігнорувати.
 *
 * Насправді дещо сумнівна річ, бо якщо не можна додавати 0, то поверни помилку
 *
 *
 * static_assert - це те саме, що і assert, але статичне, тобто опрацьовується на етапі компіляції.
 * Краще використовувати саме static_assert усюди де це можливо замість assert, бо воно працює швидше,
 * адже обраховується на етапі компіляції
 */


double calculateTimeUntilObjectHitsGround(const double initialHeight, const double gravity) {
    assert(gravity > 0.0); // The object won't reach the ground unless there is positive gravity.

    if (initialHeight <= 0.0) {
        // The object is already on the ground. Or buried.
        return 0.0;
    }

    return std::sqrt((2.0 * initialHeight) / gravity);
}

// Те саме, але asset тепер має більше інформації для дебагу
double calculateTimeUntilObjectHitsGroundV2(const double initialHeight, const double gravity) {
    assert(gravity > 0.0 && "My own error, lol"); // The object won't reach the ground unless there is positive gravity.

    if (initialHeight <= 0.0) {
        // The object is already on the ground. Or buried.
        return 0.0;
    }

    return std::sqrt((2.0 * initialHeight) / gravity);
}

int main() {
    // std::cout << "Took " << calculateTimeUntilObjectHitsGround(100.0, -9.8) << " second(s)\n";
    // std::cout << "\n Took " << calculateTimeUntilObjectHitsGroundV2(100.0, -9.8) << " second(s)\n";

    static_assert(sizeof(long) == 8, "long must be 8 bytes");
    static_assert(sizeof(int) >= 4, "int must be at least 4 bytes");

    return 0;
}