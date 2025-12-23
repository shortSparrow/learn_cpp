#include <iostream>

/**
 * C-style decay array
 * Цей механізм прийшов до нас з C (власне разом з C-style array) і він корисний
 * тим що заміняє reference у передачі параметра.
 *
 * Основа суть decay C-style array в тому, аби передати посилання на масив, а не робити
 * дороговартісну копію при передачі як аргумент у функцію. Оскільки у C немає механізму reference,
 * то це єдиний варіант.
 *
 * Тож, хоча по синтаксису виглядає так, ніби ми передаємо C-style array by value, насправді ми передаємо його by address!
 *
 * Тепер розглянемо два різні масиви з однаковим типом елементів, але різної довжини (наприклад, int[5] та int[7]).
 * Це різні типи, несумісні один з одним. Однак, вони обидва розпадаються на один і той самий тип вказівника (наприклад, int*).
 * Їхні розкладені версії взаємозамінні!
 *
 * Висновок: Два  C-style arrays з однаковим типом елементів, але різною довжиною, розпадуться на один і той самий тип вказівника.
 */

// Але такий варіант не очевидний, що ми передаємо C-style array
void print_element_zero(const int* ptr) { // pass by const address
    std::cout << ptr[0] << '\n';
}

// Так вже очевидніше що ми передаємо C-style array. Але тепер менш очевидно що це decay array
void print_element_zero_v2(const int arr[]) { // the same as const int*
    std::cout << arr[0] << '\n';
}

// Ми можемо передати length як окремий аргумент
void print_element_three(const int arr[], const int length) {
    if (length > 3){
        std::cout << arr[3] << '\n';
    }
}

void print_array_size(int arr[]) {
    std::cout << sizeof(arr) << '\n';
}


int main() {
    /**
     * У більшості випадків, коли у виразі використовується C-stylle array, масив буде неявно
     * перетворено на вказівник (pointer) на тип елемента, І цей pointer буде ініціалізований
     * адресою першого елемента (з індексом 0). Доведемо це
     *
     */

    int arr[5] {9,7,5,3,1}; // our array has elements of type int

    // First, let's prove that arr decays into an int* pointer
    auto ptr {arr}; // Мала б бути копія масиву, але ніт
    std::cout << std::boolalpha << (typeid(ptr) == typeid(int*)) << '\n'; // true. Тобто тип ptr - це int*

    // Now let's prove that the pointer holds the address of the first element of the array
    std::cout << std::boolalpha << (&arr[0] == ptr) << '\n'; // true

    /**
     * In C++, there are a few common cases where an C-style array doesn’t decay:
     *  1. When used as an argument to sizeof() or typeid().
     *  2. When taking the address of the array using operator&.
     *  3. When passed as a member of a class type.
     *  4. When passed by reference.
     *
     * Тому що C-style arrays розпажаються (decay) на pointer у більшості випадків,
     * поширеною помилкою є думка, що масиви є вказівниками.
     *
     * Важливо, що масив типу int[5] містить інформацію про довжину (length), тоді як вказівник (pointer)
     * на розкладений масив типу int* цього не містить (бо це ж pointer)
     *     int arr_size = std::ssize(arr); // length 5
     *     std::ssize(ptr); // error
     */

    auto d {arr[2]};
    std::cout << arr[2]; // prints 52. subscript decayed array to get element 2, prints 5


    /**
     * У наступному прикладі ми проілюструємо дві речі:
     *  1. Що ми можемо передавати масиви різної довжини одній функції (оскільки обидві зводяться до одного типу вказівника).
     *  2. Що параметр нашої функції, який отримує масив, може бути (константним) вказівником типу елемента масиву.
     */


    const int prime[] { 2, 3, 5, 7, 11 };
    const int squares[] { 1, 4, 9, 25, 36, 49, 64, 81 };

    print_element_zero(prime);   // 2. prime decays to an const int* pointer
    print_element_zero_v2(squares); // 1. squares decays to an const int* pointer
    print_element_three(prime, std::ssize(prime)); // 7



    std::cout << "**************************************** sizeof **************************" << '\n';
    /**
     * Хоча розпад (decay) масиву був розумним рішенням для забезпечення можливості передачі
     * C-style масивів різної довжини без створення дороговартісних копій, але втрата інформації про довжину
     * масиву спрощує різні типи помилок.
     *
     *  1. sizeof() will return different values for arrays and decayed arrays
     */

    int arr1[]{ 3, 2, 1 };
    std::cout << sizeof(arr1) << '\n'; // prints 12 (assuming 4 byte ints)
    print_array_size(arr1); // prints 8. На моїй машині розмір адреси 8 byte (нижче наведено доказ)

    int x {1};
    std::cout << sizeof(&x) << '\n'; // 8


    /**
     * Висновок
     * C-style arrays варто уникати в більшості випадків, бо хоч вони і даються змогу передачати
     * "посилання", але не передають розміру масиву коли розпадаються (decay), тож це ускоаднює з ними
     * роботу.
     */

    return 0;
}