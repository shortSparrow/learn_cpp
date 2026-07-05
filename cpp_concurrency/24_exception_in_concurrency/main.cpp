#include <algorithm>
#include <functional>
#include <future>
#include <iostream>
#include <numeric>


/**
 * Безпека винятків у паралельних алгоритмах
 * Якщо в однопоточному коді виникає виняток (exception), то він може поширитися далі і ми можемо обробити його
 * де ми захочемо. Але з паралельним програмуванням це не так. Виняток не може поширюватися, оскільки він знаходиться
 * у неправильному стеку викликів. Якщо функція, запущена в новому потоці, завершується з винятком, програма припиняє роботу.
 *
 * Тобто якщо я напишу обробник try...catch у потоці і там станеться виняток то все ок, я йог оброблю і все заїбісь,
 * тут мова саме про випадок коли виняток або не оброблений або через rethrow виходить за межі функції потоку.
 */


/**
 * Наш код починається з std::distance(first,last), тут все в порядку, навіть якщо буде виняток, ми знаходимося в одному
 * потоці, тож це не проблема, і так далі все буде добре, доки ми не дійдемо до threads[i]=std::thread(...), бо
 * після створення першого thread ми втрапимо у халепу, якщо станеться виняток. Деструктори нових об'єктів std::thread
 * викличуть std::terminate і моя програма завершиться.
 */
template<typename Iterator,typename T>
struct accumulate_block_1 {
    void operator()(Iterator first,Iterator last,T& result) {
        result=std::accumulate(first,last,result);
    }
};

template<typename Iterator,typename T>
T parallel_accumulate_1(Iterator first,Iterator last,T init) {
    unsigned long const length=std::distance(first,last);
    if(!length)
        return init;

    unsigned long const min_per_thread=25;
    unsigned long const max_threads = (length+min_per_thread-1)/min_per_thread;
    unsigned long const hardware_threads = std::thread::hardware_concurrency();
    unsigned long const num_threads = std::min(hardware_threads!=0?hardware_threads:2,max_threads);
    unsigned long const block_size = length/num_threads;
    std::vector<T> results(num_threads);
    std::vector<std::thread> threads(num_threads-1);
    Iterator block_start=first;

    for(unsigned long i=0;i<(num_threads-1);++i) {
        Iterator block_end=block_start;
        std::advance(block_end,block_size);
        threads[i]=std::thread(
            accumulate_block_1<Iterator,T>(),
            block_start,block_end,
            std::ref(results[i]));

        block_start=block_end;
    }

    accumulate_block_1<Iterator,T>()(block_start,last,results[num_threads-1]);
    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));

    return std::accumulate(results.begin(),results.end(),init);
}


/**
 * Працювати безпечно з винятками нам дозволяють std::packaged_task та std::future.
 * Ми можемо переробити код вище з використанням std::packaged_task
 *
 * Перша заміна полягає в тому, що accumulate_block тепер повертає результат, а не присвоює його через посилання.
 * Наступна зміна полягає в тому, що замість вектора результатів ви маєте вектор futures для зберігання std::future<T> для кожного створеного потоку.
 * У циклі створення потоків спочатку створюється завдання для accumulate_block std::packaged_task<T(Iterator, Iterator)> оголошує завдання,
 * яке приймає два ітератори та повертає T (результат функції). Потім ви отримуєте майбутнє для цього завдання futures[i]=task.get_future();
 * і запускаєте це завдання в новому потоці, передаючи початок і кінець блоку для обробки std::thread(std::move(task),block_start,block_end).
 * Коли задача закінчиться то результат буде запихнутий у future, як і виняток.
 * У last_result зберігаємо результат з останнього блоку на якого не вистачило потоку.
 * Далі у циклі просто отримаємо значення future кожної задачі (futures[i].get()) і додаємо їх до result. Тепер якщо
 * якийсь потік кине виняток, то futures[i].get() прокине його в основний потік де ми зможемо його опрацювати.
 *
 * Якщо більше ніж один робочий потік кидає виняток, буде поширений тільки один, але це не є великою проблемою. Якщо це має
 * значення, ви можете використовувати щось на зразок std::nested_exception, щоб захопити всі винятки і кинути їх замість цього.
 *
 */

template<typename Iterator,typename T>
struct accumulate_block_2 {
    T operator()(Iterator first,Iterator last) {
        return std::accumulate(first,last,T());
    }
};
template<typename Iterator,typename T>
T parallel_accumulate_2(Iterator first,Iterator last,T init) {
    unsigned long const length=std::distance(first,last);
    if(!length)
        return init;

    unsigned long const min_per_thread=25;
    unsigned long const max_threads = (length+min_per_thread-1)/min_per_thread;
    unsigned long const hardware_threads=std::thread::hardware_concurrency();
    unsigned long const num_threads=std::min(hardware_threads!=0?hardware_threads:2,max_threads);
    unsigned long const block_size=length/num_threads;
    std::vector<std::future<T>> futures(num_threads-1);
    std::vector<std::thread> threads(num_threads-1);
    Iterator block_start=first;

    for(unsigned long i=0;i<(num_threads-1);++i) {
        Iterator block_end=block_start;
        std::advance(block_end,block_size);
        std::packaged_task<T(Iterator,Iterator)> task(accumulate_block_2<Iterator,T>());
        futures[i]=task.get_future();
        threads[i]=std::thread(std::move(task),block_start,block_end);
        block_start=block_end;
    }

    T last_result=accumulate_block_2<Iterator,T>()(block_start,last);
    std::for_each(threads.begin(),threads.end(),std::mem_fn(&std::thread::join));
    T result=init;

    for(unsigned long i=0;i<(num_threads-1);++i) {
        result+=futures[i].get();
    }

    result += last_result;
    return result;
}



/**
 * Витік потоків (leaking threads) — це ситуація, коли фонові потоки залишаються працювати в системі самі по собі,
 * у той час, як функція, яка їх створила, уже аварійно завершилася через виняток.
 *
 *
 * Уявимо, що в коді вище під час обчислення last_result станеться виняток, тоді рядки з .join() ніколи не будуть викликані.
 * Об'єкти std::thread, які лежали у векторі threads, знищуються (розкручування стеку).
 *
 * Якщо ми не зловимо цей виняток, то деструктори std::thread викличуть std::terminate() і програма просто впаде.
 * Але навіть якщо ми десь вище перехопимо виняток (або якщо ми використовували інший механізм, який не падає відразу),
 * самі операційні потоки в ОС продовжують жити та виконувати роботу. Вони "відірвалися" від нашої програми, стали
 * безконтрольними зомбі-потоками, які споживають процесор та пам'ять.
 *
 * Способи розв'язання:
 *  1) Лобовий спосіб. Ми відловимо виняток у try...catch і у самому catch ще раз зробимо join() для всіх joinable потоків.
 *     Недоліком такого підходу є дублювання коду, нам треба робити join() як у try, так і в catch, якщо ми отримали виняток.
 *
 *  2) Елегантний (RAII та клас join_threads). Можна делегувати join() деструктору класу, який буде викликатися сам.
 *     Такий підхід реалізовано нижче у клас join_threads. Це ідіоматичний спосіб очищення ресурсів у C++.
 */

// Спосіб 1
void solution_1() {
    try {
        for(unsigned long i=0;i<(num_threads-1);++i) {
            // ... as before
        }
        T last_result=accumulate_block_2<Iterator,T>()(block_start, std::chrono::last);
        std::for_each(threads.begin(),threads.end(),std::mem_fn(&std::thread::join));
    } catch(...) {
        for(unsigned long i=0;i<(num_thread-1);++i) {
            if(threads[i].joinable())
                thread[i].join();
        }
        throw;
    }
}

/**
 * Спосіб 2
 * Варто звернути увагу, що виклики futures[i].get() будуть блокуватися, поки результати не будуть готові, тому на цьому
 * етапі нам не потрібно явно об'єднувати потоки через join() як у accumulate_block_2.
 */
class join_threads {
    std::vector<std::thread>& threads;
public:
    explicit join_threads(std::vector<std::thread>& threads_): threads(threads_) {}

    ~join_threads()  {
        for(unsigned long i=0;i<threads.size();++i) {
            if(threads[i].joinable())
                threads[i].join();
        }
    }
};


template<typename Iterator,typename T>
T parallel_accumulate_3(Iterator first,Iterator last,T init) {
    unsigned long const length=std::distance(first,last);
    if(!length)
        return init;

    unsigned long const min_per_thread=25;
    unsigned long const max_threads=(length+min_per_thread-1)/min_per_thread;
    unsigned long const hardware_threads=std::thread::hardware_concurrency();
    unsigned long const num_threads=std::min(hardware_threads!=0?hardware_threads:2,max_threads);
    unsigned long const block_size=length/num_threads;
    std::vector<std::future<T> > futures(num_threads-1);
    std::vector<std::thread> threads(num_threads-1);
    join_threads joiner(threads);
    Iterator block_start=first;

    for(unsigned long i=0;i<(num_threads-1);++i) {
        Iterator block_end=block_start;
        std::advance(block_end,block_size);
        std::packaged_task<T(Iterator,Iterator)> task(accumulate_block_2<Iterator,T>());
        futures[i]=task.get_future();
        threads[i]=std::thread(std::move(task),block_start,block_end);
        block_start=block_end;
    }

    T last_result=accumulate_block_2<Iterator,T>()(block_start,last);
    T result=init;

    for(unsigned long i=0;i<(num_threads-1);++i) {
        result+=futures[i].get();
    }
    result += last_result;
    return result;
}


/**
 * Безпечні винятки за допомогою std::async()
 *
 * Тепер, коли ми побачили, що потрібно для безпеки винятків при явному управлінні потоками, подивимося на те саме, але
 * зроблене за допомогою std::async(). В цьому випадку бібліотека бере на себе управління потоками, і всі створені потоки
 * завершуються, коли future готове. Ключовим моментом для безпеки винятків є те, що якщо ми знищуємо future, не чекаючи на нього,
 * деструктор буде чекати на завершення потоку. Це дозволяє уникнути проблеми витоку потоків, які все ще виконуються
 * і зберігають посилання на дані
 *
 * Розглянемо як уе працює із звичайним потоком std::thread та з std::async
 * 1. Звичайний std::thread (без обгорток)
 *   - У головному потоці вилітає виняток.
 *   - Об'єкт std::thread знищується.
 *   - Оскільки він просто знищується (без очікування), фоновий потік залишається «літати» в ОС сам по собі.
 *   - Це і є витік (leak): функція завершилася, а потоки досі працюють у фоні без нагляду.
 *     (В C++ це взагалі тригерить std::terminate, щоб не створювати таких зомбі).
 *
 * 2. Варіант з std::async та std::future
 *   - У головному потоці вилітає виняток.
 *   - Стек розкручується, і знищується об'єкт std::future.
 *   - Деструктор std::future каже: «О ні, ти не підеш далі, поки той потік не допрацює!». Він зупиняє розкручування стеку головного потоку і чекає.
 *   - Фоновий потік завершує роботу.
 *   - Деструктор std::future відпускає головний потік, і виняток летить далі.
 *
 *
 *
 * Наведений нижче приклад використовує рекурсивний поділ даних, а не попередній розрахунок поділу даних на фрагменти,
 * але він набагато простіший за попередню версію і він все ще захищена від винятків. Як і раніше, ми починаємо з
 * визначення довжини послідовності (length=std::distance(first,last);), і якщо вона менша за максимальний розмір ми
 * робимо прямий виклик std::accumulate(). Якщо елементів більше, ніж розмір блоку, ми знаходимо середину і запускаємо
 * асинхронне завдання для обробки цієї половини. Друга половина обробляється за допомогою прямого рекурсивного виклику.
 * Потім результати двох викликів додаються разом.
 *
 * Перевага цього підходу в тому, що у нас є паралельність і одночасно захист від винятків за допомогою std::future, бо
 * виняток з потоку буде перекинуто у головний потік і також future буде чекати закінчення потоку, тож зомбі потоків
 * при неочікуваних винятках теж не буде.
 */

template<typename Iterator,typename T>
T parallel_accumulate(Iterator first,Iterator last,T init) {
    unsigned long const length=std::distance(first,last);
    unsigned long const max_chunk_size=25;

    if(length<=max_chunk_size) {
        return std::accumulate(first,last,init);
    } else {
        Iterator mid_point=first;
        std::advance(mid_point,length/2);
        std::future<T> first_half_result=
            std::async(parallel_accumulate<Iterator,T>,
                       first,
                       mid_point,
                       init);

        T second_half_result=parallel_accumulate(mid_point,last,T());
        return first_half_result.get() + second_half_result;
    }
}