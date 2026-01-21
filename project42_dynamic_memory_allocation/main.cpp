#include <iostream>

void doSomething() {
    int* ptr{ new int{} };
}

int main() {
    new int; // dynamically allocate an integer (and discard the result)

    int* ptr{ new int }; // dynamically allocate an integer and assign the address to ptr so we can access it later
    *ptr = 7; // assign value of 7 to allocated memory


    // Initializing a dynamically allocated variable
    int* ptr1{ new int (5) }; // use direct initialization
    int* ptr2{ new int { 6 } }; // use uniform initialization

    delete ptr1; // повернути операційній системі пам'ять, на яку вказує ptr
    ptr1 = nullptr; // set ptr to be a null pointer

    /**
     * Оператор delete насправді нічого не видаляє. Він просто повертає пам'ять, на яку вказує оператор,
     * назад до операційної системи. Після цього операційна система може перепризначити цю пам'ять
     * іншій програмі (або знову цій програмі пізніше).
     *
     * Але після delete значення швидше за все все ще буде у тій комірці пам'яті куди ви його записали,
     * але це не факт, бо ця пам'ять могла вже бути перезаписана, або алокована іншою програмою.
     *
     *  In the context of dynamic memory allocation, a null pointer basically says “no memory has been
     *  allocated to this pointer”.
     *
     *  ptr1 = nullptr - потрібен тільки для того, щоб ми в програмі бачили що цей ptr1 ні на щл не
     *  посилається. Тобто це треба не системі, воно для системи нічого не робить, це треба для нас,
     *  щоб ми бачили що цю змінну було очищено, щоб ми випадково не взяли якесь старе значення.
     *
     * Best practice
     * Ставити deleted pointers значення nullptr, окрім випадків, коли ми одразу виводимо зі скоупа
     * і змінні очищаються (коли виходимо за {})
     */


    /**
     * Operator new can fail
     * Під час запиту пам'яті у операційної системи, у рідкісних випадках, операційна система може не
     * мати пам'яті для виконання запиту. Якщо це станеться то викличеться exception.
     *
     * Але можуть бути випадки, коли це не бажано, тому є можливість додати std::nothrow.
     * Якщо функція new не зможе виділити пам'ять, вона поверне null pointer замість адреси виділеної пам'яті
     *
     */

    int* value { new (std::nothrow) int }; // value will be set to a null pointer if the integer allocation fails
    if (!value) {
        std::cerr << "Could not allocate memory\n";
    }

    /**
     * Динамічно виділена пам'ять залишається виділеною, доки вона не буде явно звільнена або доки програма
     * не завершиться (і операційна система не очистить її, якщо ваша операційна система це зробить).
     */


    /**
     * Memory leaks
     * функція doSomething призведе до memory leaks, тому що воно динамічно алокує пам'ять і не викличе
     * delete, тож функція закінчить своє виконання і ми втратимо зв'язок з посиланням на алокований шматочок
     * пам'яті, і це буде у такому 'підвішеному' стані, доки не закриємо програму. Тоді система сама очистить
     * всю алоковану пам'ять.
     *
     * Також прикладом memory leaks є приклад нижче, бо ми після алокації пам'яті і записуванням
     * її адреси до ptr3 даємо ptr3 нову адресу, і відповідна стара втрачається і ця алокована пам'ять
     * буде висіти до закриття програми
     */

    int value3 = 5;
    int* ptr3{ new int{} }; // allocate memory
    ptr3 = &value3; // old address lost, memory leak results


    // ще один приклад memory leak
    int* ptr4{ new int{} };
    ptr4 = new int{}; // old address lost, memory leak results



    /**
     * Dynamically allocate C-style array
     * Оскільки C-style array і просто std::array мають фіксовану довжину і вона повинна бути
     * визначена наперед, то ми не можемо спочатку отримати значення, і потім вирішити яку
     * довжину масиву хочемо мати.
     * Так не спрацює
     * int length {};
     * std::cin >> length;
     * char my_aray[length] {'a','b',...}
     *
     * Саме тому є динамічна алокація масиву
     *
     * У сучасному C++ краще використовувати std::vector
     */

    std::cout << "Enter a positive integer: ";
    std::size_t length{};
    std::cin >> length;

    int* array {new int[length]{}};  // use array new.  Note that length does not need to be constant!
    std::cout << "I just allocated an array of integers of length " << length << '\n';
    array[0] = 5; // set element 0 to value 5
    delete[] array; // use array delete to deallocate array. Тут важливо що саме delete[], а не просто delete
    // we don't need to set array to nullptr/0 here because it's going out of scope immediately after this anyway



    return 0;
}