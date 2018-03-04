# global settngs
cmake_minimum_required (VERSION 3.5.1)

project (otlsmodels)
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_INCLUDE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../include)
set (OTLSMODELS_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../lib)

# searches for more cmake files
add_subdirectory (base)
add_subdirectory (sagtension)
add_subdirectory (thermalrating)
add_subdirectory (transmissionline)
