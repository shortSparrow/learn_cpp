#include <iostream>
#include <vector>
using namespace std;



int main() {

    const string p_message {"P - Print numbers"};
    const string a_message {"A - Add number"};
    const string m_message {"M - Display mean of the numbers"};
    const string s_message {"S - Display the smallest number"};
    const string l_message {"L - Display the largest number"};
    const string q_message {"q - Quit"};
    const string enter_your_choice {"Enter your choice:"};

    const string message {
        "\n" +
        p_message + "\n"
        + a_message + "\n"
        + m_message + "\n"
        + s_message + "\n"
        + l_message + "\n"
        + q_message + "\n\n"
        + enter_your_choice
    };
    char input_value {};
    vector<int> numbers {};
    int sum {};
    int min {};
    int max {};

    do {
        cout << message;
        cin >> input_value;

        switch (input_value) {
            case 'P':
            case 'p':
                if (numbers.empty()) {
                    cout << "[] - list is empty" << endl;
                } else {
                    cout << "[ ";
                    for (auto n: numbers ) {
                        cout << n << " ";
                    }
                    cout << "]";
                }
                break;
            case 'A':
            case 'a': {
                int num {};
                cout << "Enter an integer to add to the list: ";
                cin >> num;
                if (numbers.empty()) {
                    min = num;
                    max = num;
                }

                numbers.push_back(num);
                sum += num;

                if (num < min) {
                    min = num;
                }

                if (num > max) {
                    max = num;
                }
                cout << num << " added" << endl;
                break;
            }


            case 'M':
            case 'm': {
                // Оскільки і sum і numbers.size() є цілочисельним типами, то їх ділення теж дасть цілочисленний результат.
                // Щоб результат був double, треба, щоб хоча б один з операндів був float/double, саме тому ми й приводимо sum до double
                const double mean {static_cast<double>(sum) / numbers.size()};

                cout << "The mean is: " << mean << endl;
                break;
            }

            case 'S':
            case 's':
                cout << "The smallest number is: " << min << endl;
                break;

            case 'L':
            case 'l':
                cout << "The largest number is: " << max << endl;
                break;

            case 'Q':
            case 'q':
                cout << "Goodbye" << endl;
                break;

            default: cout << "Unknown selection, please, try again" << endl;
        }

    } while (input_value != 'Q' && input_value != 'q');


    return 0;
}