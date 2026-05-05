#include <iostream>

#include "Storage.h"
#include "TaskManager.h"


/**
 * У цьому проєкті реалізовано таск менеджер, ми можемо створити задачу,
 * позначити її як завершену або не завершену і зберегти у файл, а потім дістати.
 *
 * В цьому проєкті реалізований стандартний підхід з розбиттям на header (.h) файли
 * та .cpp де лежить сама логіка.
 * Всі .h файли лежать у директорії /include, а виконувані файли .cpp у /src.
 * Важливо зауважити, що .h файл це не лише як інтерфейс, бо там окрім заголовків методів
 * клас також лежить конструктор класу і всі змінні. Файл .h це креслення, а .cpp це
 * конкретна реалізація за цим кресленням.
 *
 * Якщо більш цікавить новий підхід з модулями, який ближче до підходу з імпортами
 * як у JS то можна глянути проєкт FuncPlotter, він реалізований на модулях, що
 * додали у C++20.
 */

int main() {

    TaskManager manager {};

    const std::string dbFile = "tasks.txt";
    // Завантажуємо дані при старті
    // manager.loadFromFile(dbFile);

    // Додаємо завдання
    manager.addTask("Learn C++ Project Structure");
    manager.addTask("Build Task Manager");
    manager.addTask("Practice Functions Plotter");

    std::cout << "--- My Tasks ---\n";
    manager.listTasks();

    // Позначаємо одне як виконане
    manager.completeTask(1);

    std::cout << "\n--- Updated Tasks ---\n";
    manager.listTasks();

    // Зберігаємо у файл
    Storage::saveToFile(dbFile, manager.getAllTasks());
    std::cout << "\nTasks saved to tasks.txt\n";

    return 0;
}
