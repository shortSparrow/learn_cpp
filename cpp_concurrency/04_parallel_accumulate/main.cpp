#include <iostream>
#include <vector>
#include <thread>
#include <numeric>
#include <algorithm>
#include <iterator>
#include <functional>


/**
 * Основна ідея — паралельне обчислення суми елементів масиву (аналог std::accumulate)
 *
 * Ось детальний розбір ключових концепцій:
 *
 * 1. Скільки потоків створювати?
 *  Це головне питання. Якщо потоків буде замало, потужність процесора не використовуватиметься повністю.
 *  Якщо забагато — система витрачатиме більше часу на перемикання між ними (overhead), ніж на саму роботу.
 *
 *  Для вирішення використовується функція: std::thread::hardware_concurrency() — вона повертає кількість "логічних"
 *  ядер вашого процесора. Якщо вона повертає 8, то ваш процесор може реально виконувати 8 завдань одночасно.
 *
 *  Алгоритм вибору в коді:
 *      Мінімум на потік: Автор встановлює min_per_thread = 25. Немає сенсу запускати окремий потік, щоб додати лише 5 чисел.
 *      Обмеження зверху: Розраховується max_threads = кількість_елементів / 25.
 *      Золота середина: Обирається найменше значення між тим, що хоче програма, і тим, що дозволяє залізо (немає сенсу на одноядерній
 *      машині запускати 30 потоків).
 *
 * 2. Як працює parallel_accumulate?
 *  Алгоритм розділяє масив на "блоки" (chunks). Кожен блок обробляється окремим потоком.
 *      - Вектор результатів: Оскільки потоки не можуть просто "повернути" значення як звичайна функція,
 *        створюється std::vector results, куди кожен потік запише свою проміжну суму.
 *      - Головний потік теж працює: Якщо ми вирішили використати 4 потоки, програма створює 3 нових потоки, а 4-ту
 *        частину роботи виконує сам "головний" потік. Це економить ресурси.
 *      - Фінальна сума: Коли всі потоки завершили роботу (join()), головний потік просто додає результати з вектора results.
 *
 * Коли паралельність закінчилась то results передається у звичайний std::accumulate і ми отримаємо фінальний результат
 *
 * 3. Важливі нюанси (Про що автор попереджає)
 *  А. Чому результати можуть відрізнятися?
 *  Для цілих чисел (int) сума завжди буде однаковою. Але для чисел з рухомою комою (float, double) порядок додавання
 *  має значення через похибки округлення. В паралельній версії ми додаємо числа групами, тому підсумковий результат
 *  може на мікроскопічну частку відрізнятися від послідовного додавання.
 *
 *  Б. Обробка помилок
 *  Код у прикладі "оптимістичний". У реальному житті конструктор std::thread може викинути виключення (exception),
 *  якщо в операційної системи закінчилася пам'ять або ліміт на потоки.
 */

// Структура для обчислення блоку даних у потоку
template<typename Iterator, typename T>
struct accumulate_block {
    void operator()(Iterator first, Iterator last, T& result) {
        // Використовуємо стандартний accumulate для конкретного діапазону
        result = std::accumulate(first, last, result);
    }
};

template<typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T init) {
    unsigned long const length = std::distance(first, last);

    // Якщо діапазон порожній, повертаємо початкове значення
    if (!length)
        return init;

    // Розраховуємо кількість потоків
    unsigned long const min_per_thread = 25;
    unsigned long const max_threads = (length + min_per_thread - 1) / min_per_thread;
    unsigned long const hardware_threads = std::thread::hardware_concurrency();

    // Вибираємо кількість потоків: не більше ніж може залізо і не більше ніж треба для малих даних
    unsigned long const num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);

    unsigned long const block_size = length / num_threads;

    std::vector<T> results(num_threads);
    std::vector<std::thread> threads(num_threads - 1); // -1, бо один блок обробить головний потік

    Iterator block_start = first;
    for (unsigned long i = 0; i < (num_threads - 1); ++i) {
        Iterator block_end = block_start;
        std::advance(block_end, block_size); // Пересуваємо ітератор на розмір блоку

        // Запуск потоку
        threads[i] = std::thread(
            accumulate_block<Iterator, T>(),
            block_start, block_end, std::ref(results[i])
        );
        block_start = block_end;
    }

    // Головний потік обробляє останню частину (вона може бути трохи більшою через залишок від ділення)
    accumulate_block<Iterator, T>()(block_start, last, results[num_threads - 1]);

    // Чекаємо завершення всіх потоків
    for (auto& entry : threads)
        entry.join();

    // Підсумовуємо всі проміжні результати
    return std::accumulate(results.begin(), results.end(), init);
}

int main() {
    // Створюємо вектор з 1000 елементів (від 1 до 1000)
    std::vector<int> v(1000);
    std::iota(v.begin(), v.end(), 1);

    int sum = parallel_accumulate(v.begin(), v.end(), 0);

    std::cout << "Parallel sum: " << sum << std::endl; // 500500
    std::cout << "Expected sum: " << (1 + 1000) * 1000 / 2 << std::endl; // 500500


    return 0;
}