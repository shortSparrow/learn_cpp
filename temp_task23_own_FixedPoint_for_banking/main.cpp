#include <cassert>
#include <cmath>
#include <cstdint>
#include <iostream>


/**
 * Умова задачі: Реалізація типу даних FixedPoint2
 *
 * Проблема: Стандартний тип double використовує двійкову систему числення, через що він не може точно представити прості
 * десяткові дроби (наприклад, 0.1 + 0.2 не дорівнює рівно 0.3). У фінансових та прецизійних розрахунках накопичення
 * таких похибок призводить до втрати даних.
 *
 * Завдання: Створити клас FixedPoint2, який реалізує арифметику з фіксованою комою (2 знаки після коми),
 * використовуючи лише цілочисельні типи (int16_t та int8_t).
 * Це гарантує:
 *  - Абсолютну точність додавання та віднімання.
 *  - Передбачуване округлення при конвертації з чисел із плаваючою крапкою.
 *  - Коректну роботу операторів порівняння (==), які в double працюють нестабільно.
 */

class FixedPoint2 {

public:

  std::int16_t m_base{}; // here's our non-fractional part
  std::int8_t m_decimal{}; // here's our factional part

  FixedPoint2(std::int16_t base = 0, std::int8_t decimal = 0):
  m_base{base}, m_decimal{decimal} {

    if (m_base < 0 || m_decimal < 0) {
      // Make sure base is negative
      if (m_base > 0)
        m_base = -m_base;
      // Make sure decimal is negative
      if (m_decimal > 0)
        m_decimal = -m_decimal;
    }


    /**
     * На випадок переповнення decimal частини, припустимо введуть 100 або 123456 а у нас максимум 99
     * Тож ми маємо у decimal лишити лише частку, а цілу частину перекинути до base
     *
     * Припустимо base=1 і decimal=100
     * Тоді base=2 і decimal=1
     *
     * Якщо base=2 і decimal=123456
     * Тоді base=1236 і decimal=56
     *
     * Спосіб наведений нижче чудово працює з від'ємними числами. Якщо base у нас від'ємна тоді і decimal теж буде
     * від'ємна (ми так написали вище), а тоді матимемо - + - і отримуємо що буде додавання двох від'ємних чисел.
     * А як би base була б позитивна то і decimal теж була б позитивна і тоді було б додавання двох додатних чисел
     */

    m_base += m_decimal / 100;    // integer division
    m_decimal = m_decimal % 100;  // integer remainder

  }


  /**
   * Коротке нагадування що робить std::trunc та std::round
   * std::trunc - відкидає дробову частину числа
   *  Для додатних чисел вона діє як "округлення вниз" (floor).  (2.8 -> 2; 2.1 -> 2)
   *  Для від'ємних чисел вона діє як "округлення вгору" (ceil). (-2.8 -> -2; -2.1 -> -2)
   *  Фактично, вона завжди округлює у бік нуля.
   *
   * std::round
   *  Якщо дробова частина < 0.5, округлюється до меншого за модулем (наприклад, 2.4 → 2.0).
   *  Якщо дробова частина > 0.5, округлюється до більшого за модулем (наприклад, 2.6 → 3.0).
   *
   *
   *   double d {123.4567};
   *   std::cout << std::trunc(d) << '\n'; // 123
   *   std::cout << std::round(d * 100) << '\n'; // 12346 (маємо 123.4567 * 100 = 12345.67 і воно округлюється отже 12346)
   *   std::cout << std::trunc(d) * 100 << '\n'; // 12300 (маємо 123 * 100 = 12300)
   *
   * Тут ми отримуємо з double цілу частину та дробову і делегуємо це все іншому конструктору, у якому вже
   * є вся потрібна обробка для цього
   */
  FixedPoint2(double d) :
    FixedPoint2(
      static_cast<std::int16_t>(std::trunc(d)),
      static_cast<std::int8_t>(std::round(d * 100) - std::trunc(d) * 100)
    )
  {
  }

  explicit operator double() const {
    return m_base + (static_cast<double>(m_decimal) / 100);
  }


  /**
   * Тут реалізовано через member function і відповідно приймає тільки один параметр.
   * Нижче поза класом я реалізував через normal function і там вже два параметри.
   * Чесно кажучи я не знаю що тут краще обрати, але оскільки це бінарний оператор +
   * то нехай буде normal function, бо там два параметри приходять і це простіше розуміти,
   * бо тут один параметр і я постійно забуваю, то лівий чи правий
   */
  // FixedPoint2 operator+ (FixedPoint2& fp) {
  //   // fp це той FixedPoint2 що справа від плюса. Той що з ліва - це і є цей клас
  //   std::int16_t base = m_base + fp.m_base;
  //   std::int8_t decimal = m_decimal + fp.m_decimal;
  //
  //   return FixedPoint2{base, decimal};
  // }


  friend std::ostream& operator<< (std::ostream& out, const FixedPoint2& fp) {
    // out << fp.m_base << '.' << fp.m_decimal;
    out << static_cast<double>(fp);
    return out;
  }


  FixedPoint2 operator- () const {
    // Повертаємо новий об'єкт, а не змінюємо поточний
    return FixedPoint2 {
      static_cast<std::int16_t>(-m_base),
      static_cast<std::int8_t>(-m_decimal)
    };
  }

};

/**
 * Перевантаження оператора + у FixedPoint2
 */
FixedPoint2 operator+ (const FixedPoint2& fp1, const FixedPoint2& fp2) {
  // Конвертуємо обидва числа в загальну кількість decimal. Число 1.23 стане 123, -1.23 стане -123
  int total1 = fp1.m_base * 100 + fp1.m_decimal;
  int total2 = fp2.m_base * 100 + fp2.m_decimal;

  int sum = total1 + total2;

  // Створюємо новий об'єкт, використовуючи цілу частину та залишок. Конструктор сам рознесе знаки та нормалізує результат
  return FixedPoint2 {
    static_cast<std::int16_t>(sum / 100),
    static_cast<std::int8_t>(sum % 100)
 };
}

/**
 * Перевантаження оператора == у FixedPoint2
 */
bool operator== (const FixedPoint2& fp1, const FixedPoint2& fp2) {
  return (fp1.m_base == fp2.m_base) && (fp1.m_decimal == fp2.m_decimal);
}


/**
 * Нижче у main функції описані тести які підтверджують коректну роботу мого класу,
 * оскільки їх багато то вони використовують однакові імена для змінних, тож за потреби
 * один блок розкоментували, запустили, закоментували і пішли до другого
 */

bool testDecimal(const FixedPoint2 &fp){
  if (fp.m_base >= 0)
    return fp.m_decimal >= 0 && fp.m_decimal < 100;
  else
    return fp.m_decimal <= 0 && fp.m_decimal > -100;
}

int main() {
  // FixedPoint2 a{ 34, 56 };
  // std::cout << a << '\n';
  // std::cout << static_cast<double>(a) << '\n';
  // assert(static_cast<double>(a) == 34.56);
  //
  // FixedPoint2 b{ -2, 8 };
  // assert(static_cast<double>(b) == -2.08);
  //
  // FixedPoint2 c{ 2, -8 };
  // assert(static_cast<double>(c) == -2.08);
  //
  // FixedPoint2 d{ -2, -8 };
  // assert(static_cast<double>(d) == -2.08);
  //
  // FixedPoint2 e{ 0, -5 };
  // assert(static_cast<double>(e) == -0.05);
  //
  // FixedPoint2 f{ 0, 10 };
  // assert(static_cast<double>(f) == 0.1);




  // FixedPoint2 a{ 1, 104 };
  // std::cout << a << '\n';
  // std::cout << static_cast<double>(a) << '\n';
  // assert(static_cast<double>(a) == 2.04);
  // assert(testDecimal(a));
  //
  // FixedPoint2 b{ 1, -104 };
  // assert(static_cast<double>(b) == -2.04);
  // assert(testDecimal(b));
  //
  // FixedPoint2 c{ -1, 104 };
  // assert(static_cast<double>(c) == -2.04);
  // assert(testDecimal(c));
  //
  // FixedPoint2 d{ -1, -104 };
  // assert(static_cast<double>(d) == -2.04);
  // assert(testDecimal(d));





  // FixedPoint2 a{ 0.01 };
  // assert(static_cast<double>(a) == 0.01);
  //
  // FixedPoint2 b{ -0.01 };
  // assert(static_cast<double>(b) == -0.01);
  //
  // FixedPoint2 c{ 1.9 }; // make sure we handle single digit decimal
  // assert(static_cast<double>(c) == 1.9);
  //
  // FixedPoint2 d{ 5.01 }; // stored as 5.0099999... so we'll need to round this
  // assert(static_cast<double>(d) == 5.01);
  //
  // FixedPoint2 e{ -5.01 }; // stored as -5.0099999... so we'll need to round this
  // assert(static_cast<double>(e) == -5.01);
  //
  // // Handle case where the argument's decimal rounds to 100 (need to increase base by 1)
  // FixedPoint2 f { 106.9978 }; // should be stored with base 107 and decimal 0
  // assert(static_cast<double>(f) == 107.0);
  //
  // // Handle case where the argument's decimal rounds to -100 (need to decrease base by 1)
  // FixedPoint2 g { -106.9978 }; // should be stored with base -107 and decimal 0
  // assert(static_cast<double>(g) == -107.0);





  assert(FixedPoint2{ 0.75 } == FixedPoint2{ 0.75 });    // Test equality true
  assert(!(FixedPoint2{ 0.75 } == FixedPoint2{ 0.76 })); // Test equality false
  assert(FixedPoint2{ 0.75 } + FixedPoint2{ 1.23 } == FixedPoint2{ 1.98 });    // both positive, no decimal overflow
  assert(FixedPoint2{ 0.75 } + FixedPoint2{ 1.50 } == FixedPoint2{ 2.25 });    // both positive, with decimal overflow
  assert(FixedPoint2{ -0.75 } + FixedPoint2{ -1.23 } == FixedPoint2{ -1.98 }); // both negative, no decimal overflow
  assert(FixedPoint2{ -0.75 } + FixedPoint2{ -1.50 } == FixedPoint2{ -2.25 }); // both negative, with decimal overflow
  assert(FixedPoint2{ 0.75 } + FixedPoint2{ -1.23 } == FixedPoint2{ -0.48 });  // second negative, no decimal overflow
  assert(FixedPoint2{ 0.75 } + FixedPoint2{ -1.50 } == FixedPoint2{ -0.75 });  // second negative, possible decimal overflow
  assert(FixedPoint2{ -0.75 } + FixedPoint2{ 1.23 } == FixedPoint2{ 0.48 });   // first negative, no decimal overflow
  assert(FixedPoint2{ -0.75 } + FixedPoint2{ 1.50 } == FixedPoint2{ 0.75 });   // first negative, possible decimal overflow

  FixedPoint2 a{ -0.48 };
  assert(static_cast<double>(a) == -0.48);
  assert(static_cast<double>(-a) == 0.48);



  return 0;
}