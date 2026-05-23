cmake_minimum_required(VERSION 4.2)
project(project71_add_library_option_1)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} main.cpp)

# Підключаємо бібліотеку напряму
target_include_directories(${PROJECT_NAME} PRIVATE "C:/Libs/nlohmann_json")
