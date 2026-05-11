#include <iostream>
#include <sstream>

int main() {
    std::stringstream os {};

    /**
     * Існує два способи покласти дані у stringstream
     *
     * 1. Use the insertion (<<) operator
     * 2. Use the str(string) function to set the value of the buffer
     */
    os << "Hola amigo\n"; // insert "en garde!" into the stringstream
    os.str("Arcadio Buendia!"); // set the stringstream buffer to "en garde!"


    /**
     * Існує два аналогічних способи отримання даних з потоку рядків
     *
     * 1. Використаnb функцію str() для отримання результатів буфера
     * 2. Викорситати >> оператор
     */
    std::cout << os.str(); // Hola amigo

    std::string strValue {};
    os >> strValue;
    std::cout << strValue; // Arcadio Buendia!



    /**
     * Conversion between strings and numbers
     * Оскільки оператори вставки та вилучення знають, як працювати з усіма основними типами даних, ми можемо
     * використовувати їх для перетворення рядків на числа або навпаки.
     *
     */
    std::stringstream os2 {};

    constexpr int nValue { 12345 };
    constexpr double dValue { 67.89 };
    os2 << nValue << ' ' << dValue; // кладемо дані у стрім

    std::string strValue1, strValue2;
    os2 >> strValue1 >> strValue2; // дістаємо дані зі стріма і кладемо у змінні

    std::cout << strValue1 << ' ' << strValue2 << '\n'; // 12345 67.89


    // Тепер перетворимо числовий рядок на число

    os2 << "12345 67.89"; // insert a string of numbers into the stream
    int nValue2 {};
    double dValue2 {};

    os2 >> nValue2 >> dValue2;

    std::cout << nValue2 << ' ' << dValue2 << '\n'; // 12345 67.89


    /**
     * Очищення потоку рядків для повторного використання
     * Існує кілька шляхів для для очищення buffer у stringstream
     *
     * 1. Закинути у str() породній рядок ""
     * 2. Закинути у str() std::string{}
     *
     * Під час очищення потоку рядків також зазвичай гарною ідеєю є виклик функції clear().
     * Функція clear() скидає всі прапорці помилок, які могли бути встановлені, і повертає потік у стан «ok».
     */

    std::stringstream os3 {};
    os3 << "Hello ";

    os3.str(std::string{}); // erase the buffer

    os3 << "World!";
    std::cout << os3.str(); // // erase the buffer

    os3 << "OMG ";
    os3.str(""); // erase the buffer
    os3.clear(); // reset error flags

    return 0;
}