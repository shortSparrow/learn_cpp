#include <iostream>

int g_x { 5 };

int main() {
     int x {1};
     int& x_ref {x};

     std::cout << x_ref << '\n'; // 1

     x = 2;
     std::cout << x << '\n'; // 2
     std::cout << x_ref << '\n'; // 2

     // & перед назвою змінною теж пропускається, але цього краще уникати
     int y {2};
     int &y_ref {y};

     // Різні типи
     int r {1};
     double& d {r}; // помилка, reference має повністю відповідати типу ідентифікатора посилання

     // Якщо об'єкт const а reference НЕ const то буде помилка
     const int a1 {1};
     int& a1_ref {a1}; // буде помилка, бо a1_ref не const

     const int a2 {2};
     const int a2_ref {a2}; // ok


     int a3{3};
     const int& a3_ref {a3}; // теж ок

     std::cout << a3_ref << '\n'; // 3
     a3 = 11;
     std::cout << a3_ref << '\n'; // 11

    // -----------------------------------------------------------------------



     // initialization reference with rvalue
     const int& ref { 5 }; // Це ок, буде посилання на числовий літерал
     std::cout << ref << '\n'; // prints 5
     /**
      * Коли це трапляється, створюється тимчасовий об'єкт та ініціалізується rvalue,
      * а посилання на const прив'язується до цього тимчасового об'єкта
      */


    // Посилання Lvalue на const можуть навіть прив'язуватися до значень іншого типу, якщо ці значення можна неявно перетворити на тип посилання
    char c { 'a' };
    const int& r2 { c };     // temporary int initialized with value 'a', r2 binds to temporary


    // -----------------------------------------------------------------------

    // ОБЕРЕЖНО
    /**
     * Зазвичай ми припускаємо, що посилання є ідентичним об'єкту, до якого воно прив'язане,
     * але це припущення не спрацьовує, коли посилання прив'язане до тимчасової копії об'єкта або до тимчасового результату
     * перетворення об'єкта. Будь-які зміни, внесені згодом до оригінального об'єкта, не будуть відображатися в посиланні
     * (оскільки воно посилається на інший об'єкт) і навпаки.
     *
     * А коли відбувається приведення типів тут, то для you створюється копія bombs яке приведене до int
     */
    short bombs { 1 };         // I can has bomb! (note: type is short)
    const int& you { bombs };  // You can has bomb too (note: type is int&)
    --bombs;
    std::cout << bombs << '\n'; // 0
    std::cout << you << '\n'; // 1

    // -----------------------------------------------------------------------

    /**
     * const expression
     * Посилання constexpr не може бути прив'язане до (нестатичної) локальної змінної. Це пояснюється тим,
     * що адреса локальних змінних невідома, доки не буде викликана функція, в якій вони визначені.
     */
    [[maybe_unused]] constexpr int& ref1 { g_x }; // ok, can bind to global

    static int s_x { 6 };
    [[maybe_unused]] constexpr int& ref2 { s_x }; // ok, can bind to static local

    int x { 6 };
    [[maybe_unused]] constexpr int& ref3 { x }; // compile error: can't bind to non-static object

     return 0;
}