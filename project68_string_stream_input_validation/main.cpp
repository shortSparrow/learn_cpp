#include <algorithm> // std::equal
#include <cctype> // std::isdigit, std::isspace, std::isalpha
#include <functional>
#include <iostream>
#include <map>
#include <ranges>
#include <string>
#include <string_view>
#include <limits>

/**
 * Stream states
 * Клас ios_base містить кілька прапорців стану, які використовуються для сигналізації різних умов,
 * що можуть виникнути під час використання потоків:
 *  - goodbit	Everything is okay
 *  - badbit	Some kind of fatal error occurred (e.g. the program tried to read past the end of a file)
 *  - eofbit	The stream has reached the end of a file
 *  - failbit	A non-fatal error occurred (e.g. the user entered letters when the program was expecting an integer)
 *
 * Також є низка функцій які надають інформацію про стан:
 *  - good()	Returns true if the goodbit is set (the stream is ok)
 *  - bad()	Returns true if the badbit is set (a fatal error occurred)
 *  - eof()	Returns true if the eofbit is set (the stream is at the end of a file)
 *  - fail()	Returns true if the failbit is set (a non-fatal error occurred)
 *  - clear()	Clears all flags and restores the stream to the goodbit state
 *  - clear(state)	Clears all flags and sets the state flag passed in
 *  - rdstate()	Returns the currently set flags
 *  - setstate(state)	Sets the state flag passed in
 *
 *
 * Найчастіше використовується failbit (біт помилки), який встановлюється, коли користувач вводить недійсні дані.
 */

bool isValidName(std::string_view name) {
 return std::ranges::all_of(name, [](char ch) {
   return std::isalpha(ch) || std::isspace(ch);
 });

 // Before C++20, without ranges
 // return std::all_of(name.begin(), name.end(), [](char ch) {
 //    return std::isalpha(ch) || std::isspace(ch);
 // });
}

bool inputMatches(std::string_view inputValue, std::string_view pattern) {
    if (inputValue.length() != pattern.length()) {
        return false;
    }

    // This table defines all special symbols that can match a range of user input
    // Each symbol is mapped to a function that determines whether the input is valid for that symbol
    // static const std::map<char, int (*)(int)> validators{
    //       { '#', std::isdigit },
    //       { '_', std::isspace },
    //       { '@', std::isalpha },
    //       { '?', [](int) { return 1; } }
    // };
    static const std::map<char, std::function<int(int)>> validators {
          { '#', [](int c) { return std::isdigit(c); } },
          { '_', [](int c) { return std::isspace(c); } },
          { '@', [](int c) { return std::isalpha(c); } },
          { '?', [](int)   { return 1; } }
    };

    return std::ranges::equal(inputValue, pattern, [](char ch, char mask) -> bool {
        /**
         * validators[mask] не буде працювати
         * В C++ оператор [] для std::map не є константним. Його логіка така: якщо ключа немає в мапі, він автоматично створює
         * новий елемент з цим ключем. Оскільки наша мапа — const, компілятор забороняє будь-яку можливість модифікації (навіть потенційну).
         *
         * Тому будемо використовувати метод .find()
         */
        auto found {validators.find(mask)};

        if (found != validators.end()) {
            return found->second(ch); // (*found->second)(ch);
        }

        // Якщо збігу по validators не знайдено, то треба перевірити чи символ буквально не такий самий як у масці, наприклад знак "-",
        // його немає у validators, але він може бути присутній у масці
        return ch == mask;


    });
}

int main() {
     /**
      * Зараз маємо помилку
      *
      * Якщо виникне помилка, і потік буде встановлено на будь-яке значення, відмінне від goodbit, подальші операції
      * з цим потоком будуть ігноруватися. Цю умову можна очистити, викликавши функцію clear().
      */

    // std::cout << "Enter your age: ";
    // int m_age {}; // Введемо "Oleks"
    // std::cin >> m_age;
    //
    // std::string next_val {};
    // std::cin >> next_val; // Це вже не висвітиться


     // Input validation
    /**
     * Перевірка введених даних – це процес перевірки того, чи відповідають введені користувачем дані певному набору критеріїв.
     * Перевірку введених даних можна загалом розділити на два типи: рядкову та числову.
     *
     * std::isalnum(int)	Returns non-zero if the parameter is a letter or a digit
     * std::isalpha(int)	Returns non-zero if the parameter is a letter
     * std::iscntrl(int)	Returns non-zero if the parameter is a control character
     * std::isdigit(int)	Returns non-zero if the parameter is a digit
     * std::isgraph(int)	Returns non-zero if the parameter is printable character that is not whitespace
     * std::isprint(int)	Returns non-zero if the parameter is printable character (including whitespace)
     * std::ispunct(int)	Returns non-zero if the parameter is neither alphanumeric nor whitespace
     * std::isspace(int)	Returns non-zero if the parameter is whitespace
     * std::isxdigit(int)	Returns non-zero if the parameter is a hexadecimal digit (0-9, a-f, A-F)
     */

     std::cout << isValidName("Oleksandr") << '\n'; // 1 (true)
     std::cout << isValidName("333") << '\n'; // 0 (false)


     /**
      * Зробимо перевірку номера телефону, оскільки у нас це визначена довжина і там мають бути лише цифри та спеціальні знаки,
      * тож нехай:
      * знак # буде відповідати будь-якій цифрі яку вводить користувач
      * знак @ буде відповідати будь-якій літері яку вводить користувач
      * знак _ буде відповідати пробілу.
      * знак ? буде відповідати будь-якому символу взагалі.
      *
      *
      * Оскільки регулярні вирази доволі важкі і довго обраховуються то можна зробити перевірку за допомогою описаних вище функцій
      */

    std::string phoneMask {"(###) ###-####"};
    std::cout << std::boolalpha << inputMatches("(066) 392-7906", phoneMask); // true


    /**
     * Розглянемо приклад валідації числа.
     * Якщо ми введемо не число яке більше 0 то у нас видасть помилку, потім вона очиститься і ми
     * знову будемо запитувати age, і так аж до моменту поки не введемо число яке більше 0
     */

    int age{};

    while (true) {
        std::cout << "Enter your age: ";
        std::cin >> age;

        if (std::cin.fail()) { // була помилка
            std::cin.clear(); // reset the state bits back to goodbit so we can use ignore()
            /**
             * Очищаємо bad input зі стріма (stream)
             * Якщо цього не зробити то буде нескінченний цикл, бо значення яке призвело до помилки все ще лишається у стрімі.
             * І з кожною ітерацією циклу while воно буде знову зчитуватися і знову видавати bad input помилку
             */
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue; // try again
        }

        /**
         * Цей код працює чудово, але що якщо користувач введе 31fefmek56 або 31efek, тоді 31 буде вставлено
         * у age, а решта лишиться у стрімі бо не буде помилки. Якщо ми хочемо щоб це також видавало помилку
         * то треба зробити додаткову обробку.
         *
         * gcount() показує кількість символів (байтів), які було вилучено з вхідного потоку під час останньої операції неформатованого введення.
         * У нашому випадку останньою такою операцією є std::cin.ignore(...).
         *
         * std::cin.ignore(max, '\n'), починає "викидати" символи з буфера один за одним, поки не зустріне знак переходу на новий рядок (\n).
         * std::cin.gcount() повертає кількість символів, які ignore встиг "з'їсти", включаючи сам символ \n.
         *
         * Для 31fefmek56 std::cin.gcount() поверне 9 (це fefmek56 + \n)
         * Для 31 std::cin.gcount() поверне 1 - це \n
         */
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear out any additional input from the stream
        if (std::cin.gcount() > 1) { //
            continue; // we'll consider this input to be invalid
        }

        if (age <= 0) // make sure age is positive
            continue;

        break;
    }

    /**
     * Хоча у JS ми б для цих цілей використали б regular expressions, однак у C++ це доволі важка бібліотека і для таких простих операцій
     * вважається надмірною і не раціональною. До того ж вони будуть працювати повільніше ніж перевірка флага на badbit і ітерація по рядку
     */


    return 0;
}