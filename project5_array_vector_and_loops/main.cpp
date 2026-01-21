#include <iostream>
#include <ranges>
#include <vector>



int main() {
    /**
     * Проблема індексів у масивах під час ітерацій пов'язана з тим, щоб .size() повертає unsigned число типу std::size_t.
     *
     * std::size_t: Це тип, який гарантовано може вмістити розмір будь-якого об'єкта у пам'яті (включно з масивами).
     * Його розмір встановлюється компілятором так, щоб відповідати ширині адреси на цій конкретній архітектурі.
     *
     * Нижче наведені приклади чому це проблема і як з цим боротися
     */

    std::vector testScore { 84, 92, 76, 81, 56 };
    for (std::size_t i=0; i<testScore.size(); i++) {
        std::cout << testScore.at(i) << ' ';
    }
    std::cout << '\n';

    /**
     * Тут ми використовуємо перетворення на int, але це не дуже ок, оскільки довжина масиву визначається
     * типом std::size_t який може бути більшим за int тож я int може не покрити весь масив, тому правильніше було б
     * робити перетворення на std::ptrdiff_t
     *
     * std::ptrdiff_t — це знаковий аналог std::size_t.
     * Він гарантовано має той самий розмір. Тому він безпечно вмістить будь-який можливий індекс масиву.
     *
     * std::cout << testScore[i] << '\n';
     * може видавати warning у компілятора, оскільки оператор [] очікує std::size_t, а ми передаємо йому std::ptrdiff_t
     *
     * щоб цього не було треба писати
     * std::cout << arr[static_cast<std::size_t>(index)] << ' ';
     */

    int length {static_cast<int>(testScore.size())};
    for (int i = length-1; i>=0; i--) {
        std::cout << testScore[i] << ' ';
    }
    std::cout << '\n';


    // Правильний варіант
    std::ptrdiff_t length2 {static_cast<std::ptrdiff_t>(testScore.size())};
    for (auto i = length2-1; i>=0; i--) {
        std::cout << testScore[static_cast<std::size_t>(i)] << ' ';
    }
    std::cout << '\n';



    /**
     * std::ssize introduced in C++20
     * std::ssize повертає signed число типу std::ptrdiff_t
     */
    for (auto index{ std::ssize(testScore)-1 }; index >= 0; --index) {
        std::cout << testScore[static_cast<std::size_t>(index)] << ' '; // static_cast щоб не було warning вді компілятора
    }
    std::cout << '\n';


    /**
     * .size() - повертає беззнакове число типу std::size_t
     *
     * 1. Головна пастка (Чому це небезпечно) Стандартна бібліотека C++ (наприклад, для std::vector) використовує
     * беззнакові цілі числа (unsigned, наприклад size_t) для зберігання розміру масиву та індексів.
     * Але беззнакові числа мають неприємну властивість: вони не можуть бути від'ємними.
     *
     * Якщо ви спробуєте написати цикл, що йде у зворотному напрямку (від останнього елемента до 0), і використаєте
     * беззнаковий тип: for (unsigned int i = size - 1; i >= 0; --i) ...це призведе до нескінченного циклу.
     * Коли i дорівнює 0 і ви робите i--, воно не стає -1. Воно «переповнюється» і стає величезним
     * додатним числом (максимальним для цього типу). Умова i >= 0 завжди істинна.
     *
     *
     * Як варіант використовувати static_cast до int, або std::ssize() (з'явилося лише в C++20)
     * Але краще взагалі не використовувати індекси вручну, краще скористатися range або iterators
     */




    //...............................Range-based loop..................................................

    // Range-based for loops
    std::vector fibonacci { 0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89 };

    for (int item:fibonacci) { // item повинен мати той самий тип що і елементи масиву (можна використати auto)
        std::cout << item << ' ';
    }
    std::cout << '\n';


    // Варто використовувати reference, інакше у word буде робитися копіювання елементів масиву що є дорогим
    std::vector<std::string> words{ "peter", "likes", "frozen", "yogurt" };

    for (const auto& word : words) {  // word is now a const reference
        std::cout << word << ' '; // peter likes frozen yogurt
    }
    std::cout << '\n';

    // Range-based for loops in reverse C++20
    for (const auto& word: std::views::reverse(words)) {
        std::cout << word << ' '; // yogurt frozen likes peter
    }
    std::cout << '\n';


    // Оскільки word reference, то можна поміняти значення в оригінальному масиві
    for (auto& word : words) {
        if (word == "peter") {
            word = "egs";
        }
    }
    for (auto& word : words) {
        std::cout << word << ' '; // egs likes frozen yogurt
    }
    std::cout << '\n';

    /**
     * auto when you want to modify copies of the elements.
     * auto& when you want to modify the original elements.
     * const auto& otherwise (when you just need to view the original elements).
     */


    /**
     * Range-based for loops won’t work with decayed C-style arrays. This is because a range-based for-loop
     * needs to know the length of the array to know when traversal is complete, and decayed C-style arrays
     * do not contain this information.
     */



    return 0;
}