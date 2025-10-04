// #include <iostream>
//
// #define PRINT_JOE
//
// int main() {
// #ifdef PRINT_JOE
//     std::cout << "Joe\n"; // will be compiled since PRINT_JOE is defined
// #endif
//
// #if defined(PRINT_JOE)
//     std::cout << "Joe 2\n";
// #endif
//
// #ifdef PRINT_BOB
//     std::cout << "Bob\n"; // will be excluded since PRINT_BOB is not defined
// #endif
//
//     return 0;
// }

/**
 * У прикладі вище буде виведено лише 'Joe', тому що індентифікатор PRINT_JOE було визначено, а
 * ідентифікатор PRINT_BOB ні, тому директива endif під час preprocessor поверне тільки
 * код для cout << Joe
 *
 * #if defined(PRINT_JOE) - це те саме, що і #ifdef PRINT_JOE. Просто інший синтаксис
 */




// #include <iostream>
//
// int main() {
// #ifndef PRINT_BOB
//     std::cout << "Bob\n";
// #endif
//
// #if !defined(PRINT_BOB)
//     std::cout << "Bob 2\n";
// #endif
//
//     return 0;
// }

/**
 * #ifndef є протилежністю #ifdef, оскільки дозволяє перевірити, чи ідентифікатор ще НЕ був #визначений
 * Саме тому оскільки PRINT_BOB НЕ БУВ визначений ми побачимо  std::cout << "Bob\n";
 *
 * #if !defined(PRINT_BOB) - це те саме, що і #ifndef PRINT_BOB, просто інакше написано
 */


// ------------------------ #if 0 and #if 1 ------------------------

// #include <iostream>
//
// int main(){
//     std::cout << "Joe\n";
//
// #if 0 // Don't compile anything starting here
//     std::cout << "Bob\n";
//     std::cout << "Steve\n";
// #endif // until this point
//
//     return 0;
// }

/**
 * Усе що між #if 0 and #endif НЕ БУДЕ зкопмільовано, тож ми побачимо тільки std::cout << "Joe\n"
 */

// #include <iostream>
//
// int main(){
//     std::cout << "Joe\n";
//
// #if 1 // always true, so the following code will be compiled
//     std::cout << "Bob\n";
//     /* Some
//      * multi-line
//      * comment here
//      */
//     std::cout << "Steve\n";
// #endif
//
//     return 0;
// }

/**
 * Усе що між #if 1 and #endif БУДЕ зкопмільовано, тож ми побачимо
 * std::cout << "Joe\n"
 * std::cout << "Bob\n"
 * std::cout << "Steve\n"
 */


#include <iostream>

void foo() {
#define MY_NAME "Alex"
}

int main(){
	std::cout << "My name is: " << MY_NAME << '\n'; // My name is: Alex

	return 0;
}