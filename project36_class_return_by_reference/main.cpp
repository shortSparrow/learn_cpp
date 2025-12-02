#include <iostream>
#include <string>

class Employee {
    std::string m_name{};
    int age {15};

    public:
        // return by value
        int getAge() {return age;}

        // отримуємо string_view, бо це легше, адже її копію зробити швидше, а потім вона все одно перетвориться на string коли копіюється у змінну
        void setName(std::string_view name) { m_name = name; }
        // return by reference
        const std::string& getName() const { return m_name; }
};


Employee createEmployee(std::string_view name) {
    Employee e;
    e.setName(name);
    return e;
}

int main() {
    Employee joe{}; // joe exists until end of function
    joe.setName("Joe");

    std::cout << joe.getName() << '\n'; // returns joe.m_name by reference


    // З reference варто бути обережними, щоб не було такого, що об'єкт вже знищено, а ми десь зберегли посилання на одне з його полів

    // Case 1: bad: save returned reference to member of rvalue class object for use later
    const std::string& ref { createEmployee("Garbo").getName() }; // reference becomes dangling when return value of createEmployee() is destroyed
    std::cout << ref << '\n'; // undefined behavior

    // Case 2: okay: copy referenced value to local variable for use later
    std::string val { createEmployee("Hans").getName() }; // makes copy of referenced member
    std::cout << val << '\n'; // okay: val is independent of referenced member



    /**
     * Варто намагатися повертати значення з методів класу як reference
     */

    return 0;
}