cmake_minimum_required(VERSION 4.3)
project(27_thread_pool)

set(CMAKE_CXX_STANDARD 20)

add_executable(27_thread_pool main.cpp
        step1_simple_pool.cpp
        step2_future_pool.cpp
        step3_local_queues.cpp
        step4_work_stealing.cpp
        function_wrapper.cpp)
