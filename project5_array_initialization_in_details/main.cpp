#include <iostream>
#include <vector>

/**
 * Приклад Aggregate initialization
 * template <typename T, unsigned long N> тут потрібний лише для того, шоб можна було динамічно
 * передавати length під різні масиви
 */
template <typename T, unsigned long N>
struct MyArray {
    T data[N]; // Публічне поле масиву

    // Ми не пишемо MyArray() { ... }

    // Можемо додати методи, вони не псують "агрегатність"
    const T& operator[](unsigned long i) const { return data[i]; }
};


/**
 * Приклад Initializer List Initialization
 */
template <typename T>
class MyDynamicArray {
private:
    T* data;
    size_t size;

public:
    // Конструктор для підтримки списку ініціалізації {1, 2, 3}
    MyDynamicArray(std::initializer_list<T> list) {
        size = list.size();
        data = new T[size]; // Виділяємо пам'ять у купі

        size_t i = 0;
        for (const auto& item : list) {
            data[i++] = item; // Копіюємо елементи
        }
        std::cout << "Викликано конструктор initializer_list\n";
    }

    // деструктор, який викличеться при видаленні цього класу
    ~MyDynamicArray() { delete[] data; }
};


/**
 * У масивах пишуть, що вони не мають конструктора, і мають aggregate initialization.
 * Ця aggregate initialization це здатність ініціалізуватися з багатьма значеннями через кому,
 * однак ми можемо зробити те саме, якщо захочемо. Для цього можна використати struct.
 *
 * Агрегат (Aggregate) — це особливий тип у C++ (зазвичай struct або class), який не має користувацьких конструкторів,
 * приватних або захищених нестатичних полів, базових класів та віртуальних функцій.
 *
 * Aggregate Initialization дозволяє ініціалізувати такий об'єкт за допомогою фігурних дужок {...},
 * де значення безпосередньо копіюються у відповідні поля структури за порядком їх оголошення.
 *
 *
 * Як же тоді працює std::vector<int> v = {1, 2, 3}
 * Це виглядає схоже на aggregate initialization, але механізм зовсім інший. Це називається Initializer List Initialization
 * Коли ви пишете фігурні дужки для вектора, компілятор:
 *  1. Створює тимчасовий об'єкт типу std::initializer_list<int>.
 *  2. Цей список ініціалізується як масив (це і є прихований агрегат).
 *  3. Викликає спеціальний конструктор вектора: vector(std::initializer_list<T> list).
 *  4. В середині цього конструктора вектор копіює дані зі списку у свою динамічну пам'ять.
 *
 */

int main() {
    // Aggregate initialization
    MyArray<int, 3> arr {1,2,3};

    // List initialization
    MyDynamicArray<int> my_vec = {10, 20, 30, 40};

    return 0;
}