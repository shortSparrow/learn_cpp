#include <array>
#include <iostream>
#include <string_view>
#include <string>




/**
 * Розглянемо приклад коли у нас є базовий клас Animal і від нього ми успадковуємося
 * на клас Dog та Cat. Всі класи мають однаковий метод .speak() який в кожному окремому
 * класі поводитися по різному.
 *
 * І стандартна поведінка при успадкуванні що спочатку викликається метод з поточного класу, а якщо такого методу немає,
 * то тоді викликається із успадкованого батьківського. Але бувають випадки коли ми приводимо наш клас до базового, і нам
 * треба викликати не базовий метод із батьківського класу, а саме метод з нашого класу, тут виникає проблема яку вирішує
 * virtual function.
 */


class Animal {
protected:
    std::string m_name;

    // We're making this constructor protected because
    // we don't want people creating Animal objects directly,
    // but we still want derived classes to be able to use it.
    Animal(std::string_view name) : m_name{ name } { }

    // Забороняємо копіювання
    // Це для того, щоб прибрати можливість передати вже готовий Animal об'єкт для копії у конструктор через посилання або оператор присвоєння
    Animal(const Animal&) = delete;
    Animal& operator=(const Animal&) = delete;


    // Дозволяємо переміщення (move)
    // Для того, щоб працював з const auto& cats{ std::to_array<Cat>({...}), бо без цього буде заборона на move
    Animal(Animal&&) = default;
    Animal& operator=(Animal&&) = default;

public:
    std::string_view getName() const { return m_name; }
    std::string_view speak() const { return "???"; }
    virtual std::string_view v_speak() const { return "???"; }
};

class Cat: public Animal {
public:
    Cat(std::string_view name) : Animal{ name } {}

    std::string_view speak() const { return "Meow"; }
    virtual std::string_view v_speak() const { return "Meow"; }
};

class Dog: public Animal {
public:
    Dog(std::string_view name) : Animal{ name } {}

    std::string_view speak() const { return "Woof"; }
    virtual std::string_view v_speak() const { return "Woof"; }
};


template <typename T>
void report(const T& rAnimal) {
    std::cout << rAnimal.getName() << " says " << rAnimal.speak() << '\n';
}




//------------------------------------
class A {
public:
    virtual std::string_view getName() const { return "A"; }
};

class B: public A {
public:
    virtual std::string_view getName() const { return "B"; }
};

class C: public B {
public:
    virtual std::string_view getName() const { return "C"; }
};

class D: public C {
public:
    virtual std::string_view getName() const { return "D"; }
};

int main() {
    const Cat cat{ "Fred" };
    std::cout << "cat is named " << cat.getName() << ", and it says " << cat.speak() << '\n'; // cat is named Fred, and it says Meow

    const Dog dog{ "Garbo" };
    std::cout << "dog is named " << dog.getName() << ", and it says " << dog.speak() << '\n'; // dog is named Garbo, and it says Woof

    /**
     * Дивовижно, що якщо я поставлю тип Animal як pointer і передам у конструктор reference на Cat то я отримую
     * reference на базовий клас Animal. Це має сенс, бо коли у нас є однакові функції speak і у Cat і у Animal то Cat
     * начебто приховує те що є у Animal, а тут навпаки Animal наче приховує що він щось знає за Cat.
     */
    const Animal* pAnimal{ &cat };
    std::cout << "pAnimal is named " << pAnimal->getName() << ", and it says " << pAnimal->speak() << '\n'; // pAnimal is named Fred, and it says ???

    pAnimal = &dog;
    std::cout << "pAnimal is named " << pAnimal->getName() << ", and it says " << pAnimal->speak() << '\n'; // pAnimal is named Garbo, and it says ???


    std::cout << "----------------------------------------------------------\n";
    /**
     * Тепер уявимо що у мене є масив Cat та Dog і мені треба викликати на них .speak(), проблема в тому, що
     * у C++ масив може мати лише один тип тож мені треба буде мати два масиви
     */
    const auto& cats{ std::to_array<Cat>({{ "Fred" }, { "Misty" }, { "Zeke" }}) };
    const auto& dogs{ std::to_array<Dog>({{ "Garbo" }, { "Pooky" }, { "Truffle" }}) };

    /**
     * Або використати батьківський клас, але оскільки батьківський клас теж має метод .speak() то буде викликаний саме він,
     * а мені треба викликати .speak() саме на Dog чи Cat.
     */
    const Cat fred{ "Fred" };
    const Cat misty{ "Misty" };
    const Cat zeke{ "Zeke" };

    const Dog garbo{ "Garbo" };
    const Dog pooky{ "Pooky" };
    const Dog truffle{ "Truffle" };

    const auto& animals {std::to_array<const Animal*>({&fred, &misty, &zeke, &garbo, &pooky, &truffle})};
    for (const auto animal : animals) {
        std::cout << animal->getName() << " says " << animal->speak() << '\n'; // Fred says ??? ....
    }

    /**
     * Можна використати підхід з template<T>, але у нього є два недоліки
     *  1) Він не прив'язаний до типу і буде працювати на будь якому об'єкті у якого є метод .speak(), не лише Dog чи Cat
     *  2) У випадку з масивом він теж не допоможе
     */
    report(fred);        // Fred says Meow
    report(*animals[0]); // Fred says ???



    /**
     * Virtual functions — це спеціальний тип функції-члена, яка під час виклику повертається до найбільш Child (Derived) версії функції
     * Щоб зробити функцію "virtual" треба просто додати ключове слово virtual перед назвою функції (методу)
     *
     * Як бачимо, у прикладі нижче будуть викликані методи саме на класі Dog та Cat, а не на батьківському Animal
     */

    std::cout << "--------------------------------------------------------\n";

    for (const auto animal : animals) {
        std::cout << animal->getName() << " says " << animal->v_speak() << '\n'; // Fred says Meow ....
    }

    const Animal* p1Animal{ &dog };
    std::cout << p1Animal->v_speak() << '\n'; // Woof (а без virtual було ???), хоча pAnimal це reference на Animal* у класі Dog
    /**
     * Але варто зауважити що virtual буде так працювати лише якщо викликана через reference чи pointer, якщо ж вона буде викликана на просто,
     * об'єкті, тоді поведінка буде як у простого метода.
     */
    C c{};
    std::cout << c.getName() << '\n'; // C - will always call C::getName

    A a { c }; // copies the A portion of c into a (don't do this)
    std::cout << a.getName() << '\n'; // A - will always call A::getName

    return 0;
}

