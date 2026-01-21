#include <iostream>

int main() {

    /**
     * Оператор * коли його застосувати на адресі пам'яті (&some_variable)
     * повертає значення, яке зберігається за цією адресою в пам'яті.
     */

    int x1 {1};

    std::cout << "x: " << x1 << '\n'; // 1
    std::cout << "x address: " << &x1 << '\n'; // 0xb8c39ff9ec
    std::cout << "x value: " << *(&x1) << '\n'; // 1


    /**
     * Pointer - це об'єкт, який містить адресу пам'яті (зазвичай іншої змінної) як своє значення.
     *
     * int;  // a normal int
     * int&; // an lvalue reference to an int value
     * int*; // a pointer to an int value (holds the address of an integer value)
     */

    int x{ 5 };

    int* ptr;        // an uninitialized pointer (holds a garbage address)
    int* ptr2{};     // a null pointer (we'll discuss these in the next lesson)
    int* ptr3{ &x }; // a pointer initialized with the address of variable x. ptr3 holds the address of x
    std::cout << ptr3 << '\n'; // ptr3 містить адресу - 0xa05a9ff9e8
    std::cout << *ptr3 << '\n'; // 5 - use dereference operator (*) to print the value at the address that ptr3 is holding (which is x's address)


    // Важливо звертати увагу на типи даних
    int i{ 5 };
    double d{ 7.0 };
    //
    // int* iPtr{ &i };     // ok: a pointer to an int can point to an int object
    // int* iPtr2 { &d };   // not okay: a pointer to an int can't point to a double object
    // double* dPtr{ &d };  // ok: a pointer to a double can point to a double object
    // double* dPtr2{ &i }; // not okay: a pointer to a double can't point to an int object
    //
    // int* ptr{ 5 }; // not okay
    // int* ptr{ 0x0012FF7C }; // not okay, 0x0012FF7C is treated as an integer literal


    // Зміна значення
    int y {5};
    int* ptr_y {&y};
    std::cout << y << '\n'; // 5
    std::cout << *ptr_y << '\n'; // 5
    *ptr_y = 6;
    std::cout << y << '\n'; // 6
    std::cout << *ptr_y << '\n'; // 6



    /**
     * There are some other differences between pointers and references worth mentioning:
     *
     * References must be initialized, pointers are not required to be initialized (but should be).
     * References are not objects, pointers are.
     * References can not be reseated (changed to reference something else), pointers can change what they are pointing at.
     * References must always be bound to an object, pointers can point to nothing (we’ll see an example of this in the next lesson).
     * References are “safe” (outside of dangling references), pointers are inherently dangerous (we’ll also discuss this in the next lesson).
     *
     */

    // The address-of operator returns a pointer
    int s{ 4 };
    std::cout << typeid(s).name() << '\n';  // i - значить int
    std::cout << typeid(&s).name() << '\n'; // Pi - значить pointer (int*)


    /**
     * The size of pointers
     * Розмір pointer залежить від архітектури машини на якій запускається.
     */

    char* chPtr{};        // chars are 1 byte
    int* iPtr3{};          // ints are usually 4 bytes
    long double* ldPtr{}; // long doubles are usually 8 or 12 bytes

    std::cout << sizeof(chPtr) << '\n'; // 8 (на машині з 32-ох бітною архітектурою мало б бути 4)
    std::cout << sizeof(iPtr3) << '\n';  // 8 (на машині з 32-ох бітною архітектурою мало б бути 4)
    std::cout << sizeof(ldPtr) << '\n'; // 8 (на машині з 32-ох бітною архітектурою мало б бути 4)



    /**
     * Видалення об'єкту, на який вказував pointer. Така поведінка призведе до невизначеної поведінки
     */
    int f {1};
    int* f_pointer = {&f};
    std::cout << "pointer value: " << *f_pointer << '\n'; // 1
    std::cout << "pointer address: " << f_pointer << '\n'; // 0xd0789ff984

    {
        int t {3};
        f_pointer = &t;
        std::cout << "pointer value: " << *f_pointer << '\n'; // 3
        std::cout << "pointer address: " << f_pointer << '\n'; // 0xd0789ff980
    } // видалення t - бо виходимо із скоупа

    // Ось тут вже починається непередбачувана поведінка, яка залежиться від компілятора
    std::cout << "pointer value: " << *f_pointer << '\n'; // 3 - garbage value
    std::cout << "pointer address: " << f_pointer << '\n'; // 0xd0789ff980 - rag badge value

    return 0;
}