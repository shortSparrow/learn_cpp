#include <iostream>

/**
 * У прикладі з функцією max було реалізовано лише варіант з int, але якщо раптом
 * я передам double то помилки не буде (IDE лише підсвітить це, можливо компілятор теж),
 * і оскільки doulbe через numberic conversion перевториться на int я втрачу частину інформації
 * і функція працюватиме не правильно, відповідно треба писати реалізацію під double і інші типи.
 *
 * Щоб самому не писати реалізацію на всі типи, можна напитати template, як з max_with_template.
 * Тоді компілятор автоматично створить потрібні перевантаження за мене.
 *
 * Важливо розуміти що tempalte створює перевантаження функції на етапі компіляції, тобто
 * якщо я отримую з сервера дані у форматі long і закину їх у max то вони приведуться до int,
 * бодо компіляції template не знає що треба робити якесь переватжаення окрім double.
 *
 *
 *
 */
char max(int x, int y) {
    return  (x > y) ? 'x' : 'y';
}

template <typename T>
char max_with_template(T x, T y) {
    return  (x > y) ? 'x' : 'y';
}


// Створюємо template для функції addOne
template <typename T>
T addOne(T x) {
    return x + 1;
}

// Забороняємо addOne викликатися з аргументом char
template <>
const char* addOne(const char* x) = delete;


int main() {

    std::cout << max(1,2) << '\n'; // y
    std::cout << max(3.5,3.4) << '\n'; // y



    std::cout << max_with_template(1,2) << '\n'; // y
    std::cout << max_with_template(3.5,3.4) << '\n'; // x

    /**
     * Краще такий варіант не використовувати, бо такий виклик бачить тільки функції створені через template.
     * Припусутимо буде функція з template і перевантажена для bool яку я додаю сам і вона буде по своємо виводити
     * bool, а саме true/false а не 1/0 як у template
     *
     * template <typename T>
     * void print(T x) {
     *   std::cout << x; // print T however it normally prints
     * }
     *
     * void print(bool x) {
     *   std::cout << std::boolalpha << x; // print bool as true or false, not 1 or 0
     * }
     *
     *
     *   print<bool>(true); // calls print<bool>(bool) -- prints 1
     *   std::cout << '\n';
     *
     *   print<>(true);     // deduces print<bool>(bool) (non-template functions not considered) -- prints 1
     *   std::cout << '\n';
     *
     *   print(true);       // calls print(bool) -- prints true
     *   std::cout << '\n';
     *
     * Як бачимо все що має <> не бачить створену мою функцію яка по особливому виводить bool, натомість вони сторюють свій варіант
     * перевантаженої функції і використовують його
     *
     */
    std::cout << max_with_template<float>(3,2) << '\n'; // Буде згенерована функція з параметрами float x, float y


    // Компілятор видасть помилку бо ми заборонили викликати функцію addOne з параметром char* (за замовченням "..." - це char*)
    std::cout << addOne("Hello, world!") << '\n'; // compile error




    return 0;
}