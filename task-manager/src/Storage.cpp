#include "Storage.h"
#include <fstream>
#include <sstream>
#include <iostream>

void Storage::saveToFile(const std::string& filename, const std::vector<Task>& tasks) {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Error: Could not open file for writing!\n";
        return;
    }

    for (const auto& task : tasks) {
        // Формат: ID|Title|IsDone
        outFile << task.getId() << "|"
                << task.getTitle() << "|"
                << (task.isCompleted() ? 1 : 0) << "\n";
    }
}

std::vector<Task> Storage::loadFromFile(const std::string& filename) {
    std::vector<Task> tasks;
    std::ifstream inFile(filename);

    if (!inFile) return tasks; // Якщо файлу немає, просто повертаємо порожній список

    std::string line;
    while (std::getline(inFile, line)) {
        std::stringstream ss(line);
        std::string idStr, title, doneStr;

        // Розбиваємо рядок по розділювачу '|'
        if (std::getline(ss, idStr, '|') &&
            std::getline(ss, title, '|') &&
            std::getline(ss, doneStr, '|')) {
                int id = std::stoi(idStr);
                bool isDone = (doneStr == "1");
                tasks.emplace_back(id, title, isDone);
            }
    }
    return tasks;
}