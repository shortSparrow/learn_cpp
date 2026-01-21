#include <iostream>


class Cents {
private:
    int m_cents{};
public:
    Cents(int cents=0)
        : m_cents{ cents }
    {
    }

    // Overloaded int cast
    operator int() const { return m_cents; }

    /**
     * Якщо додати explicit, то воно заборонить неявне (implicit) перетворення на int,
     * тобто без explicit у нас можливий варіант
     * Cents cent {10};
     * stc::cout << cents
     *
     * cents неявно приведеться до int, бо компілятор бачить що він може до нього привестися, але якщо
     * буде explicit, то таке вже не проканає і треба буде робити явне перетворення
     * Cents cent {10};
     * stc::cout << static_cast<int>(cents)
     *
     * Завжди краще додавати explicit щоб уникнути непередбачуваного перетворення
     */
    // explicit operator int() const { return m_cents; }


    int getCents() const { return m_cents; }
    void setCents(int cents) { m_cents = cents; }
};


class Dollars {
private:
    int m_dollars{};
public:
    Dollars(int dollars=0)
        : m_dollars{ dollars }
    {
    }

    // Allow us to convert Dollars into Cents
    operator Cents() const {
        return {Cents {m_dollars * 100}};
    }
};


int main() {

  /**
   * Overloading typecasts
   *
   * int n{ 5 };
   * auto d{ static_cast<double>(n) }; // int cast to a double
   * C++ вже знає, як конвертувати вбудовані типи даних, однак він не знає як конвертувати
   * наші власні класи
   *
   * Клас Cents перевантажує  operator int і повертає m_cents
   */

    Cents cents{ 7 };
    std::cout << static_cast<int>(cents) << '\n'; // 7


    /**
     * Що дуже цікаво, так це те, що ми можемо перетворювати одні класи на інші, до прикладу
     * ми можемо закастувати тип Dollars до типу Cents
     */
    Dollars dollars{ 9 };
    Cents cents2 {static_cast<Cents>(dollars)};
    std::cout << cents2.getCents() << '\n'; // 900


    /**
     * Кращою практикою певно буде замість typecast використати перетворення через конструктори,
     * тобто замість overload Cents у Dollars ми у Cents напишемо конструктор, який приймає Dollars і повертає клас Cents.
     *
     * Виключенням будуть випадки коли треба привести клас до вбудованих типів (int, bool, ...) або до класів, доступу
     * до яких ми не маємо, наприклад до std::vector або класів з інших бібліотек
     */


  return 0;
}