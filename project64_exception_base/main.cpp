#include <iostream>
#include <string>
#include <cmath>

/**
 * Ключове слово catch використовується для обробки винятку для одного типу даних.
 *
 * Винятки базових типів можна перехоплювати за значенням, але винятки небазових типів слід
 * перехоплювати за константною посиланням, щоб уникнути створення зайвої копії (а в деяких
 * випадках — щоб запобігти slicing (це коли у нас є Derived тип, але ми приймаємо Base і відбувається
 * приведення дочірнього класу до батьківського).
 *
 *
 * Для exception перетворення типів не виконується (тому exception типу int не буде перетворено
 * на відповідність блоку catch з параметром double). Тобто якщо ми робимо
 *      catch(double) {}
 * Ми точно впевнені, що в середину цього блоку потрапить лише double. int сюди не потрапить.
 * Але хоч type promotion не відбувається, приведення дочірнього класу до батьківського відбувається
 *
 */




void D() {
    std::cerr << "D throwing int exception\n";
    throw - 1;
}

void C() {
    D();
}

void B() {
    try {
        C();
    } catch (double) { // not caught: exception type mismatch
        std::cerr << "BBBBB\n";
    }
}

void A() {
    try {
        B();
    } catch (int) { // exception caught here and handled
        std::cerr << "A caught int exception\n";
    }
}



int main() {

    try {
        throw -1;
    } catch (double) {
        std::cerr << "We caught an exception of type double\n";
    } catch (int x) {
        std::cerr << "We caught an int exception with value: " << x << '\n';
    } catch (std::string&) {
        std::cerr << "We caught an exception of type std::string\n";
    }

    // Спрацює лише catch на int



    // Ось більш реалістичний приклад
    double x {-4};
    try {
        if (x < 0.0)
            throw "Can not take sqrt of negative number";

        std::cout << "The sqrt of " << x << " is " << std::sqrt(x) << '\n';
    } catch (const char* exception) {
        std::cerr << "Error: " << exception << '\n';
    }
    // Error: Can not take sqrt of negative number



    std::cerr << "--------------------------------------\n";
    try {
        A();
    } catch (int) { // not called because exception was handled by A
        std::cerr << "main caught int exception\n";
    }

    return 0;
}
