#include <iostream>


/**
 * Function Overloading
 * Цн механізм, який дозволяє функціям з однаковими іменами існувати одночасно,
 * якщо вони приймають різні параметри.
 *
 * Важливою умовою є кількість параметрів і їх типи, оператор ... теж враховується
 * Тип який функція повертає не важливий.
 *
 * Коли компілятор компілює функцію, він виконує маніпуляцію іменем, що означає,
 * що скомпільоване ім'я функції змінюється («маніпулюється») на основі різних критеріїв,
 * таких як кількість і тип параметрів, щоб лінкер мав унікальні імена для роботи.
 * Наприклад, функція з прототипом int fcn() може компілюватися в змінене ім'я __fcn_v,
 * тоді як int fcn(int) може компілюватися в змінене ім'я __fcn_i. Отже, хоча в вихідному коді
 * дві перевантажені функції мають однакову назву fcn(), у скомпільованому коді змінені
 * імена є унікальними (__fcn_v проти __fcn_i).
 *
 * Саме тому overloading працює і дозволяє мати кілька функцій з однаковими іменами.
 * Немає стандартизації щодо того, як слід спотворювати імена, тому різні компілятори
 * створюватимуть різні спотворені імена.
 *
 */


int add(int a, int b) {
    return a+b;
}

// // помилка, то тип повернення не впливає overloading
// bool add(int a, int b) {
//     return a+b;
// }

double add(double a, double b) {
    return a+b;
}

double add(double x, double y, double z) {
    return x+y+z;
}

// // Оператор ellipsis (...). Це якщо передали два параметри int і далі будь яку кількість чого завгодно
// double add(int a, int b, ...) {
//     return a+b;
// }


// --------------------------- NUMERIC PROMOTION ---------------------------

void foo(int) {}
void foo(double) {}

int promotedExample() {
    foo('a');  // promoted to match foo(int)
    foo(true); // promoted to match foo(int)
    foo(4.5f); // promoted to match foo(double)

    return 0;
}

// --------------------------- NUMERIC CONVERSION ---------------------------

void baz(double){}
void baz(std::string) {}

int numericConversionExample() {
    baz('a'); // 'a' converted to match baz(double)

    /**
     * In this case, because there is no baz(char) (exact match), and no baz(int) (promotion match),
     * the 'a' is numerically converted to a double and matched with baz(double).
     *
     * Після застосування user-defined conversion компілятор може застосувати додаткові
     * promotions або conversions, щоб знайти відповідність. Отже, якщо наше user-defined conversion
     * було б до типу char замість int, компілятор використав би user-defined conversion
     * до char, а потім підвищив би результат до int, щоб знайти відповідність.
     */
    return 0;
}

// --------------------------- USER-DEFINED CONVERSION ---------------------------

class X {
    public:
        operator int() { return 0; } // Here's a user-defined conversion from X to int
};

void loop(int){}

void loop(double){}

int userDefinedConversionExample() {
    X x; // Here, we're creating an object of type X (named x)
    loop(x); // x is converted to type int using the user-defined conversion from X to int

    return 0;
}


// --------------------------- AMBIGUOUS MATCHES ---------------------------

void ambiguous(double) {}
void ambiguous(int) {}

/**
 * Компілятор видасть помилку, оскільки немає ambiguous з типом long,
 * тож компілятор спробує зробити number promotion, але для типу long цього не можна зробити.
 * Тому компілятор спробує використати type conversions, але тут можливі два варіанти:
 *  1. Привести long до double
 *  2. Привести long до int
 *
 *  Компілятор не знатиме що обрати, тому видасть помилку
 */
void runAmbiguous() {
    long x = ambiguous(5L);
}

/**
 * Те саме, але з іншими типами
 */
void ambiguous2(unsigned int){ }
void ambiguous2(float) {}
void runAmbiguous2() {
    ambiguous2(0);       // int can be numerically converted to unsigned int or to float
    ambiguous2(3.14159); // double can be numerically converted to unsigned int or to float
}


// --------------------------- AMBIGUOUS MATCHES WITH MULTIPLE ARGUMENTS ---------------------------

void ambiguous3(char, int) {
    std::cout << 'a' << '\n';
}

void ambiguous3(char, double) {
    std::cout << 'b' << '\n';
}

void ambiguous3(char, float) {
    std::cout << 'c' << '\n';
}

/**
 * Всі три функції підходять для виклику ambiguous3('x', 'a'), але лише для першої
 * достатньо виконати numeric promotion для підвищення char до int, а іншим треба викликати
 * numeric conversion.
 * Саме тому відпрацює перша функція, яка виведе 'a'
 */
void runAmbiguous3() {
    ambiguous3('x', 'a');
}

int main() {

    std::cout << add(1,2) << '\n'; // 3
    std::cout << add(1.1,2.0) << '\n'; // 3.1

    /**
     * Коли у нас є overload функцій і типи співадають як у крикладі вище все просто,
     * функцію з параметрами double викликаємо add(1.1,2.0), а з int  add(1,2),
     * але що робити коли типи не співпадають.
     *
     * Компілятор запускає type conversion до аргументів кожної функції і дивититься чи щось підійшло.
     * Можливі чотири результати:
     *      1. Type converstion не дав збігів, тому рухаємося до наступної функції. Як відбувається превірка:
     *          1.1 З початку запускається numeric promotion, якщо він не дав результатів йдемо до 1.2
     *          1.2 Запускається numeric conversions, якщо це не дало результатів, то йдемо до 1.3
     *          1.3 Компілятор намагається знайти user-defined conversions, якщо це не дало результатів, то йдемо до 1.4
     *          1.4 Компілятор шукає збіги через оператор ellipsis (...)
     *      2. Збіг по Type converstion знайдено, подальші функції не перевіряються
     *      3. Кілька збігів знайдено. Компілятор видать помилку, бо не знає яку функцію треба обрати
     *      4. Жодна функція не дала збігу по Type converstion. Компілятор видає помилку
     */
    std::cout << add('a', 'b') << '\n'; // 195 - це тому, що відбулося numeric conversion char до int і маємо 97 + 98 = 195








    return 0;
}

