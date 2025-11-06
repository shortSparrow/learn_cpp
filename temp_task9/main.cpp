#include <iostream>
#include <random>


/**
 * Ця функція не правильна, оскільки при кожному виклику вона створює
 * новий генератор std::mt19937 mt {}, але оскільки це тестовий варіант
 * і ця функція запуститься тільки раз по нехай так і буде
 */
int get_random(const int min, const int max) {
    std::mt19937 mt {};
    std::random_device rd {};
    std::seed_seq ss {rd(), rd(), rd(), rd()};
    mt.seed(ss);

    std::uniform_int_distribution distribution {min,max};

    int random_int = distribution(mt);

    return random_int;
}

bool start_quiz(const int correct_value) {
    std::cout << "Let's play a game. I'm thinking of a number between 1 and 100. You have 7 tries to guess what it is." << "\n";

    for (int i{1}; i <=7; i++) {
        std::cout << "Guess #" << i << ":";
        int value {};
        std::cin >> value;

        if (value == correct_value) {
            std::cout << "Correct! You win!";
            return true;
        }

        if (value > correct_value) {
            std::cout << "Your guess is too high.";
            continue;
        }

        if (value < correct_value) {
            std::cout << "Your guess is too low.";
            continue;
        }
    }

    std::cout << "Sorry, you lose. The correct number was " << correct_value << "." << "\n";

    return false;
}

bool ask_again() {
    std::cout << "Would you like to play again (y/n)?" << "\n";
    char start_again {};
    std::cin >> start_again;

    switch (start_again) {
        case 'y':
            return true;
        case 'n':
            return false;
        default:
            return ask_again();
    }
}

int main() {

    const int random_int = get_random(1,100);
    std::cout << random_int;

    start_quiz(random_int);

    if (ask_again()) {
        main();
    }


    return 0;
}
