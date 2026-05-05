#include <iostream>
#include <string>
#include <memory> // for std::unique_ptr

/**
 * Уявимо приклад, коли ми у блоці try спочатку відкриваємо файл, потім записуємо туди дані
 * і потім закриваємо файл. І нехай у нас помилка стається на етапі запису, тоді ми провалимося
 * у блок catch, обробимо помилку, однак файл лишиться відкритим.
 *
 */

void openFile(std::string_view path) {}
void writeFile(std::string_view path, std::string_view data) {}
void closeFile(std::string_view path) {}
constexpr char PERSON_MALE {'m'};
class FileException: public std::runtime_error {};
class PersonException: public std::runtime_error {};
class Person {
public:
    Person(std::string name, int age, int sex) {}
};
void processPerson(Person* person) {}
void processPerson2(std::unique_ptr<Person> person) {}


int main() {
    const std::string filename {"aa.txt"};
    const std::string data {"new row"};

    try {
        openFile(filename);
        writeFile(filename, data);
        closeFile(filename);
    } catch (const FileException& exception) {
        std::cerr << "Failed to write to file: " << exception.what() << '\n';
    }

    /**
     * Такий тип помилки часто виникає коли є динамічне виділення пам'яті.
     * Якщо processPerson() кине exception ми перескочемо одразу у catch, але виділена пам'ять під
     * john нікуди не зникне. Ба більше, змінна john є локальною для блоку try і відповідно коли ми
     * вийдемо з цього блоку то catch взагалі е матиме доступ до цієї змінної. Тобто catch не має
     * змоги деалокувати пам'ять виділену під john
     */
    try {
        auto* john { new Person{ "John", 18, PERSON_MALE } };
        processPerson(john);
        delete john;
    } catch (const PersonException& exception) {
        std::cerr << "Failed to process person: " << exception.what() << '\n';
    }


    /**
     * Є два способи це виправити:
     *  1. Оголосити змінну john поза межами try catch, щоб catch мав до неї доступ та зміг деалокувати пам'ять
     *      Person* john{ nullptr };
     *      try {...} catch (const PersonException& exception) { Тут вже маємо доступ до john і можемо зробити delete john }
     *  2. Це використання smart pointer std::unique_ptr. Це найкращий варіант
     *
     */

    try {
        // Цей варіант поганий бо якщо помилка станеться до unique_ptr саме на new Person то матимемо ту саму проблему що хотіли вирішити
        // auto* john = new Person{"John", 18, PERSON_MALE};
        // std::unique_ptr<Person> john_up {john}; // unique_ptr сам деалокує дані

        // Цей варіант значно краще
        std::unique_ptr<Person> john = std::make_unique<Person>("John", 18, PERSON_MALE);

        // Або можна і так, але хорошим тоном у C++ буде саме верхній варіант. Там є якісь тонкі особливості, коротше краще використати std::make_unique
        // auto john = std::unique_ptr<Person> john_up {new Person{...}};

        processPerson2(std::move(john));
    } catch (const PersonException& exception) {
        std::cerr << "Failed to process person: " << exception.what() << '\n';
    }

    return 0;
}