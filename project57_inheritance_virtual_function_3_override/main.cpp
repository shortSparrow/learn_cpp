#include <iostream>
#include <string_view>

class A {
public:
    virtual std::string_view getName1(int x) { return "A"; }
    virtual std::string_view getName2(int x) { return "A"; }
};

class B : public A {
public:
    virtual std::string_view getName1(short x) { return "B"; } // note: parameter is a short
    virtual std::string_view getName2(int x) const { return "B"; } // note: function is const
};

//---------------------------------------------------------------------------------
class C {
public:
    virtual std::string_view getName1(int x) { return "A"; }
    virtual std::string_view getName2(int x) { return "A"; }
    virtual std::string_view getName3(int x) { return "A"; }
};

class D : public C {
public:
   // std::string_view getName1(short int x) override { return "B"; } // compile error, function is not an override
   // std::string_view getName2(int x) const override { return "B"; } // compile error, function is not an override
    std::string_view getName3(int x) override { return "B"; } // okay, function is an override of A::getName3(int)

};

int main() {
    B b{};
    A& rBase{ b };
    std::cout << rBase.getName1(1) << '\n'; // A
    std::cout << rBase.getName2(2) << '\n'; // A

    /**
     * В першому випадку виведеться "A" тому, що у класі A приймає int, а у B short
     * В другому випадку виведеться "A" тому, що у класі A функція не сonst, а у B const
     *
     * Тобто маємо розбіжності які компілятор вважає не перевантаженням через virtual function,
     * саме тому virtual key word працює не так як ми того хочемо
     */


    /**
     * Тут на допомогу приходить спеціальне слово override, якщо функція не буде перевантажена,
     * то компілятор одразу покаже помилку.
     * Як бачимо з класами C та D це працює просто чудово, ті методи які не будуть визначатися можливими
     * для перевантаження і на яких стоїть override одразу покажуть помилку.
     */

    D d {};
    C& c {d};
    std::cout << c.getName3(4) << '\n'; // B

    /**
     * Варто на кожну virtual function вішати override, бо це допоможе уникнути помилок
     * І також, якщо ми використовуємо override то нам не потрібно прописувати ключове слово virtual
     */



    /**
     * Інколи можуть бути випадки коли ми хочемо заборонити робити override на virtual function у нащадку.
     * Наприклад клас A має функції.
     *  virtual std::string_view getName() const { return "A"; }
     *
     * Клас B забороняю нащадку робити override
     *  std::string_view getName() const override final { return "B"; }
     *
     * Клас C отримає помилку якщо спробує зробити override
     *  std::string_view getName() const override { return "C"; } // compile error: overrides B::getName(), which is final
     */



    return 0;
}