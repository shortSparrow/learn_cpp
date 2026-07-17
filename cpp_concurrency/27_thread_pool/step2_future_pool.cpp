#pragma once

#include <atomic>
#include <functional>
#include <future>
#include <vector>
#include <thread>
#include <list>
#include <chrono>
#include <type_traits>

/**
 * Особливий випадок, коли доводиться чекати завершення запущеного завдання, виникає, коли основний потік потребує
 * результату, обчисленого завданням. Це видно на прикладі функції parallel_accumulate(). У цьому випадку ми можемо
 * поєднати очікування з передачею результату за допомогою ф'ючерсів.
 *
 * Тут показано зміни, необхідні для простого пулу потоків, які дозволяють чекати завершення завдань, а потім
 * передавати значення, що повертаються з завдання, до потоку, що чекає.
 *
 * Зверніть увагу, що для додавання завдання до черги необхідно використовувати std::move(),
 * оскільки std::packaged_task<> не можна копіювати.
 *
 * Оскільки екземпляри std::packaged_task<> не можна копіювати, а лише переміщувати, ми більше не можемо
 * використовувати std::function<> для записів черги, оскільки std::function<> вимагає, щоб збережені об'єкти
 * функції були копійованими. Замість цього ми повинні використовувати власний обгортувач функції, який може
 * обробляти типи, що можна тільки переміщати. Нам потрібно обробляти тільки функції, які не приймають параметрів
 * і повертають void, тому це простий віртуальний виклик в реалізації. Це робить клас обгортка function_wrapper.
 *
 *
 * функція submit() повертає std::future<> для збереження значення, що повертається завданням, і дозволяє
 * викликанню чекати на завершення завдання завдяки future.
 */



// КЛАС-ОБГОРТКА function_wrapper
class function_wrapper {
    struct impl_base {
        virtual void call() = 0;
        virtual ~impl_base() {}
    };
    std::unique_ptr<impl_base> impl;

    template<typename F>
    struct impl_type : impl_base {
        F f;
        impl_type(F&& f_) : f(std::move(f_)) {}
        void call() override { f(); }
    };

public:
    template<typename F>
    function_wrapper(F&& f) : impl(std::make_unique<impl_type<F>>(std::move(f))) {}
    
    void operator()() { impl->call(); }
    
    function_wrapper() = default;
    function_wrapper(function_wrapper&& other) : impl(std::move(other.impl)) {}

    function_wrapper& operator=(function_wrapper&& other) {
        impl = std::move(other.impl);
        return *this;
    }

    function_wrapper(const function_wrapper&) = delete;
    function_wrapper(function_wrapper&) = delete;
    function_wrapper& operator=(const function_wrapper&) = delete;
};


/**
 * КРОК 2: Пул потоків з підтримкою future та поверненням результату роботи
 */
class thread_pool {
    std::atomic_bool done;
    threadsafe_queue<function_wrapper> work_queue; // Тепер черга працює з нашою обгорткою
    std::vector<std::thread> threads;
    join_threads joiner;

    void worker_thread() {
        while (!done) {
            function_wrapper task;
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

    /**
     * Нова функція submit(), яка повертає std::future.
     * ПРИМІТКА: В оригінальному прикладі тут було `typename std::result_of<FunctionType()>::type`
     * але ми використовуємо сучасний варіант `std::invoke_result_t<FunctionType>`.
     */
    template<typename FunctionType>
    std::future<std::invoke_result_t<FunctionType>> submit(FunctionType f) {
        using result_type = std::invoke_result_t<FunctionType>;
        
        std::packaged_task<result_type()> task(std::move(f));
        std::future<result_type> res(task.get_future());
        
        // Зверніть увагу, що для додавання завдання до черги необхідно використовувати std::move(),
        // та явно загортати в function_wrapper, оскільки packaged_task не копіюється.
        work_queue.push(function_wrapper(std::move(task)));
        
        return res;
    }

    /**
     * Поки ми чекаємо на завершення задачі, яка нас блокує, варто виконувати те, що не заблоковане.
     * Для цього нам потрібен прямий доступ робочого потоку до списку завдань пулу.
     */
    void run_pending_task() {
        function_wrapper task;
        if (work_queue.try_pop(task)) {
            task();
        } else {
            std::this_thread::yield();
        }
    }
};


/**
 * ПРИКЛАД 1: parallel_accumulate
 * Особливий випадок, коли доводиться чекати завершення запущеного завдання, виникає, коли основний потік 
 * потребує результату, обчисленого завданням. Це видно на прикладі функції parallel_accumulate(). 
 * У цьому випадку ми поєднуємо очікування з передачею результату за допомогою ф'ючерсів.
 * (Реалізація суто для ознайомлення, припускаємо наявність accumulate_block).
 */
template<typename Iterator, typename T, typename AccumulateBlockFunctor>
T parallel_accumulate(Iterator first, Iterator last, T init, AccumulateBlockFunctor accumulate_block) {
    unsigned long const length = std::distance(first, last);
    if (!length) return init;

    unsigned long const block_size = 25;
    unsigned long const num_blocks = (length + block_size - 1) / block_size;
    std::vector<std::future<T>> futures(num_blocks - 1);
    thread_pool pool;
    Iterator block_start = first;

    for (unsigned long i = 0; i < (num_blocks - 1); ++i) {
        Iterator block_end = block_start;
        std::advance(block_end, block_size);
        futures[i] = pool.submit([=, &accumulate_block] {
            return accumulate_block(block_start, block_end);
        });
        block_start = block_end;
    }

    T last_result = accumulate_block(block_start, last);
    T result = init;

    for (unsigned long i = 0; i < (num_blocks - 1); ++i) {
        result += futures[i].get();
    }
    result += last_result;
    return result;
}



/**
 * ПРИКЛАД 2: АЛГОРИТМ ШВИДКОГО СОРТУВАННЯ (QuickSort)
 * 
 * Алгоритм сортування QuickSort з використанням thread_pool.
 *
 * Оскільки QuickSort розділяє весь масив на частини які сортує, то може статися так, що одна частина не може почати
 * сортування, бо залежить від сортування попередньої частини, а вона ще не завершила сортування, і тоді потоки просто
 * простоюють, бо чекають доки одна конкретна задача завершиться і розблокує решту. Отже, поки ми чекаємо на завершення
 * задачі яка нас блокує варто сортувати те, що не заблоковане, а для цього нам з QuickSort треба мати доступ до списку
 * завдань, саме тому ми додали функцію thread_pool::run_pending_task()
 */

template<typename T>
struct sorter {
    thread_pool pool;

    std::list<T> do_sort(std::list<T>& chunk_data) {
        if (chunk_data.empty()) {
            return chunk_data;
        }
        std::list<T> result;
        result.splice(result.begin(), chunk_data, chunk_data.begin());
        T const& partition_val = *result.begin();
        
        typename std::list<T>::iterator divide_point = std::partition(
            chunk_data.begin(),
            chunk_data.end(),
            [&](T const& val) { return val < partition_val; });

        std::list<T> new_lower_chunk;
        new_lower_chunk.splice(new_lower_chunk.end(),
                               chunk_data,
                               chunk_data.begin(),
                               divide_point);

        // Відправляємо ліву частину в пул
        std::future<std::list<T>> new_lower = pool.submit(
            std::bind(&sorter::do_sort, this, std::move(new_lower_chunk))
        );

        // Праву частину сортуємо локально у цьому ж потоці
        std::list<T> new_higher(do_sort(chunk_data));
        result.splice(result.end(), new_higher);

        // Замість тупого блокування очікування, потік допомагає пулу розгрібати чергу!
        while (new_lower.wait_for(std::chrono::seconds(0)) == std::future_status::timeout) {
            pool.run_pending_task();
        }
        
        result.splice(result.begin(), new_lower.get());
        return result;
    }
};

template<typename T>
std::list<T> parallel_quick_sort(std::list<T> input) {
    if (input.empty()) {
        return input;
    }
    sorter<T> s;
    return s.do_sort(input);
}

/**
 * Чому цей пул все ще не ідеальний:
 * 
 * Кожного разу, коли потік викликає submit() на певному екземплярі пулу потоків, він повинен додати новий елемент
 * до єдиної спільної черги завдань. Аналогічно, робочі потоки постійно вилучають елементи з черги, щоб виконати завдання.
 * І це означає, що зі збільшенням кількості процесорів зростає конкуренція за чергу, один потік буде її блокувати, щоб
 * додати, інший, щоб звідти щось дістати.
 *
 * Як на мене, це не дуже велика проблема, особливо коли задачі у нас не дуже маленькі і потоків +- як і я дер в системі,
 * тобто теж не геть багато. Однак система не ідеальна, тому будемо її покращувати, щоб вона працювала добре в будь-яких
 * умовах.
 *
 *
 * У чому проблема єдиної спільної черги?
 * Коли пул потоків використовує лише одну чергу: Постійне блокування: Навіть якщо обчислення займають певний час, у
 * момент завершення завдання всі вільні потоки одночасно біжать до єдиного м'ютекса черги.
 *
 * Ефект "Cache Ping-Pong": Кожного разу, коли потік модифікує спільну чергу (додає чи забирає елемент), кеш-рядок
 * процесора, що містить покажчики черги, має бути інвалідований на всіх інших ядрах. Це змушує ядра постійно
 * пересилати дані кешу між собою, що драматично знижує продуктивність системи, навіть якщо безпосередній час
 * очікування на м'ютексі здається малим.
 *
 * Потік-відправник (Submitter): Зовнішній потік, який постійно «нарізає» та закидає нові завдання через submit(),
 * постійно конкурує з робочими потоками, які намагаються ці завдання вихопити.
 */