#include <algorithm>
#include <iostream>


int main() {

    std::cout << "How many names do you wish enter?";
    size_t names_count {};
    std::cin >> names_count;


    auto* name_list {new std::string[names_count]{}};

    for (std::size_t i{ 0 }; i < names_count; ++i) {
        std::cout << "Enter the name #" << i+1 << ": ";

        std::string name {};
        std::cin >> name;
        name_list[i] = name;
    }

    std::ranges::sort(name_list, name_list + names_count); //  std::ranges::sort(*name_list) не проканає, бо ми просто передамо перший елемент масиву і sort відсортує лише його

    for (std::size_t i{ 0 }; i < names_count; ++i) {
        std::cout << "Name #" << i+1 << ": " <<  name_list[i] << '\n';
    }

    delete[] name_list;

    return 0;
}