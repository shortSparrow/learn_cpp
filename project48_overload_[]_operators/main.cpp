#include <iostream>

/**
 * Overload [] operator
 *
 * Наш клас IntList має поле з C-style array і воно приватне, тож якщо ми хочемо отримати до нього
 * доступ, то треба писати гетер, або зробити overload []
 *
 * Або це може бути корисно, якщо ми хочемо додати на C-style array boundary check, тобто обробити
 * випадок якщо індекс більший за допомогою масиву зв допомогою assert
 *  assert(index >= 0 && static_cast<std::size_t>(index) < std::size(m_list));
 * Або зробити перевірку на if
 */

class IntList {
private:
    int m_list[10]{};

public:

    // Для non-const objects: Може бути використана для читання та запису
    int& operator[] (int index) {
        // оскільки як тип ми вказали reference то компілятор автоматично виправити наш return на reference
        return m_list[index];
    }

    // Для const objects: може бути використана лише для читання
    // This function could also return by value if the type is cheap to copy
    const int& operator[] (int index) const {
        return m_list[index];
    }
};

int main() {
    // Для читання та запису
    IntList list{};
    list[2] = 3; // set a value
    std::cout << list[2] << '\n'; // 3

    // Лише для читання
    const IntList clist{};
    // clist[2] = 3; // compile error: clist[2] returns const reference, which we can't assign to
    std::cout << clist[2] << '\n';

    /**
     *
     * З pointer так не зпрацює, оскільки коли мова про pointer то для оператора []
     * використовується адреса початку (перший елемент) + кількість_байт_об'єкта * індекс,
     * тобто коли я пишу list[2] = 3 компілятор зробить (адреса list) + 2 * sizeof(IntList) = 3
     *
     * Але оскільки компілятор не може перетоврити list у число (ми не написали контуруктор, який прийма би int як арггумент - IntList(int x))
     * Я хуй зна нашо йому перетоврювати list(IntList) на число, там є якась логіка, але я її не розумію, алк якщо додати констуркутор
     * IntList(int x) то помилка зникне, але все одно результат буде непередбучуваний
     *
     *
     * Загалом НЕ можена перевантажуваня оператор [] на pointer різних об'єктів
     */
    IntList* list2{ new IntList{} };
    // list2 [2] = 3; // error: this will assume we're accessing index 2 of an array of IntLists
    (*list2)[2] = 3; // Ось так буде працювати, бо з початку ми отримали IntList object, а вже потім викликали overloaded operator[]
    delete list2;

  return 0;
}