cmake_minimum_required(VERSION 3.22)
project(example)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall")

if (UNIX)
    message("open gcc warning")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -Wpedantic")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,leak,undefined")
endif ()

if (MSVC)
    add_compile_options("/Zc:preprocessor,__cplusplus" "-utf-8" "/W3")
endif ()

add_executable(${PROJECT_NAME} 
        main.cpp
        
        test_defer.hpp
        test_instance.hpp
        test_thread_safe_queue.hpp
        test_thread_thread_pool.hpp
        test_thread_actor.hpp
        test_thread_channel.hpp
        test_random.hpp
        )
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
target_include_directories(${PROJECT_NAME} PUBLIC "../include")
if (MSVC)
    target_link_directories(${PROJECT_NAME} 
    PRIVATE "../lib/Debug" 
    PRIVATE "../lib/Release")
endif()
target_link_libraries(${PROJECT_NAME} lbox_static)