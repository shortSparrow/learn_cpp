#include <iostream>
#include "foo.h"

void print()
{
    std::cout << "Hello from main" << "\n";
}

namespace Baz {
    void print() {
        std::cout << "Baz here" << "\n";
    }
}

namespace Lop{
    void print() { // this print() lives in the Foo namespace
        std::cout << "Hello from Lop \n";
    }

    void printHelloThere() {
        print();   // calls print() in Lop namespace
        ::print(); // calls print() in global namespace
    }
}


// Вкладені namespace
namespace Nested1{
    namespace Nested2 { // Goo is a namespace inside the Foo namespace
        int add(int x, int y) {
            return x + y;
        }
    }
}

// Так само вкладені namespace, але новий варіант запису
namespace NestedNew1::NestedNew2 {
    int add(int x, int y)
    {
        return x + y;
    }
}

namespace Nested1 {
    void someOhterFunc() {

    }
}

int main() {

    Baz::print(); // print з namespace Baz
    ::print(); // print з глобального namespace (те саме що просто написати print()


    Lop::printHelloThere();

    std::cout << Foo::doSomething(1,3) << "\n";

    Nested1::Nested2::add(1,2);
    Nested1::someOhterFunc();

    return 0;
}