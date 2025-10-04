#include <iostream>

using namespace std;

int main() {
    // int number1;
    // /**
    //  * Якщо я введу '123' то number1 матиме значення 123
    //  * Якщо я введу '    123    ' то number1 матиме значення 123
    //  * Якщо я введу 'D' чи 'A' то number1 матиме значення 0
    //  */
    // cout << "Enter an integer: ";
    // cin >> number1;
    // cout << "You entered: " << number1;



    // int number1;
    // int number2;
    //
    // /**
    //  * Якщо я введу '123' і натисну etner то number1 матиме значення 123
    //  * І далі з'явиться 'Enter a second integer: ' і я введу значення 88
    //  * то number2 матиме значення 88, і я побачу фінальний виввід
    //  * "You entered: 123 and 88"
    //  *
    //  * Але я також можу на "Enter a first integer: " ввести одразу два числа
    //  * Наприклад: 123 88
    //  * Тоді я побачу 'Enter a second integer: You entered: 123 and 88'
    //  */
    // cout << "Enter a first integer: ";
    // cin >> number1;
    // cout << "Enter a second integer: ";
    // cin >> number2;
    // cout << "You entered: " << number1 << " and " << number2;




    // int number1;
    // int number2;
    //
    // /**
    //  * Якщо я введу '11 22'
    //  * Тоді я побачу 'Enter a second integer: You entered: 11 and 22'
    //  */
    // cout << "Enter a 2 integers separated with space: ";
    // cin >> number1 >> number2;
    // cout << "You entered: " << number1 << " and " << number2;


    int number1;
    double number2;

    /**
     * Якщо я введу '1 2.5'
     * Тоді я побачу 'You entered 1 and 2.5'
     *
     * Але якщо я введи лише 2.5 і натисну Enter
     * То я побачу 'You entered 2 and 0.5'
     * Це тому що перше число повинно бути int, отже це 1,
     * а потім лишається 0.5 і це і буде другим числом
     *
     * Якщо я введу 'Senya' або просто 'A'
     * Я побачу 'You entered: 0 and 3.7549e-322'
     */
    cout << "Enter a 2 numbers: ";
    cin >> number1 >> number2 ;
    cout << "You entered: " << number1 << " and " << number2;



    return 0;
}