cmake_minimum_required(VERSION 3.22)
project(LazyBox)

set(LIBRARY_NAME lbox)

# set output path
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

include_directories("include")

# found c++20 std::format
include(CheckIncludeFileCXX)
CHECK_INCLUDE_FILE_CXX(format HAS_format)

if (HAS_format)
    message(STATUS "Found C++20's format library")
else (HAS_format)
    message(STATUS "C++20's format library not found, using fmt::format")
    add_subdirectory(third_party/fmt)
    include_directories(third_party/fmt/include)
endif ()


add_subdirectory(src)
add_subdirectory(example)