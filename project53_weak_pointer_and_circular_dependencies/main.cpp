#include <iostream>
#include <memory>


/**
 * Circular dependency issues with std::shared_ptr, and std::weak_ptr
 */

class Person {
    std::string m_name;
    std::shared_ptr<Person> m_partner; // initially created empty

public:
    Person(const std::string &name): m_name(name) {
        std::cout << m_name << " created\n";
    }
    ~Person() {
        std::cout << m_name << " destroyed\n";
    }

    friend bool partnerUp(std::shared_ptr<Person> &p1, std::shared_ptr<Person> &p2) {
        if (!p1 || !p2)
            return false;

        p1->m_partner = p2;
        p2->m_partner = p1;

        std::cout << p1->m_name << " is now partnered with " << p2->m_name << '\n';

        return true;
    }
};

class Person2 {
    std::string m_name;
    std::weak_ptr<Person2> m_partner; // note: This is now a std::weak_ptr

public:

    Person2(const std::string &name): m_name(name) {
        std::cout << m_name << " created\n";
    }
    ~Person2() {
        std::cout << m_name << " destroyed\n";
    }

    friend bool partnerUp(std::shared_ptr<Person2> &p1, std::shared_ptr<Person2> &p2) {
        if (!p1 || !p2)
            return false;

        p1->m_partner = p2;
        p2->m_partner = p1;

        std::cout << p1->m_name << " is now partnered with " << p2->m_name << '\n';

        return true;
    }

    /**
     * Одним з недоліків std::weak_ptr є те, що std::weak_ptr не можна використовувати безпосередньо (вони не мають оператора->).
     * Щоб використовувати std::weak_ptr, спочатку потрібно перетворити його на std::shared_ptr. Після цього можна використовувати
     * std::shared_ptr. Щоб перетворити std::weak_ptr на std::shared_ptr, можна використовувати функцію-член lock().
     * Ось приклад:
     *  auto lucy { std::make_shared<Person>("Lucy") };
     *  auto ricky { std::make_shared<Person>("Ricky") };
     *  partnerUp(lucy, ricky);
     *
     *  auto partner = ricky->getPartner(); // get shared_ptr to Ricky's partner
     *  std::cout << ricky->getName() << "'s partner is: " << partner->getName() << '\n';
     */
    std::shared_ptr<Person2> getPartner() const { return m_partner.lock(); } // use lock() to convert weak_ptr to shared_ptr
    const std::string& getName() const { return m_name; }
};

class Resource {
public:
    std::shared_ptr<Resource> m_ptr {}; // initially created empty

    Resource() { std::cout << "Resource acquired\n"; }
    ~Resource() { std::cout << "Resource destroyed\n"; }
};

class Resource2 {
public:
	std::weak_ptr<Resource2> m_ptr {}; // use std::weak_ptr so m_ptr doesn't keep the Resource alive

	Resource2() { std::cout << "Resource acquired\n"; }
	~Resource2() { std::cout << "Resource destroyed\n"; }
};

void example_1() {
    auto lucy { std::make_shared<Person>("Lucy") }; // create a Person named "Lucy"
    auto ricky { std::make_shared<Person>("Ricky") }; // create a Person named "Ricky"

    partnerUp(lucy, ricky); // Make "Lucy" point to "Ricky" and vice-versa
}

void example_2() {
    auto ptr1 { std::make_shared<Resource>() };
    ptr1->m_ptr = ptr1; // m_ptr is now sharing the Resource that contains it
}

void example_3() {
    auto lucy2 { std::make_shared<Person2>("Lucy") };
    auto ricky2 { std::make_shared<Person2>("Ricky") };

    partnerUp(lucy2, ricky2);
}

void example_4() {
    auto ptr5 { std::make_shared<Resource2>() };
    ptr5->m_ptr = ptr5; // m_ptr is now sharing the Resource that contains it
}


int main() {

    /**
     * Коли ми викликали метод partnerUp, то у нас lucy посилається на ricky, а ricky посилається на lucy.
     * І коли ми захочемо видалити lucy то share pointer не видалить її бо на неї посилається ricky,
     * а його не видалить бо на нього посилається lucy. І виходить замкнуте коло і memory leak
     */

    example_1();
    /**
     * Ось що виведе термінал:
     *
     * Lucy created
     * Ricky created
     * Lucy is now partnered with Ricky
     */



    /**
     * Інший приклад, який показує, що circular dependency може бути навіть з одним share pointer
     * ptr1 має pointer який посилається на самого себе (ptr1->m_ptr = ptr1), і посилання на самого
     * себе призводить до того, що цей об'єкт нескінченно посилається сам на себе.
     *
     * І відповідно коли ми вийдемо зі scope то об'єкт не очиститься, бо є посилання на нього (у нього самого)
     * і це призведе до memory leak. Єдиним варіантом було б прибрати посилання на самого себе
     * давши ptr1->m_ptr якесь інше значення, але ми не можемо бо вже вийшли зі scope і втратили доступ до цього об'єкта
     */
    std::cout << "***************** another example *****************\n";
    example_2();


    /**
     * Для чого призначений std::weak_ptr?
     * std::weak_ptr був розроблений для вирішення проблеми «circular ownership», описаної вище. std::weak_ptr є спостерігачем — він
     * може спостерігати та отримувати доступ до того самого об'єкта, що й std::shared_ptr (або інші std::weak_ptrs),
     * але не вважається власником. Пам'ятайте, коли std::shared_ptr виходить за межі області дії, він враховує лише те,
     * чи є інші std::shared_ptr співвласниками об'єкта. std::weak_ptr не враховується!
     *
     *
     */
    std::cout << "***************** weak_ptr *****************\n";

    example_3();
    /**
     * Ось що виведе термінал:
     *
     * Lucy created
     * Ricky created
     * Lucy is now partnered with Ricky
     * Ricky destroyed
     * Lucy destroyed
     *
     * Як бачимо тут відбулося знищення об'єктів.
     * Так стається бо коли вказівник std::shared виходить за межі області видимості, враховується лише те,
     * чи є інші std::shared_ptr співвласниками об'єкта. std::weak_ptr не враховується!
     */


    /**
     * Виправити помилку з нескінченним посиланням самого на себе у Resource, що було описано вище
     * дуже легко з std::weak_ptr, бо shared pointer дивиться чи немає інших посилань і std::weak_ptr не рахує як посилання!
     */
    std::cout << "***************** weak_ptr for Resource *****************\n";

    example_4();
    /**
     * Ось що виведе термінал:
     *
     * Resource acquired
     * Resource destroyed
     */

    return 0;
}