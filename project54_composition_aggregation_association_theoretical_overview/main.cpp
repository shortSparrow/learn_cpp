#include <iostream>


int main() {

    /**
     * Існує два основних типи композиції (composition) об'єктів: композиція (composition) та агрегація (aggregation).
     *
     * Composition
     * Щоб вважатися композицією, об'єкт та частина повинні мати такий взаємозв'язок:
     *
     *  The part (member) is part of the object (class)
     *  The part (member) can only belong to one object (class) at a time
     *  The part (member) has its existence managed by the object (class)
     *  The part (member) does not know about the existence of the object (class)
     *
     *
     * Як я зрозумів composition це коли один клас має поле з іншим класом
     * class PrimeNumber {
     *  int value {};
     * }
     *
     * class Number {
     *  PrimeNumber num {};
     *
     *  Number(PrimeNumber p_num): mum{p_num} {...}
     * }
     *
     * Клас Number має у собі поле з типом клас PrimeNumber. Тобто створюючи клас Number
     * ми одразу створюємо клас PrimeNumber
     *
     *
     *
     * Aggregation
     * Щоб вважатися агрегацією, цілісний об'єкт та його частини повинні мати такий взаємозв'язок:
     *
     *  The part (member) is part of the object (class)
     *  The part (member) can (if desired) belong to more than one object (class) at a time
     *  The part (member) does not have its existence managed by the object (class)
     *  The part (member) does not know about the existence of the object (class)
     *
     * class PrimeNumber {
     *  int value {};
     * }
     *
     * class Number {
     *  PrimeNumber& num {};
     *
     *  Number(PrimeNumber& p_num): mum{p_num} {...}
     *
     * Тут у нас у Number передається лише reference, тобто життя PrimeNumber вже не залежить від Number
     *
     *
     * Summarizing composition and aggregation
     *
     *  Compositions:
     *      Typically use normal member variables
     *      Can use pointer members if the class handles object allocation/deallocation itself
     *      Responsible for creation/destruction of parts
     *  Aggregations:
     *      Typically use pointer or reference members that point to or reference objects that live outside
     *      the scope of the aggregate class
     *      Not responsible for creating/destroying parts
     *
     */

    /**
     * Association
     *
     * Щоб вважатися асоціацією, об'єкт та інший об'єкт повинні мати такий взаємозв'язок:
     *  The associated object (member) is otherwise unrelated to the object (class)
     *  The associated object (member) can belong to more than one object (class) at a time
     *  The associated object (member) does not have its existence managed by the object (class)
     *  The associated object (member) may or may not know about the existence of the object (class)
     *
     * На відміну від композиції або агрегації, де частина є частиною цілого об'єкта, в асоціації асоційований об'єкт
     * не пов'язаний з об'єктом. Так само, як і в агрегації, асоційований об'єкт може належати одночасно до декількох
     * об'єктів і не управляється цими об'єктами. Однак, на відміну від агрегації, де відносини завжди односпрямовані,
     * в асоціації відносини можуть бути односпрямованими або двоспрямованими (коли два об'єкти знають один про одного).
     *
     * Відносини між лікарями та пацієнтами є чудовим прикладом асоціації. Лікар, безперечно, має стосунки зі своїми
     * пацієнтами, але концептуально це не є відносинами частини/цілого (композиція об'єктів).
     *
     * Оскільки асоціації є широким типом зв'язків, їх можна реалізувати багатьма різними способами.
     * Однак найчастіше асоціації реалізуються за допомогою вказівників, де об'єкт вказує на пов'язаний об'єкт.
     *
     * class Doctor {
     *  std::vector<std::reference_wrapper<const Patient>> m_patient{};
     *  ...
     * }
     *
     * class Patient {
     *  std::vector<std::reference_wrapper<const Doctor>> m_doctor{};
     *  ...
     * }
     *
     * Тут ми бачимо що Patient і Doctor посилаються одне на одного через reference_wrapper, тобто
     * обгортку для reference (можна було б і без неї, але тоді довелося б писати через pointer)
     */


    return 0;
}