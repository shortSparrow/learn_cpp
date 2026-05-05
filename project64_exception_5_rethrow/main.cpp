#include <iostream>

class Base {
public:
    Base() {}
    virtual void print() { std::cout << "Base"; }
};

class Derived: public Base {
public:
    Derived() {}
    void print() override { std::cout << "Derived"; }
};

int main() {

    /**
    * Припустимо що ми відловили exception але не хочемо обробляти його в цьому блоці, або
    * наи треба передати інфу що сталася помилка далі в коді. Ми можемо додати інформацію про
    * exception у лог файл і викинути (rethrow) той самий exception.
    *
    * Але тут є проблемка, коли ми відловили помилку у catch то перед наступним throw компілятор викине не її,
    * він зробить її копію. І тут буде проблема, якщо ми отримали &Base від Derived об'єкта. Інформація
    * що це Derived буде втрачена для наступного catch
    */

    try {
        try {
            throw Derived{};
        } catch (Base& b) {
            std::cout << "Caught Base b, which is actually a ";
            b.print();
            std::cout << '\n';
            throw b; // the Derived object gets sliced here
        }
    } catch (Base& b) {
        std::cout << "Caught Base b, which is actually a ";
        b.print();
        std::cout << '\n';
    }
    /**
     * Буде виведено у термінал
     * Caught Base b, which is actually a Derived
     * Caught Base b, which is actually a Base
     */


    std::cout << "------------------------------------------------\n";
    /**
     * На щастя є спосіб це виправити, нам треба робити не throw b, а просто throw
     */
    try {
        try {
            throw Derived{};
        } catch (Base& b) {
            std::cout << "Caught Base b, which is actually a ";
            b.print();
            std::cout << '\n';
            throw; // the Derived object gets sliced here
        }
    } catch (Base& b) {
        std::cout << "Caught Base b, which is actually a ";
        b.print();
        std::cout << '\n';
    }

    /**
     * Буде виведено у термінал
     * Caught Base b, which is actually a Derived
     * Caught Base b, which is actually a Derived
     *
     * Бо тут ми робимо у catch не throw копії exception, ми робимо re-throw цього exception
     */

    return 0;
}
