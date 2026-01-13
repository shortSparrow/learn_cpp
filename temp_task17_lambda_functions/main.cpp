#include <algorithm>
#include <array>
#include <iostream>


struct Student {
    std::string name {};
    int point {};
};

struct Season {
    std::string_view name{};
    double averageTemperature{};
};


int main() {
    // Знайти студента з найбільшими балами
    std::array<Student,3> students {
        Student{"Senya", 12},
        Student{"Kara", 34},
        Student{"max", 4}
    };

    auto helper {
        [](const Student& first, const Student& second) {
            return second.point > first.point;
        }
    };
    Student* better_student  = std::ranges::max_element(students, helper);
    std::cout << "The better student is: " << better_student->name << '\n';





    // Відсортувати сезони за середньою температурою від меншого до більшого
    std::array<Season, 4> seasons{
            { { "Spring", 285.0 },
              { "Summer", 296.0 },
              { "Fall", 288.0 },
              { "Winter", 263.0 } }
    };

    std::ranges::sort(seasons, [](const Season& s1, const Season& s2){
        return s2.averageTemperature > s1.averageTemperature;
    });

    for (const auto& season : seasons) {
        std::cout << season.name << '\n';
    }

    return 0;
}