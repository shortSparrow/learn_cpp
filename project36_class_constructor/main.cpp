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
    };


    Foo foo1 {1,2};
    Foo foo2 {4.5};

    // implicit type conversion працює так само як і у звичайних функцій
    Foo foo3 {'a', true};

    // default constructor
    Foo foo4{}; // value initialization, calls Foo() default constructor
    Foo foo5;  // default initialization, calls Foo() default constructor





    return 0;
}