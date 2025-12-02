#include <iostream>


/**
 * struct і class дуже схожі, є конвенція що все що може один - може і інший.
 * Але ми використовуємо їх для різних задача, до прикладу, ми використовуємо
 * struct для зберігання і передачі даних, а класи для логіки
 *
 */

struct Date {
    int year {};
    int month {};
    int day {};

    void incrementDay() {
        ++day;
    }
};

struct Date2 {
    int year {};
    int month {};
    int day {};

    void print() {
        std::cout << year << '/' << month << '/' << day;
    }

    /**
     *  A const member function is a member function that guarantees it will not modify the object
     *  or call any non-const member functions (as they may modify the object)
     */
    void print2() const {
        std::cout << year << '/' << month << '/' << day;
    }
};

void doSomething(const Date2& date) {
    date.print();
}

int main() {
    struct Person {
        std::string name{};
        int age{};

        void kisses(const Person& person) {
            std::cout << name << " kisses " << person.name << '\n';
        }

        void printHello() {
            std::cout << "hello" << '\n';
        }
    };


    Person joe{ "Joe", 29 };
    Person kate{ "Kate", 27 };

    joe.kisses(kate);
    joe.printHello();



    // Modifying the data members of const objects is disallowed


    const Date today { 2020, 10, 14 }; // const

    today.day += 1;        // compile error: can't modify member of const object
    today.incrementDay();  // compile error: can't call member function that modifies member of const object


    /**
    * Const objects may not call non-const member functions
    * You may be surprised to find that this code also causes a compilation error:
    */


    const Date2 today2 { 2020, 10, 14 }; // const
    today2.print();  // compile error: can't call non-const member function
    today2.print2(); // ok



    // Const objects via pass by const reference

    Date2 today3 { 2020, 10, 14 }; // non-const
    today3.print();
    doSomething(today); // error тому що аргумент позначений як const reference


    /**
     * Member function const and non-const overloading
     * Можна перевантажити функцію-член, щоб мати константну і неконстантну версії однієї і тієї ж функції.
     * Це працює, оскільки кваліфікатор const вважається частиною сигнатури функції, тому дві функції,
     * які відрізняються лише своєю константністю, вважаються різними.
     */

    struct Something {
        void print() {
            std::cout << "non-const\n";
        }

        void print() const {
            std::cout << "const\n";
        }
    };

    Something s1{};
    s1.print(); // calls print()

    const Something s2{};
    s2.print(); // calls print() const


    return 0;
}