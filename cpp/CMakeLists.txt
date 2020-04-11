cmake_minimum_required(VERSION 3.14)
project(wargaming_test_tasks)

set(CMAKE_CXX_STANDARD 17)

# Google Test
add_subdirectory(test)
enable_testing()

set(SOURCES main.cpp src/first/is_even.h src/second/cyclic_queue.h src/second/cyclic_queue_list.h src/second/cyclic_queue_dynamic_array.h)

add_executable(wargaming_test_tasks ${SOURCES})