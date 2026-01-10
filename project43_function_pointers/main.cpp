#include <functional>
#include <iostream>

// code starts at memory address 0x002717f0
int foo() {
    return 5;
}

int goo(){
    return 6;
}

int baz(int x){
    return x;
}

bool my_callback(int x, int y) {
    return x>y;
}

void some_func(int a, int b, bool (*cb)(int,int)) {
    if (cb(3,4)) {
        std::cout << "YES\n";
    } else {
        std::cout << "NO\n";
    }
}

// Те саме що і some_func, але інакше написано передачу callback (краще використовувати приклад вище, він більш очевидний)
void some_func2(int a, int b, bool cb(int,int)) {
    if (cb(3,4)) {
        std::cout << "YES\n";
    } else {
        std::cout << "NO\n";
    }
}


// Providing default callback functions
void some_func3(int a, int cb(int) = baz) {

}

// Варто використовувати alias, так код буде виглядати гарніше.
using ValidateFunction = bool(*)(int, int);
void some_func4(int a, int b, ValidateFunction cb) { // Те саме що і some_func чи some_func2, але набагато чистіше

}

/**
 * Ми знаємо як у пам'яті зберігаються int - це тип і виділена пам'ять під 8 біт, так само ми
 * знаємо як зберігається C-style array - це непереривний блок виділеної пам'яті і сам array є
 * pointer на першу комірку і array простягається від цього pointer аж до \0 знаку, що говорить, що масив закінчився.
 *
 * З Function Pointer схожа ситуація. Коли ми створили код і зкомпілювали його він поміщається у пам'ять
 * і функція - це pointer на комірку пам'яті з якої починаються інструкції для процесора.
 * Уяви функцію sum(int a, int b):
 *  Вона починається з певної адреси (наприклад, 0x401000).
 *  Далі йдуть байти (команди), які кажуть процесору: "Візьми значення з регістра А, додай до регістра Б...".
 *  Закінчується функція спеціальною інструкцією RET (Return), яка каже процесору повернутися туди,
 *  звідки його викликали.
 * Тож ім'я функції власне і є цей pointer і коли ми робимо виклик () то це команда для виконання
 * операцій які ідуть після комірки з адресою.
 *
 * Але є цікавинка, хоч ім'я функції це просто pointer (адреса) у пам'яті комп'ютера, але
 * оператор << не знає як її вивести, тож якщо просто вивести
 * std::cout << func
 * То ми отримаємо true - тобто 1 якщо не стоїть спеціальної умови і pointer не дорівнює nullptr, тоді буде 0 (false)
 *
 */
int main() {
    std::cout << foo << '\n'; // 1 (тобто true)
    std::cout << reinterpret_cast<void*>(foo) << '\n'; // Tell C++ to interpret function foo as a void pointer (implementation-defined behavior)


    // Pointers to functions
    // int (*fcnPtr)(); // non const function pointer
    // int (*const fcnPtrConst)(); // const function pointer

    // std::function<int()> fcnPtr{ &foo }; // non const function pointer але запис через std::function - суть та сама. просто інший запис


    // Assigning a function to a function pointer
    int (*funcPointer)() {foo}; // funcPointer - це pointer на функцію foo

    std::cout << funcPointer() << '\n'; // 5
    funcPointer = &goo;
    std::cout << funcPointer() << '\n'; // 6

    // function pointer with arguments
    int (*funcPointer2)(int) { &baz};
    std::cout << funcPointer2(14) << '\n';

    std::function<int()> funPointerV2{&foo}; // просто запис function pointer через std::function
    std::function<int(int)> funPointerV3{&baz}; // просто запис function pointer через std::function

    /**
     * Варто пам'ятати, що оськільки function pointers - це pointer то вони можуть приймати
     * значення nullptr, тому варто робити перевірку перед викликом
     */
    funcPointer2 = nullptr;
    if (funcPointer2){
        std::cout << funcPointer2(24) << '\n';
    }


    /**
     * Default arguments don’t work for functions called through function pointers
     *
     * Коли компілятор зустрічає звичайний виклик функції з одним або декількома аргументами за замовчуванням,
     * він переписує виклик функції, щоб включити аргументи за замовчуванням. Цей процес відбувається під час компіляції,
     * тому може застосовуватися тільки до функцій, які можна вирішити (resolved) під час компіляції.
     *
     * Однак, коли функція викликається через вказівник на функцію, вона вирішується (resolved) під час виконання. У цьому випадку не
     * відбувається перезаписування виклику функції для включення аргументів за замовчуванням
     */


    // Одна з найкорисніших речей, які можна зробити з вказівниками на функції, це передавати функцію як аргумент іншій функції

    some_func(1,2, my_callback);
    some_func2(1,2, my_callback);



    /**
     * Висновок
     *
     * Function pointers корисні, перш за все, коли потрібно зберігати функції в масиві (або іншій структурі),
     * або коли потрібно передати функцію іншій функції (callback)
     *
     */


    return 0;
}