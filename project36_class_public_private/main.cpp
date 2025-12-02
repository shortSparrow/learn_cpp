#include <iostream>

int main() {

    /**
     * public/private поля
     * Поля у struct є public за замовченням
     * Поля у class private за замовченням
     *
     */



    struct DateStruct {
        int year {};       // public by default
        int month {};      // public by default
        int day {};        // public by default

        // public by default
        void print() const {
            // public members can be accessed in member functions of the class type
            std::cout << year << '/' << month << '/' << day;
        }
    };

    DateStruct today { 2020, 10, 14 };
    today.day = 16; // okay: the day member is public
    today.print();  // okay: the print() member function is public



    class DateClass {
        int m_year {};     // private by default
        int m_month {};    // private by default
        int m_day {};      // private by default

        // private by default
        void print() const {
            // private members can be accessed in member functions
            std::cout << m_year << '/' << m_month << '/' << m_day;
        }
    };


    DateClass todayClass { 2020, 10, 14 }; // compile error: can no longer use aggregate initialization
    todayClass.m_day = 16; // compile error: the m_day member is private
    todayClass.print();    // compile error: the print() member function is private


    // Вказуємо що буде public, а що private
    class DateClassPublic {
        public:
            void print() const {
                // private members can be accessed in member functions
                std::cout << m_year << '/' << m_month << '/' << m_day;
            }


        private:
            int m_year {};
            int m_month {};
            int m_day {};
    };

    DateClassPublic todayClassPublic {};
    todayClassPublic.print(); // ok




    return 0;
}