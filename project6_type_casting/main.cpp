#include <iostream>
#include <vector>
using namespace std;

int main() {

    // int a {3};
    // double b {5.5};
    // int result_int = a * b;
    // double result_double = a * b;
    // cout << result_int << endl; // 16 - відбувся demotion типу до нижчого
    // cout << result_double << endl; // 16.5 - відбувся promoting типу до вищого

    int total_amount {100};
    int total_number {3};
    double average {0};

    average = total_amount / total_number;
    cout << average << endl; // 33 - відповідь int, хоча average double

    average = static_cast<double> (total_amount) / total_number;
    cout << average << endl; // 33.333 - відповідь double, бо привели total_amount до double

    //
    // bool is_equal = 10 == 10.0;
    // cout << is_equal << endl; // 1 (тобто true) - відбувся promoting типа до вищого (double)
    //
    // bool is_equal2 = 10 == 9.9999999999999999;
    // cout << is_equal2 << endl;
    /**
     * 1 (тобто true) - бо важливо те як комп'ютер зберігає дробові числа.
     * Якщо потрібна точність то треба користуватися спеціальними бібліотеками
     */


    // cout << boolalpha; // для того, щоб виводилося на 1/0, а true/false
    // vector<int> a {1};
    // vector<int> b {1};
    // bool is_equal_vectors = a == b;
    // cout << is_equal_vectors << endl; // true
    //
    // vector<int> c {1};
    // vector<int> d {2};
    // bool is_equal_vectors2 = c == d;
    // cout << is_equal_vectors2 << endl; // false


    return 0;
}