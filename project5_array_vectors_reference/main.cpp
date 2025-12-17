#include <array>
#include <iostream>

int main()
{
    int x {1};
    int y {2};

    std::array arr1 {x,y}; // містить значення
    std::array arr2 {&x, &y}; // містить посиланян на змінні

    // arr2[1] = 3; // error Cannot assign to type std::array<int*, 2>::value_type from int
    *(arr2[1]) = 3; // так буде працювати

    std::cout << arr2[1] << '\n'; // 0xa77b3ffba8
    std::cout << *arr2[1] << '\n'; // 3
    std::cout << y << '\n'; // 3


    /**
     * std::reference_wrapper
     * В цьому випадку оператор .get() використовується щоб отримати елемент і мати змогу його змінити
     * в оригінальному місці, звідки походить reference (те що зверху ми робили через pointer *)
     */

    int x1 { 1 };
    int y1 { 2 };
    int z1 { 3 };

    std::array<std::reference_wrapper<int>, 3> arr { x1, y1, z1 };
    std::cout << arr[1].get() << '\n'; // 2 У прикладі вище метода .get() не існує
    std::cout << arr[1] << '\n'; // 2

    // arr[1] = 7; // error
    arr[1].get() = 7; // ok

    std::cout << y1 << '\n'; // 7
    std::cout << arr[1] << '\n'; // 7


    /**
     * std::ref and std::cref
     * Ці функції існували раніше для того щоб зробити те саме що і std::reference_wrapper але пишучи менше тексту
     * і вони працюють з auto (раніше я так розумію auto і std::reference_wrapper не зружили)
     *
     * std::cref робить те саме що і std::ref, але додає const
     */

    int x2 {1};
    int y2 {2};

    auto x2_ref = std::ref(x2);
    auto y2_ref = std::ref(y2);
    std::array arr3 {x2_ref, y2_ref};

    int x3 { 5 };
    auto ref { std::ref(x3) };   // C++11, deduces to std::reference_wrapper<int>
    auto cref { std::cref(x3) }; // C++11, deduces to std::reference_wrapper<const int>



    return 0;
}