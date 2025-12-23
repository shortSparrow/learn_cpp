#include <iostream>


void printArray(const int* begin, const int* end) {
    // iterate from begin up to (but excluding) end
    for (; begin != end; ++begin) {
        std::cout << *begin << ' '; // dereference our loop variable to get the current element
    }

    std::cout << '\n';
}


int main() {
    int arr[] { 3, 2, 1 };
    int* ptr_0 {&arr[3]};
    std::cout << ptr_0 << '\n'; // 0xf3163ff7e4
    std::cout << ptr_0 + 1 << '\n'; // 0xf3163ff7e8. Як бачимо, коли ми додали 1 то наша адреса збільшилася на 4 byte. Це тому, що pointer посилається на int, а на цій машині один int займає 4 byte


    std::cout << "**************************** how works arr[n] *********************" << '\n';

    // Спочатку використаємо індексацію, щоб отримати адресу та значення елементів нашого масиву
    std::cout << &arr[0] << ' ' << &arr[1] << ' ' << &arr[2] << '\n';
    std::cout << arr[0] << ' ' << arr[1] << ' ' << arr[2] << '\n';

    // Тепер зробимо еквівалент, використовуючи арифметику вказівників
    std::cout << arr << ' ' << (arr + 1) << ' ' << (arr + 2) << '\n';
    std::cout << *arr << ' ' << *(arr + 1) << ' ' << *(arr + 2) << '\n';

    /**
     * 0xf3163ff7e4 0xf3163ff7e8 0xf3163ff7ec
     * 3 2 1
     * 0xf3163ff7e4 0xf3163ff7e8 0xf3163ff7ec
     * 3 2 1
     *
     *
     * Компілятор петерворює ptr[n] в *((ptr) + (n))
     * Оскільки елементи масиву завжди розташовані послідовно в пам'яті, якщо arr є покажчиком
     * на елемент 0 масиву, *(arr + n) поверне n-й елемент масиву.
     * Наприклад при arr 0xf3163ff7e4:
     *  перший елемент тмасиву *(arr + 0) = 0xf3163ff7e4 + 0 = 0xf3163ff7e4
     *  третій елемент масиву *(arr + 2) 0xf3163ff7e4 + 2 (оскільки мова про int то 2 - це два int, а отже 8 byte) = 0xf3163ff7ec
     *
     *
     * Це є основною причиною, чому масиви починаються з 0, а не з 1. Це робить обчислення більш ефективними
     * (оскільки компілятор не повинен віднімати 1 при індексації)!
     */



    /**
     * Наочна ілюстарція, що зазвчиай pointer показує на перший елемент масиву
     * і тому pointer + n дає нам потрібний результат, але pointer можна змінити, щоб він
     * посилався не на перший елемент
     *
     */
    std::cout << "************************************" << '\n';
    const int arr2[] { 9, 8, 7, 6, 5 };
    const int *ptr { arr2 }; // arr2 decays into a pointer to element 0

    // Покажемо, що ptr посилається на елемент з індексом 0
    std::cout << *ptr << ' ' <<  ptr[0] << '\n'; // prints 9 9
    // Покажемо, що ptr[1] це елемент з індексом 1
    std::cout << *(ptr+1) << ' ' << ptr[1] << '\n'; // prints 8 8

    // Тепер змінимо ptr, щоб він посилався на елемент з індексом 3
    ptr = &arr2[3];

    // Покажемо, що ptr посилається на елемент з індексом 3
    std::cout << *ptr << ' ' << ptr[0] << '\n'; // prints 6 6
    // Покажемо, що тепер ptr[1] це елемент з індексом 4!
    std::cout << *(ptr+1) << ' ' << ptr[1] << '\n'; // prints 5 5


    /**
     * Цікавий факт, що для C-style arrya від'ємний індекс це цілком собі нормальна річ, бо
     * arr[-1] перевтриться у *(ptr -1) і поверне попередній об'єкт у пам'яті
     */
    std::cout << "************************** arr[-1] *******************" << '\n';
    const int arr3[] { 9, 8, 7, 6, 5 };
    const int* ptr3 {&arr3[3]}; // Нехай pointer тепер посилається не на перший елемент масиву (як за замовченням), а на третій

    std::cout << *ptr3 << ' ' << ptr3[0] << '\n'; // prints 6 6
    std::cout << *(ptr3-1) << ' ' << ptr3[-1] << '\n'; // prints 7 7


    std::cout << "************************** Iterate C-style array *******************" << '\n';
    /**
     * Оскільки при розпаді (decay) масиву він втрачає інформацію про дожину масиву, то варіант або
     * передати довжину як аргумент, або передати pointer початку і кінця і ітеруватися по ним.
     * Тут представлено приклад ітерації по pointer
     *
     */
    constexpr int arr4[]{ 9, 7, 5, 3, 1 };

    const int* begin{ arr4 };                // begin points to start element
    const int* end{ arr4 + std::size(arr4) }; // end points to one-past-the-end element

    printArray(begin, end); // 9 7 5 3 1


    /**
     * Range-based for loops over C-style arrays are implemented using pointer arithmetic
     */

    for (auto e : arr4) {   // iterate from `begin` up to (but excluding) `end`
        std::cout << e << ' '; // dereference our loop variable to get the current element
    }






    return 0;
}