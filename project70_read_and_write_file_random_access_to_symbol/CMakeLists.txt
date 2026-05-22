cmake_minimum_required(VERSION 4.2)
project(project70_read_and_write_file_random_access_to_symbol)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} main.cpp)

# Отримуємо шлях до теки з .exe файлом (куди треба копіювати)
set(TARGET_DIR "$<TARGET_FILE_DIR:${PROJECT_NAME}>")

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/res"
        "${TARGET_DIR}/res"
)