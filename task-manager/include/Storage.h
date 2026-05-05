#ifndef TASK_MANAGER_STORAGE_H
#define TASK_MANAGER_STORAGE_H

#include <vector>
#include "Task.h"


class Storage {
public:
    static void saveToFile(const std::string& filename, const std::vector<Task>& tasks);
    static std::vector<Task> loadFromFile(const std::string& filename);
};

#endif //TASK_MANAGER_STORAGE_H
