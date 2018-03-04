# defines include directory
include_directories (${CMAKE_INCLUDE_PATH})

# defines source files
set (THERMALRATING_SRC_FILES
  ${OTLSMODELS_SOURCE_DIR}/src/thermalrating/cable_heat_transfer_solver.cc
  ${OTLSMODELS_SOURCE_DIR}/src/thermalrating/cable_heat_transfer_state.cc
  ${OTLSMODELS_SOURCE_DIR}/src/thermalrating/solar_radiation_solver.cc
  ${OTLSMODELS_SOURCE_DIR}/src/thermalrating/steady_cable_current_solver.cc
  ${OTLSMODELS_SOURCE_DIR}/src/thermalrating/steady_cable_temperature_solver.cc
  ${OTLSMODELS_SOURCE_DIR}/src/thermalrating/thermal_rating_cable.cc
  ${OTLSMODELS_SOURCE_DIR}/src/thermalrating/thermal_rating_weather.cc
  ${OTLSMODELS_SOURCE_DIR}/src/thermalrating/transient_cable_temperature_solver.cc
)

# defines library
add_library (otlsmodels_thermalrating STATIC ${THERMALRATING_SRC_FILES})

# appends "d" for debug builds
if (CMAKE_BUILD_TYPE MATCHES Debug)
  set_target_properties (otlsmodels_thermalrating PROPERTIES DEBUG_POSTFIX "d")
endif()

# adds to linker include path
target_include_directories (otlsmodels_thermalrating PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
