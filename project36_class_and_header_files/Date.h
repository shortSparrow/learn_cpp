#ifndef PROJECT36_CLASS_AND_HEADER_FILES_DATE_H
#define PROJECT36_CLASS_AND_HEADER_FILES_DATE_H

// Це клас де функції визначені поза межами класу. У класі є лише function declaration
class Date {
private:
    int m_year{};
    int m_month{};
    int m_day{};

public:
    Date(int year, int month, int day); // constructor declaration

    void print() const; // print function declaration

    int getYear() const { return m_year; }
    int getMonth() const { return m_month; }
    int getDay() const  { return m_day; }
    void setDay(int day);
};



#endif //PROJECT36_CLASS_AND_HEADER_FILES_DATE_H