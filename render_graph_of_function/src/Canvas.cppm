module; // Початок глобального фрагмента модуля

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

export module Canvas; // Оголошення модуля йде ПІСЛЯ інклудів


export class Canvas {
private:
    int width, height;
    std::vector<std::string> grid;

public:
    Canvas(int width, int height)
        : width(width), height(height), grid(height, std::string(width, ' ')) {}

    void setCustomPoint(int x, int y, char symbol) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            grid[y][x] = symbol;
        }
    }

    void clear() {
        for (auto& row : grid) std::fill(row.begin(), row.end(), ' ');
    }

    void draw() const {
        for (const auto& row : grid) {
            std::cout << row << "\n";
        }
    }
};