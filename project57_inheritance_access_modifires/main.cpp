#include <iostream>



/**
 * У нас є три види видимості змінної/метода у класі
 *  public - видно всім, і самому класу і його нащадку і інстансу класу
 *  private - видно лише самому класі. Ні нащадок ні інстанс класу не може його бачити
 *  protected - видно самому класу і нащадку. Інстанс не бачить.
 */


class Base {
public:
    int a {1};
private:
    int b {2};
protected:
    int c {3};

public:
    Base() {

    }
};


class Derived : public Base {
public:
    Derived() {
        std::cout << "a from Derived constructor " << this->a << "\n"; // видимий для нас
        std::cout << "c from Derived constructor " << this->c << "\n";; // видимий для нас
    }
};


/**
 * Але модифікатори доступу public/private/protected існують не лише для змінних чи методів, їх
 * можна також використовувати і для конструкторів батьківських класів.
 *
 * Якщо модифікатор не вказати, то C++ за замовченням візьме private
 *
 *
 * public - це найпошириніший тип успадкування.
 *  Коли ми обираємо модифікатор успадкування public, то у нас все доволі прямолінійно,
 *  всі public методи/змінні успадкувалися як public, всі protected як protected, а всі
 *  private як private і лишаються недоступними.
 *
 * protected - значно менш поширений тип успадкування.
 *  Коли ми обираємо модифікатор успадкування protected, то у нас є певні зміни, а саме:
 *  всі public та protected методи/змінні успадкувалися як protected, а всі private як private і тому лишаються недоступними.
 *  Відповідно екземпляр класу не матиме доступу до жодного з полів батьківського класу, бо вони всі стануть або
 *  protected або private.
 *  А в середині дочірнього класу ми зможемо використати public/protected поля батьківського класу, бо вони всі успадковані для нас
 *  тепер як protected для Child класу. А private поля Parent лишають приватними і ми їх не бачимо
 *
 *  private - так само значно менш поширений тип успадкування.
 *  При цьому типі успадкування всі поля батьківського класу успадковуються як private, відповідно
 *  екземпляр класу не матиме доступу до всіх батьківських полів, а в середині дочірнього
 *  класу ми будемо мати доступ до public/protected полів батьківського бо вони всі успадковані як
 *  private для Child класу
 */

class Parent {
public:
    int a {1};
protected:
    int b {1};
private:
    int c {1};
};

class Child : protected Parent {
public:

    Child() {
        // a, b, доступні, бо ми викликали успадкування Parent як protected, відповідно
        // ми маємо доступу до його public та protected методів/полів і вони для нас всі мають
        // модифікатор protected

        // c нам недоступний, бо він private і доступний лише Parent

        std::cout << "value of a from Parent class: " << a << '\n';
        std::cout << "value of b from Parent class: " << b << '\n';
        // std::cout << "value of c from Parent class: " << c << '\n';
    }
};


int main() {

    Derived derived {};
    std::cout <<  "derived.a is visible " << derived.a << '\n';  // derived.b та derived.c не видимі для нас

    Child ch {};  // екземпляр класу немає жодних змінних батьківського класу бо модифікатор успадкування був protected


    return 0;
}
