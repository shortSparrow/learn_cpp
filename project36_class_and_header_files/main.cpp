#include <iostream>

#include "Date.h"


// Це оригінальний клас
class OriginalDate {
private:
    int m_year{};
    int m_month{};
    int m_day{};

public:
    OriginalDate(int year, int month, int day)
        : m_year { year }
    , m_month { month }
    , m_day { day}
    {
    }

    void print() const { std::cout << "Date(" << m_year << ", " << m_month << ", " << m_day << ")\n"; }

    int getYear() const { return m_year; }
    int getMonth() const { return m_month; }
    int getDay() const { return m_day; }
};


// Опис класу Date переміщено у header, а тут йде наповнення логікою його методів
Date::Date(int year, int month, int day) // constructor definition
    : m_year{ year }, m_month{ month }, m_day{ day } {
}

// print function definition
void Date::print() const {
    std::cout << "Date(" << m_year << ", " << m_month << ", " << m_day << ")\n";
};

// нам доступні всі методі і змінні класу
void Date::setDay(int day) {
    m_day = day;
    this->print();
    print();
}



int main() {
    OriginalDate d { 2015, 10, 14 };
    d.print();

    Date d2 { 2015, 10, 14 };
    d2.print();
    d2.setDay(13);

    /**
     * Functions defined inside the class definition are implicitly inline, which allows them to be #included into
     * multiple code files without violating the ODR.
     *
     * Functions defined outside the class definition are not implicitly inline. They can be made inline by using the inline keyword.
     */

    return 0;
}

