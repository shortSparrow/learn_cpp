#include <iostream>

int main() {
    // замість 'const int' можна написати 'constexpr', так наші дані обрахуються ще на
    // етапі компіляції, а не під час роботи коду, що теоретично має покращити перформанс
    constexpr int per_small_room {25};
    constexpr int per_large_room {35};
    constexpr float tax {0.06};

    int count_small_rooms {0};
    int count_large_rooms {0};

    std::cout << "How many small rooms would you clean?";
    std::cin >> count_small_rooms;

    std::cout << "How many large rooms would you clean?";
    std::cin >> count_large_rooms;

    std::cout << "Estimate for carpet cleaning service" << std::endl;
    std::cout << "Number of small rooms " << count_small_rooms << std::endl;
    std::cout << "Number of large rooms " << count_large_rooms << std::endl;
    std::cout << "Price per small room: $" << per_small_room << std::endl;
    std::cout << "Price per large room: $" << per_large_room << std::endl;

    const int cost = per_small_room * count_small_rooms + per_large_room * count_large_rooms;
    const float tax_cost = cost * tax;
    const float total_cost = cost + tax_cost;
    std::cout << "Cost: $" << cost << std::endl;
    std::cout << "Tax: $" << tax_cost << std::endl;
    std::cout << "=========================" << std::endl;
    std::cout << "Total cost: $" << total_cost << std::endl;

    return 0;
}