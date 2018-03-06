	# defines include directory
include_directories (${CMAKE_INCLUDE_PATH})

# defines source files
set (SAGGING_SRC_FILES
  ${OTLSMODELS_SOURCE_DIR}/src/sagging/cable_sag_position_corrector.cc
  ${OTLSMODELS_SOURCE_DIR}/src/sagging/dyno_sagger.cc
  ${OTLSMODELS_SOURCE_DIR}/src/sagging/stopwatch_sagger.cc
  ${OTLSMODELS_SOURCE_DIR}/src/sagging/transit_sagger.cc
)

# defines library
add_library (otlsmodels_sagging STATIC ${SAGGING_SRC_FILES})

# appends "d" for debug builds
if (CMAKE_BUILD_TYPE MATCHES Debug)
  set_target_properties (otlsmodels_sagging PROPERTIES DEBUG_POSTFIX "d")
endif()

# adds to linker include path
target_include_directories (otlsmodels_sagging PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
