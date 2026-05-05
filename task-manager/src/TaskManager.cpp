#include <iostream>

#include "TaskManager.h"

#include "Storage.h"
#include "StringUtils.h"


void TaskManager::addTask(const std::string& title) {
    std::string cleanTitle = Utils::toUpper(title); // Викликаємо зовнішній модуль
    tasks.emplace_back(Task{nextId++, title});
}

void TaskManager::listTasks() const {
    if (tasks.empty()) {
        std::cout << "List is empty.\n";
        return;
    }
    for (const auto& task : tasks) {
        std::cout << "[" << (task.isCompleted() ? "X" : " ") << "] " 
                  << task.getId() << ": " << task.getTitle() << "\n";
    }
}

void TaskManager::completeTask(int id) {
    for (auto& task : tasks) {
        if (task.getId() == id) {
            task.markDone();
            return;
        }
    }
    std::cout << "Task with ID " << id << " not found.\n";
}

const std::vector<Task>& TaskManager::getAllTasks() const {
    return tasks;
}

void TaskManager::loadFromFile(const std::string& filename) {
    tasks = Storage::loadFromFile(filename);
    // Оновлюємо nextId, щоб нові завдання не мали дублікатів ID
    if (!tasks.empty()) {
        int maxId = 0;
        for (const auto& t : tasks) {
            if (t.getId() > maxId) maxId = t.getId();
        }
        nextId = maxId + 1;
    }
}

void TaskManager::saveToFile(const std::string& filename) const {
    Storage::saveToFile(filename, tasks);
}


/**
 * Навіщо існує таке дивне розділення на хедер і ось такий синтаксис у .cpp
 *
 *  1. Швидкість компіляції (Найголовніша причина)
 *  C++ компілює кожен .cpp файл окремо.
 *
 *  Коли ви робите #include "TaskManager.h" у десяти різних файлах, препроцесор просто копіює вміст
 *  цього хедера в кожен із них.
 *
 *  Якщо вся логіка (тіла функцій) лежить у .h, то при зміні одного рядка коду в методі addTask
 *  компілятору доведеться перекомпілювати всі 10 файлів, які його підключають.
 *
 *  Якщо логіка в .cpp, то ви змінюєте лише один файл TaskManager.cpp. Решта проєкту просто "знає",
 *  що така функція існує, і їх перекомпілювати не треба. У великих проєктах це економить години часу.
 *
 *
 *  2. Вирішення циклічних залежностей
 *  Уявіть: Клас A має знати про клас B, а клас B — про клас A.
 *
 *  Якщо ви пишете все в хедерах, ви отримаєте нескінченну рекурсію інклудів. Розділення дозволяє в
 *  хедері лише "оголосити" клас, а повну логіку прописати в .cpp, де вже підключено все необхідне.
 *
 *
 *
 *  3. "Чистий" інтерфейс (Інкапсуляція)
 *  Коли програміст відкриває .h файл, він хоче бачити що клас робить (інтерфейс), а не як
 *  він це робить (реалізація).
 *
 *  Хедер — це документація.
 *  CPP — це "кухня", куди стороннім заглядати не обов'язково.
 *
 */


