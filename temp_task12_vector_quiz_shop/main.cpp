#include <algorithm>
#include <iostream>
#include <vector>


struct Skill {
    std::string name {};
    int price {};
};

/**
 *  Це може бути небезпечним, бо якщо оригінальний масив Skills переповниться при додаванні нових
 *  елементів (ми цього тут не робимо), але якщо таке станеться, то система створить новий масив skills
 *  і скопіює всі старі значення в нього, але це можуть бути вже зовсім інші комірки даних, тому наше поле
 *  purchase стане не валідним.
 *
 *  Тут краще буде лишити копіювання замість pointer, бо структура не складна, або використовувати індекси,
 *  як посилання на потрібний елемент skills
 */
struct PurchasedSkill {
    int count {};
    Skill* purchase {};
};


void print_skills(const std::vector<Skill>& skills){
    std::cout << "Here is our selection for today:\n";
    for (int i{0}; i<skills.size(); i++) {
        std::cout << i << ") " << skills[i].name << " costs " << skills[i].price << '\n';
    }
}

Skill* get_user_choice(std::vector<Skill>& skills, int& gold) {
    std::string user_choice {};
    std::cout << "Enter the number of the potion you'd like to buy, or 'q' to quit:";
    std::cin >> user_choice;

    if (user_choice == "q" || user_choice == "Q") {
        return nullptr;
    }

    // Перевіряємо кожен символ. Якщо це не цифра — ввід невалідний.
    bool is_number = !user_choice.empty() && std::all_of(user_choice.begin(), user_choice.end(), ::isdigit);

    if (!is_number) {
        std::cout << "That is an invalid input. Try again.\n";
        return get_user_choice(skills, gold); // Повторюємо запит
    }

    size_t index = std::stoi(user_choice);
    if (index < skills.size()) {
        if (skills[index].price <= gold) {
            gold -= skills[index].price;
            std::cout << "You purchased a potion of " << skills[index].name << ". You have " << gold << " gold left.\n";
            return &skills[index];
        }

        std::cout << "You can not buy a potion of " << skills[index].name << ".  You have not enough money.\n";
    } else {
        std::cout << "You can not afford that.\n";
    }

    print_skills(skills);
    return get_user_choice(skills, gold);

}

void print_resolution(const int gold_left, const std::vector<PurchasedSkill>& purchased_skills) {
    std::cout << "Your inventory contains: \n";

    for (auto& skill: purchased_skills) {
        std::cout << skill.count << "x" << " potion of " << skill.purchase->name << "\n";
    }

    std::cout << "You escaped with " << gold_left << " gold remaining." << '\n' << '\n';
    std::cout << "Thanks for shopping at Roscoe's potion emporium!";
}


void start_loop_question(std::vector<Skill>& skills, std::vector<PurchasedSkill>& purchased_skills, int& gold) {
    print_skills(skills);
    auto user_choice {get_user_choice(skills, gold)};

    if (!user_choice) {
        return;
    }
    bool skill_was_found {false};
    for (auto& skill: purchased_skills) {
        if (skill.purchase->name == user_choice->name) {
            skill.count++;
            skill_was_found = true;
        }
    }
    if (!skill_was_found) {
        purchased_skills.push_back({1, user_choice});
    }

    return start_loop_question(skills, purchased_skills, gold);
}

int main() {
    int gold {85};
    std::string user_name {};
    std::vector<Skill> skills {
        {"healing", 20},
        {"mana", 30},
        {"speed", 12},
        {"invisibility", 50}
    };

    std::vector<PurchasedSkill> purchased_skills {};

    std::cout << "Welcome to Roscoe's potion emporium!\n";
    std::cout << "Enter your name:";
    std::cin >> user_name;
    std::cout << "Hello, " << user_name << " you have " << gold << " gold.\n";

    start_loop_question(skills, purchased_skills, gold);
    print_resolution(gold, purchased_skills);



    return 0;
}