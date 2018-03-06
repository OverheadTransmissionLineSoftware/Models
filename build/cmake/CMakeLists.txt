# global settngs
cmake_minimum_required (VERSION 3.5.1)

project (otlsmodels)
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_INCLUDE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../include)
set (OTLSMODELS_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../lib)

# searches for individual library cmake files
add_subdirectory (base)
add_subdirectory (sagging)
add_subdirectory (sagtension)
add_subdirectory (thermalrating)
add_subdirectory (transmissionline)

# determines if tests are included in build
if (MODELS_BUILD_TESTS)
  add_subdirectory (${CMAKE_CURRENT_SOURCE_DIR}/../../test ${CMAKE_CURRENT_BINARY_DIR}/test)
endif ()
