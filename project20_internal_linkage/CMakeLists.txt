cmake_minimum_required(VERSION 4.0)
project(project20_internal_linkage)

set(CMAKE_CXX_STANDARD 20)

add_executable(project20_internal_linkage main.cpp
        foo.cpp)
