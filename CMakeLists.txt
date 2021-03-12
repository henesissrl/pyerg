project(cErgParser)
cmake_minimum_required(VERSION 3.12)

option(BUILD_PYTHON_MODULE "Build the Python module" OFF)
option(BUILD_TESTS "Build the tests" OFF)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})

add_subdirectory(erg)

if(BUILD_PYTHON_MODULE)
    message(STATUS "Building Python module")
    add_subdirectory(pyerg)
endif(BUILD_PYTHON_MODULE)

if(BUILD_TESTS)
    message(STATUS "Building test program")
    add_subdirectory(test)
endif(BUILD_TESTS)

