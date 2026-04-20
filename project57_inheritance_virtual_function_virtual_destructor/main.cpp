#include <iostream>

class Base {
public:
    // virtual ~Base()  {
    //     std::cout << "Calling ~Base()\n";
    // }
    ~Base()  {
        std::cout << "Calling ~Base()\n";
    }
};

class Derived: public Base {
private:
    int* m_array {};

public:
    Derived(int length) : m_array{ new int[length] } { }

    // virtual ~Derived() {
    //     std::cout << "Calling ~Derived()\n";
    //     delete[] m_array;
    // }
    ~Derived() {
        std::cout << "Calling ~Derived()\n";
        delete[] m_array;
    }
};

int main() {
    {
        // Тут все буде працювати чудово і без virtual destructor
        // Derived derived {1};

        Derived* derived { new Derived(5) };
        Base* base { derived };

        delete base; // delete викликає деструктор і звільняє пам'ять

    }

    /**
     * Ми очікуємо що спочатку викличеться деструктор Base, а потім Derived, але якщо запустити код, то буде:
     * Calling ~Base()
     * Деструктор Derived не викликався, це тому що ми створили нащадка Derived, але посилаємося на його батька.
     * Компілятор заплутався і викликав компілятор лише на Base
     *
     * Але якщо я у Derived та Base деструктор додам слово virtual то все запрацює як треба і я отримую
     * Calling ~Derived()
     * Calling ~Base()
     *
     * Можна визначити правило:
     *  Якщо ти бачиш у класі хоча б одну віртуальну функцію (або просто плануєш, що від цього
     *  класу будуть успадковуватися) — завжди роби деструктор віртуальним.
     */

    return 0;
}