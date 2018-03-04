# defines include directory
include_directories (${CMAKE_INCLUDE_PATH})

# defines source files
set (SAGTENSION_SRC_FILES
  ${OTLSMODELS_SOURCE_DIR}/src/sagtension/cable_component_elongation_model.cc
  ${OTLSMODELS_SOURCE_DIR}/src/sagtension/cable_elongation_model.cc
  ${OTLSMODELS_SOURCE_DIR}/src/sagtension/cable_state.cc
  ${OTLSMODELS_SOURCE_DIR}/src/sagtension/cable_strainer.cc
  ${OTLSMODELS_SOURCE_DIR}/src/sagtension/catenary_cable_reloader.cc
  ${OTLSMODELS_SOURCE_DIR}/src/sagtension/catenary_cable_unloader.cc
  ${OTLSMODELS_SOURCE_DIR}/src/sagtension/line_cable_loader_base.cc
  ${OTLSMODELS_SOURCE_DIR}/src/sagtension/line_cable_reloader.cc
  ${OTLSMODELS_SOURCE_DIR}/src/sagtension/line_cable_sagger.cc
  ${OTLSMODELS_SOURCE_DIR}/src/sagtension/line_cable_unloader.cc
  ${OTLSMODELS_SOURCE_DIR}/src/sagtension/sag_tension_cable.cc
)

# defines library
add_library (otlsmodels_sagtension STATIC ${SAGTENSION_SRC_FILES})

# appends "d" for debug builds
if (CMAKE_BUILD_TYPE MATCHES Debug)
  set_target_properties (otlsmodels_sagtension PROPERTIES DEBUG_POSTFIX "d")
endif()

# adds to linker include path
target_include_directories (otlsmodels_sagtension PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
