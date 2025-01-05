cmake_minimum_required(VERSION 3.22)
project(${LBOX_LIBRARY_NAME})

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD 20)

set(LIBRARY_THREAD_SOURCE

        thread/Lock.cpp
        thread/ThreadPool.cpp
        thread/Channel.cpp
        thread/Actor.cpp)

set(LIBRARY_TEST_SOURCE

        test/TestEngine.cpp
        test/TestCase.cpp
        test/TestAction.cpp
)

set(LIBRARY_LOGGER_SOURCE

        logger/Logger.cpp
        logger/LogFormatter.cpp
        logger/LogWriter.cpp
)

set(LIBRARY_LAZYBOS_SOURCE
    ${LIBRARY_THREAD_SOURCE}
    ${LIBRARY_TEST_SOURCE}
    ${LIBRARY_LOGGER_SOURCE}

    Chrono.cpp
)

if (BUILD_LAZYBOX_SHARED)
add_library(${LBOX_LIBRARY_NAME}
        SHARED
        ${LIBRARY_LAZYBOS_SOURCE}
)
if (MSVC)
    target_compile_options(${LBOX_LIBRARY_NAME} PUBLIC "/Zc:preprocessor,__cplusplus" "-utf-8")
endif ()
target_include_directories(${LBOX_LIBRARY_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../include")
target_link_libraries(${LBOX_LIBRARY_NAME} PUBLIC fmt)
endif()

if (BUILD_LAZYBOX_STATIC)
set(LBOX_STATIC_LIBRARY_NAME ${LBOX_LIBRARY_NAME}_static)
add_library(${LBOX_STATIC_LIBRARY_NAME}
        STATIC
        ${LIBRARY_LAZYBOS_SOURCE}
)
if (MSVC)
    target_compile_options(${LBOX_STATIC_LIBRARY_NAME} PUBLIC "/Zc:preprocessor,__cplusplus" "-utf-8")
endif ()
target_include_directories(${LBOX_STATIC_LIBRARY_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../include")
target_link_libraries(${LBOX_STATIC_LIBRARY_NAME} PUBLIC fmt)

endif()
