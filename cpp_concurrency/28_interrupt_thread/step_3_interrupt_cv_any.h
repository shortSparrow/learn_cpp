#include <iostream>
#include <thread>
#include <future>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <atomic>

/**
 * =======================================================================================
 * ЧАСТИНА 3: Фінальне рішення через std::condition_variable_any та кастомний лок
 * =======================================================================================
 *
 * Проблема попереднього кроку:
 * Милиці із wait_for(1ms) змушував процесор постійно прокидатися (polling).
 * Це марнує ресурси CPU та створює затримку реакції потоку на переривання.
 *
 * Ідея ідеального рішення:
 * Зробити так, щоб робочий потік міг спати вічно (чистий cv.wait()), але головний потік
 * міг розбудити його в будь-яку секунду без ризику втратити сигнал notify_all().
 *
 * std::condition_variable_any відрізняється від std::condition_variable тим, що працює з будь-яким типом блокування,
 * а не тільки з std::unique_lock<std::mutex>. Виявляється, це значно спрощує роботу, і ви можете досягти кращих
 * результатів з std::condition_variable_any, ніж з std::condition_variable. Оскільки він працює з будь-яким типом
 * блокування, ви можете створити власний тип блокування
 *
 *
 * Якщо раніше нам треба було додавати тайм-аут в 1 мс через wait_for, то якщо ми будемо працювати з condition_variable_any,
 * то в цьому не буде необхідності. Потік спатиме скільки завгодно і прокинеться миттєво, коли ми його перервемо.
 * Стандартна std::condition_variable вміє працювати тільки з std::unique_lock<std::mutex>. Вона жорстко зашита під цей тип.
 * А от std::condition_variable_any може працювати з абсолютно будь-яким класом, саме тому ми створюємо власний custom_lock.
 * Цей custom_lock всередині керує одночасно двома речами:
 *      1. Твоїм основним м'ютексом (lk), який захищає твої дані.
 *      2. Внутрішнім м'ютексом прапорця переривання (set_clear_mutex).
 *
 *
 * Як ми обходимо ситуацію, коли флаг ще у false, умову пройшли, потім він ставиться у true, але оскільки
 * умову ми пройшли, то засинаємо на завжди.
 * Завдяки custom_lock, перевірка прапорця (interruption_point) і сам вхід у cv.wait() відбуваються в стані,
 * коли внутрішній м'ютекс закритий для головного потоку. Головний потік фізично не може викликати свій notify_all()
 * раніше, ніж робочий потік офіційно ляже спати й відпустить цей м'ютекс всередині cv.wait().
 * Тобто поки новий потік пройшов перевірку і йде засинати, головний не може нічого змінити, бо виклик interrupt()
 * викликає flag->set(), який в свою чергу блокується м'ютексом set_clear_mutex у прапорці interruption_flag, а ми
 * пам'ятаємо що два м'ютекси set_clear_mutex та std::unique_lock<std::mutex> lk з worker_task нерозривно зв'язані.
 *
 *
 * Як працює зв'язка м'ютексів:
 * 1. Ми використовуємо std::condition_variable_any, яка може працювати з кастомними локами.
 * 2. Ми створюємо структуру-перехідник custom_lock. Вона приймає зовнішній м'ютекс задачі (std::unique_lock)
 *    та внутрішній м'ютекс нашого interruption_flag (set_clear_mutex).
 * 3. Під час виклику cv.wait(custom_lock) C++ автоматично відпускає обидва м'ютекси.
 * 4. Якщо в цей момент головний потік викликає interrupt(), він намагається захопити
 *    внутрішній м'ютекс прапорця. Оскільки робочий потік уже або спить (і відпустив м'ютекс),
 *    або ще тільки збирається заснути (і тримає м'ютекс), головний потік або заблокується
 *    на секунду, або пройде далі. Race Condition повністю усувається на рівні блокування м'ютексів.
 *
 *
 * Безпека виключень (Exception Safety):
 * Якщо під час очікування на cv.wait() виникне помилка і вилетить виключення, функція завершиться достроково.
 * Вказівник на cv залишиться всередині прапорця. Коли інший потік спробує викликати interrupt(), він звернеться до cv,
 * яка вже може бути видалена. Це призведе до падіння програми (Undefined Behavior).
 * Щоб цього уникнути ми реєструємо cv у прапорці, і гарантуємо, що коли cv знищується (наприклад,
 * функція завершилася), прапорець більше не тримає на неї вказівник. Для цього використовують
 * структуру-охоронець clear_cv_on_destruct. Коли функція завершується (нормально чи через виключення),
 * автоматично спрацьовує деструктор цієї структури й очищає вказівник.
 *
 *
 * Результат:
 * Потік спить без споживання CPU. Зупинка відбувається миттєво. Пам'ять очищується через RAII.
 *
 */

// Те саме, що і в step_1_interrupt_concept та step_2_interrupt_cv_naive
class thread_interrupted : public std::exception {
public:
    const char* what() const noexcept override {
        return "Thread was interrupted cooperatively from deep sleep.";
    }
};

/**
 * interruption_flag значно розширився, тепер він знає про condition_variable_any.
 * Потік засинає у cv.wait() вічним сном. Якщо програма працюватиме рік, і нових задач не буде, потік спатиме рік,
 * споживаючи 0% ресурсів процесора.  І ось тепер, коли головний потік каже t.interrupt(), перед нами стоїть
 * супер-задача: розбудити потік, який спить «мертвим» сном, причому зробити це миттєво і без Race Condition.
 *
 * std::condition_variable_any* thread_cv
 * Головний потік у main не має доступу до cv всередині воркера. Він викликає просто t.interrupt().
 * Щоб зсередини методу .interrupt() зробити cv->notify_all(), прапорець мусить тримати вказівник на ту cv, на якій
 * потік зараз спить.
 */
class interruption_flag {
private:
    std::atomic<bool> flag{false};
    std::condition_variable_any* thread_cv = nullptr;
public:
    std::mutex set_clear_mutex; // Тепер цей м'ютекс відкритий, бо custom_lock має його захоплювати

    void set() {
        flag.store(true, std::memory_order_relaxed);
        std::lock_guard<std::mutex> lk(set_clear_mutex);
        if (thread_cv) {
            thread_cv->notify_all(); // Миттєво будимо потік
        }
    }

    bool is_set() const {
        return flag.load(std::memory_order_relaxed);
    }

    void set_condition_variable(std::condition_variable_any& cv) {
        std::lock_guard<std::mutex> lk(set_clear_mutex);
        thread_cv = &cv;
    }

    void clear_condition_variable() {
        thread_cv = nullptr;
    }
};

// Те саме, що і в step_1_interrupt_concept та step_2_interrupt_cv_naive
inline thread_local interruption_flag* this_thread_flag = nullptr;

// Те саме, що і в step_1_interrupt_concept та step_2_interrupt_cv_naive
void interruption_point() {
    if (this_thread_flag && this_thread_flag->is_set()) {
        throw thread_interrupted();
    }
}

/**
 * Кастомний лок, який об'єднує два м'ютекси для std::condition_variable_any
 * По суті він тримає два локи в синхронізації, якщо зовнішній блокується то одразу блокується і то,
 * що у прапорця interruption_flag, якщо навпаки зовнішній розблоковується, то і внутрішній так само.
 */
template<typename Lockable>
struct custom_lock {
    interruption_flag* flag;
    Lockable& lk; // Зовнішній лок (наприклад, std::unique_lock)

    custom_lock(interruption_flag* flag_, Lockable& lk_) : flag(flag_), lk(lk_) {
        // Захоплюємо внутрішній м'ютекс прапорця. Зовнішній м'ютекс вже захоплений потоком раніше (std::unique_lock).
        flag->set_clear_mutex.lock();
    }

    // Метод, який автоматично смикне std::condition_variable_any перед засинанням
    void unlock() {
        lk.unlock();
        flag->set_clear_mutex.unlock();
    }

    // Метод, який автоматично смикне std::condition_variable_any одразу після прокидання
    void lock() {
        flag->set_clear_mutex.lock();
        lk.lock();
    }

    ~custom_lock() {
        // Тут порожньо. М'ютекс set_clear_mutex буде автоматично відпущений
        // трохи пізніше — коли знизиться сам interruption_flag (викличеться clear_cv_on_destruct)
        // наприкінці потоку або коли ми вийдемо з блоку.
    }
};

// Безпечний RAII-охоронець для очищення вказівника на CV при виході з очікування
struct clear_cv_on_destruct {
    ~clear_cv_on_destruct() {
        if (this_thread_flag) {
            this_thread_flag->clear_condition_variable();
        }
    }
};

// Фінальна функція очікування — ЧИСТИЙ ВІЧНИЙ WAIT (без таймаутів!)
void interruptible_wait(std::condition_variable_any& cv, std::unique_lock<std::mutex>& lk) {
    interruption_point(); // Перевіряємо перед очікуванням
    
    if (this_thread_flag) {
        this_thread_flag->set_condition_variable(cv);
        
        // Створюємо наш подвійний лок
        custom_lock<std::unique_lock<std::mutex>> clk(this_thread_flag, lk);
        // Охоронець гарантує: навіть якщо вилетить виключення, ми очистимо вказівник у деструкторі
        clear_cv_on_destruct guard;
        
        interruption_point(); // Перевіряємо ще раз після реєстрації
        
        // Потік засинає без обмеження часу. CPU = 0%.
        // Якщо main викличе interrupt(), він прокинеться МИТТЄВО.
        cv.wait(clk);
    } else {
        cv.wait(lk); // Захисний варіант, якщо потік звичайний
    }
    
    interruption_point();
}

// Те саме, що і в step_1_interrupt_concept та step_2_interrupt_cv_naive
class interruptible_thread {
private:
    std::thread internal_thread;
    interruption_flag* flag = nullptr;
public:
    template<typename Function>
    interruptible_thread(Function f) {
        std::promise<interruption_flag*> p;
        std::future<interruption_flag*> fut = p.get_future();

        internal_thread = std::thread([f, p = std::move(p)]() mutable {
            interruption_flag local_flag;
            this_thread_flag = &local_flag;
            p.set_value(&local_flag);

            try {
                f();
            } catch (const thread_interrupted& e) {
                std::cout << "[Thread] Interrupted cleanly from deep sleep: " << e.what() << "\n";
            }
            this_thread_flag = nullptr;
        });
        flag = fut.get();
    }

    void interrupt() {
        if (flag) flag->set();
    }

    void join() {
        if (internal_thread.joinable()) {
            internal_thread.join();
        }
    }

    ~interruptible_thread() {
        join();
    }
};

// --- Демонстрація роботи ---

std::mutex mut;
std::condition_variable_any cv; // Використовуємо _any версію[cite: 1]
bool ready = false;

void worker_task() {
    std::unique_lock<std::mutex> lk(mut);
    while (!ready) {
        std::cout << "[Worker] Going to deep sleep (0% CPU, no timeouts)...\n";
        interruptible_wait(cv, lk);
    }
}

int main_3() {
    interruptible_thread t(worker_task);
    
    // Даємо потоку міцно заснути
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    std::cout << "[Main] Interrupting now!\n";
    auto start = std::chrono::high_resolution_clock::now();
    
    t.interrupt(); 
    t.join();
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    
    std::cout << "[Main] Thread stopped. Latency: " << elapsed.count() << " ms\n";
    return 0;
}