#include <cstring>
#include <iostream>
using namespace std;

void printString(std::string str);
void printSV(std::string_view str);
std::string getName();

/**
 *
 * char завжди займає 1 byte
 * char може містити числа і символи. Char може бути signed і unsigned
 *
 * wchar_t, char8_t, char16_t, and char32_t
 * char16_t і char32_t були додані для підтримки Unicode, оскільки з char8_t містить лише 8 bit і годиться
 * тільки для ASCII, а якщо треба вписати українські літери, то треба вже використовувати unicode
 *
 * wchar_t слід уникати майже у всіх випадках (за винятком взаємодії з Windows API), оскільки його розмір визначається реалізацією.
 */

int main() {
    // --------------------------------------- CHAR -------------------------------------
    // char my_name [6] {"Senya"};
    // // my_name має лише 5 символів, але 6-ий символ - це закінчення рядка у C - \0
    //
    // char my_second_name [7] {"Senya"};
    // my_second_name[6] = 'S';
    // cout << my_second_name;
    // // Виведе Senya, це тому що, незважаючи на те що ми алокували аж 7 char
    // // ми маємо Senya\0\0, і я не можу змінити порожні символи, що дивно
    //
    //
    // char my_third_name [15] {"Senya"};
    // my_third_name[7] = 'S';
    // cout << my_third_name; // Senya


    // char a [10];
    // a[0] = 'A';
    // cout << a << endl; // A
    // a[1] = 'B';
    // cout << a << endl; // AB°_ЇгЎ⌂
    //
    // char b [10];
    // strcpy(b, "Senya");
    // cout << b << endl; // Senya
    //
    // char str[80];
    // strcpy(str, "Hello "); // copy
    // strcat(str, "there"); // concatenate
    // cout << strlen(str) << endl; // 11
    // cout << strcmp(str, "Another"); // compare string - 1
    //                                 // 0, якщо рядки ідентичні.
    //                                 // Додатнє число, якщо перший рядок більший за другий.
    //                                 // Від'ємне число, якщо перший рядок менший за другий.



    // У char також можна записувати числа, вони будуть спрйматися як код у ASCII
    // char a {88};
    // cout << a; // X


    // char ch1{5}; // initialize with integer 5 (stored as integer 5. 5 - це символ ENQ у ASCII, але система може на нього вивести свою псевдографіку)
    // char ch2{'5'}; // initialize with code point for '5' (stored as integer 53 - бо 53 - це кол в ASCII для числа  5)
    //
    // cout << ch1; // ♣
    // cout << ch2; // 5


    // --------------------------------------- STRING -----------------------------------------------------

    /**
     * Для нормальної і зручної роботи з рядками є типи std::string і std::string_view
     *
     * If std::string doesn’t have enough memory to store a string, it will request additional memory (at runtime)
     * using a form of memory allocation known as dynamic memory allocation. This ability to acquire additional memory
     * is part of what makes std::string so flexible, but also comparatively slow.
     */

    // std::string name {"Senya"};
    // name = "Oleks";
    // std::cout << name << std::endl; // Oleks
    // std::cout << name[4] << std::endl; // s
    // std::cout << name[5] << std::endl; // порожній рядок - тобто \0
    //
    // std::string empty {};
    // std::cout << '[' << empty << ']' << std::endl; // []


    // // sizeof в цьому випадку завжди повертатиме 32 не залежно від довжини рядка, бо повертає кількість пам'яті на тип, а не на вміст
    // // А ні  .size(), а ні .length() не рахують null-terminator (\0 символ, який позначає кінець рядка)
    // std::string empty {};
    // std::string one_char {'a'};
    // std::string long_str {"abcdefghijklmnopqrstxyz"}; // 23 симолів
    // std::cout << sizeof(empty) << std::endl; // 32
    // std::cout << empty.size() << std::endl; // 0
    //
    // std::cout << sizeof(one_char)  << std::endl; // 32
    // std::cout << one_char.size()  << std::endl; // 1
    //
    // std::cout << sizeof(long_str)  << std::endl; // 32
    // std::cout << long_str.size()  << std::endl; // 23
    // std::cout << long_str.length()  << std::endl; // 23
    //
    // std::string ukr_char {"ю"};
    // std::cout << std::ssize(ukr_char) << std::endl; // 2
    // std::cout << ukr_char.length() << std::endl; // 2
    // std::cout << ukr_char.size() << std::endl; // 2

    // std::string name {};
    // cin >> name; // oleksandr volkov
    // std::cout << name; // oleksandr - все що розділено пробілом потрапить у інший cin

    // std::string name {};
    // std::getline(std::cin >> std::ws, name);
    // std::cout << name; // oleksandr volkov - отримали весь рядок
    //
    // // Маніпулятор вводу std::ws повідомляє std::cin про необхідність ігнорування будь-яких початкових пробілів перед вилученням.
    // // Початковий пробіл — це будь-який символ пробілу (пробіли, табуляції, символи нового рядка), що зустрічаються на початку рядка.



    /**
     * Ініціалізація std::string є дорогою
     * Кожного разу, коли ініціалізується std::string, створюється копія рядка, що використовується для його ініціалізації.
     * Створення копій рядків є дорогим, тому слід подбати про мінімізацію кількості створюваних копій.
     *
     * Не передавайте std::string за значенням
     * Коли std::string передається до функції за значенням, параметр функції std::string повинен бути інстанційований та ініціалізований з аргументом.
     * Це призводить до дорогого копіювання. Замість цього використовуйте std::string_view
     *
     * У більшості інших випадків краще уникати повернення std::string за значенням, оскільки це призведе до дорогого копіювання.
     * Якщо повертається рядковий літерал у стилі C, використовуйте тип повернення std::string_view.
     * У певних випадках std::string також може повертатися за посиланням (const), що є ще одним способом уникнути копіювання.
     *
     */


     /**
     * Double-quoted string literals (like “Hello, world!”) are C-style strings by default (and thus, have a strange type).
     * We can create string literals with type std::string by using a 's' suffix after the double-quoted string literal. The s must be lower case.
     *
     * The “s” suffix lives in the namespace std::literals::string_literals.
     *
     * "Hello"s resolves to std::string { "Hello", 5 } which creates a temporary std::string initialized with C-style string literal “Hello”
     * (which has a length of 5, excluding the implicit null-terminator).
     *
     */

    // using namespace std::string_literals; // easy access to the s suffix
    //
    // std::cout << "foo\n";   // no suffix is a C-style string literal
    // std::cout << "goo\n"s;  // s suffix is a std::string literal

    //
    // // Наочний приклад чому треба бути обережним з length, оскільки він повертає unsinged int
    // std::cout << "Enter your full name: ";
    // std::string name{};
    // std::getline(std::cin >> std::ws, name); // read a full line of text into name
    //
    // std::cout << "Enter your age: ";
    // int age{}; // age needs to be an integer, not a string, so we can do math with it
    // std::cin >> age;
    //
    // // Тут маємо проблему, оскільки name.length() поверне unsigned int, а age singed int
    // // Тож маємо конвертувати name.length() у singed int
    // int nameLen { static_cast<int>(name.length()) };
    // std::cout << "Your age + length of name is: " << age + nameLen << '\n';

    //---------------------------------------- std::string_view ------------------------------------------

    /**
     * std::string_view
     * 1. Створюється змінна s
     * 2. Свторюється C-style string "Hello, world!"
     * 3. "Hello, world!" копіюється у змінну s (повільний процес)
     * 4. Передаємо s як параметр у printString - отже ще раз копіємо "Hello, world!"
     * 5. Виводимо  "Hello, world!" у термінал
     *
     * Як видно дуже багато копіювань одного рядку (а це повільна операція!)
     *
     * Саме тому варто використати std::string_view
     * std::string_view надає доступ лише для читання до існуючого рядка (рядка в стилі C, std::string або іншого std::string_view) без створення копії.
     *
     */
    // std::string s{ "Hello, world!" }; // s makes a copy of its initializer
    // printString(s);
    //
    //
    // // Робить те саме, але рядок більше не копіюється
    // std::string_view s2{ "Hello, world!" };
    // printSV(s2);



    // // Суфікси для std::string та std::sstring_view
    // using namespace std::string_literals;      // access the s suffix
    // using namespace std::string_view_literals; // access the sv suffix
    //
    // std::cout << "foo\n";   // no suffix is a C-style string literal
    // std::cout << "goo\n"s;  // s suffix is a std::string literal
    // std::cout << "moo\n"sv; // sv suffix is a std::string_view literal


    // // constexpr працює з std::string_view і може бути обчислений під час компіляції
    // constexpr std::string_view s{ "Hello, world!" }; // s is a string symbolic constant
    // std::cout << s << '\n'; // s will be replaced with "Hello, world!" at compile-time


    // // Можлива проблема через залежність std::string_view від джерела даних які показує
    // std::string_view sv{};
    //
    // { // create a nested block
    //     std::string s{ "Hello, world!" }; // create a std::string local to this nested block
    //     sv = s; // sv is now viewing s
    // } // s is destroyed here, so sv is now viewing an invalid string
    //
    // std::cout << sv << '\n'; // undefined behavior (У мене виводиться, але з іншим компілятором або на іншій машині може і видати помилку)

    // // Те саме тут
    // std::string_view name { getName() }; // name initialized with return value of function
    // std::cout << name << '\n'; // undefined behavior ("Alex" але може і видати помилку або нічого не вивести або вивести залишкові дані з алокованої пам'яті)
    //

    // // Інший приклад
    // std::string s { "Hello, world!" };
    // std::string_view sv { s }; // sv is now viewing s
    //
    // s = "Hello, a!";    // modifies s, which invalidates sv (s is still valid)
    // std::cout << sv << '\n';   // undefined behavior ("Hello, a!")


    // // Виправлення цієї помилки
    // std::string s { "Hello, world!" };
    // std::string_view sv { s }; // sv is now viewing s
    //
    // s = "Hello, universe!";    // modifies s, which invalidates sv (s is still valid)
    // std::cout << sv << '\n';   // undefined behavior ("▲       orld!")
    //
    // sv = s;                    // revalidate sv: sv is now viewing s again
    // std::cout << sv << '\n';   // prints "Hello, universe!"



    // // std::string_view дозволяє модифікувати себе, але не джерело!
    // std::string original {"Peach"};
    // std::string_view str{ original };
    // std::cout << str << '\n'; // Peach
    //
    // // Remove 1 character from the left side of the view
    // str.remove_prefix(1);
    // std::cout << str << '\n'; // each
    //
    // // Remove 2 characters from the right side of the view
    // str.remove_suffix(2);
    // std::cout << str << '\n'; // ea
    // std::cout << original << '\n'; // Peach
    //
    // str = "Peach"; // reset the view
    // std::cout << str << '\n'; // Peach



    /**
     * Коротка нотатка по std::sting і std::string_view
     *
     * Use a std::string variable when:
     *      You need a string that you can modify.
     *      You need to store user-inputted text.
     *      You need to store the return value of a function that returns a std::string.
     *
     * Use a std::string_view variable when:
     *      You need read-only access to part or all of a string that already exists elsewhere and will not be modified or
     *      destroyed before use of the std::string_view is complete.
     *      You need a symbolic constant for a C-style string.
     *      You need to continue viewing the return value of a function that returns a C-style string or a non-dangling std::string_view.
     *
     *
     *
     * Function parameters
     *
     * Use a std::string function parameter when:
     *      The function needs to modify the string passed in as an argument without affecting the caller. This is rare.
     *      You are using language standard C++14 or older and aren’t comfortable using references yet.
     *
     * Use a std::string_view function parameter when:
     *      The function needs a read-only string.
     *      The function needs to work with non-null-terminated strings.
     *
     *
     *
     * Return types
     *
     * Use a std::string return type when:
     *      The return value is a std::string local variable or function parameter.
     *      The return value is a function call or operator that returns a std::string by value.
     *
     * Use a std::string_view return type when:
     *      The function returns a C-style string literal or local std::string_view that has been
     *      initialized with a C-style string literal (Тому що C-style string живуть доки працює програма).
     *      The function returns a std::string_view parameter (мається на увазі функція отримала
     *      параметр x: str:sting_view і його ж повернула у return).
     *
     *
     *
     * Things to remember about std::string:
     *       Initializing and copying std::string is expensive, so avoid this as much as possible.
     *       Avoid passing std::string by value, as this makes a copy.
     *       If possible, avoid creating short-lived std::string objects.
     *       Modifying a std::string will invalidate any views to that string.
     *       It is okay to return a local std::string by value.
     *
     *
     * Things to remember about std::string_view:
     *      std::string_view is typically used for passing string function parameters and returning string literals.
     *      Because C-style string literals exist for the entire program, it is always okay to set a std::string_view to a C-style string literal.
     *      When a string is destroyed, all views to that string are invalidated.
     *      Using an invalidated view (other than using assignment to revalidate the view) will cause undefined behavior.
     *      A std::string_view may or may not be null-terminated.
     */

    return 0;
}


// str makes a copy of its initializer
void printString(std::string str){
    std::cout << str << '\n';
}

// str provides read-only access to whatever argument is passed in
void printSV(std::string_view str) {
    std::cout << str << '\n';
}

std::string getName(){
    std::string s { "Alex" };
    return s; // після return s знищиться
}