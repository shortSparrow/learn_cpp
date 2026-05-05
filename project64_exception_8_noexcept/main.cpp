

/**
 * Інколи нам треба точно знати чи функція генерує exception чи ні, аби бути впевненим,
 * що ми не отримаємо exception.
 *
 * Це можна зробити за допомогою спеціального слова noexcept
 *
 * Зверніть увагу, що noexcept насправді не забороняє функції генерувати винятки або викликати інші функції,
 * які потенційно можуть генерувати винятки. Це дозволено за умови, що функція noexcept перехоплює та обробляє
 * ці винятки внутрішньо, і що ці винятки не виходять за межі функції noexcept.
 *
 * Якщо unhandled exception з'явиться у noexcept function, тоді буде викликано std::terminate, навіть якщо у
 * стеку вище є обробник який міг би обробити цей exception.
 *
 * Тож noexcept варто сприймати як обіцянку не викидати exception власноруч
 *
 */

void do_something() noexcept;

void foo() {throw -1;}
void boo() {};
void goo() noexcept {};
struct S{};



#include <iostream>

class Doomed {
public:
    ~Doomed() {
        std::cout << "Doomed destructed\n";
    }
};

void thrower() {
    std::cout << "Throwing exception\n";
    throw 1;
}

void pt() {
    std::cout << "pt (potentally throwing) called\n";
    //This object will be destroyed during stack unwinding (if it occurs)
    Doomed doomed{};
    thrower();
    std::cout << "This never prints\n";
}

void nt() noexcept {
    std::cout << "nt (noexcept) called\n";
    //This object will be destroyed during stack unwinding (if it occurs)
    Doomed doomed{};
    thrower();
    std::cout << "this never prints\n";
}

void tester(int c) noexcept {
    std::cout << "tester (noexcept) case " << c << " called\n";
    try {
        (c == 1) ? pt() : nt();
    } catch (...) {
        std::cout << "tester caught exception\n";
    }
}

int main() {
    std::cout << std::unitbuf; // flush buffer after each insertion
    std::cout << std::boolalpha; // print boolean as true/false
    tester(1);
    std::cout << "Test successful\n\n";
    tester(2);
    std::cout << "Test successful\n";

    /**
     * Ось що виведе термінал
     * tester (noexcept) case 1 called
     * pt (potentally throwing) called
     * Throwing exception
     * Doomed destructed
     * tester caught exception
     * Test successful
     *
     * tester (noexcept) case 2 called
     * nt (noexcept) called
     * Throwing exception
     * terminate called after throwing an instance of 'int'
     *
     * Тож як бачимо у випадку 2 коли ми кидаємо exception у функції nt() яка noexcept і не обробляємо його там,
     * то не зважаючи на обробник try...catch у функції tester програма закінчується через std::terminate
     *
     */




    /**
     * Специфікатор noexcept із булевим параметром
     *
     * Специфікатор noexcept має необов’язковий булевий параметр.
     * noexcept(true) еквівалентний noexcept, що означає, що функція не генерує винятків.
     * noexcept(false) означає, що функція потенційно може генерувати винятки. Зазвичай ці параметри використовуються
     * лише у шаблонних функціях, щоб шаблонну функцію можна було динамічно створити як таку, що не генерує винятків,
     * або як таку, що потенційно може генерувати винятки, залежно від певного параметризованого значення.
     */


    /**
     * Best practice
     *  Always make move constructors, move assignment, and swap functions noexcept.
     *  Make copy constructors and copy assignment operators noexcept when you can.
     *  Use noexcept on other functions to express a no-fail or no-throw guarantee.
     */



    /**
     * The noexcept operator
     * Оператор noexcept також можна використовувати всередині виразів.
     * Він приймає вираз як аргумент і повертає true або false залежно від того, чи вважає компілятор, що цей вираз
     * призведе до виникнення exception.
     * Оператор noexcept перевіряється статично під час компіляції.
     */

    constexpr bool b1{ noexcept(5 + 3) }; // true; ints are non-throwing
    constexpr bool b2{ noexcept(foo()) }; // false; foo() throws an exception
    constexpr bool b3{ noexcept(boo()) }; // false; boo() is implicitly noexcept(false)
    constexpr bool b4{ noexcept(goo()) }; // true; goo() is explicitly noexcept(true)
    constexpr bool b5{ noexcept(S{}) };   // true; a struct's default constructor is noexcept by default

    return 0;
}