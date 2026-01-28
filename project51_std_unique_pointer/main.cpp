#include <iostream>
#include <memory>

class Resource {
public:
	Resource() { std::cout << "Resource acquired\n"; }
	~Resource() { std::cout << "Resource destroyed\n"; }
};

std::ostream& operator<<(std::ostream& out, const Resource&) {
	out << "I am a resource";
	return out;
}


class Fraction {
private:
	int m_numerator{ 0 };
	int m_denominator{ 1 };

public:
	Fraction(int numerator = 0, int denominator = 1) :
		m_numerator{ numerator }, m_denominator{ denominator } {
	}

	friend std::ostream& operator<<(std::ostream& out, const Fraction &f1) {
		out << f1.m_numerator << '/' << f1.m_denominator;
		return out;
	}
};


// This function takes ownership of the Resource, which isn't what we want
void takeOwnership(std::unique_ptr<Resource> res) {
	if (res)
		std::cout << *res << '\n';
} // the Resource is destroyed here


// The function only uses the resource, so we'll accept a pointer to the resource, not a reference to the whole std::unique_ptr<Resource>
void useResource(const Resource* res) {
	if (res)
		std::cout << *res << '\n';
	else
		std::cout << "No resource\n";
}

int main() {

    /**
     * std::unique_ptr
     * Його слід використовувати для керування будь-яким динамічно алокованим об'єктом, який не є спільним для кількох об'єктів.
     * Тобто, std::unique_ptr має повністю володіти об'єктом, яким він керує, а не ділити це право власності з іншими класами.
     */

	// алокує об'єкт Resource object та робить його власністю std::unique_ptr
	std::unique_ptr<Resource> res1{ new Resource{} }; // Resource created here
	std::unique_ptr<Resource> res2{}; // Start as nullptr

	std::cout << "res1 is " << (res1 ? "not null\n" : "null\n");
	std::cout << "res2 is " << (res2 ? "not null\n" : "null\n");

	// res2 = res1; // Won't compile: copy assignment is disabled
	res2 = std::move(res1); // res2 assumes ownership, res1 is set to null

	std::cout << "Ownership transferred\n";

	std::cout << "res1 is " << (res1 ? "not null\n" : "null\n");
	std::cout << "res2 is " << (res2 ? "not null\n" : "null\n");

	/**
	 * Ось що буде виведено:
	 * Resource acquired
	 * res1 is not null
	 * res2 is null
	 * Ownership transferred
	 * res1 is null
	 * res2 is not null
	 * Resource destroyed
	 */

	/**
	 * Оскільки std::unique_ptr розроблений з урахуванням семантики переміщення, ініціалізація копіюванням та присвоювання
	 * копіюванням вимкнені. Якщо ви хочете передати вміст, який керується std::unique_ptr, ви повинні використовувати
	 * семантику переміщення.
	 *
	 * Оскільки std::unique_ptr може не завжди керувати об'єктом (сторений порожнім (з дефолтним конструктором) або
	 * просто переміщений через std::move) то варто перевіряти чи pointer містить об'єкт через if (автоматично приведеться до true/false)
	 * if (ptr) { ... }
	 *
	 * 	if (res) // use implicit cast to bool to ensure res contains a Resource
	 * 		std::cout << *res << '\n'; // print the Resource that res is owning
	 */


	//********************************** std::make_unique ********************************************
	/**
	 * std::make_unique - це функція як створює std::unique_ptr. Краще використовувати саме її, бо вона
	 * доволі лаконічна і виразна у свої нзві.
	 */
	std::cout << "********************** make_unique ************************\n";

	// Create a single dynamically allocated Fraction with numerator 3 and denominator 5
	// We can also use automatic type deduction to good effect here
	auto f1{ std::make_unique<Fraction>(3, 5) };
	std::cout << *f1 << '\n';

	// Create a dynamically allocated array of Fractions of length 4
	auto f2{ std::make_unique<Fraction[]>(4) };
	std::cout << f2[0] << '\n';

	/**
	 * Best practice
	 * Use std::make_unique() instead of creating std::unique_ptr and using new yourself.
	 */


	//*******************************************************
	std::cout << "******************* pass ptr to function by value *********************\n";

	/**
	 * Якщо ви хочете, щоб функція отримала право власності на вміст вказівника, передайте std::unique_ptr за значенням.
	 * Зверніть увагу, що оскільки семантику копіювання вимкнено, вам потрібно буде використовувати std::move,
	 * щоб фактично передати змінну.
	 */
	auto ptr{ std::make_unique<Resource>() };

	//    takeOwnership(ptr); // This doesn't work, need to use move semantics
	takeOwnership(std::move(ptr)); // ok: use move semantics
	/**
	 * Ось що виведе:
	 *
	 * Resource acquired
	 * I am a resource
	 * Resource destroyed
	 *
	 *
	 * Зверніть увагу, що в цьому випадку право власності на Resource було передано до takeOwnership(),
	 * тому Resource було знищено в кінці takeOwnership(), а не в кінці main().
	 * Але таке нам треба рідко, швидше за все ми будемо передавати параметр як const reference
	 * void useResource(const Resource* res) {....}
	 *
	 */

	auto ptr2{ std::make_unique<Resource>() };
	useResource(ptr2.get()); // note: get() used here to get a pointer to the Resource
	/**
	 * Ось що виведе:
	 *
	 * Resource acquired
	 * I am a resource
	 * Resource destroyed
	 */


	/**
	 * Правила використання std::unique_ptr:
	 *
	 *	1) Не дозволяйте кільком об'єктам керувати одним ресурсом.
	 *	   Хоча синтаксично це допустимо, кінцевим результатом буде те, що як res1, так і res2 спробують видалити Ресурс,
	 *	   що призведе до невизначеної поведінки.
	 *
	 *		Resource* res{ new Resource() };
	 *		std::unique_ptr<Resource> res1{ res };
	 *		std::unique_ptr<Resource> res2{ res };
	 *
	 *  2) не видаляйте ресурс вручну з-під std::unique_ptr. Якщо ви це зробите, std::unique_ptr спробує видалити вже
	 *     видалений ресурс, що знову призведе до невизначеної поведінки.
	 *		Resource* res{ new Resource() };
	 *		std::unique_ptr<Resource> res1{ res };
	 *		delete res;
	 *
	 */


    return 0;
}