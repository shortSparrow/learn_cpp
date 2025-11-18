#include <iostream>

int main() {
    const int x {1};
    const int* ptr {&x};

    // *ptr = 5; // не можу зробити, бо ptr const тобто я не можу змінити значення.
    std::cout << ptr << '\n'; // 0xe96c3ff8b4
    std::cout << *ptr << '\n'; // 1

    const int y {6};
    ptr = &y; // так зробити можу і тепер pointer буде вказувати на y

    std::cout << ptr << '\n'; // 0xe96c3ff8b0
    std::cout << *ptr << '\n'; // 6

    int z {33};
    ptr = &z; // теж ок, не зважаючи на те, що z не const


    // В цьому випадку ми робимо const pointer, тобто його не можна буде змінити після створення
    int s{ 5 };
    int* const ptr_s { &s }; // const after the asterisk means this is a const pointer
    // ptr_s = &x; // помилка
    // але що цікаво ми можемо змінити значення const pointer
    *ptr_s = 6;
    std::cout << s << '\n'; // 6
    std::cout << *ptr_s << '\n'; // 6


    /**
     * І відповідно зробити const pointer const можна так
     * Тож у нас виходить і значення буде const і сам pointer буде const, тобто
     * ні змінити значення ні змінити адресу пам'яті буде не можливо
     */
    int value { 5 };
    const int* const ptr_v { &value }; // a const pointer to a const value
    // *ptr_v = 3; // error
    // ptr_v = &s; // error



    // Короткий підсумок
    int v{ 5 };

    int* ptr0 { &v };             // points to an "int" but is not const itself.  We can modify the value or the address.
    const int* ptr1 { &v };       // points to a "const int" but is not const itself.  We can only modify the address.
    int* const ptr2 { &v };       // points to an "int" and is const itself.   We can only modify the value.
    const int* const ptr3 { &v }; // points to a "const int" and is const itself.  We can't modify the value nor the address.

    // if the const is on the left side of the *, the const belongs to the value
    // if the const is on the right side of the *, the const belongs to the pointer


    return 0;
}