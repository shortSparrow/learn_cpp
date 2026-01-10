#include <iostream>



int main() {

    /**
     * void pointer, також відомий як generic pointer — це спеціальний тип pointer,
     * який може бути спрямований на об'єкти будь-якого типу даних!
     */

    void* ptr_void {}; // ptr is a void pointer


    int nValue {};
    float fValue {};

    struct Something
    {
        int n;
        float f;
    };

    Something sValue {};

    void* ptr {};
    ptr = &nValue; // valid
    ptr = &fValue; // valid
    ptr = &sValue; // valid


    /**
     * Оскільки void pointer може посилатися на будь-який тип і ми не знаємо на який саме,
     * то перед застосуванням цього pointer ми маємо привести його до якогось типу
     */
    int value {5};
    void* voidPtr {&value};
    // std::cout << *voidPtr << '\n'; // illegal: dereference of void pointer
    int* intPtr {static_cast<int*>(voidPtr)};
    std::cout << *intPtr << '\n'; // 5


    // Void pointers can be set to a null value:
    void* ptr2{ nullptr }; // ptr2 is a void pointer that is currently a null pointer


    /**
     * Загалом, гарною ідеєю є уникати використання void pointer, окрім випадків абсолютної необхідності,
     * оскільки вони ефективно дозволяють уникнути перевірки типів.
     */

    return 0;
}