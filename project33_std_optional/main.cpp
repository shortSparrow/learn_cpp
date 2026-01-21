#include <iostream>
#include <optional>

/**
 * std::optional<T>
 * У функції intDivision є обмеження на y, оскільки якщо користувач передасть 0,
 * то ми не зможемо виконати ділення, оскільки ділення на 0 повертає невизначеність.
 *
 * Але що ж тоді повернути якщо y=0? Звісно можна 0, але це не завжди підійде, раптом
 * треба якось повідомити користувача що це не просто 0, а не валідне значення.
 * std::optional<T> є один із способів це зробити, отже результат буде або типу T, який
 * зазначений у <>, або std::nullopt
 *
 * To see if a std::optional has a value, we can choose one of the following:
 * if (o1.has_value()) // call has_value() to check if o1 has a value
 * if (o2)             // use implicit conversion to bool to check if o2 has a value
 *
 */
std::optional<int> intDivision(const int x, const int y) {
    if (y == 0) {
        return  {}; // або можна std::nullopt
    }

    return  x/y;
}

void printIDNumber(std::optional<const int> id = std::nullopt) {
    if (id)
        std::cout << "Your ID number is " << *id << ".\n";
    else
        std::cout << "Your ID number is not known.\n";
}


struct Employee {
    std::string name{}; // expensive to copy
    int id;
};

void printEmployeeID(std::optional<std::reference_wrapper<Employee>> e=std::nullopt)
{
    if (e)
        std::cout << "Your Employee is " << e->get().id << ".\n";
    else
        std::cout << "Your Employee ID is not known.\n";
}

// Те саме, що і функція вище, але тут передаємо параметр як pointer
void printEmployeeID(const Employee* e=nullptr)
{
    if (e)
        std::cout << "Your ID number is " << e->id << ".\n";
    else
        std::cout << "Your ID number is not known.\n";
}

int main() {
    std::optional<int> res {intDivision(2,0)};

    if (res)
        std::cout << "Result 1: " << *res << '\n';
    else
        std::cout << "Result 1: failed\n"; // Result 1: failed

    printIDNumber(); // Your ID number is not known.

    printEmployeeID(); // Your Employee ID is not known.


    return 0;
}