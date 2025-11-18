#include <iostream>

// Function-like macro that evaluates to true if the type (or object) is equal to or smaller than
// the size of two memory addresses
#define isSmall(T) (sizeof(T) <= 2 * sizeof(void*))

struct S {
    double a;
    double b;
    double c;
};



void print_addresses(int val, int& ref){
    std::cout << "The address of the value parameter is: " << &val << '\n';
    std::cout << "The address of the reference parameter is: " << &ref << '\n';
}

/**
 * Кожен раз коли ми передаємо параметр у функцію, то створюється
 * копія значення, на простих типах даних це не є великою проблемою,
 * бо це відбувається швидко, але коли ми передаємо класи то виникає проблема,
 * бо зазвичай класи доволі важкі і треба більше часу, щоб зробити їх копії, тому
 * можна використовувати reference, тоді копія не створюється, ми просто передаємо
 * посилання на об'єкт.
 *
 *
 * Як визначити коли використовувати передачу як параметра, а коли як reference?
 * Коли ми передаємо через параметр то робиться копія, і швидкість цієї операції залежить від:
 *      1) Розміру об'єкта, чим він більше пам'яті займає, тим буде повільніше
 *      2) Будь-яка додаткова дія при ініціалізації (відкриття файлу для читання чи запису, відкриття бази даних і т.п.)
 * Коли ми передаємо через reference то виникає додаткова дія - пошук об'єкта у пам'яті, щоб зчитати.
 *
 * Також компілятор іноді може оптимізувати код, що використовує передачу за значенням, ефективніше, ніж код, що використовує
 * передачу через reference.
 *
 * Тож треба дивитися коли буде ефективніше передавати через значення, а коли через reference. Загалом є стандартне правильно -
 * прості об'єкти передаються через значення, складні через reference. Коли є спеціальні структури по типу string_view то краще
 * передавати їх як значення ніж std::string&
 *
 * Додатково є правило, що якщо об'єкт займає більше двух комірок пам'яті, то його буде швидше передати через reference, є
 * навіть макрос для тогоЮ аи визначити скільки комірок пам'яті замає об'єкт
 */

void print_str(std::string& str) {
    std::cout << str << '\n';
}

void do_something(int& x) {
    x += 1;
}

void print_non_const_ref(int& x) {}
void print_const_ref(const int& x) {}


void printVal(double d){}
void printRef(const double& d){}


void printSV(std::string_view sv) {
    std::cout << sv << '\n';
}

void printS(const std::string& s) {
    std::cout << s << '\n';
}

int main() {
    std::string my_str = "Hello";
    print_str(my_str);

    int x { 5 };
    std::cout << "The address of x is: " << &x << '\n'; // The address of x is: 0x2edcbffa8c
    print_addresses(x, x);
    // The address of the value parameter is: 0x2edcbffa60
    // The address of the reference parameter is: 0x2edcbffa8c (та сама адреса, що і у оригінального x)


    int y = {1};
    do_something(y);
    std::cout << y << '\n'; // 2



    /**
     * Порівняння передавання reference const і mutable
     *
     * На відміну від посилання на non-const (яке може зв'язуватися тільки з модифікованими lvalue),
     * посилання на const може зв'язуватися з модифікованими lvalue, немодифікованими lvalue та rvalue.
     * Тому, якщо ми зробимо параметр посилання const, то він зможе зв'язуватися з будь-яким типом аргументу
     */

    int x0 { 5 };
    print_non_const_ref(x0);   // ok: x is a modifiable lvalue, y binds to x

    const int z0 { 5 };
    print_non_const_ref(z0);   // error: z is a non-modifiable lvalue

    print_non_const_ref(5);   // error: 5 is an rvalue



    int x1 { 5 };
    print_const_ref(x1);   // ok: x is a modifiable lvalue, y binds to x

    const int z { 5 };
    print_const_ref(z);   // ok: z is a non-modifiable lvalue, y binds to z

    print_const_ref(5);   // ok: 5 is rvalue literal, y binds to temporary int object


    /**
     * Коли ми передаємо інший тип то споватку створить тимчасовий об'єкт відповдіного типу,
     * а потім він передастся (скопіюється якщо це просто передача аргумента чи передастся
     * посилання, якщо це reference) до функції
     */
    printVal(5); // 5 converted to temporary double, copied to parameter d
    printRef(5); // 5 converted to temporary double, bound to parameter d



    // Скільки size of a memory address займають різні об'єкти (якщо 2 то доцільно передати як параметр
    // і зробити копію, якщо більше, то як reference)
    std::cout << std::boolalpha; // print true or false rather than 1 or 0
    std::cout << isSmall(int) << '\n'; // true

    double d {};
    std::cout << isSmall(d) << '\n'; // true
    std::cout << isSmall(S) << '\n'; // false



    /**
     * Пояснення чому краще використовувати std::string_view замість std::string&
     * Розглянемо передачу через std::string_view
     * 1) Якщо ми передаємо аргумент std::string, компілятор перетворить std::string на std::string_view, що не вимагає великих витрат,
     *    тому це підходить.
     * 2) Якщо ми передаємо аргумент std::string_view, компілятор скопіює аргумент у параметр, що є недорогим, тому це підходить.
     * 3) Якщо ми передаємо рядок у стилі C або літерал рядка, компілятор перетворить їх на std::string_view, що є недорогим, тому це підходить.
     *
     * Розглянемо передачу через const std::string&
     * 1) Якщо ми передаємо аргумент std::string, параметр буде пов'язаний з аргументом через reference, що є недорогим, тому це нормально.
     * 2) Якщо ми передаємо аргумент std::string_view, компілятор відмовить у виконанні неявного перетворення і видасть помилку компіляції.
     *    Ми можемо використовувати static_cast для явного перетворення (в std::string), але це перетворення є дорогим
     *    (оскільки std::string створить копію рядка, що переглядається). Після завершення перетворення параметр буде пов'язаний з результатом,
     *    що є недорогим. Але ми створили дорогу копію для перетворення, тому це не дуже добре.
     * 3) Якщо ми передаємо рядок у стилі C або літерал рядка, компілятор неявно перетворить його на std::string, що є дорогим.
     *    Тому це теж не дуже добре.
     */

    std::string s{ "Hello, world" };
    std::string_view sv { s };

    // Pass to `std::string_view` parameter
    printSV(s);           // ok: inexpensive conversion from std::string to std::string_view
    printSV(sv);             // ok: inexpensive copy of std::string_view
    printSV("Hello, world"); // ok: inexpensive conversion of C-style string literal to std::string_view

    // pass to `const std::string&` parameter
    printS(s);              // ok: inexpensive bind to std::string argument
    printS(sv);             // compile error: cannot implicit convert std::string_view to std::string
    printS(static_cast<std::string>(sv)); // bad: expensive creation of std::string temporary
    printS("Hello, world"); // bad: expensive creation of std::string temporary

    return 0;
}