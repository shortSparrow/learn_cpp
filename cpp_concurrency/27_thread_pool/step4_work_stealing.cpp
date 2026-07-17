#include <atomic>
#include <functional>
#include <future>
#include <vector>
#include <thread>
#include <deque>
#include <mutex>
#include <memory>
#include <type_traits>

// Припускаємо, що function_wrapper та join_threads підключаються з попередніх кроків
#include "function_wrapper.cpp"

/**
 * work_stealing_queue
 *
 * Щоб уникнути недоліку з попереднього підходу (працює лише один потік), нам потрібно,
 * щоб потоки могли безпечно заглядати в черги інших потоків та забирати звідти задачі.
 *
 * Архітектурний базис двосторонньої черги (deque):
 * 1. Власник черги викликає push() та try_pop(). Вони працюють з ГОЛОВОЮ черги (push_front / pop_front).
 *    Потік взаємодіє зі своєю чергою як зі стеком LIFO (Last In First Out). Це дає ідеальну локальність кешу,
 *    бо найсвіжіші підзадачі (які щойно нарізали) виконуються першими.
 * 2. Потік який хоче забрати задачу викликає try_steal(). Він працює з ХВОСТОМ черги (pop_back) за принципом
 *    FIFO (First In First Out). Це забирає найбільші шматки роботи (які були додані на самому початку) і кардинально
 *    мінімізує конфлікти за м'ютекс між власником та "злодієм".
 */
class work_stealing_queue {
private:
    using data_type = function_wrapper;
    std::deque<data_type> the_queue;
    mutable std::mutex the_mutex;

public:
    work_stealing_queue() = default;
    work_stealing_queue(const work_stealing_queue& other) = delete;
    work_stealing_queue& operator=(const work_stealing_queue& other) = delete;

    // Власник додає в голову
    void push(data_type data) {
        std::lock_guard<std::mutex> lock(the_mutex);
        the_queue.push_front(std::move(data));
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(the_mutex);
        return the_queue.empty();
    }

    // Власник забирає з голови (LIFO)
    bool try_pop(data_type& res) {
        std::lock_guard<std::mutex> lock(the_mutex);
        if (the_queue.empty()) {
            return false;
        }
        res = std::move(the_queue.front());
        the_queue.pop_front();
        return true;
    }

    // Інший потік краде з хвоста (FIFO)
    bool try_steal(data_type& res) {
        std::lock_guard<std::mutex> lock(the_mutex);
        if (the_queue.empty()) {
            return false;
        }
        res = std::move(the_queue.back());
        the_queue.pop_back();
        return true;
    }
};

/**
 * Фінальний пул потоків із можливість забирати роботу в інших потоків
 *
 *
 * Основна ідея тут полягає в тому, що один і той самий код класу поводиться абсолютно по-різному залежно від того,
 * ХТО його викликає. У класі оголошено дві змінні:
 *  threadsafe_queue<task_type> pool_work_queue - це глобальна черга, вона одна
 *  std::vector<std::unique_ptr<work_stealing_queue>> queues; - це масив (вектор) локальних черг потоків
 *      queues[0] — це черга для Потоку №0.
 *      queues[1] — це черга для Потоку №1.
 *      queues[2] — це черга для Потоку №2.
 *  Цей вектор queues є спільним для всього пулу. Він потрібен лише для однієї мети: щоб потоки могли знайти черги своїх
 *  колег і вкрасти звідти роботу за допомогою індексу.
 *
 *  static thread_local work_stealing_queue* local_work_queue; - Це просто швидкий вказівник на чергу з вектора queues
 *  static thread_local unsigned my_index; - працює в парі з local_work_queue, це просто індекс локальної черги в загальній черзі
 *
 *
 * Коли потік стартує, він виконує функцію worker_thread(unsigned my_index_)
 *
 */
class thread_pool {
    using task_type = function_wrapper;

    std::atomic_bool done;
    threadsafe_queue<task_type> pool_work_queue;
    std::vector<std::unique_ptr<work_stealing_queue>> queues;
    std::vector<std::thread> threads;
    join_threads joiner;

    static inline thread_local work_stealing_queue* local_work_queue = nullptr;
    static inline thread_local unsigned my_index = 0;

    void worker_thread(unsigned my_index_) {
        my_index = my_index_;                      // Тепер цей потік знає свій унікальний індекс!
        local_work_queue = queues[my_index].get(); // Потік "прив'язує" до себе конкретну чергу з вектора

        while (!done) {
            run_pending_task();
        }
    }

    bool pop_task_from_local_queue(task_type& task) {
        return local_work_queue && local_work_queue->try_pop(task);
    }

    bool pop_task_from_pool_queue(task_type& task) {
        return pool_work_queue.try_pop(task);
    }

    /**
     * Крадіжка завдань в інших потоків
     * Коли у потоку закінчилася своя робота, він запускає цикл пошуку по чергах своїх колег
     *
     * Пояснення формули
     *  unsigned const index = (my_index + i + 1) % queues.size();
     *  
     * Якщо у нас 4 потоки (індекси 0, 1, 2, 3), і потік №2 (my_index = 2) шукає роботу:
     *   При i = 0: index = (2 + 0 + 1) % 4 = 3 (спочатку перевіряє сусіда праворуч — потік №3)
     *   При i = 1: index = (2 + 1 + 1) % 4 = 0 (потім перевіряє потік №0)
     *   При i = 2: index = (2 + 2 + 1) % 4 = 1 (потім потік №1)
     *   При i = 3: index = (2 + 3 + 1) % 4 = 2 (перевіряє сам себе — поверне false, бо він порожній)
     *
     * Чому не просто йти циклом від 0 до N?
     * Якби всі потоки починали обхід з 0-го індексу, виникла б тиснява (конкуренція) за чергу потоку №0
     * А так кожен потік починає красти у свого сусіда "праворуч", завдяки чому запити розподіляються по колу
     *
     * Чому потрібне додавання "+ i"?
     * Якщо написати просто (my_index + 1) % queues.size(), то потік завжди буде перевіряти тільки одного свого
     * конкретного сусіда праворуч, а якщо там порожньо, то ми хочемо піти до наступного сусіда, тому + i.
     */
    bool pop_task_from_other_thread_queue(task_type& task) {
        for (unsigned i = 0; i < queues.size(); ++i) {
            unsigned const index = (my_index + i + 1) % queues.size();
            if (queues[index]->try_steal(task)) {
                return true; // Вкрали задачу у колеги!
            }
        }
        return false; // Обійшли всіх, ні в кого роботи немає
    }

public:
    thread_pool() : done(false), joiner(threads) {
        unsigned const thread_count = std::thread::hardware_concurrency();
        try {
            // Крок 1: Спочатку створюємо всі черги у векторі
            for (unsigned i = 0; i < thread_count; ++i) {
                queues.push_back(std::make_unique<work_stealing_queue>());
            }
            // Крок 2: Тільки після цього запускаємо потоки, передаючи їм їхній індекс
            for (unsigned i = 0; i < thread_count; ++i) {
                threads.emplace_back(&thread_pool::worker_thread, this, i);
            }
        } catch (...) {
            done = true;
            throw;
        }
    }

    ~thread_pool() {
        done = true;
    }


    template<typename FunctionType>
    std::future<std::invoke_result_t<FunctionType>> submit(FunctionType f) {
        using result_type = std::invoke_result_t<FunctionType>;

        std::packaged_task<result_type()> task(f);
        std::future<result_type> res(task.get_future());

        /**
         * Тут стоїть умова на те, хто саме викликає submit.
         * Якщо виклик йде з функції main (або іншого потоку, який не належить пулу), то змінна local_work_queue
         * рівна nullptr, відповідно завдання кладеться у загальну чергу.
         *
         * Якщо потік всередині пулу виконує задачу і прямо з неї викликає submit() для підзадач. Для цього потоку
         * local_work_queue не є nullptr (ми його ініціалізували у worker_thread). Тому задача миттєво і без жодних
         * блокувань глобальної черги летить у його особисту локальну чергу!
         */

        if (local_work_queue) {
            local_work_queue->push(function_wrapper(std::move(task)));
        } else {
            pool_work_queue.push(function_wrapper(std::move(task)));
        }

        return res;
    }

    /**
     * Життєвий цикл потоку (Черговість розгрібання задач):
     * 1. Спочатку перевіряє свою чергу (0% конкуренції за дані, супер-локальність кешу).
     * 2. Якщо своя порожня, дивиться в загальну чергу.
     * 3. Якщо і там порожньо, намагається забрати задачу у інших потоків.
     * 4. Якщо взагалі усюди пусто віддає процесорний час через yield().
     */
    void run_pending_task() {
        task_type task;
        if (pop_task_from_local_queue(task) ||
            pop_task_from_pool_queue(task) ||
            pop_task_from_other_thread_queue(task))
        {
            task();
        } else {
            std::this_thread::yield();
        }
    }
};