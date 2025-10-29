cmake_minimum_required(VERSION 4.0)
project(project19_namespace)

set(CMAKE_CXX_STANDARD 20)

add_executable(project19_namespace main.cpp
        foo.cpp
        goo.cpp
        foo.h
)
