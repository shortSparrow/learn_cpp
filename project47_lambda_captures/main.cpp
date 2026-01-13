#include <algorithm>
#include <array>
#include <iostream>
#include <string_view>

int main() {
  /**
   * У мене є масив string_view і lambda function і коли я щукаю статично підрядок то все ок.
   * Але Якщо я захочу передати у lambda function параметр search який динамічний, то як це зробити?
   *
   * На відміну від вкладених блоків, де будь-який ідентифікатор, доступний у зовнішньому блоці, доступний і у
   * вкладеному блоці, лямбди можуть отримувати доступ лише до певних типів об'єктів, які були визначені поза лямбдою.
   * До них належать:
   *  Objects with static (or thread local) storage duration (this includes global variables and static locals)
   *  Objects that are constexpr (explicitly or implicitly)
   */
  std::array<std::string_view, 4> arr{ "apple", "banana", "walnut", "lemon" };

  // Жодних проблем, все працює
  auto found_1{ std::find_if(arr.begin(), arr.end(),
                           [](std::string_view str)
                           {
                             return str.find("nut") != std::string_view::npos;
                           }) };



  std::cout << "search for: ";
  std::string search{};
  std::cin >> search;

  // Помилка: Local variable 'search' is not captured
  auto found_2{ std::find_if(arr.begin(), arr.end(), [](std::string_view str) {
    // Search for @search rather than "nut".
    return str.find(search) != std::string_view::npos; // Error: search not accessible in this scope
  }) };


  /**
   * To access search from within the lambda, we’ll need to use a capture clause.
   *
   * capture clause використовується для (непрямого) надання лямбда-виразу доступу до змінних,
   * доступних у навколишній області видимості, до яких він зазвичай не має доступу.
   * Для цього нам треба просто помістити потрібні змінні у [] лямбда функції
   *
   * Тож як воно працює
   * Коли виконується визначення лямбда, для кожної змінної, яку лямбда захоплює, всередині лямбда
   * створюється клон цієї змінної (з ідентичною назвою). Ці клоновані змінні ініціалізуються з зовнішніх
   * змінних того ж імені на цьому етапі.
   * Хоча ці клоновані змінні мають однакову назву, вони не обов'язково мають той самий тип, що й вихідна змінна.
   */

  auto found_3{ std::find_if(arr.begin(), arr.end(), [search](std::string_view str) {
    // Search for @search rather than "nut".
    return str.find(search) != std::string_view::npos;
  }) };





 /**
  * Captures are treated as const by default
  * Коли викликається лямбда-вираз, викликається operator(). За замовчуванням цей operator() обробляє
  * захоплені значення як константні, тобто лямбда-вираз не може їх змінювати.
  */

  int ammo{ 10 };

  auto shoot{
    [ammo]() {
      // Illegal, ammo cannot be modified.
      --ammo;

      std::cout << "Pew! " << ammo << " shot(s) left.\n";
    }
  };

  /**
   * Mutable captures
   * Коли ми викликаємо shoot2 то вона стоворює копію ammo2 і ця копія живе своє життя, тому що captured variables
   * є членами lambda об'єкту, а їхні значення зберігаються і достіпні для інших викликів lambda! Це чмсось схоже на
   * те як себе ведуть static variables в середині функції
   */

  int ammo2{ 10 };

  auto shoot2{
    [ammo2]() mutable { // now mutable
      // We're allowed to modify ammo2 now
      --ammo2;

      std::cout << "Pew! " << ammo2 << " shot(s) left.\n";
    }
  };

  shoot2(); // Pew! 9 shot(s) left.
  shoot2(); // Pew! 8 shot(s) left.

  std::cout << ammo2 << " shot(s) left\n"; // 10 shot(s) left


  // Capture by reference. Якщо будемо передавати змінну як reference то тоді можемо її змінуювати
  int ammo3{ 10 };

  auto shoot3{
    [&ammo3]() {
      // We're allowed to modify ammo3 now
      --ammo3;

      std::cout << "Pew! " << ammo3 << " shot(s) left.\n";
    }
  };

  shoot3(); // Pew! 9 shot(s) left.
  shoot3(); // Pew! 8 shot(s) left.
  std::cout << ammo3 << " shot(s) left\n"; // 8 shot(s) left


  //**************************************************************************************
  // Capturing multiple variables
  int health{ 33 };
  int armor{ 100 };
  std::array<std::string_view, 4> fruits{ "apple", "banana", "walnut", "lemon" };

  // Capture health and armor by value, and enemies by reference.
  [health, armor, &fruits](){};

  /**
   * Default captures
   * Інколи перераховувати всі параметри, які мають бути передані до може бути складно.
   * Тому існує механізм автоматичного captures, який автоматично захопить всі змінні які
   * використовуються у lambda
   */

  // Default captures за допомогою знаку "="
  int x1{ 1 };
  int x2{ 2 };
  int x3{ 3 };

  auto shoot4 {
    [=]() {
      int x {x1+x2+x3};

      std::cout << "total x=" << x << '\n'; // 6
    }
  };

  shoot4();


  // Default captures by reference за допомогою знаку "&"
  int x4{ 1 };
  int x5{ 2 };
  int x6{ 3 };
  int total_x {};

  auto shoot5 {
    [&]() {
      x6 = 6;
      int x {x4+x5+x6};
      total_x = x;
      std::cout << "total x=" << total_x << '\n'; // 9
    }
  };

  shoot5();
  std::cout << "x6=" << x6 << '\n'; // 6
  std::cout << "total_x=" << total_x << '\n'; // 9



  // Defining new variables in the lambda-capture
  int width {10};
  int height {20};

  auto shoot6 {
    [use_area{width*height}](int known_area) {
      return use_area == known_area;
    }
  };

  shoot6(200); // return true



  //*****************************************************************************
  /**
   * Змінні захоплюються (captured) у лямбда, коли лямбда сторена.
   * Якщо змінна, захоплена як reference, зникає раніше за лямбду, то у лямбді вона залишається з невизначеним (dangling) посиланням.
   */


  return 0;
}