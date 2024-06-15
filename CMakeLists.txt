cmake_minimum_required(VERSION 3.22)
project(LazyBox)

set(LBOX_LIBRARY_NAME lbox)

# set output path
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_subdirectory(third_party/fmt_10_2_1)

add_subdirectory(src)

if (USE_LAZYBOX_DEMO)
add_subdirectory(example)
endif(USE_LAZYBOX_DEMO)
