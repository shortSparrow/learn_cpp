#include <iostream>
#include <thread>
#include <future>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <atomic>

/**
 * =======================================================================================
 * ЧАСТИНА 2: Проблема блокуючих операцій (std::condition_variable) та стан перегонів
 * =======================================================================================
 *
 * Чому попередній підхід ламається?
 * У попередній частині ми розганяли перевірку через interruption_point() у циклі.
 * Але в реальних серверах потоки рідко крутяться в простих циклах. Зазвичай вони «сплять»
 * на condition_variable, чекаючи на нові завдання (наприклад, чергу задач).
 *
 * Якщо потік зайшов у cv.wait(), він заблокований на рівні ОС. Якщо головний потік просто
 * викличе t.interrupt() і змінить прапорець, робочий потік про це НІКОЛИ не дізнається,
 * бо він спить і не може підійти до виклику interruption_point(). Потік зависне назавжди.
 *
 * Ідея вирішення:
 * Потрібно навчити наш interrupt() не просто міняти прапорець, а й ПРИМУСОВО БУДИТИ
 * condition variable.
 *
 * Підступний Race Condition (Стан перегонів):
 * Наївна спроба зв'язати прапорець і cv виглядає так:
 * 1. Головний потік ставить flag = true.
 * 2. Головний потік викликає cv->notify_all().
 *
 * Що піде не так? Якщо робочий потік щойно перевірив `interruption_point()`, побачив, що
 * прапорця немає, і вже ГОТУЄТЬСЯ заснути у `cv.wait()`, але ще не встиг... У цей мікромомент
 * головний потік робить notify_all(). Сигнал пробудження йде в порожнечу (бо ніхто ще не спить).
 * Наступним тактом робочий потік засинає у wait() назавжди. Сигналу більше не буде.
 *
 * Тимчасовий «костильний» вихід (Наївне рішення №1):
 * Замість вічного cv.wait() використовувати cv.wait_for() з невеликим таймаутом (наприклад, 1 мс).
 * Потік буде прокидатися кожну мілісекунду, перевіряти interruption_point() і засинати знову.
 * Мінуси: Постійне споживання CPU на «порожні» прокидання (pooling) та затримка реакції на зупинку.
 *
 */


// Те саме, що і в step_1_interrupt_concept
class thread_interrupted : public std::exception {
public:
    const char* what() const noexcept override {
        return "Thread was interrupted cooperatively while waiting.";
    }
};

// Те саме, що і в step_1_interrupt_concept
class interruption_flag {
private:
    std::atomic<bool> flag{false};
public:
    void set() {
        flag.store(true, std::memory_order_relaxed);
    }
    bool is_set() const {
        return flag.load(std::memory_order_relaxed);
    }
};

// Те саме, що і в step_1_interrupt_concept
inline thread_local interruption_flag* this_thread_flag = nullptr;

// Те саме, що і в step_1_interrupt_concept
void interruption_point() {
    if (this_thread_flag && this_thread_flag->is_set()) {
        throw thread_interrupted();
    }
}


// Наївна кастомна функція очікування, яка використовує wait_for (будильник)
void interruptible_wait(std::condition_variable& cv, std::unique_lock<std::mutex>& lk) {
    interruption_point(); // Перевіряємо перед очікуванням

    // Ось він — костиль «Будильник».
    // Замість вічного cv.wait(lk) ми засинаємо максимум на 1 мілісекунду.
    // Якщо нас розбудить notify_all() з головного потоку — круто.
    // Якщо станеться Race Condition і ми пропустимо notify — ми все одно прокинемось через 1 мс.
    cv.wait_for(lk, std::chrono::milliseconds(1));

    interruption_point(); // Перевіряємо після прокидання
}

// Те саме, що і в step_1_interrupt_concept
class interruptible_thread {
private:
    std::thread internal_thread;
    interruption_flag* flag = nullptr;
public:
    template<typename Function>
    interruptible_thread(Function f) {
        std::promise<interruption_flag*> p;
        std::future<interruption_flag*> fut = p.get_future();

        internal_thread = std::thread([f, p = std::move(p)]() mutable {
            interruption_flag local_flag;
            this_thread_flag = &local_flag;
            p.set_value(&local_flag);

            try {
                f();
            } catch (const thread_interrupted& e) {
                std::cout << "[Thread] Interrupted successfully: " << e.what() << "\n";
            }

            this_thread_flag = nullptr;
        });

        flag = fut.get();
    }

    void interrupt() {
        if (flag) flag->set();
    }

    void join() {
        if (internal_thread.joinable()) {
            internal_thread.join();
        }
    }

    ~interruptible_thread() {
        join();
    }
};


// --- Демонстрація проблеми та її обходу ---

std::mutex mut;
std::condition_variable cv;
bool ready = false; // Якась корисна умова

void worker_with_cv() {
    std::cout << "[Worker] Inside worker, preparing to wait...\n";
    std::unique_lock<std::mutex> lk(mut);

    while (!ready) {
        std::cout << "[Worker] Sleeping on condition variable...\n";
        // Замість cv.wait(lk) викликаємо нашу кастомну функцію
        interruptible_wait(cv, lk);
    }
    std::cout << "[Worker] Work done cleanly (should not print if interrupted).\n";
}

int main_2() {
    interruptible_thread t(worker_with_cv);

    // Даємо потоку заснути
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    std::cout << "[Main] Signaling interrupt to sleeping thread...\n";
    t.interrupt();

    // Потік прокинеться або від notify_all(), або (якщо не пощастило) через 1 мс
    // за таймером wait_for, побачить прапорець і кине виключення.
    return 0;
}