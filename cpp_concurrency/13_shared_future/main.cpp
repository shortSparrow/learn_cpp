#include <future>
#include <iostream>
#include <vector>


/**
 * Як ми знаємо future та promise у C++ створені для передачі даних між потоками. Ми в одному потоці
 * викликали якусь "важку" операцію і завдяки future та promise можемо отримати результат в іншому потоці,
 * але у future можна викликати .get() лише один раз. Тобто якщо у нас є кілька потоків з якими ми хочемо
 * викликати один future і отримати дані, то ми цього не зможемо зробити.
 *
 * std::shared_future вирішує цю проблему. Його можна копіювати. Кожна копія вказує на той самий результат,
 * і кожен потік може викликати .get() на своїй власній копії.
 *
 * Уяви, що ти розробляєш гоночну гру. Тобі потрібно запустити 3 потоки (машини), але вони не повинні починати
 * рух, поки не пролунає сигнал "Старт".
 */


void car_logic(int id, std::shared_future<void> start_signal) {
    // 1. Кожен потік блокується на своїй копії shared_future
    std::cout << "Машина #" << id << " готова і чекає сигналу...\n";

    // Всі потоки "зависнуть" тут одночасно
    start_signal.get();

    // 2. Як тільки сигнал подано, всі потоки прокидаються
    std::cout << "Машина #" << id << " ПУСК! Поїхали!\n";
}

int main() {
    // Створюємо обіцянку (сигнал старту)
    std::promise<void> start_promise;

    // Отримуємо звичайний future і ОДРАЗУ робимо його shared через .share()
    // Це набагато зручніше, ніж писати довгий тип std::shared_future<void>
    auto shared_f = start_promise.get_future().share();

    std::vector<std::thread> cars;
    for (int i = 1; i <= 3; ++i) {
        // Передаємо КОПІЮ shared_f у кожен потік
        cars.push_back(std::thread(car_logic, i, shared_f));
    }

    // Імітуємо підготовку (зворотний відлік)
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "[Організатор] 3... 2... 1... МАРШ!\n";

    // ОДИН ВИКЛИК активує ВСІ копії shared_future
    start_promise.set_value();

    for (auto& t : cars) {
        t.join();
    }

    return 0;
}
