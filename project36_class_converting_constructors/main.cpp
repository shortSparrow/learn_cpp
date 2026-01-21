#include <iostream>

//...............Приклад де constructor conversion працює......................
class Foo {
private:
    int m_x{};
public:
    Foo(int x)
        : m_x{ x }
    {
    }

    int getX() const { return m_x; }
};

// has a Foo parameter
void printFoo(Foo f) {
    std::cout << f.getX();
}


//......................Приклад де constructor conversion НЕ працює.................

class Employee {
private:
    std::string m_name{};

public:
    Employee(std::string_view name)
        : m_name{ name }
    {
    }

    const std::string& getName() const { return m_name; }
};

// has an Employee parameter
void printEmployee(Employee e) {
    std::cout << e.getName();
}


//............................Приклад де заборонено constructor conversion.................................
class Dollars {
private:
    int m_dollars{};

public:
    explicit Dollars(int d) // now explicit
        : m_dollars{ d }
    {
    }

    int getDollars() const { return m_dollars; }
};

void print(Dollars d) {
    std::cout << "$" << d.getDollars();
}

int main() {
    printFoo(5); //
    /**
     * Неочікувано цей код працює бо у C++ є type conversion, тут у printFoo ми передаємо 5, а він очікує Foo,
     * тож він викликає Foo(5) і оскільки у Foo є конструктор з параметром int, то у нас ініціалізується Foo.
     * Технічно тут відбувається conversion з int до Foo
     */


    printEmployee("Joe"); // we're supplying an string literal argument
    /**
     * Неочікувано, але це це працює, тому що є правило "Only one user-defined conversion may be applied"
     * В цьому випадку C-style string конвертується у string_view (1 type conversion) і потім намагається
     * конвертувати string_view у Employee, але оскільки це вже другий conversion то видає помилку
     */

    // 1 Спосіб це виправити це передати string_view напряму
    using namespace std::literals;
    printEmployee( "Joe"sv); // now a std::string_view literal

    // 2 Спосіб це виправити
    printEmployee(Employee{ "Joe" });



    /**
     * Ми можемо використовувати ключове слово explicit, щоб повідомити компілятору, не слід використовувати conversion для конструктора
     */
    print(5); // compilation error because Dollars(int) is explicit

    // але якщо використаємо static_cast, то працювати буде
    print(static_cast<Dollars>(5)); // ok: static_cast will use explicit constructors

    /**
     * Best practice
     * Робити кожний constructor explicit by default, а якщо буде потреба то прибирати explicit
     */

    return 0;
}