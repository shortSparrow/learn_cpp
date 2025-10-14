#include <cstring>
#include <iostream>
using namespace std;

/**
 *
 * char завжди займає 1 byte
 * char може містити числа і символи. Char може бути signed і unsigned
 *
 * wchar_t, char8_t, char16_t, and char32_t
 * char16_t і char32_t були додані для підтримки Unicode, оскільки з char8_t містить лише 8 bit і годиться
 * тільки для ASCII, а якщо треба вписати українські літери, то треба вже використовувати unicode
 *
 * wchar_t слід уникати майже у всіх випадках (за винятком взаємодії з Windows API), оскільки його розмір визначається реалізацією.и
 */
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


    // char a [10];
    // a[0] = 'A';
    // cout << a << endl; // A
    // a[1] = 'B';
    // cout << a << endl; // AB°_ЇгЎ⌂
    //
    // char b [10];
    // strcpy(b, "Senya");
    // cout << b << endl; // Senya
    //
    // char str[80];
    // strcpy(str, "Hello "); // copy
    // strcat(str, "there"); // concatenate
    // cout << strlen(str) << endl; // 11
    // cout << strcmp(str, "Another"); // compare string - 1
    //                                 // 0, якщо рядки ідентичні.
    //                                 // Додатнє число, якщо перший рядок більший за другий.
    //                                 // Від'ємне число, якщо перший рядок менший за другий.





    // У char також можна записувати числа, вони будуть спрйматися як код у ASCII
    // char a  {88};
    // cout << a; // X


    // char ch1{5}; // initialize with integer 5 (stored as integer 5. 5 - це символ ENQ у ASCII, але система може на нього вивести свою псевдографіку)
    // char ch2{'5'}; // initialize with code point for '5' (stored as integer 53 - бо 53 - це кол в ASCII для числа  5)
    //
    // cout << ch1; // ♣
    // cout << ch2; // 5
















    return 0;
}