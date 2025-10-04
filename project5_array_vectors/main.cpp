#include <iostream>
#include <vector>

using namespace std;

int main() {
    // // Так ми задаємо довжину масиву
    // int test_score [5]; // масив довжиною 5
    // constexpr int days_in_year {365};
    // double hi_temperatures [days_in_year]; // масив довжиною 5
    //
    // std::cout << test_score << std::endl; // 0x7f0a9ffd00 - це адреса па'яті де збереженно масив
    //
    // int test_score2 [4] {100, 95, 20, 88}; // Задали масив і наповнили його
    // int test_score3 [10] {100, 95,}; // Задали масив і заповнили його перші два елементи
    // int hi_temperatures2 [12] {0}; // Задали масив і заповнили всі його елементи 0
    // int hi_temperatures3 [12] {}; // Задали масив і заповнили всі його елементи 0
    // int hi_temperatures4 [12] {100}; // Задали масив і заповнили ЛИШЕ 0-ий елемент як 100, решта елементів порожні
    // int hi_temperatures5 [] {1,9,33,12,33,50,4}; // Задали масив і заповнили його. Розмір масиву визначився автоматично
    //
    // std::cout << hi_temperatures4[0] << std::endl; // 100
    // std::cout << hi_temperatures4[1] << std::endl; // 0


    // Отримання елементу
    // std::cout << test_score2[0] << std::endl; // 100
    // std::cout << test_score2[10] << std::endl; // 0 (вийшли за межі масиву)
    //
    // test_score2[10] = 3; // test_score2 має довжину 4. У мене помилку не видає, хоча дані не запише
    // std::cout << test_score2[10] << std::endl; //  3
    /**
     * Це дуже небезбечна помилка, бо C++ не виконує перевірку під час запуску,
     * і відповідно коли я вийшов за межі масиву і змінив щось - я міг змінити щось у іншій
     * змінній що може призвести до неправильної роботи програми, саме тому варто бути ДУЖЕ
     * обережним і користуватися Vecors замість Arrays
     */


    // // Multi dimensional arrays
    // int movie_rating [3][4]; // Масив 3x4
    // constexpr int movie_rating2 [3][4] {
    //     {0,3,3,5},
    //     {4,1,2,3},
    //     {3,4,4,1}
    // };


    // Vectors
    // vector<char> movie_list;
    // vector<int> id_list;
    //
    // vector <int> id_list2 (4); // автоматично створили vector з розміром 4 і всі елементи 0
    // cout << id_list2[1] << endl; // 0

    // vector<char> movie_list {1,2,3};
    // vector<int> temperature (360, 80); // 360 - це розмір вектора, а 80 це значення яким буде наповнений кожен елемент;
    // cout << temperature[1] << endl; // 80

    // vector<int> a1 (3);
    // a1[0] = 20;
    // a1[8] = 44; // Помилка бо ми переповнили vector
    // cout << a1[8] << endl; // Помилка бо ми переповнили vector
    // cout << a1.at(8) << endl; // Помилка бо ми переповнили vector (але не така як вище)


    // vector<int> a {3,5,7};
    // a.push_back(99); // a = {3,5,7,99}
    // cout << a.size(); // 4


    // vector<vector<int>> a {
    //     {1,2,3},
    //     {4,5,6},
    //     {7,8,9}
    // };
    // cout << a.at(1).at(0); // 4
    // cout << a[1][0]; // 4


    vector<int> vector1 {10, 20};
    vector<vector<int>> vector_2d;
    vector_2d.push_back(vector1);
    cout << vector_2d.at(0).at(0) << " "; // 10
    cout << vector_2d.at(0).at(1) << endl; // 20

    vector1.at(0) = 1000;
    cout << vector_2d.at(0).at(0) << " "; // 10
    cout << vector_2d.at(0).at(1) << endl; // 20

    cout << vector1.at(0) << endl; // 1000

    /**
     * Якщо елементом 2d вектора є інший вектор - то це не працює як посилання,
     * тут робиться копія. Саме тому хоч ми і змінили значення vector1 у vector_2d
     * воно не змінилося.
     *
     * Для того щоб це був один об'єкт то треба передати посилання на вектор
     * vector<vector<int>*> vector_2d
     * vector_2d.push_back(&vector1);
     */



    return 0;
}