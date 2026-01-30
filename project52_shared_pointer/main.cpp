#include <iostream>
#include <memory>

/**
 * std::shared_ptr is the smart pointer class used when you need multiple objects accessing the same resource.
 * The resource will not be destroyed until the last std::shared_ptr managing it is destroyed.
 */

class Resource {
public:
    Resource() { std::cout << "Resource acquired\n"; }
    ~Resource() { std::cout << "Resource destroyed\n"; }
};

void example_1() {
    // allocate a Resource object and have it owned by std::shared_ptr
    Resource* res { new Resource };
    std::shared_ptr<Resource> ptr1{ res };
    {
        std::shared_ptr<Resource> ptr2 { ptr1 }; // make another std::shared_ptr pointing to the same thing

        std::cout << "Killing one shared pointer\n";
    } // ptr2 goes out of scope here, but nothing happens

    std::cout << "Killing another shared pointer\n"; // Цей рядок треба для логічного по хронології виводу у термінал, він зовсім не означає що саме тут shared pointer знищується
}

void example_2() {
    Resource* res { new Resource };
    std::shared_ptr<Resource> ptr1 { res };
    {
        std::shared_ptr<Resource> ptr2 { res }; // create ptr2 directly from res (instead of ptr1)

        std::cout << "Killing one shared pointer\n";
    } // ptr2 goes out of scope here, and the allocated Resource is destroyed

    std::cout << "Killing another shared pointer\n"; // Цей рядок треба для логічного по хронології виводу у термінал, він зовсім не означає що саме тут shared pointer знищується
}

void example_3() {
    // allocate a Resource object and have it owned by std::shared_ptr
    auto ptr1 { std::make_shared<Resource>() };
    {
        auto ptr2 { ptr1 }; // create ptr2 using copy of ptr1

        std::cout << "Killing one shared pointer\n";
    } // ptr2 goes out of scope here, but nothing happens

    std::cout << "Killing another shared pointer\n"; // Цей рядок треба для логічного по хронології виводу у термінал, він зовсім не означає що саме тут shared pointer знищується
}

int main() {

    /**
     * На відміну від std::unique_ptr, яка призначена для окремого володіння та управління ресурсом,
     * std::shared_ptr призначена для вирішення випадку, коли вам потрібно кілька розумних вказівників,
     * що є співвласниками ресурсу.
     *
     * Поки хоча б один std::shared_ptr вказує на ресурс, ресурс не буде звільнено, навіть якщо
     * окремі std::shared_ptr будуть знищені. Щойно останній std::shared_ptr, що керує ресурсом, вийде за межі
     * області видимості (або буде перепризначений для вказівки на щось інше), ресурс буде звільнено.
     */


    example_1();
    /**
     * Ось що буде виведено у термінал:
     *
     * Resource acquired
     * Killing one shared pointer
     * Killing another shared pointer
     * Resource destroyed
     *
     * З початку ресурс було створено (Resource acquired), потім створено share_ptr, потім виходимо
     * з scope і shared_ptr видаляється (Killing one shared pointer) потім відпрацьовує
     * вивід (Killing another shared pointer) і в кінці функція закінчує виконання і вмирає сам
     * Resource і перший shared_ptr теж знищується (Resource destroyed)
     */

    std::cout << "***************************************************\n";

    // Закоментовано, бо падає програма і не виводить те що нижче
    // example_2();

    /**
     * Ось що буде виведено у термінал:
     *
     * Resource acquired
     * Killing one shared pointer
     * Resource destroyed
     * Killing another shared pointer
     * Resource destroyed
     *
     * Тут на відміну від прикладу вище було створено незалежну копію для другого share pointer,
     * ми створили її з Resource, а не з першого share pointer, тож наш Resource двічі знищиться і
     * на деяких машина це може призвести до крашу (на мої призвело), тому такої поведінки слід уникати.
     * До того ж це не буде класичний exception який можна відловити у try...catch, це буде "Undefined Behavior".
     * Якщо нам треба копія то треба її робити не з оригінального pointer, а уже зі створеного shared pointer
     */

    /**
     * Best practice
     *
     * Завжди створюйте копію існуючого std::shared_ptr, якщо вам потрібно більше одного std::shared_ptr, що вказує на той самий ресурс.
     */

    //*************************************************************
    std::cout << "********************** std::make_shared *****************************\n";

    /**
     * std::make_shared
     * Подібно до того, як std::make_unique() можна використовувати для створення std::unique_ptr у C++14,
     * std::make_shared() можна (і треба) використовувати для створення std::shared_ptr.
     *
     */

    example_3();
    /**
     * Ось що буде виведено у термінал:
     *
     * Resource acquired
     * Killing one shared pointer
     * Killing another shared pointer
     * Resource destroyed
     *
     * Причини використання std::make_shared() такі самі, як і для std::make_unique() — std::make_shared() є простішим і
     * безпечнішим (за допомогою цього методу неможливо створити два незалежних std::shared_ptr, що вказують на один і
     * той самий ресурс, але не знають один про одного).
     *
     */


    return 0;
}