#include <iostream>
#include <chrono> // for std::chrono functions

class Timer {
private:
    // Type aliases to make accessing nested type easier
    using Clock = std::chrono::steady_clock;
    using Second = std::chrono::duration<double, std::ratio<1> >; // std::ratio<1> говорить що викорстовуємо секунди

    std::chrono::time_point<Clock> m_beg;

public:
    Timer() : m_beg(Clock::now()) {} // Запускається відразу при створенні

    void start() {
        m_beg = Clock::now();
    }

    // [[nodiscard]] горить що результат функції треба обов'язково використати, інакше компілятор дасть warning
    [[nodiscard]] double end() const  {
        return std::chrono::duration_cast<Second>(Clock::now() - m_beg).count(); // повертає різницю часу у секундах
    }
};

int main() {
    const int MAX_NUM = 1'000'000'00;
    std::vector<int> num1 (1);
    std::vector<int> num2 (MAX_NUM);

    Timer t;
    t.start();
    for (int i{0}; i < MAX_NUM; i++) {
        num1.push_back(i);
    }
    std::cout << "Nums 1: " << t.end() << " seconds\n";


    t.start();
    for (int i{0}; i < MAX_NUM; i++) {
        num2[i] = i;
    }
    std::cout << "Nums 2: " << t.end() << " seconds\n";

    return 0;
}