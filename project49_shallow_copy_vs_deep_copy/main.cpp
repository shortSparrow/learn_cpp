#include <iostream>
#include <cstring> // for strlen()
#include <cassert> // for assert()

class S1 {
public:
    int m_int {};
    S1(int value): m_int{value} {

    }
};



class MyString {
private:
    char* m_data{};
    int m_length{};

public:
    MyString(const char* source = "" ) {
        assert(source); // make sure source isn't a null string

        // Find the length of the string
        // Plus one character for a terminator
        m_length = std::strlen(source) + 1;

        // Allocate a buffer equal to this length
        m_data = new char[m_length];

        // Copy the parameter string into our internal buffer
        for (int i{ 0 }; i < m_length; ++i)
            m_data[i] = source[i];
    }

    // destructor
    ~MyString() {
        // We need to deallocate our string
        delete[] m_data;
    }

    char* getString() { return m_data; }
    int getLength() { return m_length; }
};



class MyStringSafe {
private:
    char* m_data{};
    int m_length{};

public:
    MyStringSafe(const char* source = "" ) {
        assert(source); // make sure source isn't a null string

        // Find the length of the string
        // Plus one character for a terminator
        m_length = std::strlen(source) + 1;

        // Allocate a buffer equal to this length
        m_data = new char[m_length];

        // Copy the parameter string into our internal buffer
        for (int i{ 0 }; i < m_length; ++i)
            m_data[i] = source[i];
    }

    /*
     * Самі прописали конструктор копіювання і використовуємо у ньому наш deepCopy, щоб навіть pointer
     * скопіював значення поля, а не просто посилання
     */
    MyStringSafe(const MyStringSafe& source) {
        deepCopy(source);
    }

    void deepCopy(const MyStringSafe& source) {
        // first we need to deallocate any value that this string is holding!
        delete[] m_data;

        // because m_length is not a pointer, we can shallow copy it
        m_length = source.m_length;

        // m_data is a pointer, so we need to deep copy it if it is non-null
        if (source.m_data) {
            // allocate memory for our copy
            m_data = new char[m_length];

            // do the copy
            for (int i{ 0 }; i < m_length; ++i)
                m_data[i] = source.m_data[i];
        }
        else
            m_data = nullptr;
    }

    // destructor
    ~MyStringSafe() {
        // We need to deallocate our string
        delete[] m_data;
    }

    char* getString() { return m_data; }
    int getLength() { return m_length; }
};

int main() {

    // Копії повністю незалежні
    S1 s0 {5};
    S1 s1 = s0;

    s0.m_int = 3;
    std::cout << s1.m_int << '\n'; // 5


    /**
     * Shallow copying (Поверхневе копіювання)
     *
     * Здавалось би цей приклад має працювати як треба, але є нюанс, і ось у чому він полягає:
     *  Проблема у MyString copy{ hello }
     *  Ми робимо копіювання через конструктор, але ми не вказали його самотужки, тож компілятор пише його сам, і ось як він виглядає
     *      MyString::MyString(const MyString& source)
     *      : m_length { source.m_length }
     *      , m_data { source.m_data } {
     *      }
     *  Це стандартний конструктор копіювання який пише компілятор і тут він застосовує
     *  memberwise (shallow) copying (почленне (неглибоке) копіювання), тобто він почленно копіює кожне поле, а оскільки
     *  поле m_data у нас char*, тобто pointer то він і зкопіює АДРЕСУ тож вийде що наша копія і оригінал матимуть
     *  однакове поле m_data бо воно посилатиметься на одну адресу у пам'яті.
     *  Тож коли ми вийдемо за дужки {} то спрацює деструктор і позначить m_data як вільну для запису
     *
     *
     * Виправити ситуацію може використання deepCopy і застосування його у конструкторі копіювання. У методі deepCopy ми реалізуємо
     * повне копіювання, тобто якщо це pointer, то ми ідемо у нього, дістаємо його значення і робимо їх копію.
     *
     *
     *
     * Note that we have not defined a copy constructor or overloaded assignment operator
     */

    MyString hello{ "Hello, world!" };
    {
        MyString copy{ hello }; // use default copy constructor
    } // The destructor deletes copy's string, which leaves hello with a dangling pointer

    std::cout << hello.getString() << '\n'; // this will have undefined behavior


    /**
     * Правило трьох
     * Якщо клас вимагає визначеного користувачем деструктора, конструктора копіювання або оператора присвоювання (assignment =),
     * то, ймовірно, він вимагає всіх трьох. Ось чому. Якщо ми визначаємо будь-яку з цих функцій, то, ймовірно, це тому, що маємо справу
     * з динамічним розподілом пам'яті. Нам потрібен конструктор копіювання та присвоювання (assignment =) для обробки глибоких копій,
     * а також деструктор для звільнення пам'яті.
     *
     *
     * Підсумок
     *  - Конструктор копіювання за замовчуванням та оператори присвоювання за замовчуванням виконують поверхневе копіювання,
     *    що підходить для класів, які не містять динамічної алокації даних.
     *  - Класи з динамічно алокованими даними повинні мати конструктор копіювання та оператор присвоювання, які виконують глибоке копіювання.
     *
     */

  return 0;
}