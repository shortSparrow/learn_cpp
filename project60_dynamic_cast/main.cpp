#include <iostream>
#include <string>
#include <string_view>


/**
 * dynamic_cast
 * dynamic_cast робить перетворення base-class pointers у derived-class pointers.
 * dynamic_cast працює як static_cast
 *
 * І virtual function і dynamic_cast обидва працюють з поліморфізмом, і я можу обрати що використати,
 * або virtual function або dynamic cast. Вони по різному впливають на архітектуру, але кінцева мета у них одна.
 * Краще використовувати virtual function, але інколи можна і dynamic_cast, якщо наш метод лише один, то немає
 * сенсу робити його virtual і перевизначати у інших класах.
 *  std::vector<Animal*> zoo;
 *  zoo.push_back(new Cat());
 *  zoo.push_back(new Cobra());
 * У нас лише у кобри буде метод spitVenom() і поганою ідеєю є додавати його у Animal, тому коли ми будемо проходитися
 * по вектору zoo якщо ми зможемо перетворити наш елемент на кобру отже це кобра і модна виконати метод spitVenom
 *  Cobra* cobraPtr = dynamic_cast<Cobra*>(animal);
 *  if(cobraPtr) {cobraPtr->spitVenom();}
 *
 */
class Base {
protected:
    int m_value{};

public:
    Base(int value) : m_value{value} { }

    virtual ~Base() = default;
};

class Derived : public Base {
protected:
    std::string m_name{};

public:
    Derived(int value, std::string_view name)
        : Base{value}, m_name{name} {}

    const std::string& getName() const { return m_name; }
};

Base* getObject(bool returnDerived) {
    if (returnDerived)
        return new Derived{1, "Apple"};
    else {
        return new Base{2};
    }
}

/**
 * Маючи справу з поліморфізмом, ми часто стикаємося з випадками, коли у нас є вказівник на базовий клас,
 * але ми хочемо отримати доступ до певної інформації, яка існує лише в похідному класі.
 *
 */

int main() {
    Base* b{ getObject(true) };
    Derived* d{ dynamic_cast<Derived*>(b) }; // use dynamic cast to convert Base pointer into Derived pointer
    std::cout << "The name of the Derived is: " << d->getName() << '\n'; // The name of the Derived is: Apple

    delete b;

    /**
     * У прикладі вище у нас все вийшло, pointer на клас Base мав у собі Derived, бо був ним раніше,
     * але що, якби не був би? Тоді була б помилка, бо pointer буде nullptr
     */

    // ------------------------------------------------------------------------------------------------------------
    Base* b2{ getObject(false) };
    Derived* d2{ dynamic_cast<Derived*>(b2) }; // use dynamic cast to convert Base pointer into Derived pointer
    // std::cout << "The name of the Derived is: " << d2->getName() << '\n'; // The name of the Derived is:

    /**
     * Оскільки d2 - це nullptr, то при виклику d2->getName() буде Undefined Behavior (Невизначена поведінка)
     * У нас програма не падає, і вивід у термінал відбувається, але після цього все одно маємо помилку
     *
     * !! Тому завжди необхідно перевіряти що dynamic cast був успішним через перевірку на null pointer !!
     */

    if (d2) // у нас не викличеться, бо у нас d2 - це nullptr
        std::cout << "The name of the Derived is: " << d2->getName() << '\n';

    delete b2;


    /**
     * Випадки коли dynamic_cast не буде працювати:
     *  1) Коли є protected чи private успадкування (inheritance).
     *  2) Для класів, які не оголошують і не успадковують жодних віртуальних функцій (і, таким чином, не мають віртуальної таблиці).
     */


    // Downcasting with static_cast (Приведення типу за допомогою static_cast)
    /**
     * Виявляється, що приведення типу до нижчого рівня можна також здійснити за допомогою static_cast.
     * Головна відмінність полягає в тому, що static_cast не виконує перевірки типу під час виконання, щоб переконатися,
     * що ваші дії мають сенс. Це робить використання static_cast швидшим, але й небезпечнішим.
     *
     * Якщо ми приводимо Base* до Derived*, це «удасться», навіть якщо pointer Base не вказує на об’єкт Derived.
     * Це призведе до невизначеної поведінки, коли ми спробуємо отримати доступ до отриманого pointer Derived
     * (який насправді вказує на об’єкт Base).
     *
     */
    std::cout << "--------------------------------------------------------\n";
    Base* b3{ getObject(true) };
    Derived* d3{ static_cast<Derived*>(b3) };
    std::cout << "The name of the Derived is: " << d3->getName() << '\n'; // The name of the Derived is: Apple

    // Тут буде помилка, тож треба робити перевірку і простіше використати dynamic_cast
    Base* b4{ getObject(false) };
    Derived* d4{ static_cast<Derived*>(b4) };
    // std::cout << "The name of the Derived is: " << d4->getName() << '\n'; // The name of the Derived is:



    std::cout << "--------------------------------------------------------\n";
    /**
     * dynamic_cast and references
     * dynamic_cast найчастіше використовують з pointer, але також можна використовувати його і з reference
     */

    Derived banana{1, "Banana"};
    Base& b5{ banana }; // set base reference to object
    Derived& d5{ dynamic_cast<Derived&>(b5) }; // dynamic cast using a reference instead of a pointer

    std::cout << "The name of the Derived is: " << d5.getName() << '\n'; // The name of the Derived is: Banana

    /**
     * Загалом краще використовувати virtual function, але в деяких випадках dynamic_cast може буди зручним.
     */


    return 0;
}