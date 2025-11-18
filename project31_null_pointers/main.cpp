#include <iostream>
#include <cstddef> // for NULL

/**
 * null pointer - це pointer який не містить у собі значення, тобто
 * містить null
 */
int main() {
    /**
     * Способи задати null pointer
     * У сучасних архітектурах адреса 0 зазвичай використовується для представлення null pointer.
     * Однак це значення не гарантується стандартом C++, і деякі архітектури використовують інші значення.
     * Літерал 0, коли він використовується в контексті null pointer, буде перетворений на будь-яку адресу,
     * яку архітектура використовує для представлення null pointer.
     *
     * 0 and NULL should be avoided in modern C++ (use nullptr instead).
     */
    int* ptr {}; // ptr is now a null pointer, and is not holding an address
    int* ptr2 { nullptr }; // can use nullptr to initialize a pointer to be a null pointer
    std::cout << (ptr == ptr2) << '\n'; // 1 - means true

    float* pt3 { 0 };  // ptr is now a null pointer (for example only, don't do this)

    float* ptr4; // ptr2 is uninitialized
    ptr4 = 0; // ptr2 is now a null pointer (for example only, don't do this)

    //  Існує макрос препроцесора з назвою NULL (визначений у заголовку <cstddef>). Цей макрос успадкований від C,
    //  е він зазвичай використовується для позначення нульового вказівника.
    double* ptr5 { NULL }; // ptr is a null pointer





    int x {3};
    int* ptr_x {&x};
    std::cout << "ptr_x address: " << ptr_x << '\n'; // 0x34dbff714
    std::cout << "ptr_x value: " << *ptr_x << '\n'; // 3
    ptr_x = nullptr;
    std::cout << "ptr_x address: " << ptr_x << '\n'; // 0
    std::cout << "ptr_x value: " << *ptr_x << '\n'; // порожньо, нічого не виводить у консоль


    /**
     * Варто пам'ятати що якщо ми будесо використовувати pointer, який null то це може призвести до помилки.
     * Тому варто перевіряти, завдяки автоматичному приведенню типів це не складно, бо у null pointer вказівник
     * на комірку пам'яті буде 0 - тобто false
     */
    int x1 { 5 };
    int* ptr1 { &x1 };

    // pointers convert to Boolean false if they are null, and Boolean true if they are non-null
    if (ptr1) // implicit conversion to Boolean
        std::cout << "ptr is non-null\n";
    else
        std::cout << "ptr is null\n";


    /**
     * ВАЖЛИВО
     * Коли об'єкт знищується, то розробник ПОВИНЕН подбати, щоб всі pointers які на нього вказували стали nullptr
     */

    return 0;
}