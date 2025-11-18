#include <iostream>
#include <string>

// The function parameter is a copy of str
void printByValue(std::string val) {
    std::cout << val << '\n'; // print the value via the copy
}

// The function parameter is a reference that binds to str
void printByReference(const std::string& ref) {
    std::cout << ref << '\n'; // print the value via the reference
}

// The function parameter is a pointer that holds the address of str
void printByAddress(const std::string* ptr) {
    std::cout << *ptr << '\n'; // print the value via the dereferenced pointer
}

// Якщо pointer не передано, то присвоюється значення за замовченням nullptr
void printIDNumber(const int *id=nullptr) {
    if (id)
        std::cout << "Your ID number is " << *id << ".\n";
    else
        std::cout << "Your ID number is not known.\n";
}

// Приймаємо reference на pointer
void nullify(int*& refptr) {
    refptr = nullptr; // Make the function parameter a null pointer
}

int main() {
    std::string str{ "Hello, world!" };

    printByValue(str); // pass str by value, makes a copy of str
    printByReference(str); // pass str by reference, does not make a copy of str
    printByAddress(&str); // pass str by address, does not make a copy of str, but make copy of pointer - very fast (pointer is 4 or 8 byte)

    // це теж саме що і printByAddress(&str), просто інакше записано, ми передаємо pointer, а не адресу у пам'яті
    std::string* ptr { &str }; // define a pointer variable holding the address of str
    printByAddress(ptr); // pass str by address, does not make a copy of str


    // присвоєння параметру значення за замовченням
    printIDNumber(); // we don't know the user's ID yet

    int userid { 34 };
    printIDNumber(&userid); // we know the user's ID now

    // Передаємо reference на pointer. Не знаю на біса воно треба, але так можна :)
    int x {5};
    int* ref_x {&x};
    nullify(ref_x);

    return 0;
}