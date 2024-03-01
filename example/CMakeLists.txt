cmake_minimum_required(VERSION 3.22)
project(example)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

add_executable(${PROJECT_NAME} main.cpp
        test_defer.hpp
        test_instance.hpp
        test_thread_safe_queue.hpp
        test_thread_thread_pool.hpp
        test_thread_actor.hpp
        test_thread_channel.hpp)
target_link_libraries(${PROJECT_NAME} ${LIBRARY_NAME})