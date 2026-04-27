#include <algorithm>
#include <iostream>
#include <memory>
#include <string_view>

template <typename T, int size>
class StaticArray {
private:
    T m_array[size]{};

public:
    T* getArray() { return m_array; }

    const T& operator[](int index) const { return m_array[index]; }
    T& operator[](int index) { return m_array[index]; }
};

template <typename T, int size>
void print(const StaticArray<T, size>& array) {
    for (int count{ 0 }; count < size; ++count)
        std::cout << array[count] << ' ';
    std::cout << '\n';
}

// Override print() for fully specialized StaticArray<char, 14>
template <>
void print(const StaticArray<char, 14>& array) {
    for (int count{ 0 }; count < 14; ++count)
        std::cout << array[count];
    std::cout << '\n';
}

// Перевантажуємо функцію of print() через часткову спеціалізацію (partially specialized) StaticArray<char, size>.
// По факту тут не перевантаження, бо я створив нову функцію print2(), але в реальності тут було б перевантаження
template <int size> // size - це non-type parameter
void print2(const StaticArray<char, size>& array) {
    for (int count{ 0 }; count < size; ++count)
        std::cout << array[count];
}

int main() {
    /**
     * У нашому прикладі ми можемо вивести дані за допомогою функції print(), але якщо ми виводимо цифри то нехай кожен елемент
     * буде виводитися через пробіл (1 2 3), а якщо string то краще щоб було без пробілу (Hello world). Для цього у нас є
     * template для виводу через пробіл і особлива реалізація для рядків де не буде пробілу між елементами.
     *
     * І важливо зауважити, що оскільки ми працюємо з масивами які мають статичну довжину то ми у template також передаємо і
     * розмір масиву, щоб знати скільки пам'яті алокувати.
     */

    StaticArray<char, 14> char14{};
    // Копіюємо рядок "Hello, world!" у масив char14
    constexpr std::string_view hello{ "Hello, world!" };
    std::copy_n(hello.begin(), hello.size(), char14.getArray());

    print(char14); // Hello, world!


    /**
     * А що робити, якщо я хочу передати у print() масив із 12 елементів - print(char12)
     * Зараз у нас буде проблема, бо template визначений спеціально для string приймає як аргумент також і довжину масиву,
     * відповідно якщо ми передамо print(char12) не зважаючи на те що це <char, 12>  відпрацює звичайний template і
     * виведе всі елементи через пробіл.
     */

    StaticArray<char, 12> char12{};
    // Копіюємо рядок "Hello, world!" у масив char12
    std::copy_n(hello.begin(), hello.size(), char12.getArray());

    print(char12); // H e l l o ,   w o r l d


    /**
     * Саме для цього існує partial template specialization.
     * Часткова спеціалізація шаблонів дозволяє спеціалізувати класи (але не окремі функції!),
     * у яких явно визначено деякі, але не всі параметри шаблону.
     *
     * Перед функцією ми додаємо template <int size> і тепер наш print буде мати size як аргумент.
     * Я назвав функцію print2, а не print щоб зберегти попередній неправильний приклад для наочності.
     * В реальному прикладі її назва буде такою самою як і оригінальна функція із template
     */

    print2(char12); // Hello, world



    /**
     * Часткова спеціалізація — тільки для класів
     * У C++ існує суворе правило:
     *      Класи (та структури) можна спеціалізувати частково. Це означає, що якщо у вас є template <typename T, int size>,
     *      ви можете створити окрему версію класу саме для T = char, але залишити size змінним.
     *
     *      Функції можна спеціалізувати тільки повністю. Тобто ви повинні вказати конкретні значення для всіх параметрів
     *      шаблону (наприклад, і тип char, і конкретний розмір 14).
     *
     * Чому приклад void print2(StaticArray<char, size> &array) працює?
     * Насправді тут працює не механізм спеціалізації, а перевантаження шаблонів функцій (Function Template Overloading).
     * Ось що бачить компілятор:
     *      Основний шаблон: template <typename T, int size> void print(const StaticArray<T, size>& array) — підходить для всього.
     *
     *      Другий шаблон (перевантаження): template <int size> void print(const StaticArray<char, size>& array) — це нова шаблонна
     *      функція, яка просто більш специфічна.
     *
     *
     */
    return 0;
}