cmake_minimum_required(VERSION 3.22)
project(${LBOX_LIBRARY_NAME})

set(CMAKE_CXX_STANDARD 17)

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

add_library(${LBOX_LIBRARY_NAME}
        ${LIBRARY_THREAD_SOURCE}
        ${LIBRARY_TEST_SOURCE}
        ${LIBRARY_LOGGER_SOURCE}

        Chrono.cpp
)
if (MSVC)
    target_compile_options(${LBOX_LIBRARY_NAME} PUBLIC "/Zc:preprocessor,__cplusplus" "-utf-8")
endif ()
target_include_directories(${LBOX_LIBRARY_NAME} PUBLIC "../include")
target_link_libraries(${LBOX_LIBRARY_NAME} PUBLIC fmt)