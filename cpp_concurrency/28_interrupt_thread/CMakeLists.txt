cmake_minimum_required(VERSION 4.3)
project(28_interrupt_thread)

set(CMAKE_CXX_STANDARD 20)

add_executable(28_interrupt_thread main.cpp
        step_1_interrupt_concept.h
        step_2_interrupt_cv_naive.h
        step_3_interrupt_cv_any.h)
