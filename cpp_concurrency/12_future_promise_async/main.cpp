#include <future>
#include <iostream>


/**
 * Асинхронність у C++: Future, Promise та task
 *
 * std::future та std::shared_future (Отримувачі)
 * Це об'єкти, через які ми отримуємо результат, який ще не готовий.
 * std::future<T>: Унікальне володіння (як unique_ptr). Тільки один об'єкт може чекати на результат. Після виклику .get() ф'ючерс стає недійсним.
 * std::shared_future<T>: Спільне володіння (як shared_ptr). Кілька потоків можуть чекати на один і той самий результат.
 *
 * Основні методи:
 *  .get() — блокує потік, поки результат не буде готовий. Повертає значення.
 *  .wait() — просто чекає готовності, не забираючи значення.
 *
 *
 * std::promise (Ручне керування)
 * Низькорівневий інструмент, що забезпечує встановлення значення або винятку.
 * Це "точка запису". Якщо future — це вихід труби, то promise — це вхід.
 * Використовується, коли результат приходить не з конкретної функції, а як подія (з мережі, від GUI або переривання заліза).
 * Це низькорівневий інструмент для передачі даних.
 *
 * Основні методи:
 *  .get_future() - повертає future який потім можна викликати і отримати результат через .get() або .wait()
 *  .set_value(T value) - записує значення. Усі потоки, що заблоковані на future.get(), миттєво прокидаються
 *  .set_exception(std::exception_ptr p) - Це дуже крута фішка. Ти можеш передати у future не значення, а помилку.
 *                                         На стороні future.get() цей виняток буде перевикинутий.
 *                                         Це дозволяє обробляти помилки з іншого потоку так, ніби вони сталися у твоєму поточному.
 *
 * Нюанс: Якщо об'єкт std::promise знищується без виклику set_value чи set_exception, пов'язаний ф'ючерс отримає виняток
 * std::future_error з кодом broken_promise
 *
 * Важливо пам'ятати, що future — це споживач (consumer). Він не може змінити дані, він може їх тільки дочекатися.
 */

void manual_worker(std::promise<int> p) {
    // Імітуємо складну роботу або очікування події
    std::this_thread::sleep_for(std::chrono::seconds(1));
    p.set_value(42); // Записуємо результат вручну
}

void promise_example() {
    std::promise<int> p;
    std::future<int> f = p.get_future();

    std::thread t(manual_worker, std::move(p)); // promise не можна копіювати

    std::cout << "Waiting for promise... " << f.get() << std::endl;
    t.join();
}

void promise_error_example() {
    std::promise<int> p;
    bool error {true};
    try {
        // Якась логіка
        if (error) {
            throw std::runtime_error("Мережа впала!");
        }
        p.set_value(100);
    } catch (...) {
        p.set_exception(std::current_exception()); // Передаємо помилку далі
    }
}

/**
 * std::packaged_task (Напівавтомат)
 * Це обгортка над функцією (callable), яка автоматично записує результат у future після виконання.
 * Це зручно для створення черг завдань (Thread Pools). Планувальник просто викликає task(), не знаючи, що всередині.
 */

void packaged_task_example() {
    std::packaged_task<int(int, int)> task([](int a, int b) {
        return a + b;
    });
    std::future<int> res = task.get_future();

    task(10, 20); // Викликаємо вручну в будь-якому потоці

    // Приклад з запуском в іншому потоці. Важливо: task не можна копіювати, тільки переміщувати (std::move)
    // std::thread t(std::move(task), 5,8);
    // t.detach(); // Потік пішов працювати сам по собі

    std::cout << "Result: " << res.get() << std::endl;
}


/**
 * std::async (Автопілот)
 * Високорівневий інструмент. Він сам створює promise, сам (зазвичай) запускає потік і повертає future.
 *
 * Повернення значень із фонових завдань
 * Ви використовуєте std::async для запуску асинхронного завдання, для якого вам не потрібен результат відразу.
 * Замість того щоб надати вам об'єкт std::thread для очікування, std::async повертає об'єкт std::future,
 * який зрештою буде містити значення, що повертається функцією. Коли вам потрібне значення, ви просто
 * викликаєте get() на future, і потік блокується, поки future не буде готовий, а потім повертає значення.
 *
 * Додатково std::async може приймати параметр який визначить її поведінку
 * std::launch::async -  функція має виконуватися у новому потоці
 * std::launch::deferred - виклик функції має бути відкладений до виклику wait() або get() на future
 */


int find_the_answer_to_ltuae() {};
void do_other_stuff() {};

void async_example() {
    std::future<int> answer = std::async(find_the_answer_to_ltuae);
    do_other_stuff();
    std::cout<< "The answer is " << answer.get() <<std::endl;


    // Приклад із класом
    struct X {
        void foo(int, std::string const&) {};
        std::string bar(std::string const&) {};
    };
    X x;
    auto f1=std::async(&X::foo,&x,42,"hello");
    auto f2=std::async(&X::bar,x,"goodbye");
    auto f3=std::async([&x] {
        x.bar("He");
    });


    auto f4 = std::async(std::launch::async, [&x] {
        x.bar("X");
    }); // Виконати в новому потоці

    auto f5 = std::async(std::launch::deferred, [&x] {
       x.foo(1, "R");
    }); // виконається у .wait() або у .get()
    f5.wait();




    /**
     * Є одна особливість, на якій часто ловляться розробники: якщо ви отримали std::future саме 
     * від std::async (з політикою async), то його деструктор заблокує потік, поки фонова задача 
     * не завершиться.
     */

     {
        auto fut = std::async(std::launch::async, do_other_stuff);
    } // Деструктор fut заблокує виконання тут, поки do_other_stuff не допрацює.
}


/**
 * future - це базова конструкція, яка потрібна для асинхронності
 * promise - це те що створює future (або напряму, або через async чи packaged_task. Десь ми будемо використовувати promise
 *          напряму через set_value() чи set_exception(), а десь достатньо буде async
 * async - це спосіб виконати future і чекати в нашому потоці доки задача не буде виконана і отримати значення, якщо таке є
 * packaged_task - потрібно коли виконати кілька важких задач, які самі по собі не повертають future. Він потрібен для того,
 * щоб "одружити" будь-яку функцію з механізмом future.
 *
 *
 * У JS Promise — це один об'єкт, який поєднує в собі і можливість чекати результат, і можливість його встановити.
 * У C++ цю роль розділили між двома класами: std::promise (той, хто записує) та std::future (той, хто читає).
 *
 * Ось як би це було б у JS
 * const p = new Promise((resolve, reject) => {
 *   // resolve — це як set_value()
 *   // reject — це як set_exception()
 * });
 * p — це як std::future
 *
 * future та promise у C++ створені для передачі даних між потоками. Ми в одному потоці викликали якусь "важку" операцію
 * і завдяки future та promise можемо отримати результат в іншому потоці
 */


int main() {





    return 0;
}


