#include <exception> // for std::exception
#include <iostream>
#include <string>
#include <string_view>

class ArrayException : public std::exception {
private:
    std::string m_error{}; // handle our own string

public:
    ArrayException(std::string_view error) : m_error{error} {}

    // std::exception::what() returns a const char*, so we must as well
    const char* what() const noexcept override { return m_error.c_str(); }
};

class IntArray {
private:
    int m_data[3] {}; // assume array is length 3 for simplicity

public:
    IntArray() {}

    int getLength() const { return 3; }

    int& operator[](const int index) {
        if (index < 0 || index >= getLength())
            throw ArrayException("Invalid index");

        return m_data[index];
    }

};

/**
 * Ми можемо створювати власні кастомні класи з помилками. Ці класи будемо успадковувати від класу std::exception.
 * І ми так само можемо перевизначити метод .what()
 *
 * Або можна створити клас і успадкуватися від std::runtime_error, це теж ціком доцільно
 */
int main() {
    IntArray array;

    try {
        int value{ array[5] };
    } catch (const ArrayException& exception) { // derived catch blocks go first
        std::cerr << "An array exception occurred (" << exception.what() << ")\n"; // An array exception occurred (Invalid index)
    } catch (const std::exception& exception) {
        std::cerr << "Some other std::exception occurred (" << exception.what() << ")\n";
    }



    /**
     * The lifetime of exceptions
     * Коли стається exception, то компілятор робить копію об'єкта exception десь поза call stack,
     * і тоді навіть якщо stack розгорнеться (unwound) і дані будуть очищені, exception гарантовано
     * лишиться доки не буде оброблено
     */


}