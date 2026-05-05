#ifndef TASK_MANAGER_TASK_H
#define TASK_MANAGER_TASK_H

#include <string>

class Task {
public:
    Task(int id, const std::string& title, bool isDone = false);

    int getId() const;
    std::string getTitle() const;
    bool isCompleted() const;

    void markDone();

private:
    int id;
    std::string title;
    bool isDone;
};

#endif //TASK_MANAGER_TASK_H
