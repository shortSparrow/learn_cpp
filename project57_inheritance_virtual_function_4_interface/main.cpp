#include <cmath>
#include <iostream>
#include <string_view>

class Base{
public:
    virtual ~Base() = default;
    std::string_view sayHi() const { return "Hi"; } // a normal non-virtual function

    virtual std::string_view getName() const { return "Base"; } // a normal virtual function

    virtual int getValue() const = 0; // a pure virtual function
};

/**
 * Зазвичай функції мають назву і тіло, однак у C++ є спеціальний вид virtual function які
 * можуть не містити тіла. Це так звана pure virtual function чи abstract function.
 * Такі функції діють як placeholder який призначений для перевизначення похідними (Derived) класами.
 *
 * Також цікавою особливістю таких функцій є те, що якщо клас має хоча б одну таку функцію, то
 * на цей клас не можна створити інстанс, бо клас стає абстрактним.
 *
 * Тобто логіка така: Коли ми додаємо pure virtual function до нашого класу, ми говоримо, що це обов'язок
 * батьківського класу (Derived) реалізувати цю функцію.
 * Якщо батьківський клас не дасть тіло для pure virtual function, то він теж стане абстрактним.
 *
 */

class Animal {
protected:
    std::string m_name {};

    // We're making this constructor protected because
    // we don't want people creating Animal objects directly,
    // but we still want derived classes to be able to use it.
    Animal(std::string_view name) : m_name{ name } { }

public:
    const std::string& getName() const { return m_name; }
    // virtual std::string_view speak() const { return "???"; } // Оскільки це просто virtual, а не pure virtual то її Cow може і не перевизначати якщо не хоче
    virtual std::string_view speak() const = 0; // Тепер це pure virtual function і якщо Cow її не перевизначить, то буде помилка. І тепер Animal став абстрактним

    virtual ~Animal() = default;
};

class Cow : public Animal {
public:
    Cow(std::string_view name) : Animal{ name } { }

    // Якби не перевизначили цю функцію то Cow був би абстрактним класом і ми б не змогли створити його інстанс.
    std::string_view speak() const override {return "Me-ee-ee";};
};


// ------------------------------------------------------------------------------------------------------------------------------

class Base_1 {
// constructor у protected, щоб ми не змогли б зробити це як інстанс, хоча ми і так би не змогли, бо Base_1 це abstract class
protected:
    Base_1() {

    }
public:

    virtual ~Base_1() = default;

    // Робимо цю функцію pure virtual (якщо вони без тіла - то це pure virtual function)
    virtual void speak() const = 0;


};
// Це дефолтна реалізація методу .speak(). Коли ми вище визначаємо цю функцію як pure virtual ми зобов'язуємо
// будь-який клас який успадкує Base_1 зробити свою реалізацію .speak() (окрім випадків якщо вони хочуть бути абстрактними класами)
// Реалізувати її в класі ми не можемо, бо буде видавати помилку, що така функція вже є (буде посилатися на pure virtual function)
void Base_1::speak() const {
    std::cout << "I'm base class\n";
}

class Derived_1: public Base_1 {
public:
    Derived_1() {

    }

    void speak() const override {
        // Припустимо що в конкретно цьому випадку ми не хочемо реалізовувати свій метод, тому явно
        // викликаємо батьківський метод
        Base_1::speak();
    }
};

/**
 * Тобто краса і потужність цих pure virtual function в тому, що якщо я визначив якийсь базовий клас, і він містить таку
 * функцію то цей клас:
 *  1) Стає абстрактним, тобто від нього не можна створити інстанс
 *  2) Всі хто від нього успадкуються будуть зобов'язані перевизначити функцію, інакше вони теж будуть абстрактними, тобто
 *     ми так убезпечуємося від того, аби хтось забув перевизначити функцію
 *  3) Маємо можливість передавати базовий клас як тип, щоб могти в один масив запхати Cow, Dog, Cat, бо це масив Animal,
 *     і якщо на них викликати .speak() то ми точно гарантуємо що він буде викликаний саме на Cow, Dog, Cat, а не на Animal,
 *     бо функція virtual і оскільки вона pure то ми точно певні, що ці класи перевизначили цей метод.
 */

int main() {
    // Base b {}; // Компілятор видає помилку: Base is abstract; function int Base::getValue() const is pure virtual

    Cow cow{"Betsy"};
    std::cout << cow.getName() << " says " << cow.speak() << '\n'; // Betsy says Me-ee-ee

    Derived_1 derived_1 {};
    derived_1.speak(); // I'm base class

    return 0;
}

/**
 * Так ми плавно переходимо до поняття Інтерфейс класу.
 * Інтерфейс класу - це клас, де всі функції pure virtual, і він зобов'язує того, хто від нього
 * успадкувався перевизначити ці функції. Це треба, коли ми хочемо визначити методи класу, але
 * лишити реалізацію внутрішньою.
 */

class IErrorLog {
public:
    virtual bool openLog(std::string_view filename) = 0;
    virtual bool closeLog() = 0;

    virtual bool writeError(std::string_view errorMessage) = 0;

    virtual ~IErrorLog() {} // make a virtual destructor in case we delete an IErrorLog pointer, so the proper derived destructor is called
};

class FileErrorLog {
public:
    void writeError(std::string_view) {}
};

/**
 * Ця функція mySqrt приймає аргумент типу FileErrorLog і це конкретна реалізація, це може
 * бути чого ми хочемо, а може і ні. Краще в цьому випадку приймати не FileErrorLog, а IErrorLog,
 * тоді ми зможемо використовувати цю функцію з будь-яким логером, який реалізовує інтерфейс IErrorLog
 */
double mySqrt(double value, FileErrorLog& log) {
    if (value < 0.0) {
        log.writeError("Tried to take square root of value less than 0");
        return 0.0;
    }

    return std::sqrt(value);
}