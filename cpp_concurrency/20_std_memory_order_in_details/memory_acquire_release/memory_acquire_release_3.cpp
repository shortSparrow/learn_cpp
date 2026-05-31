#include <atomic>
#include <thread>
#include <assert.h>
#include "../include/memory_acquire_release/memory_acquire_release_3.h"

/**
 * В цьому випадку не зважаючи на те що багато де вказано std::memory_order_relaxed у нас
 * все одно буде нормальна послідовність і коли thread_3 буде зчитувати дані, то всі
 * дані у data вже будуть записані.
 *
 * У thread_1 sync1.store() встановлено std::memory_order_release, отже всі записи які вище,
 * все одно виконаються до нього. data[0].store() та data[1].store() можуть помінятися місцями,
 * але вони точно не опустяться нижче sync1.store(). Тож спочатку будуть записані дані у data,
 * а вже потім буде sync1.store().
 *
 * У thread_2 sync1.load має прапорець std::memory_order_acquire, відповідно буде синхронізація
 * synced-with зі змінною sync1, а отже ми отримаємо актуальне значення змінної.
 * Потім відбувається sync2.store(true) із прапорцем std::memory_order_release, і відповідно
 * теж буде synced-with зі змінною sync2, і в thread_3 коли буде sync2.load() ми отримаємо
 * актуальне значення. І оскільки sync1.store(true) неодмінно буде перед sync2.store(true),
 * то в thread_3, коли ми дійдемо до перевірки то всі дані в data вже точно будуть записані.
 *
 */
namespace order_acquire_release_3 {

    std::atomic<int> data[5];
    std::atomic<bool> sync1(false),sync2(false);
    void thread_1() {
        data[0].store(42,std::memory_order_relaxed);
        data[1].store(97,std::memory_order_relaxed);
        data[2].store(17,std::memory_order_relaxed);
        data[3].store(-141,std::memory_order_relaxed);
        data[4].store(2003,std::memory_order_relaxed);
        sync1.store(true,std::memory_order_release);
    }
    void thread_2() {
        while(!sync1.load(std::memory_order_acquire));
        sync2.store(true,std::memory_order_release);
    }
    void thread_3() {
        while(!sync2.load(std::memory_order_acquire));
        assert(data[0].load(std::memory_order_relaxed)==42);
        assert(data[1].load(std::memory_order_relaxed)==97);
        assert(data[2].load(std::memory_order_relaxed)==17);
        assert(data[3].load(std::memory_order_relaxed)==-141);
        assert(data[4].load(std::memory_order_relaxed)==2003);
    }

    void run_code() {
        std::thread a(thread_1);
        std::thread b(thread_2);
        std::thread c(thread_3);
        a.join();
        b.join();
        c.join();
    }

}
