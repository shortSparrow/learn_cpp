#include <cassert>
#include <iostream>

class IntArray {
private:
    int m_length {};
    int* m_data{};

public:
    IntArray() = default;

    IntArray(int length)
        : m_length{ length }
    , m_data{ new int[static_cast<std::size_t>(length)] {} } {

    }

    IntArray(std::initializer_list<int> list) // allow IntArray to be initialized via list initialization
        : IntArray(static_cast<int>(list.size())) // use delegating constructor to set up initial array
    {
        // Now initialize our array from the list
        std::copy(list.begin(), list.end(), m_data);
    }

    ~IntArray() {
        delete[] m_data;
        // we don't need to set m_data to null or m_length to 0 here, since the object will be destroyed immediately after this function anyway
    }

    IntArray(const IntArray&) = delete; // to avoid shallow copies
    IntArray& operator=(const IntArray& list) = delete; // to avoid shallow copies

    int& operator[](int index)
    {
        assert(index >= 0 && index < m_length);
        return m_data[index];
    }

    int getLength() const { return m_length; }
};


class Foo {
public:
    Foo(int, int) {
        std::cout << "Foo(int, int)" << '\n';
    }

    // We've added a list constructor
    Foo(std::initializer_list<int>) {
        std::cout << "Foo(std::initializer_list<int>)" << '\n';
    }

};

int main() {

    /**
     * std::initializer_list
     *  Уявимо клас
     *  class IntArray {
     *      IntArray() {...}
     *  }
     *  Він повинен мати змогу ініціалізуватися як std::vector чи C-style array
     *  intArray arr {1,2,3,4...}
     *
     *  Але як це зробити? У нас же може бути будь-яка кількість елементів якими ми захочемо
     *  ініціалізувати наш клас, який конструктор для цього написати?
     *
     *  Ось тут і приходить на допомогу std::initializer_list
     */


    IntArray a1(5);   // uses IntArray(int), allocates an array of size 5
    IntArray a2{ 5 }; // uses IntArray<std::initializer_list<int>, allocates array of size 1


    /**
     * У list constructor є одна проблемка, його не варто додавати до класів у яких він може спричинити
     * невизначену поведінку, як у класі Foo.
     * У класі Foo у нас є конструктор який приимає два int, але якщо ми додамо конструктор який приймає
     * std::initializer_list<int> і викличмо Foo{1,2} то який конструктор має викликатися? З двома int чи std::initializer_list.
     *
     * Викличеться std::initializer_list
     */
    Foo f1{ 1, 2 }; // now calls Foo(std::initializer_list<int>)


    /**
     * Зверніть увагу, що якщо ви реалізуєте конструктор, який приймає std::initializer_list, вам слід переконатися,
     * що ви виконуєте принаймні одне з наступного:
     *  - Provide an overloaded list assignment operator
     *  - Provide a proper deep-copying copy assignment operator
     *  - Delete the copy assignment operator
     *
     *
     *
     *
     * У чому суть проблеми?
     * Коли ти пишеш array = { 1, 3, 5, 7, 9, 11 };, а у твоєму класі немає спеціального оператора присвоювання для
     * списку (тобто немає operator=(std::initializer_list<int>)), компілятор не здається.
     * Він намагається "врятувати" ситуацію і робить наступне:
     *     1) Створює тимчасовий об'єкт: Він бачить список {1, 3...} і бачить, що у тебе є конструктор IntArray(std::initializer_list<int>).
     *        Він викликає цей конструктор, щоб створити тимчасовий (анонімний) об'єкт IntArray.
     *
     *     2) Викликає копіювання: Тепер у нього є тимчасовий об'єкт праворуч від знака = і твій об'єкт array ліворуч.
     *        Він викликає стандартний (implicit) оператор присвоювання, який просто копіює значення членів класу.
     *
     *     3) Поверхневе копіювання (Shallow Copy): Оскільки ти не написав власний оператор присвоювання, компілятор просто
     *        копіює вказівник: array.m_data = temporary.m_data. Тепер два об'єкти вказують на одну і ту саму ділянку пам'яті в купі (heap).
     *
     *     4) Катастрофа: Рядок закінчується. Тимчасовий об'єкт більше не потрібен, і викликається його деструктор. Деструктор робить delete[] m_data.
     *
     * Результат: Твій основний об'єкт array тепер має вказівник m_data, який веде в "пустоту" (пам'ять уже звільнена).
     * Це dangling pointer (вказівник, що завис). Наступна спроба звернутися до array — це Undefined Behavior.
     *
     * Саме тому кожен із запропонованих трьох варіантів вище підійде для вирішення проблеми. У нашому класі intArray ми використали
     * останній спосіб - видалити (позначити delete) конструктор копіювання і оператор присвоєння
     */

    return 0;
}