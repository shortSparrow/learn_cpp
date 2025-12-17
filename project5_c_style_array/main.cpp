#include <array>
#include <iostream>
#include <vector>

int main() {
    int arr1[5];    // Members default initialized int elements are left uninitialized)
    int arr2[5] {}; // Members value initialized (int elements are zero uninitialized) (preferred)
    const int prime2[] { 2, 3, 5, 7, 11 }; // ми можемо опустити length і це не буде помилкою. Ми отримаємо масив з 5 елементів
    // prime[0] = 17; // compile error: can't change const int


    int test_score[30] {}; // сторює масив з 30 елементами, які заповнені дефолтним значенням - 0
    std::cout << test_score[0] << '\n'; // 0
    test_score[0] = 22;

    /**
     * Їхня довжина так само як і у std::array, std::vector має тип std::size_t
     * Але от тип індексb arr[i] навіміну від інших контейнерів у c-style array можуть бути
     * як signed int, так і unsigned int
     */

    std::cout << "****************** Цикли ***********\n";
    const int arr[] { 9, 8, 7, 6, 5 };
    int s { 2 };
    std::cout << arr[s] << '\n'; // okay to use signed index

    unsigned int u { 2 };
    std::cout << arr[u] << '\n'; // okay to use unsigned index


    int fibonnaci[6] {0, 1,1,2,3,5};

    for (int item: fibonnaci) {
        std::cout << item << ' ';
    }
    std::cout << '\n';

    // Хоча std::size() повертає тип std::size_t тут автоматичне приведення до int схоже працює.
    // Імовірно, тому що немає обмеження на unsigned int, як у std::array
    for (int i=0; i<std::size(fibonnaci); i++) {
        std::cout << fibonnaci[i] << ' ';
    }
    std::cout << '\n';


    /**
     * The sizeof a C-style array
     */

    std::cout << "****************** Size of ***********\n";

    const int prime3[] {1,2,3,5,7};
    std::cout << sizeof(prime3) << '\n'; // 20. кожен int на цій машині - 4 byte => 4*5 = 20

    std::array<int,5> prime4 {1,2,3,5,7}; // З std::array так само
    std::cout << sizeof(prime4) << '\n'; // 20. кожен int на цій машині - 4 byte => 4*5 = 20

    std::vector<int> prime5 {1,2,3,5,7};
    std::cout << sizeof(prime5) << '\n'; // 24. З std::vector так не працює, повертається розмір типу std::vector
    std::cout << prime5.size() * sizeof(int) << '\n'; // 20. Треба довжину вектора помножити на розмі типу який він містить


    std::cout << "****************** Length ***********\n";
    // Getting the length of a C-style array
    const int prime6[] { 2, 3, 5, 7, 11 };   // the compiler will deduce prime to have length 5

    std::cout << std::size(prime6) << '\n';  // C++17, returns unsigned integral value 5
    std::cout << std::ssize(prime6) << '\n'; // C++20, returns signed integral value 5



    /**
     * C-style arrays don’t support assignment
     * Неочікувано, але C-style arrays не можуть прийняти нове значення після створення, тобто
     * елементи в них ми можемо міняти, а от замінити повінстю масив (навіть якшо буде однакова довжина) не можемо.
     */
    int arr_x[] {1,2,3};
    arr_x[0] = 4;
    // arr_x = {5,6,7}; // error

    // Якщо треба зробити копію, то можна використати std::copy
    int arr33[] { 1, 2, 3 };
    int src[] { 5, 6, 7 };

    // Copy src into arr
    std::cout << "******************* Copy ***********" << '\n';
    std::copy(std::begin(src), std::end(src), std::begin(arr33));

    // опис того як парцює std::copy
    std::cout << &src << '\n'; // 0x2a7fff81c
    std::cout << std::begin(src) << '\n'; // 0x2a7fff81c. Як бачимо, це просто адреса масиву src

    std::cout << &src[2] << '\n'; // 0xddd77ffac4
    std::cout << std::end(src) << '\n'; // 0xddd77ffac8. Як бачимо, це наступна адреса яка буде після останнього елемента у src (як раз на 4 байти більша за останній елемент)

    /**
     * Ось як працює std::copy під капотом
     * first - це std::begin(src)
     * last - std::end(src)
     * d_first: Ітератор на початок цільового контейнера (std::begin(arr33)).
     *
     * while (first != last) {
     *  *d_first = *first;
     *  ++first;
     *  ++d_first;
     * }
     *
     * Важливе зауваження: ++first збільшить адерсу не на 1, а на 4, бо воно знає, що це int і він займає 4 byte
     */

    std::cout << "**************** Приклад збільшення pointer *************\n";
    int* ptr = &src[2];
    std::cout << ptr << '\n'; // 0x4f46dff504
    ++ptr;
    std::cout << ptr << '\n'; // 0x4f46dff508

    return 0;
}
