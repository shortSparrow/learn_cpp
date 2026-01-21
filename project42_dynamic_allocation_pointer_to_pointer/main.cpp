#include <iostream>



int main() {

    /**
     * Pointers to pointers
     * Pointer зазвичай посилається на якесь значення, але pointer може посилатися також і
     * на інший pointer (тобто pointer містить адресу іншого pointer).
     * Якщо у звичайному pointer використовується одна * то в цьому випадку використовується
     * дві зірочки **
     *
     */

    int* simple_ptr; // pointer to an int, one asterisk
    int** ptr_to_ptr; // pointer to a pointer to an int, two asterisks



    int value { 5 };
    int* ptr { &value };
    std::cout << ptr << '\n'; // 0xa4d6dffd04
    std::cout << *ptr << '\n'; // 5

    int** ptrptr { &ptr };
    std::cout << *ptrptr << '\n'; // 0xa4d6dffd04
    std::cout << **ptrptr << '\n'; // 5


    // Pointer to a pointer to a pointer to… Просто використаємо три зірочки *** і так далі
    int*** ptrx3;


    /**
     * Вказівники на вказівники мають кілька застосувань.
     * Найпоширеніше використання — динамічне виділення масиву вказівників
     *
     * Це працює так само, як стандартний динамічно розподілений масив, за винятком того, що
     * елементи масиву мають тип «pointer to integer», а не integer.
     */
    int** array { new int*[10] }; // allocate an array of 10 int pointers


    /**
     * Ще одне поширене використання вказівників на вказівники — це полегшена динамічна алокація
     * багато вимірних масивів
     */

    /**
     * Якщо крайній правий вимір масиву — constexpr, ви можете зробити це.
     * Дужки потрібні для того, щоб компілятор знав, що масив має бути вказівником на масив із 5 цілих
     * чисел (який у цьому випадку є першим рядком багатовимірного масиву з 7 рядків). Якщо крайній правий
     * вимір масиву — constexpr, ви можете зробити це. Дужки потрібні для того, щоб компілятор знав, що масив
     * має бути вказівником на масив із 5 цілих чисел (який у цьому випадку є першим рядком багатовимірного
     * масиву з 7 рядків).
     */
    int x { 7 }; // non-constant
    int (*array2)[5] { new int[x][5] }; // крайній правий вимір масиву повинен бути constexpr

    // Це гарне місце для використання автоматичного визначення типу
    auto array3 { new int[x][5] }; // Набагато простіше


    /**
     * На жаль, це відносно просте рішення не працює, якщо крайній правий вимір масиву не є константою часу компіляції.
     */
    int** array5 { new int*[10] }; // allocate an array of 10 int pointers — these are our rows
    for (int count { 0 }; count < 10; ++count)
        array5[count] = new int[5]; // these are our columns


    /**
     * За допомогою цього методу, оскільки кожен стовпець масиву динамічно розподіляється незалежно,
     * можна створювати динамічно розподілені двовимірні масиви, які не є прямокутними.
     * Наприклад, ми можемо створити масив трикутної форми.
     *
     * У наведеному вище прикладі зверніть увагу, що масив[0] – це масив довжини 1,
     * масив[1] – це масив довжини 2 тощо…
     */
    int** array6 { new int*[10] }; // allocate an array of 10 int pointers — these are our rows
    for (int count { 0 }; count < 10; ++count)
        array6[count] = new int[count+1]; // these are our columns


    // Deallocating a dynamically allocated two-dimensional array using this method requires a loop as well:
    for (int count { 0 }; count < 10; ++count)
        delete[] array6[count];
    delete[] array6; // this needs to be done last



    return 0;
}