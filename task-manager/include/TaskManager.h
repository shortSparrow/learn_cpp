#ifndef TASK_MANAGER_TASKMANAGER_H
#define TASK_MANAGER_TASKMANAGER_H

#include <vector>
#include "Task.h" // Тепер має працювати без ../

class TaskManager {
public:
    void addTask(const std::string& title);
    void listTasks() const;
    void completeTask(int id);

    // Це знадобиться для збереження у файл
    const std::vector<Task>& getAllTasks() const;

    void loadFromFile(const std::string& filename);
    void saveToFile(const std::string& filename) const;

private:
    std::vector<Task> tasks;
    int nextId = 1;
};

#endif //TASK_MANAGER_TASKMANAGER_H
