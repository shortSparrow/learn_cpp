#include <iostream>
#include "IntArray.h"


/**
 * Файл IntArray.h містить реалізацію intArray взяту з https://www.learncpp.com/cpp-tutorial/container-classes/
 * Це довільна реалізація яка містить основні моменти:
 *  1) Алокація пам'яті під масив
 *  2) Перевантаженням оператора присвоєння "=" та конструктора копіювання для коректної роботи з алокованими даними (видалення старих
 *     і алокація пам'яті під нові дані)
 *  3) методи insertBefore, insertAtEnd та insertAtBeginning
 *  4) Прихований метод реалокації даних і метод очистки даних
 */

int main() {
    // Declare an array with 10 elements
    IntArray array(10);

    // Fill the array with numbers 1 through 10
    for (int i{ 0 }; i<10; ++i)
        array[i] = i+1;

    // Resize the array to 8 elements
    array.resize(8);

    // Insert the number 20 before element with index 5
    array.insertBefore(20, 5);

    // Remove the element with index 3
    array.remove(3);

    // Add 30 and 40 to the end and beginning
    array.insertAtEnd(30);
    array.insertAtBeginning(40);

    // A few more tests to ensure copy constructing / assigning arrays
    // doesn't break things
    {
        IntArray b{ array };
        b = array;
        b = b;
        array = array;
    }

    // Print out all the numbers
    for (int i{ 0 }; i<array.getLength(); ++i)
        std::cout << array[i] << ' ';

    std::cout << '\n'; // 40 1 2 3 5 20 6 7 8 30

    return 0;
}