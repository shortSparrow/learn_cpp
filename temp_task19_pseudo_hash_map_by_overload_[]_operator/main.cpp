#include <iostream>
#include <vector>

struct StudentGrade {
  std::string name {};
  char grade {};
};


/**
 * GradeMap містить std::vector<StudentGrade> m_map. Ми робимо overload оператора []
 * щоб емулювати роботу hashMap. Оператор [] буде повертати reference на grade у StudentGrade
 * і ми зможемо його прочитати і за потреби змінити на своє значення
 *
 * Реальний hashMap використовує хешування
 * hash("Joe") % 10 -> індекс 3
 * і є два варіанти роботи з колізіями:
 * 1) Якщо буде однаковий хеш, запхнути обидва елементи з однаковим хешем в одну комірку і потім використати пошук
 * 2) Беремо наступну вільну комірку після комірки на яку вказав hash
 */

class GradeMap {
private:
  std::vector<StudentGrade> m_map {};

public:

  char& operator[] (std::string_view name) {
    auto match = std::ranges::find_if(m_map, [&name](const StudentGrade& st) {
      return st.name == name;
    });

    if (match != m_map.end()) { // Якщо елемент у масиві присутній
      return match->grade;
    } else {
      m_map.push_back(StudentGrade{.name = static_cast<std::string>(name) });

      return m_map.back().grade; // повертаємо reference на grade у щойно доданий елемент масиву
    }
  };
};

int main() {
  GradeMap grades{};

  grades["Joe"] = 'A';
  grades["Frank"] = 'B';

  std::cout << "Joe has a grade of " << grades["Joe"] << '\n';
  std::cout << "Frank has a grade of " << grades["Frank"] << '\n';


  /**
   * А тут є потенційна небезпека, то ми повернули reference char, що по суті є адресою пам'яті,
   * але коли ми додамо новий елемент у std::vector наш вектор може переповнитися і він перестовриться
   * наново і цілком можливо буде мати абсолютно іншу адресу у пам'яті.
   */

  char& gradeOleksandr{ grades["Oleksandr"] }; // does an emplace_back
  gradeOleksandr = 'A';

  char& gradeLiubov{ grades["Liubov"] }; // does a emplace_back
  gradeLiubov = 'B';

  std::cout << "Oleksandr has a grade of " << gradeOleksandr << '\n';
  std::cout << "Liubov has a grade of " << gradeLiubov << '\n';

  return 0;
}