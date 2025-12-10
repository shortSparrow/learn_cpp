cmake_minimum_required(VERSION 4.1)
project(temp_task10_friend_class_method_in_multiple_files)

set(CMAKE_CXX_STANDARD 20)

add_executable(temp_task10_friend_class_method_in_multiple_files main.cpp
        Vector3d.cpp
        Vector3d.h
        Point3d.cpp
        Point3d.h)
