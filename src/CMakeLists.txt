cmake_minimum_required(VERSION 3.22)
project(${LIBRARY_NAME})

add_library(${LIBRARY_NAME}
        Lock.cpp
        ThreadPool.cpp
        MessageQueue.cpp
)
