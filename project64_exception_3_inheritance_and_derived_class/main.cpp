#include <memory>
#include <iostream>

#include <cstddef>
#include <exception>
#include <limits>
#include <string>

/**
 * Якщо конструктор з якихось причин не може виконатися (наприклад, користувач передав недійсні дані), просто
 * кидаємо виняток, щоб вказати на те, що об’єкт не вдалося створити. У такому випадку створення об'єкта
 * припиняється, а всі члени класу (які вже були створені та ініціалізовані до виконання тіла конструктора)
 * знищуються у звичайному порядку.
 *
 * Але варто зауважити, що в цьому випадку деструктор ніколи не викличеться, адже створення класу не було
 * завершено, і відповідно ми не можемо покладатися на очищення даних через деструктор.
 * Тому якщо алокуємо дані в конструкторі, то самим попіклуватися про їх видалення або робити так, щоб
 * дані алокувалися у змінних класу (member value), тоді вони самі очистяться
 *
 */


/**
 * У першому випадку, якщо конструктор класу Foo завершиться з помилкою після того, як ptr виділить динамічну пам’ять,
 * Foo буде відповідати за очищення, що може виявитися складним завданням.
 *
 * У другому випадку, якщо конструктор класу Foo завершиться з помилкою після того, як ptr виділить динамічну пам’ять,
 * буде виконано деструктор ptr, який поверне цю пам’ять системі. Foo не потрібно виконувати явне очищення,
 * коли управління ресурсами делеговано елементам, що підтримують RAII!
 *
 */


class Foo {
private:
    int* ptr; // Foo will handle allocation/deallocation
};


class Foo2 {
private:
    std::unique_ptr<int> ptr; // std::unique_ptr will handle allocation/deallocation
};

/**
 * Коли ми відловлюємо нашу кастомну помилку, то найкраще у catch отримувати її як reference, а не копію.
 * Це швидше і убезпечить нас від object slicing коли маємо справу з Derived класом.
 *
 * І також варто брати помилки не проста як reference, а як const reference.
 */

class ArrayException {
private:
    std::string m_error;

public:
    ArrayException(std::string_view error) : m_error{ error } { }

    const std::string& getError() const { return m_error; }
};


// try {
//     int value{ array[5] }; // out of range subscript
// } catch (const ArrayException& exception) {
//     std::cerr << "An array exception occurred (" << exception.getError() << ")\n";
// }



/**
 * Проблема із успадкуванням
 * Розглянемо приклад коли є Base та Derived клас.
 * Оскільки ми приймаємо їх як const reference то вийняток Derived буде спіймано першим catch (const Base& base),
 * бо по суті це один клас, а другий catch (const Derived& derived) уже не відпрацює, бо цю помилку ми обробили
 * у верхньому catch.
 *
 * Якби спочатку у нас йшов catch (const Derived& derived) то опрацювався б він, а catch (const Base& base) було б пропущено.
 *
 *
 * Насправді наслідування у exception це дуже корисна річ, бо C++ standard library може генерувати 28 різних вийнятків,
 * і з кожним новим стандартом їх кількість збільшується, але оскільки всі ці вийнятки походять від одного класу і ми
 * можемо відловити всі ці помилки за допомогою catch (const std::exception& exception)
 *
 */

class Base {
public:
    Base() {}
};

class Derived: public Base {
public:
    Derived() {}
};

int main() {
    try {
        throw Derived();
    } catch (const Base& base) {
        std::cerr << "caught Base\n"; // Це буде виведено у термінал
    } catch (const Derived& derived) {
        std::cerr << "caught Derived";
    }


    /**
     * Обробка помилок від standard library
     * exception std::exception має метод .what() який несе інформацію про помилку, однак вона може
     * дещо видозмінюватися в залежності від компілятора
     */
    try {
        std::string s;
        s.resize(std::numeric_limits<std::size_t>::max()); // will trigger a std::length_error or allocation exception
    } catch (const std::exception& exception) {     // This handler will catch std::exception and all the derived exceptions too
        std::cerr << "Standard exception: " << exception.what() << '\n'; // Standard exception: basic_string::_M_replace_aux
    }


    // А це приклад, який показує як можна викликати одну зі стандартних помилок
    try {
        throw std::runtime_error("Bad things happened");
    } catch (const std::exception& exception) { // This handler will catch std::exception and all the derived exceptions too
        std::cerr << "Standard exception: " << exception.what() << '\n'; // Standard exception: Bad things happened
    }


    return 0;
}