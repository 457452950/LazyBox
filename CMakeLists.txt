cmake_minimum_required(VERSION 3.22)
project(LazyBox)

option(USE_LAZYBOX_DEMO "Build lazy box example" ON)

set(LBOX_LIBRARY_NAME lbox)

if (MSVC)
    message("CMAKE_C_FLAGS          " "${CMAKE_C_FLAGS}")
    message("CMAKE_C_FLAGS_DEBUG    " "${CMAKE_C_FLAGS}")
    message("CMAKE_C_FLAGS_RELEASE  " "${CMAKE_C_FLAGS}")
    message("CMAKE_CXX_FLAGS        " "${CMAKE_C_FLAGS}")
    message("CMAKE_CXX_FLAGS_DEBUG  " "${CMAKE_C_FLAGS}")
    message("CMAKE_CXX_FLAGS_RELEASE " "${CMAKE_C_FLAGS}")


    # set(CMAKE_CXX_FLAGS_RELEASE "/MT")
    # set(CMAKE_CXX_FLAGS_DEBUG   "/MTd")
endif ()

# set output path
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_subdirectory(third_party/fmt_10_2_1)

add_subdirectory(src)

if (USE_LAZYBOX_DEMO)
    add_subdirectory(example)
endif (USE_LAZYBOX_DEMO)
