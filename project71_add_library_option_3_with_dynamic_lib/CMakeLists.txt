cmake_minimum_required(VERSION 4.2)
project(project71_add_library_option_3_with_dynamic_lib)

set(CMAKE_CXX_STANDARD 20)

# Просто шукаємо бібліотеку (vcpkg інтегрується в цю команду)
find_package(fmt CONFIG REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)

# Лінкуємо
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)