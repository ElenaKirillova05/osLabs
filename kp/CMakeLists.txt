cmake_minimum_required(VERSION 3.10)
project(ZMQGame CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

include_directories(${CMAKE_SOURCE_DIR} ${ZMQ_INCLUDE_DIRS})

add_executable(server_exec server.cpp)
target_link_libraries(server_exec PUBLIC ${ZMQ_LIBRARIES} Threads::Threads)

add_executable(main main.cpp)
target_link_libraries(main PUBLIC ${ZMQ_LIBRARIES} Threads::Threads)

add_executable(tests test.cpp main.cpp server.cpp)
target_compile_definitions(tests PRIVATE UNIT_TEST)
target_link_libraries(tests PRIVATE ${ZMQ_LIBRARIES} Threads::Threads gtest gtest_main)

include(CTest)
add_test(NAME tests COMMAND tests)
