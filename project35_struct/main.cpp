#include <iostream>
#include <optional>


/**
 * Struct - це program-defined type
 * Він потрібний, щоб мати структуру об'єкта, наприклад є employee з 10 полями,
 * от щоб не робити 10 змінних можна все запахти у struct
 *
 */

// Employee is just a type definition -- no objects are actually created at this point.
struct Employee {
    int id {};
    int age {};
    double weight {};
    double height {170.2};
};


struct Something {
    int x;       // no initialization value (bad)
    int y {};    // value-initialized by default
    int z { 2 }; // explicit default value
};


struct Point3d {
    double x { 0.0 };
    double y { 0.0 };
    double z { 0.0 };

    void printValues() {
        std::cout << "x: " << x << ", y: " << y << ", z: " << z << '\n';
    }
};


// Компілятор достатьо розумний щоб зрозуміти що ми стоврюємо struct який зазначений у типі повернення
Point3d getZeroPoint(){
    // We already specified the type at the function declaration
    // so we don't need to do so here again
    return { 0.0, 0.0, 0.0 }; // return an unnamed Point3d
}

Point3d getZeroPoint2(){
    // We can use empty curly braces to value-initialize all members
    return {};
}


int main() {

    Employee joe {};
    joe.id = 1;
    joe.age = 32;
    std::cout << joe.weight << '\n'; // 0 - undefined behavior

    Employee liubov {2,28,65.5}; // Це найкращий спосіб
    Employee shahs = {2,28,65.5};
    Employee robert ( 3, 45, 62500.0 );

    Employee maria { .id {3}, .age {45}, .weight {24.1}}; // Це найкращий спосіб якщо треба іменовані параметри
    Employee oleks { .id=3, .age=45, .weight = 24.1}; // Це найкращий спосіб якщо треба іменовані параметри
    Employee frieren { .id=3, .age={45}, .weight{24.1}};


    // Можна перевизначати та копіювати дані
    Employee nick {14,34,133};
    nick = {.id=nick.id, .age=35, .weight = 155, .height = nick.height};


    // Default initialization

    Something s1; // s1.x is uninitialized, s1.y is 0, and s1.z is 2

    // s3.x (which has no default member initializer) is value initialized to 0
    Something s3 {}; // value initialize s3.x, use default values for s3.y and s3.z


    Something s10;             // No initializer list: s1.x is uninitialized, s1.y and s1.z use defaults
    Something s20 { 5, 6, 7 }; // Explicit initializers: s2.x, s2.y, and s2.z use explicit values (no default values are used)
    Something s30 {};          // Missing initializers: s3.x is value initialized, s3.y and s3.z use defaults


    // У struct можна покласти функції
    Point3d point3d {Point3d{1,2,3}};
    point3d.printValues();

    return 0;
}