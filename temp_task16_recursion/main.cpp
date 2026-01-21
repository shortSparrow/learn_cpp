#include <bitset>
#include <iostream>
#include <cmath>

int factorial(int x) {
    if (x == 1) {
        return 1;
    }
    return x * factorial(x - 1);
}

// Напишіть рекурсивну функцію, яка приймає ціле число на вхід і повертає суму кожної окремої цифри цього числа (наприклад, 357 = 3 + 5 + 7 = 15).
int sum_of_numbers(int x) {
    if (x < 10) {
        return x;
    }
    return sum_of_numbers(x/10) + x%10;
}

/**
 * Моє рішення, але на жаль воно не працює для від'ємних значень, хіба що робити той самий лайвгак, що і у print_binary з unsigned int
 * power = 32, бо на цій машині sizeof(int) = 4 => 4*8=32
 * Насправді можна було б і без power зробити рішення, щоб 15 виводило як 01111, але я вирішив що так краще.
 * Щоб зробити варіант з 01111 треба видалити рядки 32-35 (де є доставляння 0 на перед і умову if(power=0) замінити
 * на дві умови if(x==1) return 1 і на випадок передачі 0 if(x==0) return 0
 */
std::string convert_int_to_binary(int x, int power=32) {
    // 2^0 + 2^1 + 2^2 + 2^3 = 15 => 1111
    // 2^0 + 2^1 + 2^2 + 2^3 + 2^4= 16 => 00001
    // 15 => 15/2=7  7/2=3  3/2=1  1/2=0
    // 15 => 15%2=1  7%2=1  3%2=1  1%2=1

    if (x < std::pow(2,power) && power > 0) {
        std::string res {"0"};
        return res.append(convert_int_to_binary(x, power-1));
    }


    if (power == 0) {
        return std::to_string(x);
    }

    int reminder {std::abs(x%2)};
    return convert_int_to_binary(x/2, power-1).append( std::to_string(reminder) );
}


// Рішення з сайту
void print_binary(unsigned int x) {
    /**
     * Порядок виводу у термінал буде зворотній, тобто з низу у верх (з початку 1, потім 0, потім 0, потім 0)
     * 16 ->
     *    8 (print 0)
     *    4 (print 0)
     *    2 (print 0)
     *    1 (print 1)
     *
     *
     * З мінусовими числами є хитрий лайфгак. Припустимо є число -15 і це 11111111111111111111111111110001
     * але якщо ми перетворимо його у unsigned int 4294967281 - то у бітах це буде те саме число, але вже позитивне
     * і ми можемо запустити нашу функцію як для позитивного числа.
     *
     */

    /**
     * ця перевірка потрібна не для заборони від'ємних чисел, бо x unsigned int і він завжди > 0, але якщо x = 0
     * то у нас буде нескінченна рекурсія, бо 1/2=0   =>   0/2=0  =>   0/2=0 ....
     */
    if (x > 0) {
        print_binary(x / 2);
    }

    // Print out the remainders (in reverse order)
    std::cout << x % 2;
}


int main() {

    std::cout << "factorial of 7! = " << factorial(7) << '\n'; // 5040

    std::cout << "sum each numbers of 357 = " << sum_of_numbers(357) << '\n'; // 15

    std::cout << "print_binary binary representation of 16 = ";
    print_binary(static_cast<unsigned int>(16)); // 010000
    std::cout << '\n';

    std::cout << "print_binary binary representation of 15 = ";
    print_binary(static_cast<unsigned int>(15)); // 01111
    std::cout << '\n';

    std::cout << "print_binary binary representation of -15 = ";
    print_binary(static_cast<unsigned int>(-15)); // 011111111111111111111111111110001
    std::cout << '\n';

    std::cout << "convert_int_to_binary binary representation of 0 = " << convert_int_to_binary(0) << '\n'; // 000000000000000000000000000000000
    std::cout << "convert_int_to_binary binary representation of 16 = " << convert_int_to_binary(16) << '\n'; // 000000000000000000000000000010000
    std::cout << "convert_int_to_binary binary representation of  15 = " << convert_int_to_binary(15) << '\n'; // 000000000000000000000000000001111

    // треба доробити convert_int_to_binary щоб воо приймало unsigned int
    // std::cout << "convert_int_to_binary binary representation of -15 = " << convert_int_to_binary(-15) << '\n';



    return 0;
}
