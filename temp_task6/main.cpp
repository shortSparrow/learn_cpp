#include <iostream>
using namespace  std;

int char_as_int(char c){
    return c;
}

int get_int(int c){
    return c;
}

int main() {
    /**
     * Task 1
     * Write a short program where the user is asked to enter a single character.
     * Print the value of the character and its ASCII code, using static_cast.
     *
     * Use explicit type conversion (тобто static_cast)
     */
    // char my_value {};
    // cin >> my_value; // enter A
    // cout << "Character: " << my_value << "\nASCII code: " << static_cast<int>(my_value) << endl;
    /**
     * A
     * Character: A
     * ASCII code: 65
     *
     * 351
     * Character: 3
     * ASCII code: 51
     *
     * ю
     * Character: ю
     * ASCII code: -18
     *
     * i (ukr i)
     * Character: ?
     * ASCII code: 63
     */

    /**
     * Працює так само як і static_cast. Це неявне перетворення (implicit type conversion).
     * Краще завжди використовувати явне приведення типів - static_cast
     *
     * Особливо цікавий спосіб char_as_int і get_int, з цим треба бути дуже обережним, бо ми не отримали
     * повідомлення про помилку у get_int, коли передали не той тип
     */

    char my_value {};
    cin >> my_value; // enter A
    int my_value_int {my_value};
    cout << "Character: " << my_value << "\nASCII code: " << my_value_int << endl;
    cout << "Character: " << my_value << "\nASCII code: " << char_as_int(my_value) << endl;
    cout << "Character: " << my_value << "\nASCII code: " << get_int(my_value) << endl;


    return 0;
}