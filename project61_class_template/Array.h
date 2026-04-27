#ifndef PROJECT61_CLASS_TEMPLATE_ARRAY_H
#define PROJECT61_CLASS_TEMPLATE_ARRAY_H
#include <memory>
#include <cassert>


template <typename T>
class Array {
private:
    int m_length{};
    std::unique_ptr<T[]> m_data{}; // Використовуємо unique_ptr для масиву

public:
    Array(int length) : m_length{ length } {
        assert(length > 0);
        m_data = std::make_unique<T[]>(length); // Автоматичне виділення
    }

    // Правило 5: unique_ptr за замовчуванням забороняє копіювання,
    // тому delete писати навіть не обов'язково, але можна для ясності.
    Array(const Array&) = delete;
    Array& operator=(const Array&) = delete;

    // Деструктор ~Array() НЕ ПОТРІБЕН! unique_ptr сам видалить пам'ять.

    void erase() {
        m_data.reset(); // Очищує вказівник і видаляє масив
        m_length = 0;
    }

    T& operator[](int index) {
        assert(index >= 0 && index < m_length);
        return m_data[index];
    }

    int getLength() const { return m_length; }
};
#endif //PROJECT61_CLASS_TEMPLATE_ARRAY_H
