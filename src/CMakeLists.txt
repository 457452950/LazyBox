cmake_minimum_required(VERSION 3.22)
project(${LIBRARY_NAME})

set(CMAKE_CXX_STANDARD 20)

add_library(${LIBRARY_NAME}
        thread/Lock.cpp
        thread/ThreadPool.cpp
        thread/Channel.cpp
        thread/Actor.cpp
        test/TestEngine.cpp
        test/TestCase.cpp
)
