#include <iostream>


/**
 * Множинне успадкування (Multiple Inheritance) — це механізм
 * Це ситуація, коли один клас успадковує властивості та поведінку від кількох батьківських класів.
 *
 * Мета: Моделювання зв'язків типу "є". Наприклад, Teacher (Вчитель) є одночасно і Person (Людиною), і Employee (Співробітником).
 * Але це приходить до проблеми "алмазу" (Diamond Problem). Це коли у нас кілька дочірніх класів успадковуються від одного батька,
 * у прикладі нижче Printer та Scanner обидва успадкувалися від PoweredDevice. Тож Copier має два успадкування PoweredDevice, одне
 * від Scanner, а інше від Printer.
 * Відповідно я не зможу взяти поле 'a' у інстанса класу Copier, бо буде помилка
 */

class PoweredDevice {
public:
    int a {1};
};

class Scanner: public PoweredDevice {};

class Printer: public PoweredDevice {};

class Copier: public Scanner, public Printer {};


/**
 * Mixin (Міксин) — це концепція (паттерн)
 * Міксин — це невеликий клас, який призначений для "підмішування" певної конкретної функції до інших класів.
 *
 * Мета: Додати невелику, чітко визначену "можливість" без створення глибокої ієрархії. Наприклад,
 * клас Serializable, який додає функцію збереження у файл, або Printable, що додає можливість виводу на екран.
 *
 * Головна відмінність: Міксини не призначені для самостійного створення об'єктів (ви ніколи не створите new Printable()).
 * Вони існують лише для того, щоб "доповнити" інші класи.
 *
 * Тобто mixin це просто підхід, але програмно це буквально те саме що і множинне успадкування.
 * Зазвичай mixin мають методи, але не мають власних змінних (але це не обов'язкове правило)
 * Ми ніколи не створюємо об'єкт самого mixin, лише успадковуємо його і створюємо вже об'єкт на Child
 */

struct Point2D {
    int x{};
    int y{};
};

// mixin Box class
class Box {
public:
    void setTopLeft(Point2D point) { m_topLeft = point; }
    void setBottomRight(Point2D point) { m_bottomRight = point; }
private:
    Point2D m_topLeft{};
    Point2D m_bottomRight{};
};

// mixin Label class
class Label {
public:
    void setText(const std::string_view str) { m_text = str; }
    void setFontSize(int fontSize) { m_fontSize = fontSize; }
private:
    std::string m_text{};
    int m_fontSize{};
};

// mixin Tooltip class
class Tooltip  {
public:
    void setText(const std::string_view str) { m_text = str; }
private:
    std::string m_text{};
};

class Button : public Box, public Label, public Tooltip {}; // Button using three mixins





int main() {

    Copier copier {};
    // int b {copier.a}; // Non-static member 'a' found in multiple base-class subobjects of type 'PoweredDevice'
    int b1 {copier.Printer::a}; // так працювати буде, бо я точно вказав звідки брати поле 'a'


    // Приклад mixin
    // Ми напряму вказуємо у якого класу викличемо метод, щоб не було колізії, якщо у кількох з успадкованих класів є однакова назва методу
    Button button{};
    button.Box::setTopLeft({ 1, 1 });
    button.Box::setBottomRight({ 10, 10 });
    button.Label::setText("Submit");
    button.Label::setFontSize(6);
    button.Tooltip::setText("Submit the form to the server");

    return 0;
}