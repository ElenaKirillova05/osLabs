cmake_minimum_required(VERSION 3.10)
project(SharedMemoryIPC)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Пути к исходным файлам
set(SRC_DIR "${CMAKE_SOURCE_DIR}")
set(BUILD_DIR "${CMAKE_BINARY_DIR}")

# Установка заголовочных файлов
include_directories(${SRC_DIR})

# Fetch GoogleTest
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

# Компиляция исполняемых файлов
add_executable(parent ${SRC_DIR}/parent.c)
add_executable(child1 ${SRC_DIR}/child1.c)
add_executable(child2 ${SRC_DIR}/child2.c)

# Линковка системных библиотек
target_link_libraries(parent PRIVATE rt)
target_link_libraries(child1 PRIVATE rt)
target_link_libraries(child2 PRIVATE rt)

# Установка директорий для исполняемых файлов
set_target_properties(parent PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR})
set_target_properties(child1 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR})
set_target_properties(child2 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR})

# Добавление тестов
enable_testing()
add_executable(shared_memory_tests ${SRC_DIR}/tests.cpp)
target_link_libraries(shared_memory_tests PRIVATE gtest gtest_main pthread rt)

# Регистрация тестов
add_test(NAME shared_memory_tests COMMAND shared_memory_tests)