#include <cmath>
#include <iostream>

void example1() {
    // Приклад використання goto
    double x{};
    tryAgain: // this is a statement label
        std::cout << "Enter a non-negative number: ";
    std::cin >> x;

    if (x < 0.0)
        goto tryAgain; // this is the goto statement

    std::cout << "The square root of " << x << " is " << std::sqrt(x) << '\n';
}

void example2() {
    for (int i = 1; i < 5; ++i) {
        for (int j = 1; j < 5; ++j) {
            std::cout << i << " * " << j << " is " << i*j << '\n';

            // If the product is divisible by 9, jump to the "end" label
            if (i*j % 9 == 0) {
                std::cout << "Found product divisible by 9.  Ending early.\n";
                goto end;
            }
        }
        std::cout << "Incrementing the first factor.\n";
    }
    end:
        std::cout << "And we're done." << '\n';
}


int main() {
    example1();

    /**
     * Цей приклад є одним з небагатьох прикладів, коли goto дозволено.
     * Звісно це можна зробити і без нього, наприкод пододавати змінні should_go_text = false
     * і ставити true коли if (i*j % 9 == 0) спрацював, але так треба буде мати змінну на
     * кожний батьківський if, що може збільшити складність коду
     */
    example2();


    return 0;
}