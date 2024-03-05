cmake_minimum_required(VERSION 3.22)
project(LazyBox)

# set output path
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

include_directories("include")

set(LIBRARY_NAME lbox)

add_subdirectory(src)
add_subdirectory(example)