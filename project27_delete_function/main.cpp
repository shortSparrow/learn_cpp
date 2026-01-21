#include <iostream>


/**
 * Оскільки у С++ є numeric promotion та numeric conversions то бувають
 * моменти, коли ми не хочемо, щоб такі операції відбувалися для аргументів функції.
 * Для цього існує оператор delete. Він забороняє виклик функції з таким аргументом,
 * для якої вказаний
 *
 */

// За замоченням
void printInt(int x) {
    std::cout << x << '\n';
}

/**
 * Якби цього не було то printInt('a') та printInt(true) працювали і видали
 * 97 і 1 відповідно. Але коли ми вказали для функцій з цими параметрами delete,
 * то виклик буде заблоковано
 */
void printInt(char) = delete; // calls to this function will halt compilation
void printInt(bool) = delete; // calls to this function will halt compilation

int main() {

    printInt(97);   // okay

    printInt('a');  // compile error: function deleted
    printInt(true); // compile error: function deleted

    /**
     * Важливий момент, delete не вливає на видимість функції, тобто у випадку
     * з double компілятор буде шукати printInt з аргументом double. Коли не знайде,
     * то буде намагатися привести тип double до тих що є. І це можна буде зробити
     * для типів int та char. І хоч для char вказаний delete, та це не говорить,
     * що компілятор не повинен там шукати type conversions, тож буде помилка,
     * оскільки дві функції підходять для запуску аргументу double (хоча одна з них
     * і не запуститься бо там стоїть delete).
     *
     * = delete means “I forbid this”, not “this doesn’t exist”.
     */
    printInt(5.0);  // compile error: ambiguous match



    return 0;
}

/**
 * Якщо ми хочемо, щоб функція sumFunc працювала лише для тих аргументів,
 * що ми вказали то це можна зробити за допомогою template
 */

// This function will take precedence for arguments of type int
void someFunc(int x) {
    std::cout << x << '\n';
}

// This function template will take precedence for arguments of other types
// Since this function template is deleted, calls to it will halt compilation
template <typename T>
void someFunc(T x) = delete;