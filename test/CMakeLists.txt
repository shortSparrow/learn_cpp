cmake_minimum_required(VERSION 4.0)
project(test)

set(CMAKE_CXX_STANDARD 20)

add_executable(test main.cpp
        feature/add/add.cpp
        feature/add/add.h
        feature/add_2/add.cpp
        feature/add_2/add.h)
