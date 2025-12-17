#include <iostream>
#include <vector>


void printCapLen(const std::vector<int>& v) {
    std::cout << "Capacity: " << v.capacity() << " Length:"	<< v.size() << '\n';
}

int main() {

    /**
     * Both std::array and C-style arrays are fixed-size array types
     * On the other hand, std::vector is a dynamic array. A dynamic array (also called a resizable array)
     *
     *
     * У std::vector є capacity і length:
     *      The length of a vector is how many elements are “in use”.
     *      The capacity of a vector is how many elements have been allocated in memory.
     *
     */

    std::vector v{ 0, 1, 2 }; // length is initially 3

    printCapLen(v); // Capacity: 3 Length:3

    for (auto i : v)
        std::cout << i << ' '; // 0 1 2
    std::cout << '\n';

    v.resize(5); // resize to 5 elements

    printCapLen(v); // Capacity: 6 Length:5

    for (auto i : v)
        std::cout << i << ' '; // 0 1 2 0 0
    std::cout << '\n';


    //......................................
    std::cout << ".................Capacity.................\n";
    std::vector<int> v1(10); // [0,0,0,0,0,0,0,0,0,0] - масив з 10 елементів

    printCapLen(v1); // Capacity: 10 Length:10

    v1.push_back((11));
    printCapLen(v1); // Capacity: 20 Length:11

    /**
     * sizeof(std::vector) завжди поверне 24 (на цій машині), тому що це розмір самого об'єкта,
     * а не елементів що у ньому.
     * Vector містить:
     *      покажчик на start - 8 byte
     *      покажчик на end - 8 byte
     *      покажчик на ємність - 8 byte
     * 8*8*8 = 24
     * А от щоб дзінатися скільки пам'яті за'має вектор треба sizeof(arrayElementType) * array.capacity()
     *
     * std::vector<bool> - це виключення, він має додаткові поля тому його контейнер важить більше (він не є
     * спаржнім контейнером у C++). std::vector<bool> взагалі краще уникати, бо такий код з ним працювати не буде:
     *
     * template<typename T>
     * void foo( std::vector<T>& v ) {
     *      T& first = v[0]; // get a reference to the first element
     * }
     */
    // std::cout << sizeof(std::vector<int> {1,2}); // 24
    // std::cout << sizeof(std::vector<int> {1,2,3}); // 24
    // std::cout << sizeof(std::vector<bool>) << '\n'; // 40 byte

    std::cout << "..............Sizeof....................\n";
    int arr1[2] {1,2};
    int arr2[3] {1,2,3};
    int arr3[3] {};
    std::cout << sizeof(arr1) << '\n'; // 8
    std::cout << sizeof(arr2) << '\n'; // 12
    std::cout << sizeof(arr3) << '\n'; // 12


    /**
     * Shrinking a std::vector
     * Коли ми робимо resizing vector для збільшення то збільнується його length і capacity, однак
     * якщо ми змінуємо вектор через resizing то зменшиться лише його length, а capacity лишиться старим.
     * Однак, якщо у нас був дуже великий масив і ми все таки хочемо вивілнити пам'ять, то треба використати shrink_to_fit
     *
     */
    std::cout << "................Resizing..................\n";
    std::vector<int> v2 {1,2,3};
    printCapLen(v2); // Capacity: 3 Length:3
    v2.resize(6);
    printCapLen(v2);  // Capacity: 6 Length:6
    v2.resize(3);
    printCapLen(v2);  // Capacity: 6 Length:3
    v2.shrink_to_fit();
    printCapLen(v2); // Capacity: 3 Length:3


    //...........................
    /**
     * Я хочу зробити вектор з length 1 і capacity 10. Як це зробити?
     * std::vector v(10) - дасть length 10 і capacity 10 і всі елементи масиву будуть заповнені 0
     *
     * std::vecor v {1};
     * v.resize(10); - дасть length 10 і capacity 10 і всі елементи масиву будуть заповнені 0
     *
     * std::vecor v {1};
     * v.reserve(10) - дасть length 1 і capacity 10
     */
    std::cout << "................Change capacity but not length..................\n";
    std::vector s1 {1};
    s1.resize(10);
    printCapLen(s1); // Capacity: 10 Length: 10
    for (auto i : s1)
        std::cout << i << ' '; // 1 0 0 0 0 0 0 0 0 0
    std::cout << '\n';

    std::vector s2 {1};
    s2.reserve(10); //  reserve space for 10 elements (but do not change length)
    printCapLen(s2); // Capacity: 10 Length: 1
    for (auto i : s2)
        std::cout << i << ' '; // 1
    std::cout << '\n';

    return 0;
}