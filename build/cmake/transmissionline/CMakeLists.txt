# defines include directory
include_directories (${CMAKE_INCLUDE_PATH})

# defines source files
set (TRANSMISSIONLINE_SRC_FILES
  ${OTLSMODELS_SOURCE_DIR}/src/transmissionline/alignment.cc
  ${OTLSMODELS_SOURCE_DIR}/src/transmissionline/cable.cc
  ${OTLSMODELS_SOURCE_DIR}/src/transmissionline/cable_attachment_equilibrium_solver.cc
  ${OTLSMODELS_SOURCE_DIR}/src/transmissionline/cable_constraint.cc
  ${OTLSMODELS_SOURCE_DIR}/src/transmissionline/cable_position_locator.cc
  ${OTLSMODELS_SOURCE_DIR}/src/transmissionline/cable_unit_load_solver.cc
  ${OTLSMODELS_SOURCE_DIR}/src/transmissionline/catenary.cc
  ${OTLSMODELS_SOURCE_DIR}/src/transmissionline/catenary_solver.cc
  ${OTLSMODELS_SOURCE_DIR}/src/transmissionline/hardware.cc
  ${OTLSMODELS_SOURCE_DIR}/src/transmissionline/line_cable.cc
  ${OTLSMODELS_SOURCE_DIR}/src/transmissionline/line_structure.cc
  ${OTLSMODELS_SOURCE_DIR}/src/transmissionline/structure.cc
  ${OTLSMODELS_SOURCE_DIR}/src/transmissionline/transmission_line.cc
  ${OTLSMODELS_SOURCE_DIR}/src/transmissionline/weather_load_case.cc
)

# defines library
add_library (otlsmodels_transmissionline STATIC ${TRANSMISSIONLINE_SRC_FILES})

# appends "d" for debug builds
if (CMAKE_BUILD_TYPE MATCHES Debug)
  set_target_properties (otlsmodels_transmissionline PROPERTIES DEBUG_POSTFIX "d")
endif()

# adds to linker include path
target_include_directories (otlsmodels_transmissionline PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
