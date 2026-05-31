#include "../include/memory_acquire_release/memory_acquire_release_2.h"

#include <atomic>
#include <thread>
#include <assert.h>

// Додатково про acquire та release
/**
 * В цьому прикладі z не може бути 0, навіть незважаючи на те, що в деяких місцях стоїть memory_order_relaxed
 *
 * Ось як вибудовується залізобетонний ланцюжок залежностей (транзитивність):
 *      1) x.store(true, relaxed) стоїть ПЕРЕД y.store(true, release) в одному потоці. Хоча запис в x є relaxed,
 *         правило режиму release для y каже: "Жодна інструкція згори не може опуститися нижче мене".
 *         Отже, запис в x відбувається раніше ніж запис в y. І що вкрай важливо – це дає команду: «Злити весь буфер!»
 *         і у шину пам'яті летить як x, так і y разом. Вони рухаються по шині як один пакет оновлень.
 *      2) Потік Б крутиться в циклі while(!y.load(acquire)). Він чекає, поки y стане true. Як тільки він бачить true,
 *         створюється магічний міст: операція release (запис y) синхронізується з (synchronizes-with) операцією acquire (читання y).
 *      3) Після цього читання x, який прийшов вже актуальним разом з x, і тому буде ++z.
 *
 *
 */

namespace order_acquire_release_2 {
    std::atomic<bool> x,y;
    std::atomic<int> z;

    void write_x_then_y() {
        x.store(true,std::memory_order_relaxed);
        y.store(true,std::memory_order_release);
    }

    void read_y_then_x() {
        while(!y.load(std::memory_order_acquire));
        if(x.load(std::memory_order_relaxed))
            ++z;
    }

    void run_code() {
        x=false;
        y=false;
        z=0;
        std::thread a(write_x_then_y);
        std::thread b(read_y_then_x);
        a.join();
        b.join();
        assert(z.load()!=0);
    }
}