#include <iostream>



/**
 * Приклад з кількома успадкуваннями
 */
class A {
public:
    A() {std::cout << "A\n";}
};

class B: public A {
public:
    B() {std::cout << "B\n";}
};

class C: public B {
public:
    C() {std::cout << "C\n";}
};

class D: public C {
public:
    D() {std::cout << "D\n";}
};



// Приклад з одним успадкуванням і передаванням параметрів через конструктор

class Base {
    int m_id {};
public:
    Base(int id=0) : m_id{ id } {
    }

    int getId() const { return m_id; }
};

class Derived: public Base {
    double m_cost {};
public:
    Derived(double cost=0.0, int id=0)
        :Base{id}, // Call Base(int) constructor with value id!
        m_cost{ cost } {
    }

    double getCost() const { return m_cost; }
};

int main() {

    /**
     * Коли ми створюємо baseball_player то у нас з початку викличеться дефолтний конструктор Person,
     * а потім BaseballPlayer і ми отримаємо інстанс класу BaseballPlayer.
     *
     * Це наочно можна побачити на класі D який має багато батьків
     */

    std::cout << "Constructing D: \n";
    D d;
    /**
     * Ось що буде виведено у термінал:
     *
     * Constructing D:
     * A
     * B
     * C
     * D
     */

    /**
     * Якщо наш клас має батька і батьківський конструктор потребує параметру для ініціалізації якоїсь константи, то як її передати?
     * Все дуже просто, дочірній клас (Derived) отримає параметр у своєму конструкторі і потім викличе батьківський і передасть
     * параметр туди, такий спосіб дозволить нам ініціалізувати константу, а також reference
     */
    std::cout << "******************************************\n";
    Derived derived {1.0,2};
    std::cout << "Id: " << derived.getId() << '\n'; // Id: 2
    std::cout << "Cost: " << derived.getCost() << '\n'; // Cost: 1
    /**
     * Ось, що відбулося детальніше:
     *  1. Memory for derived is allocated.
     *  2. The Derived(double, int) constructor is called, where cost = 1.3, and id = 5.
     *  3. The compiler looks to see if we’ve asked for a particular Base class constructor. We have! So it calls Base(int) with id = 5.
     *  4. The base class constructor member initializer list sets m_id to 5.
     *  5. The base class constructor body executes, which does nothing.
     *  6. The base class constructor returns.
     *  7. The derived class constructor member initializer list sets m_cost to 1.3.
     *  8. The derived class constructor body executes, which does nothing.
     *  9. The derived class constructor returns.
     */


    /**
     * Destructors
     * На відміну від конструкторів, деструктори викликаються у зворотньому порядку, тобто з початку
     * буде викликаний дочірній, а потім батьківський
     */

    return 0;
}
