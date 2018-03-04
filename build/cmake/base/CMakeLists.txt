# defines include directory
include_directories (${CMAKE_INCLUDE_PATH})

# defines source files
set (BASE_SRC_FILES
  ${OTLSMODELS_SOURCE_DIR}/src/base/geometric_shapes.cc
  ${OTLSMODELS_SOURCE_DIR}/src/base/helper.cc
  ${OTLSMODELS_SOURCE_DIR}/src/base/polynomial.cc
  ${OTLSMODELS_SOURCE_DIR}/src/base/units.cc
  ${OTLSMODELS_SOURCE_DIR}/src/base/vector.cc
)

# defines library
add_library (otlsmodels_base STATIC ${BASE_SRC_FILES})

# appends "d" for debug builds
if (CMAKE_BUILD_TYPE MATCHES Debug)
  set_target_properties (otlsmodels_base PROPERTIES DEBUG_POSTFIX "d")
endif()

# adds to linker include path
target_include_directories (otlsmodels_base PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
