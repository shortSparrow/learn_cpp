#include <iostream>
#include <utility> // For std::pair, std::make_pair, std::move, std::move_if_noexcept
#include <stdexcept> // std::runtime_error



/**
 * Уявимо ситуацію, ми присвоюємо якомусь об'єкту новий об'єкт, якщо під час цієї ситуації станеться exception
 * (наприклад нестача пам'яті абощо) то нічого страшного не виникне, об'єкт не змінить своє значення і ми можемо
 * повторити операцію.
 *
 * А от якщо ми робимо move одного об'єкта в інше місце, то ми змінюємо власника об'єкта, і якщо під час цієї
 * процедури виникне exception, об'єкт зависне, бо старий власник вже не володіє об'єктом, а новий все ще не володіє.
 * По хорошому нам треба вернути об'єкт старому власнику.
 *
 */


/**
 * std::pair це стандартний клас який має два поля fist та second.
 * Тут ми беремо rvalue старого об'єкта (old) і переносимо його значення у новий об'єкт шляхом std::move
 */
// template <typename T1, typename T2>
// struct pair {
//   T1 first;  // Перше значення
//   T2 second; // Друге значення
//
//   // Конструктор переміщення, який ви навели:
//   pair(pair&& old)
//     : first(std::move(old.first)),
//       second(std::move(old.second))
//   {}
// };


/**
 * Коротке нагадування про std::move
 * Коли ми пишемо std::pair moved_pair{ std::move(my_pair) }; відбувається наступне:
 *  1. std::move перетворює my_pair на rvalue. (сам std::move нічого не переміщує, він просто з lvalue робить rvalue)
 *  2. Викликається Move Constructor класу std::pair.
 *
 * Зазвичай Move Constructor генерується компілятором автоматично і він запихне туди усі поля класу, але у нас лише одне
 * поле тому для наглядності ми робимо це вручну.
 *
 *
 * Автоматична генерація конструктора за правилом п'яти.
 * Компілятор автоматично генерує конструктор переміщення лише тоді, коли ми не написали власноруч жодного з цих методів:
 *  1) Конструктор копіювання.
 *  2) Оператор присвоювання копіюванням.
 *  3) Оператор присвоювання переміщенням.
 *  4) Деструктор.
 *
 *
 * Що відбувається у нашому випадку з CopyClass, ми викликаємо Move Constructor, але його
 * у нас немає, але у нас є конструктор копіювання, і компілятор думає, я можу його скопіювати,
 * і викликаємо Copy constructor який кидає exception
 *
 *
 */

class MoveClass {
private:
  int* m_resource{};

public:
  MoveClass() = default;

  MoveClass(int resource) : m_resource{ new int{ resource } } {}

  // Copy constructor
  MoveClass(const MoveClass& that) {
    // deep copy
    if (that.m_resource != nullptr) {
      m_resource = new int{ *that.m_resource };
    }
  }

  // Move constructor
  MoveClass(MoveClass&& that) noexcept : m_resource{ that.m_resource } {
    that.m_resource = nullptr;
  }

  ~MoveClass() {
    std::cout << "destroying " << *this << '\n';

    delete m_resource;
  }

  friend std::ostream& operator<<(std::ostream& out, const MoveClass& moveClass) {
    out << "MoveClass(";

    if (moveClass.m_resource == nullptr) {
      out << "empty";
    } else {
      out << *moveClass.m_resource;
    }

    out << ')';

    return out;
  }
};

class CopyClass {
public:
  bool m_throw{};

  CopyClass() = default;

  // Copy constructor throws an exception when copying from
  // a CopyClass object where its m_throw is 'true'
  CopyClass(const CopyClass& that) : m_throw { that.m_throw } {
    if (m_throw) {
      throw std::runtime_error{ "abort!" };
    }
  }
};

int main() {

  /**
   * Коротко що тут відбувається
   * Ми робимо pair з MoveClass та CopyClass (це відбувається шляхом std::move і для MoveClass викликається
   * Move Constructor, а для CopyClass Copy Constructor, оскільки у нього немає Move Constructor. Тут все відбувається
   * без проблем.
   *
   * Далі прописуємо my_pair.second.m_throw = true щоб під час виклику Copy Constructor для CopyClass було
   * кинуто exception.
   *
   * std::pair moved_pair{ std::move(my_pair) }; Переміщає (move) значення одного pair в інше, і це робиться за
   * допомогою виклику Move Constructor дял MoveClass і Copy Constructor для CopyClass. І в цей момент CopyClass
   * викидає exception. І відповідно MoveClass встиг перенестися в нову pair і обнулити значення в старій, а от
   * CopyClass ні і лишився в старій.
   *
   * Осб що буде виведено у термінал
   * destroying MoveClass(empty)
   * my_pair.first: MoveClass(13)
   * destroying MoveClass(13)
   * Error found: abort!
   * my_pair.first: MoveClass(empty)
   * my_pair.second.m_throw: 1
   * destroying MoveClass(empty) - тут вийшли за межі скоупа і викликався деструктор для MoveClass
   */


  // Тут ми можемо зробити std::pair беж жодних проблем:
  std::pair my_pair{ MoveClass{ 13 }, CopyClass{} };

  std::cout << "my_pair.first: " << my_pair.first << '\n';

  try {
    my_pair.second.m_throw = true; // To trigger copy constructor exception

    // Цей рядок викине exception
    std::pair moved_pair{ std::move(my_pair) }; // We'll comment out this line later

    std::cout << "moved pair exists\n"; // Ніколи не буде виведено, бо було кинуто exception вище
  } catch (const std::exception& ex) {
      std::cout << "Error found: " << ex.what() << '\n';
  }

  std::cout << "my_pair.first: " << my_pair.first << '\n'; // my_pair.fist порожній, бо його значення було перенесено у moved_pair, а my_pair.first перетворено на nullptr
  std::cout << "my_pair.second.m_throw: " << my_pair.second.m_throw << '\n'; // 1 (тобто true) що означає, що ці значення не бли перенесені і не обнулилися.


  /**
   * std::move_if_noexcept виправляє ситуацію
   * Якби ми з самого початку для std::pair використовували Copy Constructor, а не Move Constructor то ми б отримали
   * так само помилку на етапі moved_pair, але my_pair не було б змінено, бо Copy Constructor не обнуляв би попереднє значення.
   *
   * Але копіювання замість переміщення (move) має певні витрати на продуктивність, які ми не хочемо платити за всі
   * об'єкти — в ідеалі ми хочемо переміщувати об'єкти, якщо це можливо безпечно, а копіювати — в іншому випадку.
   *
   *
   * Якщо компілятор може визначити, що об’єкт, переданий як аргумент функції std::move_if_noexcept, не викличе exception
   * під час Move Constructor (або якщо об’єкт є об'єктом, доступним лише для переміщення, і не має Copy Constructor),
   * то std::move_if_noexcept працюватиме так само, як і std::move() (і поверне об’єкт, перетворений на r-значення).
   * В іншому випадку std::move_if_noexcept поверне звичайну посилання на об’єкт у вигляді лівого значення.
   *
   *
   */


  std::cout << "-----------------------------------------------------------------\n";
  // Тут ми можемо зробити std::pair беж жодних проблем:
  std::pair my_pair2{ MoveClass{ 13 }, CopyClass{} };

  std::cout << "my_pair.first: " << my_pair2.first << '\n';

  try {
    my_pair2.second.m_throw = true; // To trigger copy constructor exception

    std::pair moved_pair2{ std::move_if_noexcept(my_pair2) }; // Це рядок вставлено замість попереднього

    std::cout << "moved pair exists\n"; // Ніколи не буде виведено, бо було кинуто exception вище
  } catch (const std::exception& ex) {
    std::cout << "Error found: " << ex.what() << '\n';
  }

  std::cout << "my_pair.first: " << my_pair2.first << '\n'; // my_pair.first: MoveClass(13)


  /**
   * Ось що виведе термінал:
   *
   * destroying MoveClass(empty)
   * my_pair.first: MoveClass(13)
   * destroying MoveClass(13)
   * Error found: abort!
   * my_pair.first: MoveClass(13)
   * destroying MoveClass(13)
   * destroying MoveClass(empty)
   *
   * Як бачимо відбувся exception, але my_pair2 не було змінено.
   *
   * Move Constructor std::pair не є noexcept, тож std::move_if_noexcept повертає my_pair як l-value reference.
   * Це призводить до того, що moved_pair створений через Copy Constructor, а не Move Constructor.
   * І відповідно Copy Constructor не змінює об'єкт копіювання.
   */


  return 0;
}