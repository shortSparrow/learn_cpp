cmake_minimum_required(VERSION 4.0)
project(project21_external_linkage)

set(CMAKE_CXX_STANDARD 20)

add_executable(project21_external_linkage main.cpp
        foo.cpp)
