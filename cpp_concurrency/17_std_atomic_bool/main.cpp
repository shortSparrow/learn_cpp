#include <iostream>
#include <atomic>
#include <thread>
#include <chrono>


/**
 * std::atomic<bool> — це повна спеціалізація шаблону std::atomic для типу bool.
 * Головна відмінність від std::atomic_flag полягає в тому, що це повноцінна змінна, яку можна безпосередньо читати,
 * записувати та порівнювати, не обов'язково модифікуючи її стан при кожному зверненні.
 *
 * Має методи
 *  - load() щоб отримати значення. Отримаємо саме значення (копію), а не посилання.
 *   Цікава особливість присвоєння: Коли ти пишеш b = false; для atomic<bool>, оператор повертає не посилання на b,
 *   а саме значення false. Навіщо? Якщо б він повертав посилання, і ти захотів би його використати, тобі довелося б
 *   знову звертатися до змінної в пам'яті. А за цей мікрочас інший потік міг би вже змінити значення на true.
 *   Повертаючи копію значення, мова гарантує: ти знаєш точно, що саме ти записав.
 *
 *  - store(bool) записує значення
 *  - exchange() –  замінює значення на нове і отримає старе в замін.
 *
 *  Методи можуть також приймати order, ось їх повний вигляд цих методів.
 *  bool load(std::memory_order order = std::memory_order_seq_cst) const noexcept
 *  void store(bool desired, std::memory_order order = std::memory_order_seq_cst) noexcept
 *  bool exchange(bool desired, std::memory_order order = std::memory_order_seq_cst) noexcept
 *
 *
 * Опис коду що виконується нижче
 * Спочатку запускається окремий потік на producer_func і робить якось важку роботу,
 * потім запускаються ще два потоки які виконують одну функцію consumer_func. Вони обидва зависнуть
 * на while до поки producer_func() не завершить важку роботу і не поставить data_ready у значення true.
 *
 * Потім перший двох потоків який встигне зреагувати вийде з циклу і через exchange візьме старе значення
 * у data_taken (а воно false), і поставить нове true. Так ми зможемо визначити, що якщо старе значення
 * false то наш потік взяв дані першим, а якщо буде true то другим.
 */


std::atomic<bool> data_ready(false);
std::atomic<bool> data_taken(false);
int shared_data = 0;

void producer_func() {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    shared_data = 42;

    // Сигналимо, що дані готові. Цей прапорець залишається true.
    data_ready.store(true, std::memory_order_release);
    std::cout << "[Producer]: Data is ready and written.\n";
}

void consumer_func(int id) {
    // Тепер обидва потоки гарантовано вийдуть з цього циклу,
    // бо продюсер встановив true назавжди
    while (!data_ready.load(std::memory_order_acquire)) {
        // spin-wait
    }

    // Хто перший виконає exchange(true), той і забере дані.
    // exchange(true) запише true, і поверне false (бо початково data_taken == false)
    bool already_taken = data_taken.exchange(true, std::memory_order_acq_rel);

    if (!already_taken) {
        // Якщо повернулося false — ми перші, хто змінив прапорець на true
        // Оскільки коли cout має кілька операторів << то це різні операції то при такому виводу у нас будуть змішуватися cout з різних потоків
        // std::cout << "[Consumer " << id << "]: Success! I am the first to get the data: " << shared_data << "\n";
        std::cout << std::format("[Consumer {} ]: Success! I am the first to get the data: {}\n", id, shared_data); // Цей щось типу template string. Все виводиться за один раз
    } else {
        // Якщо повернулося true — хтось уже встиг зробити exchange(true) до нас
        // Оскільки коли cout має кілька операторів << то це різні операції то при такому виводу у нас будуть змішуватися cout з різних потоків
        // std::cout << "[Consumer " << id << "]: Too late, another thread has already taken the data.\n";
        std::cout << std::format("[Consumer {} ]: Too late, another thread has already taken the data.\n", id); // Цей щось типу template string. Все виводиться за один раз

    }
}

int main() {
    std::thread t1(producer_func);
    std::thread t2(consumer_func, 1);
    std::thread t3(consumer_func, 2);

    t1.join();
    t2.join();
    t3.join();

    /**
     * Ось що виведе термінал:
     *
     * [Producer]: Data is ready and written.
     * [Consumer 1 ]: Success! I am the first to get the data: 42
     * [Consumer 2 ]: Too late, another thread has already taken the data.
     */

    return 0;
}


// TODO під питанням, можливо додати про compare_exchange_strong та compare_exchange_weak
