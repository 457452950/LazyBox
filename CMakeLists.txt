cmake_minimum_required(VERSION 3.22)
project(LazyBox)

option(USE_LAZYBOX_DEMO "Build lazy box example" ON)
option(BUILD_LAZYBOX_SHARED "Build lazy box shared lib" ON)
option(BUILD_LAZYBOX_STATIC "Build lazy box static lib" ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(LBOX_LIBRARY_NAME lbox)

# set output path
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_subdirectory(third_party/fmt_10_2_1)

add_subdirectory(src)

if (USE_LAZYBOX_DEMO)
    add_subdirectory(example)
endif (USE_LAZYBOX_DEMO)
