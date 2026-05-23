cmake_minimum_required(VERSION 4.2)
project(project71_add_library_option_2_with_dynamic_lib)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG        10.2.1
)

# КРИТИЧНИЙ РЯДОК: Кажемо CMake збирати ВСІ наступні залежності як ДИНАМІЧНІ (.dll)
set(BUILD_SHARED_LIBS ON)

FetchContent_MakeAvailable(fmt)

add_executable(${PROJECT_NAME} main.cpp)

# Зв'язуємо так само, як і раніше
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)

# Додаємо в самий кінець файлу
if(WIN32)
    # Після збірки копіюємо всі DLL-ки у директорію з нашим виконуваним файлом
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
            COMMAND_EXPAND_LISTS
    )
endif()