// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "include/CableLoadCase.h"

CableLoadCase::CableLoadCase() {
  temperature_cable = -999999;
}

CableLoadCase::~CableLoadCase() {}

bool CableLoadCase::Validate (bool is_included_warnings,
                              std::list<std::string>* messages_error) const {
  bool is_valid = true;

  // validate load-unit transverse
  if (load_unit.x() < 0
      || ((25 < load_unit.x()) && (is_included_warnings == true))) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back ("CABLE LOAD CASE - Invalid transverse load ");
    }
  }

  // validate load-unit vertical
  if (load_unit.y() <= 0
      || ((25 < load_unit.y()) && (is_included_warnings == true))) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back ("CABLE LOAD CASE - Invalid vertical load ");
    }
  }

  // validate temperature-cable
  if (temperature_cable < -50
      || ((500 < temperature_cable) && (is_included_warnings == true))) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back ("CABLE LOAD CASE - Invalid cable temperature");
    }
  }

  return is_valid;
}
