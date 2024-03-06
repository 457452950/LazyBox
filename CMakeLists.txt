cmake_minimum_required(VERSION 3.22)
project(LazyBox)

set(LIBRARY_NAME lbox)

# set output path
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

include_directories("include")

if (MSVC)
    add_compile_options("/Zc:preprocessor")
endif ()

add_subdirectory(third_party/fmt)
include_directories(third_party/fmt/include)


add_subdirectory(src)
add_subdirectory(example)