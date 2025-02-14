cmake_minimum_required(VERSION 3.10)

project(lab2 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(lab2_lib
    src/KMeans.cpp
    src/Timer.cpp
    src/Utils.cpp
)

target_include_directories(lab2_lib PUBLIC include)

find_package(Threads REQUIRED)
target_link_libraries(lab2_lib PRIVATE Threads::Threads)

add_executable(lab2 main.cpp)
target_link_libraries(lab2 PRIVATE lab2_lib)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(tests test.cpp)
target_link_libraries(tests PRIVATE lab2_lib gtest gtest_main)

enable_testing()
add_test(NAME lab2_tests COMMAND tests)