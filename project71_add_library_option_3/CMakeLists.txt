cmake_minimum_required(VERSION 4.2)
project(project71_add_library_option_3)

set(CMAKE_CXX_STANDARD 20)

# Шукаємо бібліотеку в системі
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)


# Лікуємо її до проєкту
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)