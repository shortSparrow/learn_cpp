#include <iostream>
#include <cassert>
#include <cstddef>

class IntArray{
private:
    int* m_array{};
    int m_length{};

public:
    IntArray(int length) {
        assert(length > 0);

        m_array = new int[static_cast<std::size_t>(length)]{};
        m_length = length;
    }

    // destructor
    ~IntArray() {
        // Dynamically delete the array we allocated earlier
        std::cout << "destructor run";
        delete[] m_array;
    }

    void setValue(int index, int value) { m_array[index] = value; }
    int getValue(int index) { return m_array[index]; }

    int getLength() { return m_length; }
};


class Simple {
private:
    int m_nID{};

public:
    Simple(int nID)
        : m_nID{ nID }
    {
        std::cout << "Constructing Simple " << nID << '\n';
    }

    ~Simple() {
        std::cout << "Destructing Simple " << m_nID << '\n';
    }

    int getID() { return m_nID; }
};

int main() {
    /**
     * Приклад delete при dynamic allocation у класах
     * IntArray використовує підхід RAII (Resource Acquisition Is Initialization) - дані під які треба пам'ять створили у
     * конструкторі, і вони достіпні протягов усього життя класу. А як тільки клас помирає то викликається деструктор і очищає
     * всі дані під які була алокована пам'ять
     */
    // IntArray ar ( 10 ); // allocate 10 integers
    // for (int count{ 0 }; count < ar.getLength(); ++count)
    //     ar.setValue(count, count+1);
    //
    // std::cout << "The value of element 5 is: " << ar.getValue(5) << '\n';


    //******************** Те як впливає delete на класи ************************
    // Allocate a Simple on the stack
    Simple simple{ 1 };
    std::cout << simple.getID() << '\n';

    Simple* pSimple{ new Simple{ 2 } };
    std::cout << pSimple->getID() << '\n';

    // We allocated pSimple dynamically, so we have to delete it.
    delete pSimple;

    /**
     * Constructing Simple 1
     * 1
     * Constructing Simple 2
     * 2
     * Destructing Simple 2 (цей клас знищено до Simple 1 тому що ми на ньому викликали delete і на ньому були викликано destructor раніше)
     * Destructing Simple 1
     */


    /**
     * A warning about the std::exit() function
     * Зверніть увагу, що якщо ви використовуєте функцію std::exit(), ваша програма завершиться, і жодні деструктори не будуть викликані.
     * Будьте обережні, якщо ви покладаєтеся на свої деструктори для виконання необхідної роботи з очищення (наприклад, запису чогось
     * у файл журналу або базу даних перед виходом).
     */

    return 0;
}