#include <cstdint>
#include <iostream>


/**
 * Напишіть клас із назвою Average, який буде відстежувати середнє значення всіх цілих чисел, що йому передаються.
 * Використовуйте два члени: перший повинен бути типу std::int32_t і використовуватися для відстеження суми всіх чисел,
 * які ви бачили до цього моменту. Другий повинен відстежувати, скільки чисел ви бачили до цього моменту.
 * Ви можете розділити їх, щоб знайти середнє значення.
 *
 * Чому варто використовувати саме std::int32_t
 * На деяких платформах значення int може бути 16-бітним, що означатиме, що наш об'єкт Average може мати максимальне значення
 * чисельника лише 32 767. Використання std::int32_t гарантує 32-бітове ціле значення, що дає нам набагато ширший діапазон для роботи.
 *
 */

class Average {
private:
    std::int32_t m_value {};
    int count {0};

public:
    Average& operator+=(const std::int32_t x) {
        m_value += x;
        count++;

        return *this;
    }

    friend std::ostream& operator<< (std::ostream& out, const Average& source) {
        if (source.count > 0) {
            std::cout << static_cast<double>(source.m_value)/source.count; // бо ми не хочемо втратити дробову частину при діленні
        } else {
            std::cout << source.m_value;
        }

        return out;
    }
};

int main() {
    Average avg{};
    std::cout << avg << '\n';

    avg += 4;
    std::cout << avg << '\n'; // 4 / 1 = 4

    avg += 8;
    std::cout << avg << '\n'; // (4 + 8) / 2 = 6

    avg += 24;
    std::cout << avg << '\n'; // (4 + 8 + 24) / 3 = 12

    avg += -10;
    std::cout << avg << '\n'; // (4 + 8 + 24 - 10) / 4 = 6.5

    (avg += 6) += 10; // 2 calls chained together
    std::cout << avg << '\n'; // (4 + 8 + 24 - 10 + 6 + 10) / 6 = 7

    Average copy{ avg };
    std::cout << copy << '\n';

    return 0;
}