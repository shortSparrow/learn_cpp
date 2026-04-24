#include <iostream>
#include <string_view>
#include <vector>

class Base {
protected:
    int m_value{};

public:
    Base(int value) : m_value{ value } { }

    virtual ~Base() = default;

    virtual std::string_view getName() const { return "Base"; }
    int getValue() const { return m_value; }
};

class Derived: public Base {
public:
    Derived(int value) : Base{ value } { }

    std::string_view getName() const override { return "Derived"; }
};

void print_name(Base base) {
    std::cout << base.getName() << '\n';
}

void print_name_ref(Base& base) {
    std::cout << base.getName() << '\n';
}


int main() {
    Derived derived{ 5 };
    std::cout << "derived is a " << derived.getName() << " and has value " << derived.getValue() << '\n'; // Derived

    Base& ref{ derived };
    std::cout << "ref is a " << ref.getName() << " and has value " << ref.getValue() << '\n'; // Derived

    Base* ptr{ &derived };
    std::cout << "ptr is a " << ptr->getName() << " and has value " << ptr->getValue() << '\n'; // Derived



    Derived derived2{ 5 };
    Base base2{ derived2 }; // Тут тип Base не посилання, і компілятор робить копію з derived2 і створює Base
    std::cout << "base is a " << base2.getName() << " and has value " << base2.getValue() << '\n'; // Base

    std::cout << "--------------------------------------------------------------\n";
    /**
     * Поведінка з Derived зробити Base доволі безглузда, але подекуди вона використовується у функціях.
     * Можна не помітити, що у функції параметр не reference, а просто клас. І тоді virtual function
     * нам не допоможе
     */
    print_name(derived2); // Base
    print_name_ref(derived2); // Derived

    std::cout << "--------------------------------------------------------------\n";
    /**
     * Те саме матимемо, якщо у нас буде вектор з базовим класом, а не поінтером на базовий клас.
     * Ми втратимо інформацію про Derived і virtual function не допоможе.
     */
    std::vector<Base> vector_list {};
    vector_list.push_back(Base {1});
    vector_list.push_back(Base {2});

    for(const auto& v: vector_list) {
        std::cout << v.getName() << '\n'; // Base, Base
    }

    std::cout << "--------------------------------------------------------------\n";

    // Або якщо хочемо це виправити, то треба робити такий вектор
    std::vector<Base*> vector_list_2 {};

    Base b{ 5 };
	Derived d{ 6 };

    vector_list_2.push_back(&b);
    vector_list_2.push_back(&d);

    for(const auto& v: vector_list_2) {
        std::cout << v->getName() << '\n'; // Base, Derived
    }


    // Або можна замість pointer використати reference_wrapper і тобі пушить не reference
    std::vector<std::reference_wrapper<Base>> vector_list_3{};
    vector_list_3.push_back(b);
    vector_list_3.push_back(d);

    for(const auto& v: vector_list_3) {
        std::cout << v.get().getName() << '\n'; // Base, Derived
    }

    return 0;
}