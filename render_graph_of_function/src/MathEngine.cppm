module; // Початок глобального фрагмента модуля

#include <cmath>
#include <functional>

export module MathEngine; // Оголошуємо назву модуля


export namespace Math {
    using MathFunc = std::function<double(double)>;

    double sine(double x) { return std::sin(x); }
    double quadratic(double x) { return (x * x) * 0.1; } // стиснуто для екрану
}