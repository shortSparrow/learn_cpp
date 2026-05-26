#include <iostream>
#include <thread>
#include <vector>
#include <latch>
#include <chrono>


/**
 * std::latch - це одноразовий лічильник, що реалізує бар'єр очікування. Він є одноразовим засобом синхронізації потоків
 * і використовується для того, щоб заблокувати один або кілька потоків доти, доки лічильник всередині latch не
 * зменшиться до нуля. Він корисний коли у мене є n потоків які щось роблять і мені треба точно знати коли всі вони
 * закінчили роботу, тоді я ставлю std::latch(5) і в кожному з них після їх роботи віднімаю 1, відповідно коли
 * настане 0 – це означатиме що кожен з потоків закінчив свою роботу.
 *
 * Методи:
 *  count_down(n = 1): Атомарно зменшує лічильник. Якщо лічильник стає 0, розблоковує всі потоки, що заблоковані на цьому latch.
 *  wait(): Блокує поточний потік, поки лічильник не стане 0. Якщо він вже 0 — повертає керування негайно.
 *
 * Наведений нижче код демонструє приклад, коли std::latch заблокує головний потік доки три побічні
 * потоки не завершать свою роботу і не викличуть count_down, щоб count став 0, тоді start_latch.wait()
 * прокинеться і код піде далі
 */

// Глобальний не атомарний буфер даних
std::vector<int> data_buffer(3, 0);

void worker_thread(int id, std::latch& start_latch) {
    // Імітація важких обчислень
    std::this_thread::sleep_for(std::chrono::milliseconds(100 * (id + 1)));

    // Запис результату у свій слот. Операція безпечна,
    // оскільки потоки пишуть у різні індекси буфера.
    data_buffer[id] = (id + 1) * 10;

    // Зменшуємо лічильник латча.
    // Операція виконує "release" синхронізацію пам'яті.
    start_latch.count_down();
}

int main() {
    const int num_threads = 3;

    // Ініціалізуємо латч значенням 3
    std::latch start_latch(num_threads);
    std::vector<std::thread> threads;

    // Запуск робочих потоків
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(worker_thread, i, std::ref(start_latch));
    }

    // Головний потік блокується і чекає, поки лічильник стане 0.
    // Операція виконує "acquire" синхронізацію пам'яті.
    start_latch.wait();

    // Завдяки memory order, гарантується, що зміни,
    // внесені всіма воркерами до data_buffer, вже видимі тут.
    std::cout << "All thread complete the work. The result is in buffer: ";
    for (int val : data_buffer) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    // Обов'язковий join потоків
    for (auto& t : threads) {
        t.join();
    }

    return 0;
}