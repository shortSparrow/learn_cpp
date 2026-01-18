#include <algorithm>
#include <array>
#include <functional>
#include <iostream>
#include <string_view>


// returns a lambda
auto makeWalrus(const std::string& name) {
  // Capture name by reference and return the lambda.
  return [&]() {
    std::cout << "I am a walrus, my name is " << name << '\n'; // Undefined behavior
  };
}

// Щоб уникнути проблеми dangling reference краще копіювати дані у lambda якщо не треба їх міняти
auto makeWalrusSafe(const std::string& name) {
  return [name]() {
    std::cout << "I am a walrus, my name is " << name << '\n'; // Ok behavior
  };
}

void myInvoke(const std::function<void()>& fn) {
  fn();
}


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
   *
   * Capture не потрбіно для static та constexpr змінних
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
   * Dangling captured variables
   * Змінні захоплюються (captured) у лямбда, коли лямбда сторена.
   * Якщо змінна, захоплена як reference, зникає раніше за лямбду, то у лямбді вона залишається з невизначеним (dangling) посиланням.
   *
   * Виклик makeWalrus() створює тимчасовий std::string з рядкового літералу "Roofus". Лямбда в makeWalrus() захоплює
   * тимчасовий рядок за посиланням. Тимчасовий рядок зникає в кінці повного виразу, що містить виклик makeWalrus(),
   * але лямбда sayName все ще посилається на нього після цього моменту. Таким чином, коли ми викликаємо sayName,
   * відбувається доступ до dangling reference, що призводить до невизначеної поведінки.
   */

  auto sayName{ makeWalrus("Roofus") };
  sayName();


  //************************ Копії lambda function ***********************************
  /**
   * Замість того, щоб вивести 1, 2, 3, код двічі виводить 2.
   * Коли ми створили otherCount як копію count, ми створили копію count у його поточному стані.
   * count мав значення i = 1, тому otherCount також має значення i = 1. А потім ми викликаємо
   * count який збільшує свою внутрішню змінну і otherCount, який збільшує свою
   */
  int i { 0 };

  // Create a new lambda named count
  auto count{ [i]() mutable {
    std::cout << ++i << '\n';
  } };

  count(); // 1

  auto otherCount{ count }; // create a copy of count

  // invoke both count and the copy
  count(); // 2
  otherCount(); // 2



  /**
   * Менш очевидний приклад
   *
   * Ми отримаємо вивід 1 три рази, тому що коли ми викликаємо myInvoke(count), компілятор бачить,
   * що count (який має тип лямбда) не відповідає типу параметра-посилання (std::function<void()>).
   * Він перетворює лямбду в тимчасову std::function, щоб параметр-посилання міг зв'язатися з нею, і це
   * створює копію лямбди. Таким чином, наш виклик fn() фактично виконується на копії нашої лямбди,
   * яка існує як частина тимчасової std::function, а не на самій лямбді.
   *
   * Спосіб виправити - це дати тип ляибді такий самий, як і у параметра myInvoke - std::function
   * Або другий спосіб - це обгорнути count при передачі як аршумент у reference wrapper, який робиться
   * за допомогою std::ref
   */
  int k {0};

  // Increments and prints its local copy of k.
  auto count2{ [k]() mutable {
    std::cout << ++k << '\n';
  } };

  myInvoke(count2); // 1
  myInvoke(count2); // 1
  myInvoke(count2); // 1

  myInvoke(std::ref(count2)); // 1
  myInvoke(std::ref(count2)); // 2
  myInvoke(std::ref(count2)); // 3


  std::function count2Safe{ [k]() mutable { // lambda object stored in a std::function
    std::cout << ++k << '\n';
  } };

  myInvoke(count2Safe); // 1
  myInvoke(count2Safe); // 2
  myInvoke(count2Safe); // 3


  /**
   * Варто пам'ятати, що lambda function - це не зовсім функції, це об'єкти, і тому якщо їх передавати
   * як аргумент, то вони будуть копіюватися. Саме тому варто використовувати std::ref, що передедавати їх
   * як посилання на функцію
   */




  //*******************************************************
  /**
   * Capture змінних відбувається коли функція написана (скомпільована), а не коли викликається
   */
  std::string favoriteFruit{ "grapes" };

  auto printFavoriteFruit{
    [=]() {
      std::cout << "I like " << favoriteFruit << '\n';
    }
  };

  favoriteFruit = "bananas with chocolate";

  printFavoriteFruit(); // I like grapes



  return 0;
}