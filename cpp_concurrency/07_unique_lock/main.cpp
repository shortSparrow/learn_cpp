#include <mutex>



/**
 * Flexible locking with std::unique_lock
 * Екземпляр std::unique_lock не завжди володіє м'ютексом, з яким він пов'язаний. По-перше, оскільки ви можете
 * передати std::adopt_lock як другий аргумент конструктора, щоб об'єкт блокування керував блокуванням м'ютекса, ви також
 * можете передати std::defer_lock як другий аргумент, щоб вказати, що м'ютекс повинен залишатися розблокованим під час побудови
 *
 * Тобто в прикладі нижче ми з початку створюємо об'єкт unique_lock з параметром std::defer_lock. Сам unique_lock нам треба, щоб
 * м'ютекси автоматично розблокувалися в кінці функції, а std::defer_lock, щоб м'ютекс лишався відкритим (не lock) і далі
 * std::lock блокує два м'ютекса і потім у кінці функції вони автоматично розблоковуються.
 *
 * Це той самий приклад з 06_deadlock, який ми вирішували за допомогою
 *      std::lock(lhs.m,rhs.m);
 *      std::lock_guard<std::mutex> lock_a(lhs.m,std::adopt_lock);
 *      std::lock_guard<std::mutex> lock_b(rhs.m,std::adopt_lock);
 *
 *      або
 *      std::scoped_lock guard(lhs.m, rhs.m);
 *
 *
 * Одне невелике зауваження: std::unique_lock займає більше місця і працює трохи повільніше, ніж std::lock_guard.
 * Гнучкість, що дозволяє екземпляру std::unique_lock не володіти м'ютексом, має свою ціну: ця інформація повинна
 * зберігатися і оновлюватися.
 *
 * В більшості випадків (окрім випадків коли ми хочемо передати право власності на блокування) краще використовувати std::scoped_lock
 */

class Some_big_object {};
void swap(Some_big_object& lhs,Some_big_object& rhs);
class X {
private:
    Some_big_object some_detail;
    std::mutex m;
public:
    X(Some_big_object const& sd):some_detail(sd){}
    friend void swap(X& lhs, X& rhs) {
        if(&lhs==&rhs)
            return;
        std::unique_lock<std::mutex> lock_a(lhs.m,std::defer_lock);
        std::unique_lock<std::mutex> lock_b(rhs.m,std::defer_lock);
        std::lock(lock_a,lock_b);
        swap(lhs.some_detail,rhs.some_detail);
    }
};


/**
 * Передача права власності на м'ютекс між областями (Transferring mutex ownership between scopes)
 * std::unique_lock не можна копіювати, але можна переміщувати (move). Це означає, що ви можете передати
 * захоплений м'ютекс з однієї функції в іншу.
 *
 * З lock_guard чи lock_scope так зробити не вийде, вони не вміють «рухатися» (move semantics).
 * Вони занадто суворий у своїй поведінці.
 */
void prepare_data() {};
void do_something() {};

std::unique_lock<std::mutex> get_lock() {
    extern std::mutex some_mutex;
    std::unique_lock<std::mutex> lk(some_mutex); // 1. Захопили
    prepare_data();                              // 2. Щось зробили
    return lk;                                   // 3. Передали замок далі
}

void process_data() {
    std::unique_lock<std::mutex> lk(get_lock()); // 4. Отримали замок
    do_something();                              // 5. Працюємо під захистом
}                                                // 6. Тільки тут замок відпуститься


/**
 * Загалом, блокування повинно тривати лише мінімальний час, необхідний для
 * виконання необхідних операцій.
 *
 * У прикладі нижче ми створили м'ютекс і заблокували його, потім перед трудомісткою
 * операцією process(data_to_process) розблокували, щоб не затримувати інші потоки, і
 * коли трудомістка операція виконана то заблокували знову.
 */

int get_next_data_chunk() {return 0;};
int process(int data) {return 1000;};
void write_result(int,int) {};
void get_and_process_data() {
    std::mutex the_mutex;
    std::unique_lock<std::mutex> my_lock(the_mutex);
    int data_to_process=get_next_data_chunk();
    my_lock.unlock();
    int result=process(data_to_process);
    my_lock.lock();
    write_result(data_to_process,result);
}



int main() {



    return 0;
}
