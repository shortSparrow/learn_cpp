#include <iostream>

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

// Окремий варіант реалізації функції print для типу double
template<>
void Storage<double>::print() {
    std::cout << std::scientific << m_value << '\n';
}

int main() {
    /**
     * У нас є клас Storage який буде містити різні дані. Ми використовуємо template щоб мати можливість покласти туди
     * будь-який тип даних. Але що якщо я хочу щоб на print була різна поведінка в залежності від типу даних.
     *
     * Нехай якщо буде int то поведінка буде стандартною, а якщо double то будемо використовувати scientific нотацію.
     * Це робиться дуже просто, ми просто додаємо template на функцію print і пишемо варіант для спеціального типу
     * за межами класу.
     */

    Storage i { 5 };
    Storage d { 6.7 };

    i.print(); // 5            - calls Storage<int>::print (instantiated from Storage<T>)
    d.print(); // 6.700000e+00 - calls Storage<double>::print (called from explicit specialization of Storage<double>::print())
}