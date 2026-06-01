#include <atomic>
#include <thread>
#include <assert.h>

/**
 * std::atomic_thread_fence
 * std::atomic_thread_fence - це свого роду бар'єр який приймає прапорці memory_order, і завдяки ним він може ставити
 * бар'єри:
 *  memory_order_release - щоб жодна команда яка вище не опустилася нижче нього.
 *  memory_order_acquire - щоб жодна команда яка нижче не піднялася вище його.
 *
 * Однак на відміну від звичних нам .load() чи store() які працюють з конкретною атомарною змінною, std::atomic_thread_fence
 * працює з усіма що над ним чи під ним (залежно від memory_order).
 *
 * Не зважаючи на те, що у нас у x.store() та y.store() стоять std::memory_order_relaxed код виконається правильно.
 * У write_x_then_y першим стоїть x.store(), а під ним std::atomic_thread_fence, який забороняє x.store() опуститися нижче нього.
 * А під бар'єром стоїть y.store(), тож ми маємо чітку послідовність дії.
 * А у read_y_then_x крутитися y.load(), а під ним std::atomic_thread_fence(memory_order_acquire), який забороняє
 * x.load() піднятися вище нього.
 *
 *
 *
 * Може виникнути думка, якщо в нас вже є memory_order і ми можемо їх використовувати на атомарних операціях, то навіщо нам
 * std::atomic_thread_fence?
 * Є дві причини для цього механізму:
 *  1) Оптимізація для архітектур типу ARM / PowerPC: Якщо у тебе є 10 різних змінних, які треба записати, а потім подати сигнал,
 *     то використання store(release) на кожній змінній змусить ARM процесор ставити легкі бар'єри на кожному кроці.
 *     Якщо ж зробити 10 записів як relaxed, а в кінці поставити один великий std::atomic_thread_fence(release), процесор
 *     зробить один важкий "злив" кешу замість десяти дрібних. Це працює швидше.
 *  2) Синхронізація звичайних (не атомарних) даних: Як пише Ентоні Вільямс у книзі C++ Concurrency in Action:
 *     "they can enforce an ordering on non-atomic operations". Ми можемо заповнити величезний масив звичайних double
 *     або std::string (не атомарних!), поставити один release-fence, і один єдиний атомарний relaxed-прапорець.
 *     Fence проштовхне по шині даних навіть звичайні, не атомарні змінні. Це уберігає програму від стану гонки даних (data race)
 *     та невизначеної поведінки (Undefined Behavior).
 *
 *
 */

std::atomic<bool> x,y;
std::atomic<int> z;

void write_x_then_y() {
    x.store(true,std::memory_order_relaxed);
    std::atomic_thread_fence(std::memory_order_release);
    y.store(true,std::memory_order_relaxed);
}

void read_y_then_x() {
    while(!y.load(std::memory_order_relaxed));
    std::atomic_thread_fence(std::memory_order_acquire);
    if(x.load(std::memory_order_relaxed))
        ++z;
}

int main() {
    x=false;
    y=false;
    z=0;
    std::thread a(write_x_then_y);
    std::thread b(read_y_then_x);
    a.join();
    b.join();
    assert(z.load()!=0);
}