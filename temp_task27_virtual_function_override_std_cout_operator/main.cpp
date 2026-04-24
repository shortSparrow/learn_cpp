#include <iostream>
#include <vector>

class Point {
private:
	int m_x{};
	int m_y{};

public:
	Point(int x, int y) : m_x{ x }, m_y{ y } {}

	friend std::ostream& operator<<(std::ostream& out, const Point& p) {
		return out << "Point(" << p.m_x << ", " << p.m_y << ')';
	}
};

class Shape {
public:
	virtual std::ostream& print(std::ostream& out) const = 0;

	/**
	 * Для того аби перевантаження оператора << працювало з virtual function ми спочатку робимо просте перевантаження,
	 * і далі у ньому викликаємо virtual function яка приймаючи поточний std::ostream& out і якось його обробляє.
	 * Обробка вже буде у Derived класах і для кожної фігури може бути своя
	 */
	friend std::ostream& operator<<(std::ostream& out, const Shape& p) {
		return p.print(out);
	}
	virtual ~Shape() = default;
};

class Triangle : public Shape {
private:
	Point m_p1;
	Point m_p2;
	Point m_p3;

public:
	Triangle(const Point& p1, const Point& p2, const Point& p3)
		: m_p1{ p1 }, m_p2{ p2 }, m_p3{ p3 } { }

	std::ostream& print(std::ostream& out) const override {
		return out << "Triangle(" << m_p1 << ", " << m_p2 << ", " << m_p3 << ')';
	}
};

class Circle : public Shape {
private:
	Point m_center;
	int m_radius;

public:
	Circle(const Point& center, int radius)
		: m_center{ center }, m_radius{ radius } { }

	std::ostream& print(std::ostream& out) const override {
		return out << "Circle(" << m_center << ", radius " << m_radius << ')';
	}

	int get_radius(){return m_radius;}
};

int getLargestRadius(const std::vector<Shape*>& v) {
	int radius {0};
	for (Shape* item: v) {
		if (const auto circle_ptr = dynamic_cast<Circle*>(item)) {
			radius = std::max(radius, circle_ptr->get_radius());
		}
	}

	return radius;
}

int main() {
	Circle c{ Point{ 1, 2 }, 7 };
	std::cout << c << '\n';

	Triangle t{ Point{ 1, 2 }, Point{ 3, 4 }, Point{ 5, 6 } };
	std::cout << t << '\n';

	std::cout << "----------------------------------------------------------------\n";

	// Робимо масив з pointer на Shape з динамічною алокацією.
	std::vector<Shape*> v{
		new Circle{Point{ 1, 2 }, 7},
		new Triangle{Point{ 1, 2 }, Point{ 3, 4 }, Point{ 5, 6 }},
		new Circle{Point{ 7, 8 }, 3}
	};

	// print each shape in vector v on its own line here
	for (const Shape* item: v) {
		std::cout << *item << '\n';
		// item->print(std::cout); // або можна так, оскільки наш print(std::ostream& out) очікує std::cout як аргумент
	}
	std::cout << "The largest radius is: " << getLargestRadius(v) << '\n';

	for (const auto* item: v) {
		delete item;
	}

	return 0;
}