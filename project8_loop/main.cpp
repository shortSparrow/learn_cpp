#include <iostream>
#include <vector>
using namespace std;

int main() {

    // -------------------------- FOR LOOP -----------------------------------

    // for(int i {0}; i<10; i++){
    //     cout << i << endl;
    // }
    //
    // for (int i {1}, j{5};  i <= 5; i++, j++) {
    //     cout << i << " * " << j << " = " << i * j << endl;
    // }
    //
    // vector<int> nums {10,20,30,40};
    // for (unsigned int i {0}; i < nums.size(); i++) {
    //     cout << nums.at(i) << endl;
    // }


    // -------------------------- RANGE BASED FOR LOOP ---------------------------

    // int scores [] {100, 90, 87};
    // for (int score: scores) {
    //     cout << score << endl;
    // }
    //
    // for (int scores2 [] {200, 120, 33}; const unsigned int score: scores2) {
    //     cout << score << endl;
    // }
    //
    // // В цьому випадку у циклі ми не вказуємо ти score, complier визначить його автоматично
    // vector<double> temperature {33.2,44.12,55.08};
    // double sum_temperature  {0};
    // for (auto temp: temperature) {
    //     sum_temperature += temp;
    // }
    // cout << sum_temperature << endl; // 132.4
    //
    //
    // for (auto c: "Frank") {
    //     cout << c << endl;
    // }

    // ---------------------------------- WHILE LOOP ---------------------------------
    // int i {0};
    // while (i<10) {
    //     cout << i << endl;
    //     i++;
    // }

    // int number {};
    // cout << "Enter the number grater than 100: ";
    // cin >> number;
    //
    // while (number <= 100) {
    //     cout << "Your previous number was lass than 100, try again: ";
    //     cin >> number;
    // }
    //
    // cout << "Thanks" << endl;


    // ---------------------------------- DO WHILE LOOP ---------------------------------
    // int number {0};
    //
    // do {
    //     cout << "Enter an integer between 1 and 5: ";
    //     cin >> number;
    // } while ( number <= 0 || number >= 6);
    //
    // cout << "Thank you!";


    // ---------------------------------- INFINITE LOOP ---------------------------------
    while (true) {
        char again {};
        cout << "Do you want to loop again? (Y/N): ";
        cin >> again;

        if (again == 'n' || again == 'N') {
            break;
        }
    }



    return 0;
}