#include <memory>
#include <mutex>



/**
 * Альтернативні засоби захисту спільних даних
 * Особливо крайнім (але надзвичайно поширеним) випадком є ситуація, коли спільні дані потребують захисту лише
 * від одночасного доступу під час їх ініціалізації, але після цього явна синхронізація не потрібна.
 *
 * Припустимо, у вас є спільний ресурс, створення якого є настільки дорогим, що ви хочете це робити тільки в разі
 * необхідності; можливо, він відкриває з'єднання з базою даних або виділяє багато пам'яті. Це ілюструє Приклад 1
 */

// Приклад 1
class Some_resource {
public:
    void do_something() {};
};
std::shared_ptr<Some_resource> resource_ptr;
void foo() {
    if(!resource_ptr) {
        resource_ptr.reset(new Some_resource);
        resource_ptr->do_something();
    }
};



/**
 * Рішення через mutex.
 * Це рішення працює чудово, але є проблема серіалізації.
 * У багатопотоковому програмуванні ми хочемо, щоб потоки працювали паралельно (одночасно). Серіалізація — це стан,
 * коли потоки змушені шикуватися в "чергу" (серію) і проходити через певну ділянку коду по одному.
 * Відповідно ми отримаємо втрату продуктивності на цій ділянці суто через перевірку.
 * Захоплення м'ютекса — це відносно "важка" операція для процесора.
 */

std::mutex my_mutex;
void foo2() {
    std::lock_guard guard(my_mutex);
    if(!resource_ptr) {
        resource_ptr.reset(new Some_resource);
        resource_ptr->do_something();
    }
};




/**
 * std::once_flag та std::call_once.
 * Це спеціальний механізм для «лінивої ініціалізації» (lazy initialization).
 *
 * Як це працює:
 *  std::once_flag: Це спеціальна мітка. Вона зберігає стан: "вже ініціалізовано" чи "ще ні".
 *  std::call_once: Ця функція приймає прапорець і функцію-ініціалізатор.
 *      Перший потік, який дійде до цього рядка, виконає ініціалізацію.
 *      Якщо в цей час прийдуть інші потоки, вони просто зачекають, поки перший закінчить.
 *      Усі наступні потоки (коли прапорець resource_flag уже в стані «готово») просто пролетять цей рядок без жодних блокувань.
 */

std::once_flag resource_flag;
void init_resource() {
    resource_ptr.reset(new Some_resource);
}
void foo3() {
    std::call_once(resource_flag,init_resource); // ініціалізація викликається рівно один раз
    // Звісно можна зробити через lambda
    // std::call_once(resource_flag, [](){
    //     resource_ptr.reset(new Some_resource);
    // });
    resource_ptr->do_something();
}

/**
 * Приклад із класом
 *
 * У цьому прикладі ініціалізація виконується або першим викликом send_data(), або першим викликом receive_data().
 * Використання функції-члена open_connection() для ініціалізації даних також вимагає передачі покажчика this. Так само, як
 * для інших функцій у стандартній бібліотеці, які приймають об'єкти, що викликаються, такі як конструктори для
 * std::thread і std::bind(), це робиться шляхом передачі додаткового аргументу до std::call_once()
 */
class data_packet {};
class connection_info {};
class connection_handle {
public:
    data_packet receive_data() {
        return data_packet{};
    }
    void send_data(const data_packet& data) {}
};
class Connection_manager {
public:
    connection_handle open(connection_info info) {return connection_handle{};};
};
Connection_manager connection_manager {};

class X {
private:
    connection_info connection_details;
    connection_handle connection;
    std::once_flag connection_init_flag;
    void open_connection() {
        connection=connection_manager.open(connection_details);
    }
public:
    X(connection_info const& connection_details_):
        connection_details(connection_details_) {
    }
    void send_data(data_packet const& data) {
        std::call_once(connection_init_flag,&X::open_connection,this);
        connection.send_data(data);
    }
    data_packet receive_data() {
        std::call_once(connection_init_flag,&X::open_connection,this);

        // Або приклад з lambda function
        // std::call_once(connection_init_flag, [this] {
        //     this->open_connection();
        // });
        return connection.receive_data();
    }
};


int main() {



    return 0;
}
