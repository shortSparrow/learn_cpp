cmake_minimum_required(VERSION 4.0)
project(project14_plog CXX) # Завжди вказуйте мову проєкту

set(CMAKE_CXX_STANDARD 20)

# 1. Знайдіть пакет plog. Vcpkg, завдяки toolchain file, знайде його
find_package(plog CONFIG REQUIRED)

add_executable(project14_plog main.cpp)

# 2. Прив'яжіть бібліотеку plog до вашої програми
# project14_plog – це ім'я вашого виконуваного файлу
# plog::plog – це ім'я цілі, яку експортує бібліотека plog
target_link_libraries(project14_plog PRIVATE plog::plog)