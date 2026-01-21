#include <iostream>

class Matrix {
private:
    double m_data[4][4]{};
public:
    // Я свідомо для прикладу не пишу if для перевірки довжини висоти і ширини масиву, щоб спростити приклад

    double operator() (int row, int col) const { // для const obj
        return m_data[row][col];
    }

    double& operator() (int row, int col) {
        return m_data[row][col];
    }

    void operator() () {
        // reset all elements of the matrix to 0.0
        for (int row{ 0 }; row < 4; ++row) {
            for (int col{ 0 }; col < 4; ++col) {
                m_data[row][col] = 0.0;
            }
        }
    }

};


int main() {
    /**
     * Усі перевантаження операторів до цього мали визначену кількість параметрів, але
     * оператор () не має обмежень (можливо має хіба що для якихось неадекватних кількостей),
     * можна від 0 до скількох захочеш.
     *
     * І це відкриває для нас ряд можливостей, наприклад звертатися як до матриці arr[1][3]
     * (У C++23 ми це можемо зробити і через перевантаження [] але до не могли)
     *
     * Хоча перевантаження оператор () навряд чи гарна ідея, бо він сприймається як виклик функції, або на крайняк як виклик конструктора
     */

    Matrix matrix{};
    matrix(1, 2) = 4.5;
    std::cout << matrix(1, 2) << '\n'; // 4.5

    matrix(); // erase matrix
    std::cout << matrix(1, 2) << '\n'; // 0

    return 0;
}