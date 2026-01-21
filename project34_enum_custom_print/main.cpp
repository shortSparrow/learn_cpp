#include <iostream>

/**
 * Якщо ви хочете виводити у термінал enum не як цифрові індекси,
 * чим власне enum і є, а більш зручно то це можна зробити двома шляхами:
 *  1) Написати функцію getName(enum)
 *  2) Перевантажити (overload) оператор виводу <<
 *
 */

enum Color {
    black,
    red,
    blue,
};


/**
 * Ми повертаємо string_view, але оскільки "black" - це C-style string,
 * то у них час життя до закінчення програми, тож вони не знищаться після
 * завершення функції getColorName і string_view зможе на них посилатися
 */
constexpr std::string_view getColorName(Color color) {
    switch (color) {
        case black: return "black";
        case red:   return "red";
        case blue:  return "blue";
        default:    return "???";
    }
}


/**
 * Overload оператора <<
 * Teach operator<< how to print a Color
 * std::ostream is the type of std::cout, std::cerr, etc...
 * The return type and parameter type are references (to prevent copies from being made)
 *
 */

std::ostream& operator<<(std::ostream& out, Color color) {
    out << getColorName(color); // print our color's name to whatever output stream out
    return out;                 // operator<< conventionally returns its left operand
}



int main() {
    Color shirt{ blue };
    std::cout << "Your shirt is " << shirt << '\n'; // it works!




    return 0;
}