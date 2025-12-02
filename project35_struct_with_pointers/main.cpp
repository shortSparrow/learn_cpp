#include <iostream>

struct Role {
    int name {};
};

struct Employee {
    int id{};
    int age{};
    double wage{};

    Role role {};
    Role* rolePtr {};
};


struct Point {
    double x {};
    double y {};
};

struct Triangle {
    Point* a {};
    Point* b {};
    Point* c {};
};

int main() {
    Role role {15};
    Role* rolePtr {&role};

    Employee joe{ 1, 34, 65000.0 };
    // joe.role = rolePtr; // error
    joe.rolePtr = rolePtr;

    ++joe.age;
    joe.wage = 68000.0;

    // ............................................

    // Доступ до значення з pointer
    Employee* ptrJoe{ &joe };
    // std::cout << ptr.id << '\n'; // Compile error: can't use operator. with pointers
    std::cout << (*ptrJoe).id << '\n'; // Not great but works: First dereference ptr, then use member selection

    std::cout << ptrJoe -> id << '\n'; // Це більш зручний синтаксис аби не писати (*ptr).id

    // ............................................

    Point a {1,2};
    Point b {3,7};
    Point c {10,2};

    Triangle tr { &a, &b, &c };
    Triangle* ptrTr {&tr};

    // ptr is a pointer to a Triangle, which contains members that are pointers to a Point
    // To access member y of Point c of the Triangle pointed to by ptr, the following are equivalent:

    // access via operator.
    std::cout << (*(*ptrTr).c).y << '\n'; // ugly!

    // access via operator->
    std::cout << ptrTr -> c -> y << '\n'; // much nicer



    // ......................................
    /**
     * Mixing pointers and non-pointers to members
     *
     * pumaPtr - це pointer на puma відповідно через -> отримуємо доступ до paw.
     * Але тпер маючи доступ до paw -> більше не потрібен бо paw не pointer, тому можемо
     * отримати доступ до значення через "."
     */
    struct Paw {
        int claws{};
    };

    struct Animal {
        std::string name{};
        Paw paw{};
    };

    Animal puma {"Puma", {5}};
    Animal* pumaPtr {&puma};

    std::cout << pumaPtr->paw.claws << '\n'; // 5






    return 0;
}