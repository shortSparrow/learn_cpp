cmake_minimum_required(VERSION 4.2)
project(task_manager)

set(CMAKE_CXX_STANDARD 20)

# Вказуємо, де лежать наші заголовочні файли (.h)
include_directories(include)

# Список усіх сирцевих файлів (.cpp)
set(SOURCES
        src/main.cpp
        src/Task.cpp
        src/TaskManager.cpp
        src/Storage.cpp
        include/StringUtils.h
        src/Utils.cpp
)

# Створюємо виконуваний файл
add_executable(task_manager ${SOURCES})

# Тепер кажемо CMake: "Коли збираєш TaskManager, шукай хедери в папці include"
# Цей рядок має йти після add_executable. І воно потрібно для того, аби в умовному
# Task.cpp ми могли б імпортувати #include "Task.h" без умовного шляху "../include/Task.h"
target_include_directories(task_manager PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)