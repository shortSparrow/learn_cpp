#include <iostream>
using namespace std;


int main() {

    enum Color {red, green, blue, lightblue};
    Color my_color {green};

    switch (my_color) {
        case red:
            cout << "red" << endl;
            break;
        case green:
            cout << "green" << endl;
            break;
        case blue:
        case lightblue:
            cout << "blue or light blue" << endl;
            break;
        default:
            cout << "default" << endl;
    }





















    return 0;
}