#include <array>
#include <iostream>
#include <vector>

/**
 * Оскільки тип (int, double, ..) і length є частиною типу std::array, то ми повинні
 * вказувати їх коли передаємо масив як reference, це видно у функції printArr.
 *
 * Але як же тоді зробити template з передачею масиву як reference
 */

void printArr(const std::array<int, 5>& arr){
    for (const auto& item : arr) {
        std::cout << item << ' ';
    }
}

template <typename T, std::size_t N> // note that this template parameter declaration matches the one for std::array
void passByRef(const std::array<T, N>& arr) {
    std::cout << arr[0] << '\n';
}

// Те саме що і зверху, але тип не динамічний, а статичний (int)
template <std::size_t N> // note: only the length has been templated here
void passByRef2(const std::array<int, N>& arr) {
    std::cout << arr[0] << '\n';
}


int main() {

    /**
     * Навідміну від std::vector std::array не може змінювати свою довжину і завжди може
     * бути constexpr (std::vector може бути constexpr у дуже обмеженому числі випадків)
     *
     * Use std::array for constexpr arrays, and std::vector for non-constexpr arrays.
     *
     */


    std::array<int, 5> arr {}; // array of 5 int (всі 5 елементів будуть 0)
    std::vector<int> arr2(5);    // Те саме, що і arr
    std::cout << arr[0] << '\n'; // 0

    std::array<int, 4> arr3 { 1, 2 }; // Ті елементи, які не були задані будуть 0 - 1 2 0 0

    // Можливі варіанти запису
    std::array a {1,2,3};
    std::array<double,3> b {1.1,2.2,3.3};
    // std::array<double> c {1.1,2.2,3.3} // error;
    // std::array<3> d {1.1,2.2,3.3} // error;

    /**
     * Для випадків, коли ми не можемо точно визначити тип (нехай буде short оскільки він не має префікса)
     * можна використати std::to_array, але це значно важча операція ніж просто std::array тому варто використовувати
     * лише для обмежених випадків, як у прикладі
     */
    constexpr auto shortArray { std::to_array<short>({ 9, 7, 5, 3, 1 }) };



    std::cout << "length: " << arr.size() << '\n'; // 5 - returns length as type `size_type` (alias for `std::size_t`)
    //  std::ssize() працює так само як і для std::vector


    constexpr std::array<int, 5> af {1,2,3,4,5};
    printArr(af); // 1 2 3 4 5


    /**
     * std::get() does compile-time bounds checking for constexpr indices
     * Оскільки std::array визначається на етапі компіляції (бо не може змінити свою довжину) то метод
     * std::get() виконує bounds checking, тобто якщо довжина масиву 5 а ми беремо 6-ий елмент то подачимо помилку
     * ще на етапі компіляції (або навіть до цього, коли лише написали код)
     */

    constexpr std::array prime{ 2, 3, 5, 7, 11 };
    std::cout << std::get<3>(prime); // print the value of element with index 3
    // std::cout << std::get<9>(prime); // invalid index (compile error)




    /**
     * Якщо масив зберігається неперервно, то як зберігається багатовимірний масив?
     *
     * col 0   col 1   col 2   col 3   col 4
     * [0][0]  [0][1]  [0][2]  [0][3]  [0][4]  row 0
     * [1][0]  [1][1]  [1][2]  [1][3]  [1][4]  row 1
     * [2][0]  [2][1]  [2][2]  [2][3]  [2][4]  row 2
     *
     * C++ використовує порядок за основними рядками, де елементи послідовно розміщуються
     * в пам'яті рядок за рядком, упорядковані зліва направо, зверху вниз
     *
     * [0][0] [0][1] [0][2] [0][3] [0][4] [1][0] [1][1] [1][2] [1][3] [1][4] [2][0] [2][1] [2][2] [2][3] [2][4]
     *
     * Інші мови програмування можуть зберігати по іншому. Наприклад іти по колонкам зверху до низу і потім переходити до іншої колонки
     *
     * Нижче показано на прикладі C-style array, але суть та сама
     */


        constexpr int numRows{ 10 };
        constexpr int numCols{ 10 };

        int product[numRows][numCols]{}; // Declare a 10x10 array

        for (std::size_t row{ 0 }; row < numRows; ++row) {
            for (std::size_t col{ 0 }; col < numCols; ++col) {
                std::cout << product[row][col] << '\t';
            }

            std::cout << '\n';
        }

    /**
     * Як бачимо, як і з одновимірними масиваи, при створенні масив масивів запоанений деолтним для int значенням
     *  0       0       0       0       0       0       0       0       0       0
     *  0       0       0       0       0       0       0       0       0       0
     *  0       0       0       0       0       0       0       0       0       0
     *  0       0       0       0       0       0       0       0       0       0
     *  0       0       0       0       0       0       0       0       0       0
     *  0       0       0       0       0       0       0       0       0       0
     *  0       0       0       0       0       0       0       0       0       0
     *  0       0       0       0       0       0       0       0       0       0
     *  0       0       0       0       0       0       0       0       0       0
     *  0       0       0       0       0       0       0       0       0       0
     */
    return 0;
}