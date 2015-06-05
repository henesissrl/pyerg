set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_BUILD_TYPE Release)

aux_source_directory(. SOURCES)
add_library(erg SHARED ${SOURCES})
add_library(erg_s STATIC ${SOURCES})
target_include_directories(erg PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(erg_s PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(erg erg_s 
                      PROPERTIES
                      LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
                      ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
