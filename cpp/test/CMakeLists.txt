cmake_minimum_required(VERSION 3.14)
project(wargaming_test_tasks_tests)

set(CMAKE_CXX_STANDARD 17)


set(TEST_SOURCES
        ../src/first/is_even.h
        ../src/second/cyclic_queue.h
        ../src/second/cyclic_queue_list.h
        test-cyclic-queue.cpp
        test-cyclic-queue-list.cpp simple_class.h test_even.cpp)

add_subdirectory(googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})


add_executable(wargaming_test_tasks_tests ${TEST_SOURCES})

target_link_libraries(${PROJECT_NAME} gtest gtest_main)

