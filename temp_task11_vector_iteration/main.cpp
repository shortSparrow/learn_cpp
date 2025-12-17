#include <iostream>
#include <string>
#include <string_view>
#include <vector>

/**
 * Задача:
 * Є перелік імен у масиві, треба написати функцію, щоб користувач вводив одне ім'я,
 * і якщо це ім'я є у масиві то виводилося, що воно є, інакше, що його немає *
 */

template <typename T, typename E>
bool isValueInArray(const std::vector<T>& v, const E& value) {
    for (const T& item: v) {
        if (item == value) {
            return true;
        }
    }

    return false;
}

int main() {
    std::vector<std::string_view> names{ "Alex", "Betty", "Caroline", "Dave",
        "Emily", "Fred", "Greg", "Holly" };

    std::cout << "Enter a name: ";
    std::string username{};
    std::cin >> username;

    bool found{ isValueInArray(names, username) };


    if (found)
        std::cout << username << " was found.\n";
    else
        std::cout << username << " was not found.\n";

    return 0;
}