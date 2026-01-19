#include <iostream>

class MyString {
private:
    std::string m_str {};

public:
    MyString(std::string_view str): m_str{str} {

    }

    friend std::ostream& operator<< (std::ostream& out, const MyString& obj) {
        out << obj.m_str;
        return out;
    }

    /**
     * Ми робимо дорогу копію, бо хочемо протестувати як буде виводитися MyString у термінал, в реальності
     * тут звіно треба повертати std::string або string_view
     *
     * Приклад із поверненням string_view:
     *  std::string_view{ m_str }.substr(start, count)
     *
     * Ми створили string_view на m_str, який нікуди не зникне (поки існує об'єкт MyString).
     * Тому, хоча тимчасовий об'єкт string_view після виконання методу знищиться,
     * дані в пам'яті, на які він посилався, залишаться цілими.
     */
    MyString operator() (const int start, const int count) {
        // Можна використати метод substr, або написати все руками =)
        // return m_str.substr(start, count);
        std::string sub_str = "";
        for (int i {0}; i<count; i++) {
            sub_str += m_str[start+i];
        }

        return MyString(sub_str);
    }

};

int main() {
    MyString s { "Hello, world!" };
    std::cout << s << '\n'; // Hello, world!
    std::cout << s(7, 5) << '\n'; // world - start at index 7 and return 5 characters

    return 0;
}