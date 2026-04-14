#include <iostream>


/**
 * Перевантаження методів при успадкуванні
 * Коли ми робимо наслідування і у класу Child немає класу .say_hello() а у Parent є, то якщо ми
 * на інстансі Child викличемо .say_hello() то компілятор подивиться, що у Child цього методу немає, однак
 * він є у Parent тому викличе батьківський метод.
 * А як би у Child цей метод був би, то викликався б саме він, а не батьківський, тобто було б перевантаження в певному сенсі.
 *
 * Якщо у нас перевизначена функція .say_hello() і у Child і у Parent, але ми не хочемо повністю замінити функціонал який ця
 * функція надає у Parent, а лише доповнити його, то ми у Child можемо викликати саме батьківську функцію як Parent::say_hello()
 * і далі доповнити потрібний код
 *
 */

class Base {

public:
    Base() { }
    int my_value {33};

    void identify() const { std::cout << "Base::identify()\n"; }

    void say_hello() const {
        std::cout << "Base hello\n";
    }

    friend std::ostream& operator<< (std::ostream& out, const Base&) {
		out << "In Base\n";
		return out;
	}

    void print(int)    { std::cout << "Base::print(int)\n"; }
    void print(double) { std::cout << "Base::print(double)\n"; }
    void print(short) { std::cout << "Base::print(short)\n"; }

    int get_value() const { return 1; }
};

class Derived: public Base {
public:
    Derived() { }

    void identify() const { std::cout << "Derived::identify()\n"; }

    void say_hello() const {
        std::cout << "extra hello\n";      // додаткова логіка
        Base::say_hello();                 // виклик базової логіки з класу Base
        std::cout << "additional hello\n"; // додаткова логіка

        // Якщо тут замість Base::say_hello() написати просто say_hello() то буде викликано версію саме
        // з Derived класу і ми матимемо нескінченну рекурсію
    }


    // У випадку з friend функцією все не так просто, friend функція не є частиною класу, тому
    // через синтаксис myClass:friend_func() викликати метод у батьківському класі не можна.
    // Але на щастя тут допоможе static_cast
    friend std::ostream& operator<< (std::ostream& out, const Derived& d) {
		out << "In Derived\n";
		// static_cast Derived to a Base object, so we call the right version of operator<<
		out << static_cast<const Base&>(d);
		return out;
    }


    // Якщо ми успадкували якийсь метод з Base, але не хочемо що його можна було б викликати з Derived
    // то можна використати delete. Тоді якщо ми спробуємо викликати цей метод на інстансі класу Derived
    // компілятор скаже, що метод недоступний і видасть помилку.
    int get_value() = delete;

    // using Base::print; // Зробить так, щоб функції print з класу Base були використані якщо у нас буде перевантаження типів
    void print(int) { std::cout << "Derived::print(double)\n"; }


    // Використання using Base::... може змінити модифікатори доступу. У нас у Base є public поле my_value
    // ми можемо зробити його private у Derived через using Base::my_value у частині з private.
    // Але от зробити private поле з Base публічним у Derived неможливо, бо це поле відсутнє у класі Derived, адже воно private
private:
    using Base::my_value;
};

int main() {
    Base base {};
    base.identify(); // Base::identify()

    Derived derived {};
    derived.identify(); // Derived::identify()

    std::cout << "------------------------------\n";
    derived.say_hello();

    std::cout << "------------------------------\n";
    std::cout << derived; // In Derived\nIn Base

    std::cout << "------------------------------\n";
    // Викличе функцію Derived::print(double)
    // Тобто у нас компілятор не знайшов у Derived print(double) і тому він просто приведе int до double, у батьківському класі він не буде шукати
    derived.print(1.0);

    // Викличе функцію Derived::print(double)
    // Тут так само, відбувається type promotion до більшого типу і буде викликано Derived::print(int)
    constexpr short a {1};
    derived.print(a);

    // Якщо ми все-таки хочемо використати саме .print() з Base то треба у Derived прописати над методом print() using Base::print
    // Тоді і derived.print(1.0) і derived.print(a) викличуть Base::print()

    std::cout << "------------------------------\n";
    // std::cout << derived.my_value; // Ми це поле бачимо у інстансі, але якщо спробуємо його взяти, то отримаємо помилку 'int Base::my_value' is inaccessible

    // derived.get_value(); // Ми це поле бачимо у інстансі, але якщо спробуємо його взяти, то отримаємо помилку Attempt to use deleted function int Derived::get_value()
    std::cout << derived.Base::get_value() << '\n'; // А ось так помилки не буде, бо ми викликаємо не у Derived, а у Base
    std::cout << static_cast<Base&>(derived).get_value() << '\n'; // Так теж спрацює, бо ми вказуємо що викликати треба метод з Base

    return 0;
}