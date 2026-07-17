#pragma once

#include <atomic>
#include <functional>
#include <vector>
#include <thread>

/**
 * Пул потоків
 * У найпростішому випадку пул потоків — це фіксована кількість робочих потоків (зазвичай така сама кількість, як
 * значення, що повертається std::thread::hardware_concurrency()), які обробляють роботу. Коли у вас є робота,
 * ви викликаєте функцію, щоб помістити її в чергу очікуючих завдань. Кожен робочий потік бере роботу з черги,
 * виконує вказане завдання, а потім повертається до черги за новою роботою.
 *
 * Припускаємо, що у нас є потокобезпечна черга threadsafe_queue, її реалізацію можна подивитися в попередніх нотатках.
 * Її суть в тому, що вона безпечно працює з кількома потоками і сама керує race condition.
 *
 * Якщо будь-який потік закінчується помилкою, то ми відловлюємо це у catch і ставимо atomic_bool значення done у true
 * і закінчуємо роботу.
 *
 * join_threads нам потрібний, щоб у разі помилки ми очистили всі запущені потоки. Тут у деструкторі перед знищенням об'єкта
 * ми пройдемося по кожному потоку і дочекаємося їх завершення через join()
 *
 *
 * Якщо в черзі немає завдань, функція викликає std::this_thread::yield(), щоб зробити невелику паузу перед наступною
 * перевіркою, що не сильно завантажувати процесор
 */



class join_threads {
    std::vector<std::thread>& threads;
public:
    explicit join_threads(std::vector<std::thread>& threads_) : threads(threads_) {}

    ~join_threads() {
        for (unsigned long i = 0; i < threads.size(); ++i) {
            if (threads[i].joinable()) {
                threads[i].join();
            }
        }
    }
};

/**
 * КРОК 1: базовий thread_pool
 */
class thread_pool {
    std::atomic_bool done;
    threadsafe_queue<std::function<void()>> work_queue;
    std::vector<std::thread> threads;
    join_threads joiner;

    /**
     * Нескінченний цикл, у якому крутиться кожен потік пулу.
     * Якщо в черзі немає завдань, функція викликає std::this_thread::yield(), щоб зробити 
     * невелику паузу перед наступною перевіркою, щоб марно не завантажувати процесор.
     */
    void worker_thread() {
        while (!done) {
            std::function<void()> task;
            if (work_queue.try_pop(task)) {
                task();
            } else {
                std::this_thread::yield();
            }
        }
    }

public:
    thread_pool() : done(false), joiner(threads) {
        unsigned const thread_count = std::thread::hardware_concurrency();
        try {
            for (unsigned i = 0; i < thread_count; ++i) {
                // Запускаємо потоки на виконання worker_thread
                threads.emplace_back(&thread_pool::worker_thread, this);
            }
        } catch (...) {
            done = true;
            throw;
        }
    }

    ~thread_pool() {
        done = true;
    }

    // Додавання нового завдання в пул. Приймає будь-який callable-об'єкт (лямбду, функтор, вказівник на функцію).
    template<typename FunctionType>
    void submit(FunctionType f) {
        work_queue.push(std::function<void()>(f));
    }
};

/**
 * Чому цей пул потоків не ідеальний (Обмеження Кроку 1):
 * 
 * 1. Повернення результату: `submit()` повертає `void`. Ми ніяк не можемо дізнатися, коли задача завершилася,
 *    і не можемо отримати результат обчислень назад у потік, що викликав метод.
 * 2. Тільки копійовані типи: Через використання `std::function<void()>`, пул вимагає, щоб об'єкти завдань 
 *    можна було копіювати. Проте такі речі, як `std::packaged_task`, можна лише переміщувати (`std::move`).
 * 
 * Ці проблеми ми вирішимо на наступному кроці за допомогою `std::future` та `function_wrapper`.
 */