#include <iostream>

/**
 * constructor — це спеціальна функція-член, яка автоматично викликається після створення об'єкта типу класу, що не є агрегатом.
 *
 * Багато нових програмістів не розуміють, чи створюють конструктори об'єкти. Ні, не створюють — компілятор алокує пам'ять для об'єкта
 * до виклику конструктора. Потім конструктор викликається для неініціалізованого об'єкта.
 *
 * Якщо для набору ініціалізаторів не вдається знайти відповідний конструктор, компілятор видасть помилку.
 *
 */

int main() {

    class Foo {
        int x {};
        int y {};

        double z {};

        const int f_const {};

    public:
        Foo(const int a, const int b) {
            x = a;
            y = b;

            std::cout << "Foo(" << x << ", " << y << ") constructed\n";
        }

        Foo(double z) {
            this->z = z;
            std::cout << "Foo(" << z << ") constructed\n";
        }

        // member initialization list - це короткий спосіб призначити аргументи з constructor. Дає можливість ініціалізувати const
        Foo (int a, int b, double c)
            : x {a}, y {b}, z {c}, f_const{a} {
            std::cout << "Foo(" << x << ", " << y << ", " << z << ", " << f_const << ") constructed\n";
        }

        Foo() {
            std::cout << "Foo() constructed\n";
        }

        // буде помилка, бо лише один дефолтний constructor дозволений і ми вже створили його вище
        // Foo(int x=0, int y=0) {}


        /**
         * Using = default to generate an explicitly defaulted default constructor
         * Варто обирати саме такий синтаксис, якщо конструктор буде з порожнім тілом.
         * Є невелика різниця між конструкторами через default, самостійно написаним з порожнім тілом
         * та відсутнім (компілятор напише за нас) у тому, як будуть ініціалізуватися змінні якщо у них
         * не вказати ініціалізатор {}. Але це деталі які я не буду тут описувати, вважатимемо що default - найліпше рішення
         */
        // Foo() = default; // generates an explicitly defaulted default constructor
    };


    Foo foo1 {1,2};
    Foo foo2 {4.5};

    // implicit type conversion працює так само як і у звичайних функцій
    Foo foo3 {'a', true};

    // default constructor
    Foo foo4{}; // value initialization, calls Foo() default constructor
    Foo foo5;  // default initialization, calls Foo() default constructor



    /**
     * Delegating constructors
     * Можуть виникнути випадки коли ми викликаємо один конструктор має мало параметрів і має передати якісь дефолтні,
     * то він може делегувати ініціалізацію іншому конструктору.
     *
     * Тут конструктор викликається з аргументом "James" і він делегує ініціалізацію змінних іншому конструктору,
     * передаючи name="James" і id=0
     *
     */

    class Employee {
    private:
        // у локальному класі static не дозволена, але якби цей клас було створено поза межами main то все було б ок
        static constexpr int default_id { 0 }; // define a named constant with our desired initialization value
        std::string m_name { "???" };
        int m_id { 0 };

    public:
        Employee(std::string_view name)
            : Employee{ name, 0 } // delegate initialization to Employee(std::string_view, int) constructor
        { }

        Employee(std::string_view name, int id)
            : m_name{ name }, m_id { id } // actually initializes the members
        {
            std::cout << "Employee " << m_name << " created\n";
        }

        /**
         * Якщо в багатьох конструкторах треба однакове значення аргументу за замовченням, то краще створити
         * static змінну і використовувати її
         */
        Employee(char name, int id = default_id) // and we can use it here
            : m_name { name }, m_id { id }
        {
            std::cout << "Employee " << m_name << " created\n";
        }
    };

    Employee e1{ "James" };       // Employee James created
    Employee e2{ "Dave", 42 }; // Employee James created

    return 0;
}