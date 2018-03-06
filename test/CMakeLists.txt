# defines include directories
include_directories (..)
include_directories (${CMAKE_INCLUDE_PATH})
include_directories (../external/googletest/googletest/include)

# defines source files
set (TEST_SRC_FILES
  ${OTLSMODELS_SOURCE_DIR}/test/factory.cc
  ${OTLSMODELS_SOURCE_DIR}/test/test.cc
  ${OTLSMODELS_SOURCE_DIR}/test/base/geometric_shapes_test.cc
  ${OTLSMODELS_SOURCE_DIR}/test/base/helper_test.cc
  ${OTLSMODELS_SOURCE_DIR}/test/base/polynomial_test.cc
  ${OTLSMODELS_SOURCE_DIR}/test/base/units_test.cc
  ${OTLSMODELS_SOURCE_DIR}/test/base/vector_test.cc
  ${OTLSMODELS_SOURCE_DIR}/test/sagging/cable_sag_position_corrector_test.cc
  ${OTLSMODELS_SOURCE_DIR}/test/sagging/dyno_sagger_test.cc
  ${OTLSMODELS_SOURCE_DIR}/test/sagging/stopwatch_sagger_test.cc
  ${OTLSMODELS_SOURCE_DIR}/test/sagging/transit_sagger_test.cc
  ${OTLSMODELS_SOURCE_DIR}/test/sagtension/cable_component_elongation_model_test.cc
  ${OTLSMODELS_SOURCE_DIR}/test/sagtension/cable_elongation_model_test.cc
  ${OTLSMODELS_SOURCE_DIR}/test/sagtension/cable_state_test.cc
  ${OTLSMODELS_SOURCE_DIR}/test/sagtension/cable_strainer_test.cc
  ${OTLSMODELS_SOURCE_DIR}/test/sagtension/catenary_cable_reloader_test.cc
  ${OTLSMODELS_SOURCE_DIR}/test/sagtension/catenary_cable_unloader_test.cc
  ${OTLSMODELS_SOURCE_DIR}/test/sagtension/line_cable_loader_base_test.cc
  ${OTLSMODELS_SOURCE_DIR}/test/sagtension/line_cable_reloader_test.cc
  ${OTLSMODELS_SOURCE_DIR}/test/sagtension/line_cable_sagger_test.cc
  ${OTLSMODELS_SOURCE_DIR}/test/sagtension/line_cable_unloader_test.cc
  ${OTLSMODELS_SOURCE_DIR}/test/sagtension/sag_tension_cable_test.cc
  ${OTLSMODELS_SOURCE_DIR}/test/thermalrating/cable_heat_transfer_solver_test.cc
  ${OTLSMODELS_SOURCE_DIR}/test/thermalrating/cable_heat_transfer_state_test.cc
  ${OTLSMODELS_SOURCE_DIR}/test/thermalrating/solar_radiation_solver_test.cc
  ${OTLSMODELS_SOURCE_DIR}/test/thermalrating/steady_cable_current_solver_test.cc
  ${OTLSMODELS_SOURCE_DIR}/test/thermalrating/steady_cable_temperature_solver_test.cc
  ${OTLSMODELS_SOURCE_DIR}/test/thermalrating/thermal_rating_cable_test.cc
  ${OTLSMODELS_SOURCE_DIR}/test/thermalrating/thermal_rating_weather_test.cc
  ${OTLSMODELS_SOURCE_DIR}/test/thermalrating/transient_cable_temperature_solver_test.cc
  ${OTLSMODELS_SOURCE_DIR}/test/transmissionline/alignment_test.cc
  ${OTLSMODELS_SOURCE_DIR}/test/transmissionline/cable_attachment_equilibrium_solver_test.cc
  ${OTLSMODELS_SOURCE_DIR}/test/transmissionline/cable_constraint_test.cc
  ${OTLSMODELS_SOURCE_DIR}/test/transmissionline/cable_position_locator_test.cc
  ${OTLSMODELS_SOURCE_DIR}/test/transmissionline/cable_test.cc
  ${OTLSMODELS_SOURCE_DIR}/test/transmissionline/cable_unit_load_solver_test.cc
  ${OTLSMODELS_SOURCE_DIR}/test/transmissionline/catenary_solver_test.cc
  ${OTLSMODELS_SOURCE_DIR}/test/transmissionline/catenary_test.cc
  ${OTLSMODELS_SOURCE_DIR}/test/transmissionline/hardware_test.cc
  ${OTLSMODELS_SOURCE_DIR}/test/transmissionline/line_cable_test.cc
  ${OTLSMODELS_SOURCE_DIR}/test/transmissionline/line_structure_test.cc
  ${OTLSMODELS_SOURCE_DIR}/test/transmissionline/structure_test.cc
  ${OTLSMODELS_SOURCE_DIR}/test/transmissionline/transmission_line_test.cc
  ${OTLSMODELS_SOURCE_DIR}/test/transmissionline/weather_load_case_test.cc
)

# adds external library
add_library (gtest STATIC IMPORTED)
set_target_properties (gtest PROPERTIES IMPORTED_LOCATION ${OTLSMODELS_SOURCE_DIR}/external/googletest/googletest/make/gtest_main.a)

# defines executable
add_executable (otlsmodels_test ${TEST_SRC_FILES})

# links to libraries
target_link_libraries (otlsmodels_test LINK_PUBLIC
  otlsmodels_sagtension
  otlsmodels_sagging
  otlsmodels_thermalrating
  otlsmodels_transmissionline
  otlsmodels_base
  gtest
  pthread)
