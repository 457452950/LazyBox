cmake_minimum_required(VERSION 3.22)
project(example CXX)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} ${LIBRARY_NAME})