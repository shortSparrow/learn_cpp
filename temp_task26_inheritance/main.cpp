#include <iostream>
#include <random>
#include <string>
#include <string_view>

/**
 * Маємо три класи:
 *      Creature - базовий
 *      Monster - монстр що буде атакувати гравця
 *      Player - гравець що атакує монстра
 *
 * Це основа для текстової гри, за бажанням можна дописати механізм бою, мета набрати 100 золота
 */
class Creature {
private:
    std::string name {};
    char symbol {};
    int health {};
    int damage {};
    int gold_amount {};

public:
    Creature(const std::string_view name, char symbol, int health, int damage, int gold_amount)
        : name(name),
          symbol(symbol),
          health(health),
          damage(damage),
          gold_amount(gold_amount)
    {
    }

    std::string get_name() const { return name; }
    void set_name(const std::string_view name) { this->name = name; }

    int get_health() const { return health; }
    void set_health(const int health) { this->health = health; }

    int get_damage() const { return damage; }
    void set_damage(const int damage) { this->damage = damage; }

    int get_gold_amount() const { return gold_amount; }
    void set_gold_amount(const int gold_amount) { this->gold_amount = gold_amount;}

    char get_symbol() const {return symbol; }

    void reduce_health(const int reduce_health) {
        health = health - reduce_health;
    }

    bool is_dead() const {
        return health > 0;
    }

    void add_gold(const int amount) {
        gold_amount+=amount;
    }
};

class Player: public Creature {
private:
    int level {1};
public:
    Player(const std::string_view name): Creature(name, '@', 10, 1, 0) {

    }

    void level_up() {
        level+=1;
        set_damage(get_damage() + 1);
    }

    int get_level() const { return level; }

    bool has_won() const { return level >= 20; }
};

class Monster: public Creature {
public:
    enum class Type {dragon, orc, smile, max_types};

    static Monster get_random_monster() {
        std::random_device rd;  // Seed source
        std::mt19937 gen(rd()); // Generator engine
        std::uniform_int_distribution<> distrib(0, static_cast<int>(Type::max_types)-1); // Define range [1, max_types-1]

        return Monster(static_cast<Monster::Type>(distrib(gen)));
    }

    Monster(const Type t) : Creature(get_creature(t)) {
    }


private:
    Creature get_creature(const Type t) {
        switch (t) {
        case Type::dragon:
            return Creature("dragon", 'D', 20, 4, 100);
        case Type::orc:
            return Creature("orc", 'o', 4, 2, 25);
        case Type::smile:
            return Creature("smile", 's', 1, 1, 10);
        }
    }
};



int main() {
    // Creature o{ "orc", 'o', 4, 2, 10 };
    // o.add_gold(5);
    // o.reduce_health(1);
    // std::cout << "The " << o.get_name() << " has " << o.get_health() << " health and is carrying " << o.get_gold_amount() << " gold.\n";


    // Monster m{ Monster::Type::orc };
    // std::cout << "A " << m.get_name() << " (" << m.get_symbol() << ") was created.\n";

    for (int i{ 0 }; i < 10; ++i) {
        Monster m{ Monster::get_random_monster() };
        std::cout << "A " << m.get_name() << " (" << m.get_symbol() << ") was created.\n";
    }



    return 0;
}