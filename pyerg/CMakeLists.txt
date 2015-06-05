set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_BUILD_TYPE Release)

find_package(PythonInterp)
find_package(PythonLibs)
find_package(NumPy REQUIRED)

include_directories(${PYTHON_INCLUDE_DIRS})
include_directories(${NUMPY_INCLUDE_DIRS})
include_directories(../erg)

aux_source_directory(. SOURCES)
aux_source_directory(../erg SOURCES_ERG)
add_library(pyerg SHARED ${SOURCES} ${SOURCES_ERG} pyerg_docstrings.h)
set_target_properties(pyerg PROPERTIES PREFIX "")
target_link_libraries(pyerg ${PYTHON_LIBRARIES})
target_include_directories(pyerg PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(pyerg PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
