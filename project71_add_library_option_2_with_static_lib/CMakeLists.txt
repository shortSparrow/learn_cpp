cmake_minimum_required(VERSION 4.2)
project(project71_add_library_option_2_with_static_lib)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG        10.2.1
)

FetchContent_MakeAvailable(fmt)

add_executable(${PROJECT_NAME} main.cpp)

# Зв'язуємо так само, як і раніше
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)
