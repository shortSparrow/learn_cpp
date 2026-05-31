#include <iostream>
#include <atomic>
#include <thread>


/**
 * Ми можемо створювати власні типи для std::atomic<T>
 * І всі такі atomic будуть мати звичні для нього функції, як от: .load(), .store(),
 * .exchange(), .compare_exchange_strong() / _weak().
 *
 * Однак не всі типи можна для цього використати
 * Компілятор дозволить загорнути ваш клас у std::atomic<> лише якщо цей клас є максимально простим.
 * Технічно це називається Trivially Copyable (тривіально копійований) (тобто мати тривіальний конструктор копіювання).
 * Тобто:
 *  - У класу не повинно бути віртуальних функцій (чи віртуального успадкування).
 *  - Ми не повинні писати свій оператор копіювання/присвоювання або деструктор (має
 *    використовуватися дефолтний від компілятора: = default).
 *  - Усі поля всередині нашого класу також мають бути такими ж простими
 *
 *
 * Які типи підходять для std::atomic
 *  - Вбудовані примітивні типи: int, long, char, bool, float, double, size_t тощо.
 *  - Звичайні вказівники (Сирі вказівники): int*, MyClass*, void*. (Копіювання вказівника — це просто
 *    копіювання 8 байтів адреси, тому це тривіальна операція).
 *  - enum та enum class.
 *
 * Які типи НЕ підходять для std::atomic
 *   - Усі динамічні контейнери (STL-контейнери). Ці типи керують пам'яттю в купі (heap).
 *     У них складні конструктори та деструктори.
 *     std::string
 *     std::vector<T>
 *     std::list<T>, std::forward_list<T>
 *     std::map<K, V>, std::unordered_map<K, V>
 *     std::set<T>, std::unordered_set<T>
 *     std::deque<T>
 *   - Розумні вказівники (Smart Pointers)
 *     std::unique_ptr<T>
 *     std::shared_ptr<T>
 *     std::weak_ptr<T>
 *   - Класи з віртуальними функціями або віртуальним успадкуванням.
 *     Якщо в класі є хоча б одне віртуальне слово (наприклад, virtual void foo(); або
 *     virtual ~MyClass();), у такого об'єкта з'являється приховане поле — вказівник на таблицю
 *     віртуальних методів (vpointer).
 *   - Будь-які класи, де ми самі написали конструктор копіювання чи деструктор
 *
 *
 * Особлива ситуація з std::atomic<float> та std::atomic<double>, хоча вони можуть бути std::atomic,
 * однак compare_exchange_weak завжди може завершитися невдачею з довільних внутрішніх причин.
 *  Операція може завершитися невдачею, навіть якщо старе збережене значення було рівним значенню
 *  порівняння, якщо збережене значення мало інше представлення
 */

// 1. Створюємо простий тип (Trivially Copyable).
// Тут немає віртуальних функцій, конструкторів чи деструкторів.
struct Point {
    int x;
    int y;
};

int main() {
    // Перевірка що struct Point може бути нормально скопійована (тривіально) для std::atomic
    static_assert(std::is_trivially_copyable_v<Point>, "Point має бути тривіально копійованим!");


    // 2. Ініціалізуємо атомарну структуру
    std::atomic<Point> atomic_point({0, 0});

    // --- ЩО РОБИТИ МОЖНА ---

    // .store() — безпечно записуємо нове значення
    Point new_point{10, 20};
    atomic_point.store(new_point);

    // .load() — безпечно зчитуємо значення
    Point current_point = atomic_point.load();
    std::cout << "Current Point: X = " << current_point.x << ", Y = " << current_point.y << "\n";

    // .exchange() — замінюємо значення на нове й отримуємо те, що було до цього
    Point old_point = atomic_point.exchange({50, 60});
    std::cout << "Old Point was: X = " << old_point.x << ", Y = " << old_point.y << "\n";


    // --- ПРИКЛАД З COMPARE_EXCHANGE (Порівняння та заміна) ---

    Point expected{50, 60}; // Очікуємо, що в atomic_point зараз лежить {50, 60}
    Point desired{100, 200}; // Якщо це так, хочемо замінити на {100, 200}

    // Функція побайтово порівняє atomic_point та expected.
    // Якщо вони збігаються, запише desired і поверне true.
    bool success = atomic_point.compare_exchange_strong(expected, desired);

    if (success) {
        std::cout << "Successfully changed to 100, 200!\n";
    } else {
        // Якщо значення змінив інший потік, expected оновиться поточним значенням з atomic_point
        std::cout << "Error! Values was already changed\n";
    }


    // --- ЩО РОБИТИ ЗАБОРОНЕНО (Код навіть не скомпілюється) ---

    // atomic_point.x = 5;      // ПОМИЛКА! Не можна міняти поля напряму всередині атоміка.
    // atomic_point++;          // ПОМИЛКА! Для custom-типів немає арифметичних операцій.
    // atomic_point += new_point; // ПОМИЛКА!

    return 0;
}