
// #include "../include/Task.h" # Оскільки ми у CMakeLists.txt додали target_include_directories то тепер у відносному шляху немає потреби
#include "Task.h"

Task::Task(int id, const std::string& title, bool isDone)
    : id(id), title(title), isDone(isDone) {}

int Task::getId() const { return id; }
std::string Task::getTitle() const { return title; }
bool Task::isCompleted() const { return isDone; }

void Task::markDone() { isDone = true; }




