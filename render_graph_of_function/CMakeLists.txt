cmake_minimum_required(VERSION 4.2)
project(render_graph_of_function)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(render_graph_of_function src/main.cpp)

# Спеціальна команда для модулів у новому CMake
target_sources(render_graph_of_function
        PUBLIC
        FILE_SET CXX_MODULES FILES
        src/Config.cppm
        src/Canvas.cppm
        src/MathEngine.cppm
        src/CoordinateSystem.cppm
        src/Plotter.cppm
)