#include <algorithm>
#include <array>
#include <iostream>
#include <vector>

// Our function will return true if the element matches
bool containsNut(std::string_view str) {
    // std::string_view::find returns std::string_view::npos if it doesn't find
    // the substring. Otherwise it returns the index where the substring occurs
    // in str.
    return str.find("nut") != std::string_view::npos;
}

bool grater(int a, int b) {
    return (a > b);
}

void multiply_by_2(int& x) {
    x *= 2;
}

int main() {
    // swap
    int x{ 2 };
    int y{ 4 };
    std::cout << "Before swap: x = " << x << ", y = " << y << '\n'; // x = 2, y = 4
    std::swap(x, y); // swap the values of x and y
    std::cout << "After swap:  x = " << x << ", y = " << y << '\n'; // x = 4, y = 2

    std::vector v0 {1,2};
    std::swap(v0[0], v0[1]);
    std::cout << "After swap: " << v0[0] << ", " << v0[1] << '\n'; // 2, 1



    // sort
    std::vector<int> nums {5,7,25,14,3,7,0,3};
    std::sort(nums.begin(), nums.end()); // Сортуємо весь вектор 0 3 3 5 7 7 14 25
    std::sort(nums.begin(), nums.end(), grater); // Сортуємо весь вектор у зворотньому порядку 25 14 7 7 5 3 3 0




    // find/find_if
    auto found_value = std::find(nums.begin(), nums.end(), 25); // Шукаємо число 25
    // Algorithms that don't find what they were looking for return the end iterator.
    // We can access it by using the end() member function.
    if (found_value == nums.end()) {
        std::cout << "Could not find " << 25 << '\n';
    }

    /**
     * std::find_if
     * Іноді ми хочемо перевірити, чи є в контейнері значення, яке відповідає певній умові
     * (наприклад, рядок, що містить певний підрядок), а не точне значення. У таких випадках std::find_if
     * є ідеальним рішенням.
     *
     * The std::find_if function works similarly to std::find, but instead of passing in a specific value to search
     */

    std::array<std::string_view, 4> fruits{ "apple", "banana", "walnut", "lemon" };
    auto found_if_value {std::find_if(fruits.begin(), fruits.end(), containsNut)};
    if (found_if_value == fruits.end()) {
        std::cout << "No founds in fruit\n";
    }




    // std::count and std::count_if
    /**
     * std::count і std::count_if підраховують скільки разів щоб зустрічається у ітеруємому об'єкті
     */

    auto count_3 {std::count(nums.begin(), nums.end(), 3)};
    std::cout << "Counted 3: " << count_3 << " times \n"; // 2 times

    auto count_nut {std::count_if(fruits.begin(), fruits.end(), containsNut)};
    std::cout << "Counted nut substring: " << count_nut << " times\n"; // 1 times




    // std::for_each
    std::array arr{ 1, 2, 3, 4 };
    std::for_each(arr.begin(), arr.end(), multiply_by_2); // 2 4 6 8

    std::for_each(std::next(arr.begin()), arr.end(), multiply_by_2); // 4 6 8 Перший елемент було пропущено



    //......................................................................
    std::vector<int> v1 {1,2,3,4,5,6,7,8,9};
    std::make_heap(v1.begin(), v1.end());


    return 0;
}
