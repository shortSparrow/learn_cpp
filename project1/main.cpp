#include <iostream>

/**
 *  Це інший спосіб написати main функцію. Цей варіант приймає параметри які передаються під час виклику,
 *  корисно для програм які заточені під термінал
 *
 *  argc - arguments count, тобто скільки аргументів було передано
 *  *argv[] - це масив рядків, тобто це і є самі параметри які було передано
 *
 */
// int main(int argc, char *argv[]) {}

// using namespace std; // це дозволяє не писати std::
// using std::cout; // це дозволяє не писати std:: лише для cout
// using std::cin; // це дозволяє не писати std:: лише для cin
// using  std::endl; // це дозволяє не писати std:: лише для endl


int main() {
    int favorite_number;

    std::cout << "What is your favorite number?" << std::endl;
    std::cin >> favorite_number;
    std::cout << "Oh my God, this is amazing " << favorite_number << " is also my favorite number" << std::endl;

    return 0; // 0 - це значить що програма завершилася успішно, інше значення, що була помилка
}