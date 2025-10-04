#include <cstring>
#include <iostream>
using namespace std;

int main() {
    // char my_name [6] {"Senya"};
    // // my_name має лише 5 символів, але 6-ий символ - це закінчення рядка у C - \0
    //
    // char my_second_name [7] {"Senya"};
    // my_second_name[6] = 'S';
    // cout << my_second_name;
    // // Виведе Senya, це тому що, незважаючи на те що ми алокували аж 7 char
    // // ми маємо Senya\0\0, і я не можу змінити породні символи, що дивно
    //
    //
    // char my_third_name [15] {"Senya"};
    // my_third_name[7] = 'S';
    // cout << my_third_name; // Senya


    char a [10];
    a[0] = 'A';
    cout << a << endl; // A
    a[1] = 'B';
    cout << a << endl; // AB°_ЇгЎ⌂

    char b [10];
    strcpy(b, "Senya");
    cout << b << endl; // Senya

    char str[80];
    strcpy(str, "Hello "); // copy
    strcat(str, "there"); // concatenate
    cout << strlen(str) << endl; // 11
    cout << strcmp(str, "Another"); // compare string - 1
                                    // 0, якщо рядки ідентичні.
                                    // Додатнє число, якщо перший рядок більший за другий.
                                    // Від'ємне число, якщо перший рядок менший за другий.




























    return 0;
}