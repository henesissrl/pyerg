set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

aux_source_directory(. SOURCES)
add_executable(test ${SOURCES})
target_link_libraries(test LINK_PUBLIC erg ${GTEST_BOTH_LIBRARIES} pthread)
add_test(AllTests test)

set_target_properties(test PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
