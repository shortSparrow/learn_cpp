#include <iostream>

/**
 * Деструктор - це метод який буде викликано перед знищенням класу.
 * Може бути корисно якщо хочемо закрити database connection або щось схоже.
 *
 * Деструктор створюється додаванням тільди ~ перед назвою методу який ПОВИНЕН називатися так само як і клас.
 * Деструктор не приймає аргументів і не має типу повернення. За замовченням компілятор сам генерує
 * порожній деструктор який нічого не робить.
 */
class Simple {
private:
    int m_id {};

public:
    Simple(int id)
        : m_id { id }
    {
        std::cout << "Constructing Simple " << m_id << '\n';
    }

    // here's our destructor
    ~Simple() {
        std::cout << "Destructing Simple " << m_id << '\n';
    }

    int getID() const { return m_id; }
};

int main() {
    // Allocate a Simple
    Simple simple1{ 1 };
    {
        Simple simple2{ 2 };
    } // simple2 dies here

    /**
     * Ось що буде виведено у термінал
     * Constructing Simple 1
     * Constructing Simple 2
     * Destructing Simple 2
     * Destructing Simple 1
     */

    return 0;
} // simple1 dies here