cmake_minimum_required(VERSION 3.22)
project(example)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall")

if (UNIX)
    message("open gcc warning")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wextra -Wpedantic")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,leak,undefined")
endif ()

if (MSVC)
    add_compile_options("/Zc:preprocessor,__cplusplus" "-utf-8")
endif ()

add_executable(${PROJECT_NAME} main.cpp
        test_defer.hpp
        test_instance.hpp
        test_thread_safe_queue.hpp
        test_thread_thread_pool.hpp
        test_thread_actor.hpp
        test_thread_channel.hpp)
target_include_directories(${PROJECT_NAME} PUBLIC "../include")
target_link_libraries(${PROJECT_NAME} ${LIBRARY_NAME})