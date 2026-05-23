cmake_minimum_required(VERSION 4.2)
project(project71_add_library_option_2)

set(CMAKE_CXX_STANDARD 20)

# Включаємо модуль для завантаження контенту
include(FetchContent)

# Оголошуємо, звідки качати бібліотеку
FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG        v3.11.3 # або інша стабільна версія
)

# Завантажуємо та робимо доступною для проєкту
FetchContent_MakeAvailable(json)

add_executable(${PROJECT_NAME} main.cpp)

# Підключаємо (nlohmann_json — це внутрішнє ім'я, яке створює сама бібліотека)
# Це ім'я зазначено на github сторінці бібліотеки в розділі CMake - nlohmann_json::nlohmann_json
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)