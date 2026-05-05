#include <cmath>
#include <functional>

import Canvas;
import MathEngine;
import Plotter;
import Config;


/**
 * Цей проєкт на відміну від TaskManager буде написаний на модулях, які додали в C++20.
 * У нас вже не буде структури з .h та .cpp файлами, а все буде в .ixx або .cppm, але
 * при цьому зберігається швидкість компіляції.
 *
 * На початку файлу ми маємо #include <functional>, без нього проєкт не скомпілюється, бо хоч у main.cpp немає
 * нічого для чого потрібний functional, однак це є у модулі, що ми імпортуємо, а саме у MathEngine. І хоча
 * це мало б додатися автоматично, однак чомусь у мене не додалося, тому треба було вписати його вручну, аби
 * збірка не падала.
 *
 * Як ідея модулі мені подобаються значно більше ніж поєднання .h та .cpp, однак наразі все ще
 * не досконала річ для новачка, можуть бути проблеми як от з #include <functional>
 */


int main() {
    const int W = 80;
    const int H = 30;

    Canvas canvas(W, H);
    Plotter plotter(canvas);

    // Задаємо діапазон X від -10 до 10
    PlotSettings::Range xRange = {-10.0, 10.0};

    // Малюємо синус
    plotter.render(Math::sine, xRange, W, H);

    // Малюємо log
    // Варіант, якщо не хочемо прописувати функцію у namespace Math, то можна написати через lambda function
    // plotter.render([] (const double x)->double {return std::exp(x);}, xRange, W, H);

    canvas.draw();

    return 0;
}