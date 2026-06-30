#include <exception>
#include <stack>
#include <mutex>
#include <memory>


 /**
  * Ця нотатка розповідає як будувати багатопоточні структури даних на основі mutex, тобто це lock-based concurrent data structures.
  * Коли замість mutex будуть атомарні операції тоді це буде lock-free concurrent data structures.
  *
  * Ця нотатка є компіляцією розділу 6 книги Concurrency in Action by Anthony Williams
  *
  * Ця нотатка містить 6 прикладів, вони ідуть по збільшенню складності, від простіших прикладів, з гіршою паралелізацію до складніших з кращою.
  * Реалізовано три структури даних: threadsafe_stack, threadsafe_queue, threadsafe_lookup_table.
  *
  *
  * Зміст:
  * Приклад №1: threadsafe_stack
  * Приклад №2: threadsafe_queue (Базовий варіант)
  * Приклад №3: threadsafe_queue (Оптимізація виділення пам'яті)
  * Приклад №4: Однопотокова queue на зв'язному списку (Проміжний крок)
  * Приклад №5: threadsafe_queue з фіктивним вузлом (Dummy Node)
  * Приклад №6: threadsafe_lookup_table (Хеш-таблиця)
  *
  *
  * Приклад №1: threadsafe_stack
  *     Суть: Базова стекова структура з одним глобальним м'ютексом.
  *
  *
  * Приклад №2: threadsafe_queue (Базовий варіант)
  *     Суть: Проста черга на базі std::queue з одним м'ютексом та умовною змінною (std::condition_variable).
  *     Проблема: Низький паралелізм. Поки один потік пише (push) або читає (pop), весь контейнер заблоковано.
  *               Неможливо розділити блокування початку (head) та кінця (tail) черги.
  *
  *
  * Приклад №3: threadsafe_queue (Оптимізація виділення пам'яті)
  *     Суть: Черга, що зберігає std::shared_ptr<T> замість чистих значень T.
  *     Ключова ідея: Винесення важкої операції виділення пам'яті (std::make_shared<T>) за межі критичної секції (до захоплення м'ютексу).
  *                   Це мінімізує час блокування інших потоків при додаванні великих об'єктів.
  *
  *
  * Приклад №4: Однопотокова queue на зв'язному списку (Проміжний крок)
  *     Суть: Власна реалізація черги через вузли (node) з використанням std::unique_ptr та сирих вказівників для tail.
  *
  *
  * Приклад №5: threadsafe_queue з фіктивним вузлом (Dummy Node)
  *     Суть: Черга з двома окремими м'ютексами (head_mutex та tail_mutex).
  *     Ключова ідея: Використання порожнього фіктивного вузла (dummy node), щоб head і tail ніколи не вказували на один і той
  *                   самий елемент у порожній черзі або черзі з одного елемента. Це повністю усуває стан гонитви між
  *                   push() та pop() без синхронізації обох локів.
  *
  *
  * Приклад №6: threadsafe_lookup_table (Хеш-таблиця)
  *     Суть: Потокобезпечна таблиця пошуку (стрибаючий хеш) із фіксованою кількістю бакетів (кошиків).
  *     Ключова ідея: Дрібнозернисте блокування на рівні окремих бакетів за допомогою std::shared_mutex (дозволяє множинне
  *                   читання через shared_lock та монопольний запис через unique_lock).
  */


// Приклад №1: threadsafe_stack
struct empty_stack: std::exception {
    const char* what() const noexcept override;
};

template<typename T>
class threadsafe_stack {
private:
    std::stack<T> data;
    mutable std::mutex m;
public:
    threadsafe_stack(){}

    threadsafe_stack(const threadsafe_stack& other) {
        std::lock_guard<std::mutex> lock(other.m);
        data=other.data;
    }

    threadsafe_stack& operator=(const threadsafe_stack&) = delete;

    void push(T new_value) {
        std::lock_guard<std::mutex> lock(m);
        data.push(std::move(new_value));
    }

    // Варіант pop() із поверненням shared_ptr (безпечно щодо виключень)
    std::shared_ptr<T> pop() {
        std::lock_guard<std::mutex> lock(m);
        if(data.empty()) throw empty_stack();
        std::shared_ptr<T> const res(std::make_shared<T>(std::move(data.top())));
        data.pop();
        return res;
    }

    // Варіант pop() із передачею посилання
    void pop(T& value) {
        std::lock_guard<std::mutex> lock(m);
        if(data.empty()) throw empty_stack();
        value=std::move(data.top());
        data.pop();
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(m);
        return data.empty();
    }
};

//-------------------------------------------------------------------------------
/**
 * Приклад №2: threadsafe_queue (Базовий варіант)
 *
 * Ми хочемо зробити потокобезпечну структуру даних, візьмемо для прикладу queue.
 * Цей приклад потокобезпечний і містить один mutex, але проблема в тому, що коли один
 * потік записує інший не може зчитати дані, і кілька потоків не можуть одночасно записувати
 * (навіть зробити підготовчу роботу з виділення пам'яті не можуть). Тож не дивлячись на те,
 * що цей варіант працює, він не є по справжньому багатопоточним.
 * Було б добре замість одного глобального mutex мати два, один який блокує head, а другий tail,
 * це дозволить досягти справжнього паралелізму
 *
 */
#include <queue>
#include <condition_variable>

template<typename T>
class threadsafe_queue_1 {
private:
    mutable std::mutex mut;
    std::queue<T> data_queue;
    std::condition_variable data_cond;
public:
    threadsafe_queue_1() {}

    void push(T new_value) {
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(std::move(new_value));
        data_cond.notify_one();
    }

    void wait_and_pop(T& value) {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk,[this]{return !data_queue.empty();});
        value=std::move(data_queue.front());
        data_queue.pop();
    }

    std::shared_ptr<T> wait_and_pop() {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk,[this]{return !data_queue.empty();});
        std::shared_ptr<T> res(
            std::make_shared<T>(std::move(data_queue.front())));
        data_queue.pop();
        return res;
    }

    bool try_pop(T& value) {
        std::lock_guard<std::mutex> lk(mut);
        if(data_queue.empty())
            return false;
        value=std::move(data_queue.front());
        data_queue.pop();
        return true;
    }

    std::shared_ptr<T> try_pop() {
        std::lock_guard<std::mutex> lk(mut);
        if(data_queue.empty())
            return std::shared_ptr<T>();
        std::shared_ptr<T> res(
            std::make_shared<T>(std::move(data_queue.front())));
        data_queue.pop();
        return res;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lk(mut);
        return data_queue.empty();
    }
};


// ___________________________________________________________________
/**
 * Приклад №3: threadsafe_queue (Оптимізація виділення пам'яті)
 *
 * Різниця між вищим кодом і цим лише в data_queue, вище це було std::queue<T>, а зараз
 * std::queue<std::shared_ptr<T>>, тобто черга з pointer.
 *
 * І ключова різниця у методі push, бо раніше па'ять виділялася при std::move коли м'ютекс був заблокований,
 * і якби там копіювався або переміщався великий об'єкт, скажімо на 10 МБ, то це б блокувало м'ютекс,
 * а є правильно, що блокувати м'ютекс треба мінімально по часу, щоб не блочити інші потоки.
 *
 * А в цьому випадку виділення пам'яті йде до блокування mutex, тож mutex буде заблокований мінімальний час.
 */
template<typename T>

class threadsafe_queue_2 {
private:
    mutable std::mutex mut;
    std::queue<std::shared_ptr<T>> data_queue;
    std::condition_variable data_cond;

public:
    threadsafe_queue_2() {}

    void wait_and_pop(T& value) {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk,[this]{return !data_queue.empty();});
        value=std::move(*data_queue.front());
        data_queue.pop();
    }

    bool try_pop(T& value) {
        std::lock_guard<std::mutex> lk(mut);
        if(data_queue.empty())
            return false;
        value=std::move(*data_queue.front());
        data_queue.pop();
        return true;
    }

    std::shared_ptr<T> wait_and_pop() {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk,[this]{return !data_queue.empty();});
        std::shared_ptr<T> res=data_queue.front();
        data_queue.pop();
        return res;
    }

    std::shared_ptr<T> try_pop() {
        std::lock_guard<std::mutex> lk(mut);
        if(data_queue.empty())
            return std::shared_ptr<T>();
        std::shared_ptr<T> res=data_queue.front();
        data_queue.pop();
        return res;
    }

    void push(T new_value) {
        std::shared_ptr<T> data(
            std::make_shared<T>(std::move(new_value)));
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(data);
        data_cond.notify_one();
    }

    bool empty() const {
        std::lock_guard<std::mutex> lk(mut);
        return data_queue.empty();
    }
};

// ______________________________________________________________
/**
 * Приклад №4: Однопотокова queue на зв'язному списку (Проміжний крок)
 *
 * Це проміжний крок. Він показує, що стандартний std::queue не дозволяє розділити head та tail, бо його внутрішня
 * реалізація приховує вузли. Тому ми пишемо власну чергу з нуля. В наступному лістингу ми додамо до неї паралелізацію
 * через mutex
 */
template<typename T>
class queue {
private:
    struct node {
        T data;
        std::unique_ptr<node> next;
        node(T data_):
            data(std::move(data_))
        {}
    };
    std::unique_ptr<node> head;
    node* tail; // тут доцільно використовувати саме pointer, а не smart pointer

public:
    queue(): tail(nullptr) {}
    queue(const queue& other)=delete;
    queue& operator=(const queue& other)=delete;

    // Видаляємо перший елемент і повертаємо його
    std::shared_ptr<T> try_pop() {
        if(!head) {
            return std::shared_ptr<T>();
        }
        std::shared_ptr<T> const res( std::make_shared<T>(std::move(head->data)) ); // забрали дані з head
        std::unique_ptr<node> const old_head=std::move(head); // порожній head (оболонку) перемістили у old_head, а head стає nullptr
        head=std::move(old_head->next); // Тепер head стає тим елементом, що раніше йшов після оригінального head
        if(!head)
            tail=nullptr;
        return res;
    }
    void push(T new_value) {
        std::unique_ptr<node> p(new node(std::move(new_value))); // створили pointer який посилається на new_value (поле next посилається на nullptr)
        node* const new_tail=p.get(); // зберігаємо адресу на яку посилається pointer p
        if(tail) {
            tail->next=std::move(p); // старий tail->next починає володіти об'єктом на який посилався p (тобто new_value). А сам p стає порожнім об'єктом (nullptr)
        } else {
            head=std::move(p);
        }
        tail=new_tail; // new_tail — це сирий вказівник (raw pointer) типу node*. Сирий вказівник — це просто число (адреса в пам'яті, наприклад 0x7ffdad32).
                       // Він не володіє об'єктом, він просто на нього «показує»
    }
};



// ____________________________________________________________________
/**
 * Приклад №5: threadsafe_queue з фіктивним вузлом (Dummy Node)
 *
 * В цьому варіанті додамо два mutex для кращої паралелізації.
 *
 * Якщо ми просто спробуємо додати head_mutex та tail_mutex, ми зіткнемося з критичною проблемою, коли в черзі
 * залишається рівно один елемент.
 *      - У черзі з одного елемента head та tail вказують на один і той самий вузол.
 *      - Функція push() намагається оновити tail->next.
 *      - Функція try_pop() намагається прочитати head->next.
 *      - Оскільки вузол один і той самий, tail->next та head->next — це одна й та сама ділянка пам'яті. Виникає стан гонитви (data race).
 *      - Щоб дізнатися, чи черга порожня або чи в ній один елемент, потрібно перевірити і head, і tail.
 *        Через це обидва методи змушені блокувати обидва м'ютекси одночасно, що повністю нівелює перевагу роздільних локів.
 * Рішення — Фіктивний вузол (Dummy Node)
 * черга ніколи не буває абсолютно порожньою. При створенні черги в неї одразу поміщається один "фіктивний" (порожній) вузол,
 * який не містить корисних даних.
 *
 */
template<typename T>
class threadsafe_queue_3 {
private:
    struct node {
        std::shared_ptr<T> data;
        std::unique_ptr<node> next;
    };
    std::mutex head_mutex;
    std::unique_ptr<node> head;
    std::mutex tail_mutex;
    node* tail;
    node* get_tail() {
        std::lock_guard<std::mutex> tail_lock(tail_mutex);
        return tail;
    }
    std::unique_ptr<node> pop_head() {
        std::lock_guard<std::mutex> head_lock(head_mutex);

        if(head.get()==get_tail()) {
            return nullptr;
        }
        std::unique_ptr<node> old_head=std::move(head);
        head=std::move(old_head->next);
        return old_head;
    }
public:
    threadsafe_queue_3():
        head(new node),
        tail(head.get())
    {}

    threadsafe_queue_3(const threadsafe_queue_3& other)=delete;
    threadsafe_queue_3& operator=(const threadsafe_queue_3& other)=delete;
    std::shared_ptr<T> try_pop() {
        std::unique_ptr<node> old_head=pop_head();
        return old_head ? old_head->data : std::shared_ptr<T>();
    }
    void push(T new_value) {
        std::shared_ptr<T> new_data(
            std::make_shared<T>(std::move(new_value)));
        std::unique_ptr<node> p(new node);
        node* const new_tail=p.get(); // зберігаємо адресу на яку посилається pointer p
        std::lock_guard<std::mutex> tail_lock(tail_mutex);
        tail->data=new_data;
        tail->next=std::move(p); // старий tail->next починає володіти об'єктом на який посилався p (тобто new_value). А сам p стає порожнім об'єктом (nullptr)
        tail=new_tail; // new_tail — це сирий вказівник (raw pointer) типу node*. Сирий вказівник — це просто число (адреса в пам'яті, наприклад 0x7ffdad32).
                       // Він не володіє об'єктом, він просто на нього «показує»
    }
};


// _________________________________________________________________________________________________________

/**
 * Приклад №6: threadsafe_lookup_table (Хеш-таблиця)
 *
 * Designing more complex lock-based data structures
 * Stack та queue є простими: їх інтерфейс надзвичайно обмежений, і вони суворо зосереджені на конкретній меті.
 * Не всі структури даних є настільки простими; більшість структур даних підтримують різноманітні операції. В принципі, це може
 * призвести до більших можливостей для паралельності, але також ускладнює завдання захисту даних, оскільки необхідно
 * враховувати різні моделі доступу. Точність різних операцій, які можна виконувати, є важливою при проєктування цих
 * структур даних для паралельного доступу.
 *
 *
 * Це дрібнозернисте блокування (fine-grained locking) на рівні окремих бакетів (кошиків). Замість того щоб закривати на
 * замок увесь масив, ми закриваємо лише ту комірку, в яку безпосередньо ліземо. Якщо два потоки одночасно звертаються
 * до різних бакетів, вони працюють паралельно і взагалі не чекають один на одного.
 *
 * Як опрацьовуються колізії при хешуванні:
 *  threadsafe_lookup_table містить std::list buckets - це елементи структури
 *  Кожен buckets - це bucket_type, який містить поле data (де лежать дані цього елемента структури)
 *  Поле data - це std::list<bucket_value> тобто якщо на один елемент структури буде 2 значення, то в списку data
 *  буде два елементи bucket_value.
 *
 * Тобто у нас є список з bucket_type (це окремий елемент структури), а він в свою чергу містить поле data, яке є
 * списком std::list<bucket_value>, тобто якщо на один елемент структури буде 2 значення, то в полі data буде два елементи списку.
 */

#include <list>
#include <shared_mutex>
#include <algorithm>
#include <map>

template<typename Key, typename Value, typename Hash=std::hash<Key>>
class threadsafe_lookup_table {
private:
    class bucket_type {
    private:
        typedef std::pair<Key,Value> bucket_value;
        typedef std::list<bucket_value> bucket_data;
        typedef typename bucket_data::iterator bucket_iterator;
        bucket_data data;
        mutable std::shared_mutex mutex;

        bucket_iterator find_entry_for(Key const& key) const {
            return std::find_if(
                data.begin(),data.end(),
                [&](bucket_value const& item) {return item.first==key;}
            );
        }
    public:
        Value value_for(Key const& key,Value const& default_value) const {
            std::shared_lock<std::shared_mutex> lock(mutex);
            bucket_iterator const found_entry=find_entry_for(key);
            return (found_entry==data.end()) ? default_value : found_entry->second;
        }
        void add_or_update_mapping(Key const& key,Value const& value) {
            std::unique_lock<std::shared_mutex> lock(mutex);
            bucket_iterator const found_entry=find_entry_for(key);
            if(found_entry==data.end()) {
                data.push_back(bucket_value(key,value));
            } else {
                found_entry->second=value;
            }
        }
        void remove_mapping(Key const& key) {
            std::unique_lock<std::shared_mutex> lock(mutex);
            bucket_iterator const found_entry=find_entry_for(key);
            if(found_entry!=data.end()) {
                data.erase(found_entry);
            }
        }
    };
    std::vector<std::unique_ptr<bucket_type>> buckets;
    Hash hasher;
    bucket_type& get_bucket(Key const& key) const {
        std::size_t const bucket_index=hasher(key)%buckets.size();
        return *buckets[bucket_index];
    }
public:
    typedef Key key_type;
    typedef Value mapped_type;
    typedef Hash hash_type;
    threadsafe_lookup_table(unsigned num_buckets=19, Hash const& hasher_=Hash()):
        buckets(num_buckets), hasher(hasher_) {
        /**
         * buckets(num_buckets)
         * Оскільки buckets це вектор std::unique_ptr, він заповнюється дефолтними порожніми покажчиками (nullptr).
         * Тобто місця під самі бакети в пам'яті ще немає, є тільки 19 порожніх комірок для адрес.
         *
         * Цикл for проходить по кожній з цих 19 комірок, викликає new bucket_type (виділяє пам'ять у купі під сам об'єкт,
         * де створюється список і м'ютекс) і за допомогою .reset() віддає цю адресу в керування std::unique_ptr.
         *
         * Тобто у нас структура займає 19 комірок і більше не розширюється, розширюватися можуть тільки конкретні елементи
         * структури при колізії (тобто третій елемент (індекс 2) буде мати 4 значення через колізію).
         *
         *
         * Чому структура має 19 комірок, чому б не зробити її динамічною через push?
         * Якщо спробувати зробити цю таблицю динамічною (щоб вона автоматично розширювалася, як стандартний std::unordered_map),
         * виникне серйозна архітектурна проблема:
         * Під час зміни розміру (динамічної реаллокацій вектора та перерахунку хешів для всіх елементів) потрібно повністю заблокувати
         * всю таблицю. Тобто один потік, який робить push і викликає розширення, має захопити unique_lock на всі бакети одразу,
         * щоб жоден інший потік у цей момент не намагався нічого прочитати чи записати.
         * До того ж, динамічне оновлення розміру таблиці - це Rehash (Рехешуванням). Це коли ми перераховуємо хеші кожного елемента у таблиці.
         *
         * Якщо ти знаєш, що в тебе буде зберігатися, наприклад, близько 10 000 елементів, ти просто передаєш у конструктор просте число
         * побільше (наприклад, num_buckets = 997). Тоді елементи рівномірно розподіляться по цих 997 бакетах
         * (в середньому по 10 елементів на бакет), і все працюватиме швидко та паралельно.
         *
         *
         * Окремо про рехешування
         * Нехай у нас 20 комірок і коли їх стає замало і ланцюжки колізій подовжуються, структура робить Rehash:
         *      1) Виділяє новий, більший масив бакетів (зазвичай розмір збільшується вдвічі).
         *      2) Проходить по всіх елементах старого масиву.
         *      3) Перераховує новий індекс для кожного елемента: новий_індекс = hash % новий_розмір_таблиці.
         *      4) Переносить елементи в нові комірки.
         *
         * Це здається неефективним, але при однопоточності це ок.
         * Це плата за те, що 99% часу пошук, додавання та видалення елементів відбуваються миттєво (за O(1)).
         * Рехешування відбувається вкрай рідко.
         *
         * А в багатопоточному програмуванні рехешування - це дуже складно, бо треба заблокувати всю структуру (і це
         * або 20 м'ютексів на кожен елемент, або мати м'ютекс вищого порядку який блокує все), потім зупинити n потоків
         * які читають чи змінюють дані і не втрапити у deadlock
         */
            for(unsigned i=0;i<num_buckets;++i) {
                buckets[i].reset(new bucket_type);
            }
    }
    threadsafe_lookup_table(threadsafe_lookup_table const& other)=delete;
    threadsafe_lookup_table& operator=(threadsafe_lookup_table const& other)=delete;
    Value value_for(Key const& key, Value const& default_value=Value()) const {
        return get_bucket(key).value_for(key,default_value);
    }
    void add_or_update_mapping(Key const& key,Value const& value) {
        get_bucket(key).add_or_update_mapping(key,value);
    }
    void remove_mapping(Key const& key) {
        get_bucket(key).remove_mapping(key);
    }


    /**
     * Якщо ж нам треба зробити зліпок всієї структури, то ми можемо заблокувати всі окремі mutex,
     * щоб безпечно прочитати всю систему. Якщо ми будемо робити це завжди в одному порядку то deadlock
     * не буде. Бо у нас один bucket блокує лише себе і змінює/читає лише себе, тому deadlock неможливий.
     * Але такий підхід тут працює, бо наша структура доволі проста, якби вона була складнішою, то
     * такий підхід міг би бути набагато складнішим.
     *
     * Тут ми створили vector з кожним mutex з усіх 19 buckets, і почергово їх додаємо і одразу блокуємо
     * (блокується автоматично при створенні unique_lock і розблоковується при виході зі scope)
     */
    std::map<Key,Value> get_map() const {
        std::vector<std::unique_lock<std::shared_mutex>> locks;
        for(unsigned i=0;i<buckets.size();++i) {
            locks.push_back(std::unique_lock<std::shared_mutex>(buckets[i].mutex));
        }
        std::map<Key,Value> res;
        for(unsigned i=0;i<buckets.size();++i) {
            for(typename bucket_type::bucket_iterator it=buckets[i].data.begin(); it!=buckets[i].data.end(); ++it) {
                res.insert(*it);
            }
        }
        return res;
    }
};


/**
 * Основна ідея дрібнозернистого блокування для зв'язного списку полягає в тому, щоб мати один м'ютекс на кожен вузол.
 * Якщо список стає великим, це означає багато mutex! Перевага тут полягає в тому, що операції над окремими частинами списку є
 * дійсно паралельними: кожна операція утримує блокування тільки на тих вузлах, які її цікавлять, і розблоковує кожен вузол,
 * переходячи до наступного.
 *
 */

/**
 * Приклад №7: threadsafe_list (однонаправлений linked list)
 *
 * Це багатопоточна реалізація однонаправленого linked list
 *
 * Вона дозволяє іншим потокам паралельно працювати зі списком, доки один потік іде по ньому. Замість того щоб заблокувати
 * весь список одразу, потік блокує вузли по черзі, і коли блокує поточний вузол, то відпускає попередній і тоді з попереднім
 * вже може працювати інший потік.
 *
 *
 * Суть цього дрібнозернистого блокування полягає в поліпшенні можливостей паралельності над одним м'ютексом, і ми досягли цього,
 * бо різні потоки можуть одночасно працювати над різними вузлами в списку, незалежно від того, чи обробляють вони кожен елемент за
 * допомогою for_each(), шукають за допомогою find_first_if() або видаляють елементи за допомогою remove_if().
 */
template<typename T>
class threadsafe_list {
    struct node {
        std::mutex m;
        std::shared_ptr<T> data;
        std::unique_ptr<node> next;
        node(): next() {}
        node(T const& value):
            data(std::make_shared<T>(value)) {}
    };
    node head;
public:
    threadsafe_list() {}
    ~threadsafe_list() {
        remove_if([](node const&){return true;});
    }
    threadsafe_list(threadsafe_list const& other)=delete;
    threadsafe_list& operator=(threadsafe_list const& other)=delete;

    void push_front(T const& value) {
        std::unique_ptr<node> new_node(new node(value));
        std::lock_guard<std::mutex> lk(head.m);
        new_node->next=std::move(head.next); // new_node->next тепер володіє об'єктом яким раніше володів head.next. А сам head.next тепер nullptr
        head.next=std::move(new_node); // head.next тепер володіє new_node який володіє тим, чим раніше володів head.next
    }

    template<typename Function>
    void for_each(Function f) {
        node* current=&head;
        std::unique_lock<std::mutex> lk(head.m);
        while(node* const next=current->next.get()) {
            std::unique_lock<std::mutex> next_lk(next->m); // Ми блокуємо м'ютекс наступного вузла (next->m) ДО ТОГО, як відпустимо замок поточного вузла.
            lk.unlock(); // відпускаємо замок поточного вузла
            f(*next->data); // Оскільки вузол next зараз заблокований нашим потоком, ми можемо безпечно застосувати функцію f до його даних (*next->data).
                            // Жоден інший потік не зможе видалити цей вузол чи змінити його, поки ми всередині f
            current=next;
            lk=std::move(next_lk); // в кінці записуємо у попередній замок поточний об'єкт, щоб при наступній ітерації розблокувати його і працювати з наступним
        }
    }

    template<typename Predicate>
    std::shared_ptr<T> find_first_if(Predicate p) {
        node* current=&head;
        std::unique_lock<std::mutex> lk(head.m);
        while(node* const next=current->next.get()) {
            std::unique_lock<std::mutex> next_lk(next->m);
            lk.unlock();
            if(p(*next->data)) {
                return next->data;
            }
            current=next;
            lk=std::move(next_lk);
        }
        return std::shared_ptr<T>();
    }

    /**
     * Може здатися що тут небезпека при роботі з std::move(next->next), бо поки ми заблокували поточний і попередній елемент, то
     * інший потік може видалити next->next, але НІ! Бо ми завжди блокуємо два м'ютекси, поточний і попередній, тобто потоку
     * який буде працювати з next->next потрібно також буде заблокувати і next, але він вже заблокований нами, тож тому
     * потоку доведеться чекати.
     */
    template<typename Predicate>
    void remove_if(Predicate p) {
        node* current=&head;
        std::unique_lock<std::mutex> lk(head.m);
        while(node* const next=current->next.get()) {
            std::unique_lock<std::mutex> next_lk(next->m);
            if(p(*next->data)) {
                std::unique_ptr<node> old_next=std::move(current->next); // переміщаємо current->next в змінну old_next, тепер current->next буде nullptr
                current->next=std::move(next->next); // current->next тепер має значення з next->text (тобто ми видалили один проміжний елемент і зберегли його в old_next)
                next_lk.unlock();
            } else {
                lk.unlock();
                current=next;
                lk=std::move(next_lk);
            }
        }
    }
};