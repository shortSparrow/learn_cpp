#include <functional>
#include <iostream>

int get_integer() {
    std::cout << "write an integer: ";
    int val {};
    std::cin >> val;

    return val;
}

char get_operation() {
    char op{};

    do {
        std::cout << "Enter an operation ('+', '-', '*', '/'): ";
        std::cin >> op;
    }
    while (op!='+' && op!='-' && op!='*' && op!='/');

    return op;
}


int add(int x, int y) {
    return x + y;
}

int sub(int x, int y) {
    return x - y;
}

int multiplication(int x, int y) {
    return x * y;
}

int division(int x, int y) {
    return x / y;
}

using OperationFuncType = std::function<int(int,int)>;

OperationFuncType select_operation(char operation) {
    switch (operation) {
    case '*':
        return &multiplication;
    case '+':
        return &add;
    case '-':
        return &sub;
    case '/':
        return &division;
    }
}



int main() {

    // Калькулятор з function pointers
    int x {get_integer()};
    char math_operation {get_operation()};
    int y {get_integer()};

    OperationFuncType make_calculation {select_operation(math_operation)};
    std::cout << x << math_operation << y << " = " << make_calculation(x,y);

    return 0;
}
