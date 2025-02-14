cmake_minimum_required(VERSION 3.10)
project(OS4)

# Установка стандарта C
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Пути к исходным файлам
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(BUILD_DIR "${CMAKE_BINARY_DIR}")

# Установка заголовочных файлов
include_directories(${INCLUDE_DIR})

# Fetch GoogleTest
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

# Сборка библиотек
add_library(gcd_euclid_and_binary SHARED ${SRC_DIR}/gcd_euclid_and_binary.c)
add_library(gcd_naive_and_ternary SHARED ${SRC_DIR}/gcd_naive_and_ternary.c)

# Установка директорий для библиотек
set_target_properties(gcd_euclid_and_binary PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${BUILD_DIR})
set_target_properties(gcd_naive_and_ternary PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${BUILD_DIR})

# Компиляция программ
add_executable(program1 ${SRC_DIR}/program1.c)
add_executable(program2 ${SRC_DIR}/program2.c)

# Линковка библиотек для program1
target_link_libraries(program1 PRIVATE gcd_euclid_and_binary)

# Линковка системной библиотеки для program2
target_link_libraries(program2 PRIVATE dl)

# Установка директорий для исполняемых файлов
set_target_properties(program1 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR})
set_target_properties(program2 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR})

# Добавление тестов
enable_testing()
add_executable(gcd_tests test.cpp)
target_link_libraries(gcd_tests PRIVATE gcd_euclid_and_binary gcd_naive_and_ternary gtest gtest_main)

# Регистрация тестов
add_test(NAME gcd_tests COMMAND gcd_tests)

