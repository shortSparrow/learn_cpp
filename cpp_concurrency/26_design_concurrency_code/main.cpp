#include <algorithm>
#include <barrier>
#include <future>
#include <numeric>
#include <thread>
#include <vector>

/**
 * Ми розглянемо реалізацію паралельних версій трьох функцій зі стандартної бібліотеки C++.
 *
 * 1. Паралельна реалізація std::for_each
 * Щоб реалізувати паралельну версію цього, потрібно розділити діапазон на набори елементів для обробки в
 * кожному потоці. Ви заздалегідь знаєте кількість елементів, тому можете розділити дані перед початком обробки.
 *
 * Наведена нижче реалізація нагадує код із 24_exception_in_concurrency parallel_accumulate  Ключова відмінність
 * полягає в тому, що вектор futures зберігає std::future<void>, оскільки робочі потоки не повертають значення,
 * нам треба просто виконати лямбда-функцію на діапазоні від block_start до block_end
 */



// 1. Паралельна реалізація std::for_each
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


template<typename Iterator,typename Func>
void parallel_for_each_1(Iterator first,Iterator last,Func f) {
    unsigned long const length=std::distance(first,last);

    if(!length)
        return;

    unsigned long const min_per_thread=25;
    unsigned long const max_threads=(length+min_per_thread-1)/min_per_thread;
    unsigned long const hardware_threads=std::thread::hardware_concurrency();
    unsigned long const num_threads=std::min(hardware_threads!=0?hardware_threads:2,max_threads);
    unsigned long const block_size=length/num_threads;
    std::vector<std::future<void> > futures(num_threads-1);
    std::vector<std::thread> threads(num_threads-1);
    join_threads joiner(threads);
    Iterator block_start=first;

    for(unsigned long i=0;i<(num_threads-1);++i) {
        Iterator block_end=block_start;
        std::advance(block_end,block_size);
        std::packaged_task<void(void)> task(
            [=](){
                std::for_each(block_start,block_end,f);
            });
        futures[i]=task.get_future();
        threads[i]=std::thread(std::move(task));
        block_start=block_end;
    }
    std::for_each(block_start,last,f);

    for(unsigned long i=0;i<(num_threads-1);++i) {
        futures[i].get();
    }
}


/**
 * Код можна значно спростити якщо використати std::async яка сама створить нові потоки за потреби, тепер нам
 * не потрібно перейматися створенням потоків чи мати join_threads де у деструкторі буде обов'язково join()
 * на всі потоки в разі винятку.
 *
 * Як і у випадку з parallel_accumulate на основі std::async, ми розділяємо дані рекурсивно, а не перед виконанням,
 * оскільки не знаємо, скільки потоків буде використовувати бібліотека. Як і раніше, ви ділите дані навпіл на кожному
 * етапі, запускається одна половина асинхронно, а інша безпосередньо, доки залишок даних не стане занадто малим,
 * щоб його варто було розділяти, і в цьому випадку ви відкладаєте виконання до std::for_each
 */
template<typename Iterator,typename Func>
void parallel_for_each_2(Iterator first,Iterator last,Func f) {
    unsigned long const length=std::distance(first,last);
    if(!length)
        return;

    unsigned long const min_per_thread=25;
    if(length<(2*min_per_thread)) {
        std::for_each(first,last,f);
    } else {
        Iterator const mid_point=first+length/2;
        std::future<void> first_half=
            std::async(&parallel_for_each_2<Iterator,Func>,
                       first,mid_point,f);
        parallel_for_each_2(mid_point,last,f);
        first_half.get();
    }
}



/**
 * Перейдемо від алгоритмів, які повинні виконувати одну і ту ж операцію над кожним елементом до дещо складнішого
 * прикладу у вигляді std::find.
 *
 * std::find — це корисний алгоритм, який варто розглянути далі, оскільки він є одним із декількох алгоритмів,
 * які можуть виконуватися без обробки кожного елемента. Наприклад, якщо перший елемент у діапазоні відповідає критерію
 * пошуку, нема потреби перевіряти інші елементи. Це важлива властивість для продуктивності, яка має прямий вплив
 * на дизайн паралельної реалізації. Бо якщо наш елемент знаходиться на початку першої частини, і у нас є 3 потоки,
 * і вони не будуть зупинені при знайденні елемента то однопоточний варіант відпрацює швидше.
 * (Інші алгоритми цієї категорії включають std::equal і std::any_of).
 *
 * У нас є два варіанти, як повернути значення і як поширити будь-які винятки. Ми можемо використовувати масив future,
 * std::packaged_task, для передачі значень і винятків, а потім обробити результати назад в головному потоці;
 * або ми можемо використовувати std::promise, щоб встановити кінцевий результат безпосередньо з робочих потоків.
 *
 * Все залежить від того, як ви хочете обробляти винятки з робочих потоків. Якщо ми хочемо зупинитися на першому
 * виняткові (навіть якщо ми не обробили всі елементи), ми можемо використовувати std::promise для встановлення як
 * значення, так і винятку. З іншого боку, якщо ви хочете дозволити іншим робочим потокам продовжувати пошук,
 * ви можете використовувати std::packaged_task, зберегти всі винятки, а потім повторно кинути один з них, якщо збіг не знайдено.
 *
 * Будемо використовувати std::promise
 */

template<typename Iterator,typename MatchType>
Iterator parallel_find(Iterator first,Iterator last,MatchType match) {
    struct find_element {
        void operator()(Iterator begin, Iterator end, MatchType match, std::promise<Iterator>* result, std::atomic<bool>* done_flag) {
            try {
                for(;(begin!=end) && !done_flag->load();++begin) {
                    if(*begin==match) {
                        result->set_value(begin);
                        done_flag->store(true);
                        return;
                    }
                }
            } catch(...) {
                try {
                    result->set_exception(std::current_exception());
                    done_flag->store(true);
                } catch(...) {}
            }
        }
    };

    unsigned long const length=std::distance(first,last);

    if(!length)
        return last;

    unsigned long const min_per_thread=25;
    unsigned long const max_threads=(length+min_per_thread-1)/min_per_thread;
    unsigned long const hardware_threads=std::thread::hardware_concurrency();
    unsigned long const num_threads=std::min(hardware_threads!=0?hardware_threads:2,max_threads);
    unsigned long const block_size=length/num_threads;
    std::promise<Iterator> result;
    std::atomic<bool> done_flag(false);
    std::vector<std::thread> threads(num_threads-1);

    {
        join_threads joiner(threads);
        Iterator block_start=first;
        for(unsigned long i=0;i<(num_threads-1);++i) {
            Iterator block_end=block_start;
            std::advance(block_end,block_size);
            threads[i]=std::thread(find_element(),
                                   block_start,block_end,match,
                                   &result,&done_flag);
            block_start=block_end;
        }
        find_element()(block_start,last,match,&result,&done_flag);
    }

    if(!done_flag.load()) {
        return last;
    }

    return result.get_future().get();
}

/**
 * Варіант з std::async де сама бібліотека C++ буде вирішувати чи треба створювати новий потік.
 * Цей варіант коротший і простіший.
 *
 * Ми передаємо прапорець done в усі потоки через reference, щоб будь-який з них міг поставити його у true, якщо
 * знайде збіг.
 *
 * Якщо прямий пошук (той що у direct_result) повернув mid_point, то він не знайшов збігу, тому нам потрібно
 * отримати результат асинхронного пошуку, саме доя цього використовуємо рядок
 * return (direct_result==mid_point) ? async_result.get() : direct_result;
 *
 * Чому mid_point означає що результат не знайдено? Тому що ми передаємо mid_point як аргумент last у parallel_find_impl,
 * і у циклі, якщо шуканий елемент не знайдено, то повертається last, і в нашому випадку це mid_point.
 *
 * Може здатися, що direct_result == mid_point спрацює максимум один раз, але ні, він спрацює на кожному рівні
 * рекурсії, де пошук у лівій половині завершився невдачею. Бо повернутий результат у direct_result буде дорівнювати
 * mid_point, і кожен рівень рекурсії передає свій mid_point (як аргумент last), тобто, нехай у нас є число 100:
 * 1. first = 0; last = 100; mid_point=50;
 * 2. first = 0; last = 50; mid_point=25;
 * 3. first = 0; last = 25; mid_point=12;
 *
 * Крок 2 чекає відповіді від кроку 3, і якщо той не знайшов шукане, то поверне свій last, тобто 25
 * Крок 1 чекає відповіді від кроку 2, той отримав відповідь 25, вона дорівнює його mid_point, тож
 * він поверне свій last, а це 50 і так далі.
 *
 *
 * Як і раніше, використання std::async забезпечує безпеку від винятків та функції поширення винятків. Якщо пряма
 * рекурсія викликає виняток, деструктор future забезпечить, що потік, який виконує асинхронний виклик, буде завершений
 * до повернення функції, а якщо асинхронний виклик викликає виняток, виняток поширюється через виклик get().
 *
 * Використання блоку try/catch навколо всього цього служить лише для встановлення прапорця done на винятку та
 * забезпечення швидкого завершення всіх потоків у разі виникнення винятку. Без цього реалізація все одно була б
 * правильною, але продовжувала б перевіряти елементи, поки не завершився б кожен потік.
 */
template<typename Iterator,typename MatchType>
Iterator parallel_find_impl(Iterator first, Iterator last, MatchType match, std::atomic<bool>& done) {
    try {
        unsigned long const length=std::distance(first,last);
        unsigned long const min_per_thread=25;

        if(length<(2*min_per_thread)) {
            for(;(first!=last) && !done.load();++first) {
                if(*first==match) {
                    done=true;
                    return first;
                }
            }
            return last;
        } else {
            Iterator const mid_point=first+(length/2);
            std::future<Iterator> async_result=
                std::async(&parallel_find_impl<Iterator,MatchType>,
                           mid_point,last,match,std::ref(done));

            Iterator const direct_result=parallel_find_impl(first,mid_point,match,done);
            return (direct_result==mid_point) ? async_result.get() : direct_result;
        }
    } catch(...) {
        done=true;
        throw;
    }
}

template<typename Iterator,typename MatchType>
Iterator parallel_find_2(Iterator first, Iterator last, MatchType match) {
    std::atomic<bool> done(false);
    return parallel_find_impl(first,last,match,done);
}



/**
 * Паралельна реалізація std::partial_sum
 * std::partial_sum приймає масив чисел, нехай [1,2,3,4,5] і повертає новий масив такої самої довжини,
 * але значення будуть мати проміжні суми, тобто [1, 1+2,1+2+3, 1+2+3+4, 1+2+3+4+5] і на виході маємо
 * [1, 3, 6, 10, 15]
 * Це цікаво для паралелізації, оскільки ми не можемо просто розділити діапазон на частини і обчислювати
 * кожну частину окремо. Бо, наприклад, нам треба початкове значення кожного елемента додати до кожного
 * іншого елементу. Є два варіанти для паралельного обчислення:
 * Ми маємо масив [1, 2, 3, 4, 5, 6, 7, 8, 9]
 *
 * Варіант 1 - Алгоритм Блеллока (Blelloch Scan / Block-based Scan).
 *  - Розділяємо масив нехай на 3 частини і рахуємо проміжну суму в їх межах
 *      {1, 3, 6}, {4, 9, 15}, {7, 15, 24}
 *    тобто перший елемент в цих блоках такий як і в оригінальному масиві, другий є сумою оригінального першого і другого,
 *    а тертій є сумою модифікованого другого і оригінального третього.
 *
 * - Додаємо останній елемент першого блоку (6) до всіх елементів у другому блоці
 *      {1, 3, 6}, {10, 15, 21}, {7, 15, 24}
 * - Додаємо останній елемент другого блоку (21) до всіх елементів у третьому блоці
 *      {1, 3, 6}, {10, 15, 21}, {28, 36, 45}
 *      Фінальний результат
 *      [1, 3, 6, 10, 15, 21, 28, 36, 45]
 *
 * Варіант 2 - Алгоритм Хілліса-Стіла (Hillis-Steele Scan).
 * - Спочатку сумуємо поточний елемент і його сусіда зліва (зсув на 2^0)
 *      [1, 1+2, 2+3, 3+4, 4+5, 5+6, 6+7, 7+8, 8+9] -> [1, 3, 5, 7, 9, 11, 13, 15, 17]
 *      (оскільки у 1 сусіднього елемента зліва немає то буде +0, тобто лишиться просто 1)
 * - До вже утворених елементів додаємо другий сусідній елемент зліва (зсув на 2^1)
 *      [1, 3, 1+5, 3+7, 5+9, 7+11, 9+13, 11+15, 13+17] -> [1, 3, 6, 10, 14, 18, 22, 26, 30]
 *      (оскільки у 3 третього елемента зліва немає то буде +0, тобто лишиться просто 3)
 * - До вже утворених елементів додаємо четвертий сусідній елемент зліва (зсув на 2^2)
 *      [1, 3, 6, 10, 1+14, 3+18, 6+22, 10+26, 14+30] -> [1, 3, 6, 10, 15, 21, 28, 36, 44]
 * - До вже утворених елементів додаємо восьмий сусідній елемент зліва (зсув на 2^3)
 *      [1, 3, 6, 10, 15, 21, 28, 36, 1+44] -> [1, 3, 6, 10, 15, 21, 28, 36, 45]
 *
 * У варіанті 2 кількість кроків строго обмежена log(N). Тобто в другому варіанті складність буде O(n*log(n),
 * а в першому O(n).
 *
 * Початок з виділення потоків такий самий як і у прикладах вище
 * std::advance - зміщує ітератор на вказану кількість елементів std::advance(iterator, 3) - змістить ітератор
 * на 3 елементи.
 *
 * process_chunk виконує роботу в межах одного виділеного шматка пам'яті.
 * previous_end_value — вказівник на std::future, з якого потік чекатиме фінальну суму від попереднього шматка.
 * end_value — вказівник на std::promise, куди цей потік запише свій результат для наступного шматка.
 *
 *
 * Як працює цикл for
 * Цикл нарізає масив на шматки розміром block_size та зв'язує їх ф'ючерсами (future) та промісами (promise) в один ланцюжок.
 *
 * Пояснення за рядок: (i!=0) ? &previous_end_values[i-1] : 0.
 *  Коли i = 0 (перший потік), ми передаємо йому 0 (null-pointer) як previous_end_value, бо йому нічого чекати не
 *  треба (він працює з початку масиву). Він виконає локальний std::partial_sum і одразу закине свій останній елемент в end_values[0].
 *
 *  Коли i = 1 (другий потік), ми передаємо йому &previous_end_values[0]. Він почне працювати, зробить локальну суму,
 *  але потім зупиниться на .get(), чекаючи першого потоку.
 *
 * Завдяки вектору previous_end_values ми робимо "естафетну паличку": кожен наступний потік отримує future від promise
 * попереднього потоку.
 */

template<typename Iterator>
void parallel_partial_sum(Iterator first,Iterator last) {
    typedef typename Iterator::value_type value_type;

    struct process_chunk {
        void operator()(Iterator begin,Iterator last, std::future<value_type>* previous_end_value, std::promise<value_type>* end_value) {
            try {
                Iterator end=last;
                ++end;
                std::partial_sum(begin,end,begin);

                if(previous_end_value) {
                    value_type& addend=previous_end_value->get();
                    *last+=addend;
                    if(end_value) {
                        end_value->set_value(*last);
                    }
                    std::for_each(begin,last,[addend](value_type& item)
                                  {
                                      item+=addend;
                                  });
                } else if(end_value) {
                    end_value->set_value(*last);
                }
            } catch(...) {
                if(end_value) {
                    end_value->set_exception(std::current_exception());
                } else {
                    throw;
                }
            }
        }
    };

    unsigned long const length=std::distance(first,last);

    if(!length)
        return;

    unsigned long const min_per_thread=25;
    unsigned long const max_threads=(length+min_per_thread-1)/min_per_thread;
    unsigned long const hardware_threads=std::thread::hardware_concurrency();
    unsigned long const num_threads=std::min(hardware_threads!=0 ? hardware_threads : 2,max_threads);
    unsigned long const block_size=length/num_threads;
    typedef typename Iterator::value_type value_type;
    std::vector<std::thread> threads(num_threads-1);
    std::vector<std::promise<value_type>> end_values(num_threads-1);
    std::vector<std::future<value_type>> previous_end_values;
    previous_end_values.reserve(num_threads-1);
    join_threads joiner(threads);
    Iterator block_start=first;

    for(unsigned long i=0;i<(num_threads-1);++i) {
        Iterator block_last=block_start;
        std::advance(block_last,block_size-1);
        threads[i]=std::thread(process_chunk(),
                               block_start,block_last,
                               (i!=0) ? &previous_end_values[i-1] : 0,
                               &end_values[i]);
        block_start=block_last;
        ++block_start;
        previous_end_values.push_back(end_values[i].get_future());
    }

    Iterator final_element=block_start;
    std::advance(final_element,std::distance(block_start,last)-1); // беремо останній шматочок масиву
    process_chunk()(block_start,final_element, (num_threads>1) ? &previous_end_values.back(): 0, 0); // виконуємо останній шматочок масиву
}


/**
 * Другий підхід до обчислення часткових сум шляхом додавання елементів, що знаходяться все далі і далі, найкраще працює,
 * коли наші процесори можуть виконувати додавання синхронно. У цьому випадку подальша синхронізація не потрібна, оскільки
 * всі проміжні результати можуть бути передані безпосередньо до наступного процесора, який їх потребує. Але на практиці
 * ми рідко маємо справу з такими системами, за винятком тих випадків, коли один процесор може одночасно виконувати одну
 * і ту ж інструкцію для невеликої кількості елементів даних за допомогою так званих інструкцій SIMD
 * (Single-Instruction/Multiple-Data). Тому ми повинні розробляти код для загального випадку і явно синхронізувати
 * потоки на кожному кроці.
 *
 * Один із способів зробити це — використовувати barrier (бар'єр) — механізм синхронізації, який змушує потоки чекати,
 * поки необхідна кількість потоків не досягне бар'єру. Після того як всі потоки досягнуть бар'єру, вони всі
 * розблоковуються і можуть продовжувати роботу.
 *
 * Уявимо собі американські гірки на ярмарку. Якщо кількість людей, що чекають, є розумною, персонал ярмарку подбає про
 * те, щоб усі місця були зайняті, перш ніж американські гірки покинуть платформу. Бар'єр працює таким самим чином: ми
 * заздалегідь вказуємо кількість «місць», і потоки повинні чекати, поки всі «місця» не будуть зайняті. Як тільки
 * накопичиться достатня кількість потоків, що чекають, вони всі можуть продовжувати роботу; бар'єр скидається і
 * починає чекати наступну партію потоків. Часто ця конструкція використовується в циклі, де ті самі потоки
 * повертаються і чекають до наступного разу. Ідея полягає в тому, щоб тримати потоки в синхронізації, щоб один потік
 * не вирвався вперед інших і не вийшов із синхронізації. Для такого алгоритму це було б катастрофічно, оскільки потік,
 * що вирвався вперед, міг би потенційно змінити дані, які все ще використовувалися іншими потоками, або
 * використовувати дані, які ще не були правильно оновлені.
 *
 *
 * Покажемо просту реалізацію бар'єру.
 *
 * За допомогою цієї реалізації ми створюємо бар'єр з кількістю «місць», яка зберігається у змінній count.
 * Спочатку кількість місць на бар'єрі дорівнює цьому значенню. У міру очікування кожного потоку кількість місць
 * зменшується. Коли вона досягає нуля, кількість місць скидається назад до значення count, а покоління збільшується,
 * щоб сигналізувати іншим потокам, що вони можуть продовжувати.
 *
 * Якщо кількість вільних місць не досягає нуля, нам доведеться чекати. Ця реалізація використовує
 * простий спін-лок ( while(generation== my_generation) ), перевіряючи generation щодо значення, яке ми отримали на
 * початку wait(). Оскільки покоління generation тільки тоді, коли всі потоки досягли бар'єра, ми виконуємо yield()
 * під час очікування, щоб потік, що очікує, не забирав всі ресурси процесора в режимі зайнятого очікування.
 */

class barrier {
    unsigned const count;
    std::atomic<unsigned> spaces;
    std::atomic<unsigned> generation;
public:
    explicit barrier(unsigned count_):
        count(count_),spaces(count),generation(0)
    {}
    void wait() {
        unsigned const my_generation=generation;
        if(!--spaces) {
            spaces=count;
            ++generation;
        } else {
            while(generation== my_generation)
                std::this_thread::yield();
        }
    }

};

/**
 * У нас є фіксована кількість потоків, які повинні виконуватися в циклі з синхронізацією. Ну, це майже фіксована
 * кількість потоків, бо елементи на початку списку отримують свої остаточні значення після декількох кроків.
 * Це означає, що нам доведеться або підтримувати цикл цих потоків доти, доки не буде оброблено весь діапазон,
 * або дозволити бар'єру обробляти випадання потоків і зменшення їх кількості. Оберемо другий варіант.
 *
 * Це означає, що ми повинні змінити count на атомарну змінну, щоб ми могли оновлювати її з декількох потоків
 * без зовнішньої синхронізації.
 *
 *
 */



template<typename Iterator>
void parallel_partial_sum(Iterator first,Iterator last) {
    typedef typename Iterator::value_type value_type;
    struct process_element {
        void operator()(Iterator first,Iterator last,
                        std::vector<value_type>& buffer,
                        unsigned i, std::barrier<>& b) {
            value_type& ith_element=*(first+i);
            bool update_source=false;

            // Потік з індексом i оновлює свій елемент лише доти, доки поточний крок (stride) не став більшим за його власну позицію в масиві.
            for(unsigned step=0,stride=1;stride<=i;++step,stride*=2) {
                value_type const& source=(step%2) ? buffer[i] : ith_element;
                value_type& dest=(step%2) ? ith_element : buffer[i];
                value_type const& addend=(step%2) ? buffer[i-stride] : *(first+i-stride);
                dest=source+addend;
                update_source=!(step%2);
                b.arrive_and_wait();
            }

            if(update_source) {
                ith_element=buffer[i];
            }
            b.arrive_and_drop();
        }
    };

    unsigned long const length=std::distance(first,last);

    if(length<=1)
        return;

    std::vector<value_type> buffer(length);
    barrier b(length);
    std::vector<std::thread> threads(length-1);
    join_threads joiner(threads);
    Iterator block_start=first;
    for(unsigned long i=0;i<(length-1);++i) {
        threads[i]=std::thread(process_element(),first,last, std::ref(buffer),i,std::ref(b));
    }

    process_element()(first,last,buffer,length-1,b);
}