#include <assert.h>
#include <iostream>


/**
 * Напишіть власний клас цілочисельного масиву з назвою IntArray з нуля (не використовуйте std::array або std::vector).
 * Користувачі повинні вказати розмір масиву під час його створення, а масив повинен бути динамічно алокований.
 * Використовуйте оператори assert для захисту від неправильних даних. Створіть будь-які конструктори або перевантажені
 * оператори, необхідні для коректної роботи програми
 */


class IntArray {
    int* arr {nullptr};
    size_t length;
public:
    explicit IntArray(const size_t size): length{size} {
        assert(size > 0 && "IntArray length should be a positive integer");
        arr = new int[size];
    }

    ~IntArray() {
        delete[] arr;
    }

    IntArray(const IntArray& source): length{source.length} {
        // у конструкторі копіювання об'єкт тільки-но народжується, тому видаляти ще нічого. Тому delete[] тут не потрібен
        arr = new int[length];

        // створюємо незалежну копію
        for (size_t i{0}; i<length; i++) {
            arr[i] = source.arr[i];
        }
    }

    IntArray& operator= (const IntArray& source) {
        if (this == &source) {
            std::cout <<"You try to assinemnt the same class" << '\n';
            return *this;
        }

        // Якщо цей масив вже існує, видалимо його, щоб він не займав пам'ять (бо вона стане dangling)
        delete[] arr;
        length = source.length;

        arr = new int[length];

        for (size_t i{0}; i<length; i++) {
            arr[i] = source.arr[i];
        }

        // return the existing object so we can chain this operator
        return *this;
    }


   int& operator[] (const int idx) const {
       assert(idx >= 0);
       assert(idx < length);

       return arr[idx];
    }


    friend std::ostream& operator<< (std::ostream& cout, const IntArray& source) {
        for (size_t i {0}; i<source.length; ++i) {
            cout << source.arr[i] << ' ';
        }

        return cout;
    }
};


IntArray fillArray() {
    IntArray a(5);

    a[0] = 5;
    a[1] = 8;
    a[2] = 2;
    a[3] = 3;
    a[4] = 6;

    return a;
}


int main() {

    IntArray a{ fillArray() }; // Створили масив [5,8,2,3,6]

    std::cout << a << '\n'; // 58236

    auto& ref{ a }; // we're using this reference to avoid compiler self-assignment errors
    a = ref; // буде посилання на самого себе

    IntArray b(1); // Створили масив [1]

    /**
     * b був масив [1], а масив a [5,8,2,3,6], тепер відбувається глибоке копіювання масиву a у масив b (у b видаляється
     * старий масив, виділяється нова пам'ять під новий і туди заносяться нові значення). Тепер b = [5,8,2,3,6]
     */
    b = a;

    a[4] = 7; // a = [5,8,2,3,7]

    std::cout << a << '\n'; // 5 8 2 3 7
    std::cout << b << '\n'; // 5 8 2 3 6


  return 0;
}