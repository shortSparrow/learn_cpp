#include "../include/memory_order_seq_cst.h"
#include <atomic>
#include <thread>
#include <assert.h>


/**
 * В цьому варіанті z не може бути 0 в кінці
 *
 * Оскільки у нас на всіх потоках стоїть join то до перевірки assert(z.load()!=0); у нас гарантовано виконуються всі потоки.
 * Тому у нас можливі лише 2 варіанти:
 *      Варіант 1: Спочатку виконався запис X.store(true), а потім Y.store(true).
 *      Варіант 2: Спочатку виконався запис Y.store(true), а потім X.store(true).
 *
 * Розглянемо варіант 1.
 * Ми можемо зайти або в read_x_then_y або в read_y_then_x.
 *
 * Нехай ми зайшли в read_x_then_y і у нас x-true, тоді ми виходимо з циклу while і тепер y міг вже записатися у true, а
 * міг і ні. Якщо записався ++z, якщо ні, то інкременту z не буде
 *
 * Нехай ми зайшли у read_y_then_x. Ми чекаємо доки y не стане true. Коли це станеться то ми вийдемо із циклу while і
 * оскільки x у нас і так вже true то буде ++z.
 *
 * Розглянемо варіант 2.
 * Ми можемо зайти або в read_x_then_y або в read_y_then_x.
 *
 * Нехай ми зайшли в read_x_then_y і у нас x-false, тоді ми чекаємо в циклі while доки x не запишеться у true, тоді ми
 * вийдемо з циклу і побачимо що y вже true і тоді збільшуємо ++z
 *
 * Нехай ми зайшли у read_y_then_x. y-true, отже ми виходимо із циклу while і тепер x міг вже записатися у true, а
 * міг і ні. Якщо записався ++z, якщо ні, то інкременту z не буде
 *
 * Нагадування
 * Важливо розуміти, що у межах одного потоку, компілятор (не залежно від memory_order) зобов'язаний
 * зберегти логіку програми. Тобто у функції order_seq_cst
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

namespace order_seq_cst {
    std::atomic<bool> x,y;
    std::atomic<int> z;

    void write_x() {
        x.store(true,std::memory_order_seq_cst);
    }

    void write_y() {
        y.store(true,std::memory_order_seq_cst);
    }

    void read_x_then_y() {
        while(!x.load(std::memory_order_seq_cst));
        if(y.load(std::memory_order_seq_cst))
            ++z;
    }

    void read_y_then_x() {
        while(!y.load(std::memory_order_seq_cst));
        if(x.load(std::memory_order_seq_cst))
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
