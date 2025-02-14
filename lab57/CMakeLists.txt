cmake_minimum_required(VERSION 3.10)
project(lab57 C CXX)

include(FetchContent)
FetchContent_Declare(
  gtest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG main 
)
FetchContent_MakeAvailable(gtest)

find_package(PkgConfig REQUIRED)
pkg_check_modules(ZMQ REQUIRED libzmq)

find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${CMAKE_SOURCE_DIR} ${ZMQ_INCLUDE_DIRS})

add_library(lab57_lib
    client.cpp
    server.cpp
    send_recv.hpp
)

target_link_libraries(lab57_lib
    PUBLIC
    ${ZMQ_LIBRARIES}
    Threads::Threads
)

add_executable(test
    tests.cpp
)

target_link_libraries(test PRIVATE lab57_lib gtest gtest_main)

add_test(NAME test COMMAND test)
