#include "../include/memory_order_relaxed.h"
#include <atomic>
#include <thread>
#include <assert.h>


/**
 * У випадку із memory_order_relaxed z може бути 0
 *
 * Оскільки немає глобального порядку, кожен потік бачить світ по-своєму:
 *  Потік read_x_then_y може побачити, що x став true, вийти з циклу, але через затримки в кеші процесора або перестановку
 *  інструкцій компілятором, він усе ще бачить y як false.
 *
 *  Одночасно з цим потік read_y_then_x бачить, що y став true, але для нього x усе ще залишається false.
 *
 *
 * Розберемося детальніше
 * Компілятор може переставляти інструкції місцями для своєї зручності і швидкості
 *      while(!x.load(std::memory_order_relaxed));
 *      if(y.load(std::memory_order_relaxed)) ++z;
 * може перетворитися на
 *      bool y_cached = y.load(std::memory_order_relaxed);
 *      while(!x.load(std::memory_order_relaxed));
 *      if(y_cached) ++z;
 *
 *
 * Або також можливо що у нас спочатку відбувся write_x, а потім write_y, і у read_x_then_y ми можемо побачити x та y
 * false, а у read_x_then_y x true, а y false.
 *
 * У багатоядерних процесорах кожне ядро має свій локальний кеш (L1, L2). Коли Потік 1 робить x.store(true), ця зміна не
 * потрапляє миттєво в оперативну пам'ять чи в кеші інших ядер. Вона спочатку потрапляє у буфер запису (store buffer) першого ядра.
 * Тому може виникнути така послідовність:
 *  - Потік 1 записав x = true (але це бачить тільки він сам у своєму буфері).
 *  - Потік 2 записав y = true.
 *  - Потік 3 (read_x_then_y) починає читати. Через затримки поширення даних по шині процесора (cache coherence latency),
 *    до ядра, на якому працює Потік 3, інформація про оновлення x та y ще просто не дійшла. Для нього обидві змінні в
 *    його кеші все ще залишаються false.
 *
 *
 * Нагадування
 * Важливо розуміти, що у межах одного потоку, компілятор (не залежно від memory_order) зобов'язаний
 * зберегти логіку програми. Тобто у функції order_relaxed
 *      x=false;
 *      y=false;
 *      z=0;
 *      std::thread a(write_x);
 *      std::thread b(write_y);
 *      std::thread c(read_x_then_y);
 *      std::thread d(read_y_then_x);
 *
 * У нас x=false та y=false можуть помінятися місцями, а от x=false та td::thread a(write_x) вже ні.
 */

namespace order_relaxed {
    std::atomic<bool> x,y;
    std::atomic<int> z;

    void write_x() {
        x.store(true,std::memory_order_relaxed);
    }

    void write_y() {
        y.store(true,std::memory_order_relaxed);
    }

    void read_x_then_y() {
        while(!x.load(std::memory_order_relaxed));
        if(y.load(std::memory_order_relaxed))
            ++z;
    }

    void read_y_then_x() {
        while(!y.load(std::memory_order_relaxed));
        if(x.load(std::memory_order_relaxed))
            ++z;
    }

    void run_code() {
        x=false;
        y=false;
        z=0;
        std::thread a(write_x);
        std::thread b(write_y);
        std::thread c(read_x_then_y);
        std::thread d(read_y_then_x);
        a.join();
        b.join();
        c.join();
        d.join();
        assert(z.load()!=0);
    }
}
