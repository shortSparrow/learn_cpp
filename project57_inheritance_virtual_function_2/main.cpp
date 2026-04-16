#include <iostream>
#include <string>
#include <string_view>

class Animal {
protected:
    std::string m_name {};

    // We're making this constructor protected because
    // we don't want people creating Animal objects directly,
    // but we still want derived classes to be able to use it.
    Animal(std::string_view name) : m_name{ name } { }

public:
    const std::string& getName() const { return m_name; }
    virtual std::string_view speak() const { return "???"; }
};

class Cat: public Animal {
public:
    Cat(std::string_view name) : Animal{ name } { }

    virtual std::string_view speak() const { return "Meow"; }
};

class Dog: public Animal {
public:
    Dog(std::string_view name) : Animal{ name } { }

    virtual std::string_view speak() const { return "Woof"; }
};

// Якби тут був би тип не reference (Animal&), а просто копія (Animal) то virtual function не допоміг би
// і був би виклик .getName() на базовому класі і ми б отримали "???"
void report(const Animal& animal) {
    std::cout << animal.getName() << " says " << animal.speak() << '\n';
}

int main() {
    Cat cat{ "Fred" };
    Dog dog{ "Garbo" };

    report(cat);
    report(dog);


    /**
     * Важливе уточнення
     * Якщо функція написана як virtual, то в усіх перевантаженнях в дочірніх (похідних Derived) класах вона
     * лишається virtual, навіть якщо це не зазначено явно
     *
     * Також
     * Virtual functions не можна викликати у конструкторах та деструкторах.
     * У конструкторах не можна, бо якщо у нас є Base та Derived який від нього успадкувався, то спочатку буде викликатися конструктор
     * Base і частина Derived ще не створена, тож буде викликано функцію із Base.
     * Пам'ятаймо, що до виклику конструктора класу ще немає, є лише виділена пам'ять, а у конструкторі відбувається створення, пам'ятаймо,
     * що саме у конструкторі ми можемо визначити значення для наших константних полів класу, тобто це як раз процес створення класу.
     *
     * Те саме і з деструкторами, але у зворотньому порядку. Спочатку викликається деструктор Derived і перестає існувати, тож викликатися
     * може лише функція з Base
     *
     * Best practice
     * Never call virtual functions from constructors or destructors.
     *
     *
     * Виникає питання, чому б тоді не робити всі функції virtual?
     * Відповідь в тому, що це не ефективно, звичайні функції будуть працювати швидше, бо їм не треба визначати, запуститися на Base чи Derived.
     *
     * Якщо виникнуть проблеми із розумінням як працює virtual function, то можна глянути на сайті
     * https://www.learncpp.com/cpp-tutorial/virtual-functions/ у секції quizz, як в низу
     */

    return 0;
}