#include <iostream>
#include <type_traits> // for std::is_pointer_v and std::is_null_pointer_v
#include <memory>

template <typename T>
class Storage {
private:
    T m_value {};
public:
    Storage(T value) : m_value { value } { }

    void print() {
        std::cout << m_value << '\n';
    }
};

template<>
void Storage<double>::print() { // fully specialized for type double
    std::cout << std::scientific << m_value << '\n';
}

/**
 * Розглянемо приклад із класом Storage який приймає будь який тип даних у m_value,
 * і ми хочемо на функцію print зробити окрему обробку якщо клас зберігає double.
 * Це не проблема, ми просто робимо template на double для цієї функції.
 *
 * Але що буде якщо ми передамо pointer? Тоді наш клас зламається. Рішенням може бути
 * клас StoragePtr (це те саме, що і Storage, але адаптоване для роботи з pointer).
 * У клас StoragePtr template <typename T> ми перевіряємо що дані точно не pointer,
 * а у класі StoragePtr<T*> використовуємо unique_ptr, і постійно робимо перевірку,
 * щоб pointer не був nullptr.
 */


template <typename T>
class StoragePtr {
    // Make sure T isn't a pointer or a std::nullptr_t
    static_assert(!std::is_pointer_v<T> && !std::is_null_pointer_v<T>, "Storage<T*> and Storage<nullptr> disallowed");

private:
    T m_value {};

public:
    StoragePtr(T value) : m_value { value } {}

    void print() {
        std::cout << m_value << '\n';
    }
};

template <typename T>
class StoragePtr<T*> {
private:
    std::unique_ptr<T> m_value {}; // use std::unique_ptr to automatically deallocate when Storage is destroyed

public:
    StoragePtr(T* value) : m_value { std::make_unique<T>(value ? *value : 0) } // or throw exception when !value
    {
    }

    void print() {
        if (m_value)
            std::cout << std::scientific << *m_value << '\n';
    }
};


int main() {
    // Storage i { 5 };
    // Storage d { 6.7 }; // will cause Storage<double> to be implicitly instantiated
    //
    // i.print(); // calls Storage<int>::print (instantiated from Storage<T>)
    // d.print(); // calls Storage<double>::print (called from explicit specialization of Storage<double>::print())
    //

    double d { 1.2 };
    double *ptr { &d };

    Storage s { ptr };
    s.print(); // 0x21a01ff8b0

    /**
     * Чому вивелося 0x21a01ff8b0
     * Тому що ptr це double*, і s має тип Storage<double*>, що означає, що m_value має тип double*.
     * Коли викликається конструктор, m_value ініціалізується копією адреси яку має ptr
     *
     * Тож як це виправити?
     * Один варіант це просто зробити окрему обробку на double*
     *      template<>
     *      void Storage<double*>::print()
     *
     * Або взагалі зробити окремий клас на T* і тоді через template у нас буде
     *      template <typename T>
     *      class Storage<T*> {...}
     * Але із T* є проблема, бо pointer може бути змінений на nullptr, тому варто використовувати make_unique і
     * мати перевірку що під час обробки значення pointer існує.
     *
     */

    std::cout << "---------------------------------------\n";
    double d2 { 1.2 };

    StoragePtr s1 { d2 }; // 1.2
    s1.print();

    StoragePtr s2 { &d2 }; // 1.200000e+00
    s2.print();

    // StoragePtr s3 { nullptr }; // static_assert видасть помилку, бо T - це nullptr
    // s3.print();
}