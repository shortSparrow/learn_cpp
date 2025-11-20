#include <iostream>

std::string&       returnByReference(); // returns a reference to an existing std::string (cheap)
const std::string& returnByReferenceToConst(); // returns a const reference to an existing std::string (cheap)

// returns a const reference
const std::string& getProgramName() {
    static const std::string s_programName { "Calculator" }; // has static duration, destroyed at end of program

    return s_programName;
}

std::string& getName() {
    std::string name = "Oleks";

    return name;
}


const int& returnByConstReference(const int& ref) {
    return ref;
}


const std::string& foo(const std::string& s) {
    return s;
}

std::string getHello() {
    return "Hello"; // implicit conversion to std::string
}

std::string& getRef(); // some function that returns a reference

const std::string& getConstRef(); // some function that returns a reference to const


std::string* getPtr(); // some function that returns a pointer

/**
 * Never return a (non-static) local variable or temporary by reference.
 */
int main() {

    // std::cout << "This program is named " << getProgramName() << '\n'; // Calculator
    //
    // // На моїй машині порожньо, бо змінна знищилася і reference не може на неї послатися
    // std::cout << "My name is " << getName() << '\n'; // Undefined behavior. Ламає код далі
    //
    // std::string name = getName(); //  // makes a copy of a dangling reference
    // std::cout << "My name is " << name << '\n'; // Undefined behavior. Ламає код далі


    // case 1: direct binding
    const int& ref1 { 5 }; // extends lifetime
    std::cout << ref1 << '\n'; // 5
    const int& ref2 { returnByConstReference(ref1) };
    std::cout << ref2 << '\n'; // 5



    /**
     * It’s okay for an rvalue passed by const reference to be returned by const reference
     *
     * Коли константне посилання (const T&) прив'язується до тимчасового об'єкта (rvalue), час життя цього тимчасового
     * об'єкта продовжується до кінця часу життя самого посилання.
     *
     * У нас getHello повертає тимчасовий об'єкт а далі foo отримує посилання на нього, тож описане вище правило діє,
     * і час життя тимчасового об'єкта поверненого getHello продовдується.
     *
     * foo() повертає константне посилання на той самий тимчасовий об'єкт "Hello", час життя якого було подовжено
     * s ініціалізується копією того що повертає foo. Тимчасовий об'єкт "Hello" знищується.
     *
     * Але це небезпечний підхід, кращим варіантом є
     * std::string foo_safe(const std::string& s) {return s;}
     * Тут foo поверне не посиалння, а саме значення
     */
    const std::string s{ foo(getHello()) };
    std::cout << s;


    /**
     * Головною перевагою повернення за адресою (pointer) над поверненням за посиланням (reference) є те, що ми можемо змусити функцію
     * повернути nullptr, якщо немає дійсного об'єкта для повернення. Наприклад, припустимо, що ми маємо список студентів, яких хочемо знайти.
     * Якщо ми знайдемо потрібного студента в списку, ми можемо повернути покажчик на об'єкт, що представляє відповідного студента.
     * Якщо ми не знайдемо жодного відповідного студента, ми можемо повернути nullptr, щоб вказати, що відповідний об'єкт студента не знайдено.
     *
     * Основним недоліком повернення за адресою (pointer) є те, що треба пам'ятати про необхідність перевірки nullptr перед
     * опрацюванням значення, що повертається, інакше може статися опрацювання нульового покажчика (nullptr), що призведе до невизначеної поведінки.
     * Через цю небезпеку повернення за посиланням (reference) слід вважати кращим за повернення за адресою (pointer), якщо тільки
     * не потрібна можливість повернення «відсутнього об'єкта».
     *
     * Висновок
     * Надавайте перевагу поверненню за посиланням (referecne), а не за адресою (pointer), окрім випадків,
     * коли важлива можливість повернення «відсутнього об'єкта» (за допомогою nullptr).
     */



    /**
     * auto з reference
     * Так само як у
     * const double a { 7.8 };
     * auto b { a }; // Тим double (const dropped)
     * const чи constexpr опускається, так само і з referecne. Keyword auto прибирає reference
     */
    auto ref {getRef()}; // Тип буде std::string (НЕ std::string&)
    auto& ref3 { getRef() }; // Тип буде std::string& (reference dropped, reference reapplied). Рівно так само як і з const
    auto ref4{ getConstRef() }; // std::string (reference dropped, then top-level const dropped from result)
    const auto& ref5{ getConstRef() }; // const std::string& Нічого не було опущено

    // Для pointer auto не обрізає *
    auto ptr1{ getPtr() };  // std::string*
    auto* ptr2{ getPtr() }; // std::string*

    return 0;
}