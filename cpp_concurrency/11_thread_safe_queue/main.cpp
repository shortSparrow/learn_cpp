#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <iostream>


/**
 * Стандартна черга не є безпечною для потоків. Якщо один потік додає елемент (push), а інший у цей же час його видаляє (pop),
 * програма може "впасти" (Race Condition).
 *
 * threadsafe_queue - це потоко безпечна версія std::queue
 *
 *
 * push(T new_value)
 *  - блокує м'ютекс.
 *  - Додає дані в чергу.
 *  - Викликає data_cond.notify_one(). Це "будить" один із потоків, які чекають на дані.
 *  - Розблоковує м'ютекс
 *
 *
 * try_pop(T& value)
 * Цей метод швидкий, використовуємо, коли не хочемо чекати
 *  - Блокуємо м'ютекс
 *  - Дивимося чи є дані у черзі чи немає. Якщо є, то записуємо їх у reference, який передали і повертаємо true,
 *    якщо їх немає, то повертаємо false.
 *  - Розблоковуємо м'ютекс
 *
 *
 * std::shared_ptr<T> try_pop()
 * Сценарій: Комбінація "не хочу чекати" + "хочу повернути pointer". По суті те саме що і try_pop(T& value), але
 * не копіюємо об'єкт, а повертаємо pointer. Якщо в черзі лежить відеокадр на 10 МБ, то копіювати його в T& value — це повільно.
 * (make_shared все одно зробить копію, але код можна змінити так щоб він не робив копію)
 *  - Блокуємо м'ютекс
 *  - Дивимося чи є дані у черзі чи немає. Якщо є, то записуємо їх у reference, який передали і повертаємо true,
 *    якщо їх немає, то повертаємо false.
 *  - Розблоковуємо м'ютекс
 * Використовуємо std::shared_ptr для того, аби:
 *  1) Автоматично викликався delete і очистилися дані pointer (бо якщо передати просто pointer ми швидше за все про це забудемо
 *  2) Захист від винятків (Exception Safety)). Тут магія в тому, коли видаляється елемент із черги. Спочатку make_shared виділяє
 *     пам'ять і копіює туди дані. Якщо на цьому етапі вилетить помилка (наприклад, закінчилася оперативка), черга ще не змінена!
 *     Рядок data_queue.pop() не виконається. Дані залишаться в черзі, і програма не втратить їх. Тільки якщо копіювання пройшло
 *     успішно, ми робимо pop().
 *
 *
 *
 *
 * wait_and_pop(T& value)
 * Це як підписка, ми заснули і чекаємо коли з'являться дані. Може через секунду, а може через хвилину.
 *  - Блокуємо м'ютекс. unique_lock нам потрібно замість std::lock_guard, тому що .wait() не може працювати з lock_guard,
 *    бо й нього немає методів .lock() та .unlock(), він сам їх викликає коли виходить за scope.
 *  - Викликає data_cond.wait(). Він засинає, якщо черга порожня (!data_queue.empty() повертає false). Коли приходить сповіщення від push,
 *    він прокидається, знову замикає м'ютекс і перевіряє умову ще раз.
 *  - Як тільки дані з'явилися, він записує їх у reference і видаляє з черги.
 *
 *
 * std::shared_ptr<T> wait_and_pop()
 * Те саме, що і std::shared_ptr<T> try_pop(), тільки тут з wait()
 *
 */

template<typename T>
class Threadsafe_Queue {
private:
 mutable std::mutex mut;
 std::queue<T> data_queue;
 std::condition_variable data_cond;

public:
    Threadsafe_Queue() {}
    Threadsafe_Queue(Threadsafe_Queue const& other) {
         std::lock_guard<std::mutex> lk(other.mut);
        data_queue=other.data_queue;
    }

    void push(T new_value) {
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(new_value);
        data_cond.notify_one();
    }

    void wait_and_pop(T& value) {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk,[this]{return !data_queue.empty();}); // якщо поверне false, то засне до наступного .notify_one()
        value=data_queue.front();
        data_queue.pop();
    }

    std::shared_ptr<T> wait_and_pop() {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk,[this]{return !data_queue.empty();});
        std::shared_ptr<T> res(std::make_shared<T>(data_queue.front())); // Спочатку алокуємо пам'ять під об'єкт (раптом пам'яті не вистачить), і тільки потім робимо .pop()
        data_queue.pop();
        return res;
    }

    bool try_pop(T& value) {
         std::lock_guard<std::mutex> lk(mut);
         if(data_queue.empty())
            return false;
         value=data_queue.front();
         data_queue.pop();
        return true;
    }

    std::shared_ptr<T> try_pop() {
         std::lock_guard<std::mutex> lk(mut);
         if(data_queue.empty())
            return std::shared_ptr<T>(); // повертаємо порожній pointer
         std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));  // Спочатку алокуємо пам'ять під об'єкт (раптом пам'яті не вистачить), і тільки потім робимо .pop()
         data_queue.pop();
        return res;
    }

     bool empty() const {
         std::lock_guard<std::mutex> lk(mut);
         return data_queue.empty();
     }
};

/**
 *
 */



int main() {
    Threadsafe_Queue<std::string> ts_queue {};

    // Приклад try_pop(T& value)
    std::string data;
    // Не чекає! Повертає true, якщо забрав дані, або false, якщо порожньо.
    if (ts_queue.try_pop(data)) {
        std::cout << "Took fast: " << data << std::endl;
    } else {
        std::cout << "Queue is empty, go to drink some coffee..." << std::endl;
    }


    // Приклад std::shared_ptr<T> try_pop()
    auto res = ts_queue.try_pop(); // Якщо порожньо — поверне NULL (nullptr)

    if (res) {
        std::cout << "Took fast and without expensive copy: " << *res << std::endl;
    }


    // Приклад wait_and_pop(T& value)
    std::string data2;
    ts_queue.wait_and_pop(data2); // Потік зупиниться тут і буде спати, поки щось не з'явиться
    std::cout << "Done: " << data2 << std::endl; // Коли прокинувся — дані вже в змінній data


    // Приклад std::shared_ptr<T> wait_and_pop()
    std::shared_ptr<std::string> data_ptr = ts_queue.wait_and_pop(); // Повертає розумний вказівник. Якщо черга порожня — спить.
    if (data_ptr) {
        std::cout << "Get data as smart pointer: " << *data_ptr << std::endl;
    }
    return 0;
}


