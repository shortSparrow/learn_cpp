cmake_minimum_required(VERSION 4.2)
project(20_std_memory_order_in_details)

set(CMAKE_CXX_STANDARD 20)

add_executable(20_std_memory_order_in_details main.cpp
        memory_order_relaxed/memory_order_relaxed.cpp
        memory_order_seq_cst/memory_order_seq_cst.cpp
        include/memory_order_seq_cst.h
        include/memory_acquire_release/memory_acquire_release_1.h
        memory_acquire_release/memory_acquire_release_1.cpp
        include/memory_acquire_release/memory_acquire_release_2.h
        memory_acquire_release/memory_acquire_release_2.cpp
        include/memory_acquire_release/memory_acquire_release_3.h
        memory_acquire_release/memory_acquire_release_3.cpp
)
