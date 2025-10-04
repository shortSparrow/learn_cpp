#include <iostream>
using namespace std;

int main() {

    /**
     * 1 dollar is 100 cents
     * 1 quarter is 25 cents
     * 1 dime is 10 cents
     * 1 nickel is 5 cents
     * 1 penny is 1 cent
     */

    int dollars {};
    int quarters {};
    int dimes {};
    int nickels {};
    int pennies {};

    int value {};
    cout << "Enter amount in cetns: ";
    cin >> value;

    int rest {value};
    dollars = rest/100;
    rest = rest % 100;

    quarters = rest/25;
    rest = rest % 25;

    dimes = rest/10;
    rest = rest % 10;

    nickels = rest/5;
    rest = rest % 5;

    pennies = rest/1;
    rest = rest % 1;

    cout << "dollars:" << dollars << endl;
    cout << "quarters:" << quarters << endl;
    cout << "dimes:" << dimes << endl;
    cout << "nickels:" << nickels << endl;
    cout << "pennies:" << pennies << endl;

    return 0;
}