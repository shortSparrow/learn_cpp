cmake_minimum_required(VERSION 4.0)
project(project24_random)

set(CMAKE_CXX_STANDARD 20)

add_executable(project24_random main.cpp
        random_by_time/random_by_time.cpp
        random_by_random_device/random_by_random_device.cpp
        random_by_time/random_by_time.h
        random_by_random_device/random_by_random_device.h
        base/base.cpp
        base/base.h
        global_random/global_random.h)
