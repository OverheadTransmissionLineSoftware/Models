// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/sagtension/cable_state.h"

CableState::CableState() {
  load_stretch = -999999;
  temperature = -999999;
  temperature_stretch = -999999;
}

CableState::~CableState() {
}

bool CableState::Validate(const bool& is_included_warnings,
                              std::list<std::string>* messages_error) const {
  bool is_valid = true;

  // validates load-stretch
  if (load_stretch < 0) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back(
          "CABLE STATE - Invalid stretch load");
    }
  }

  // validates temperature
  if (temperature < -50) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back(
          "CABLE STATE - Invalid temperature");
    }
  }

  // validates temperature - stretch
  if (temperature_stretch < -50) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back(
          "CABLE STATE - Invalid stretch temperature");
    }
  }

  return is_valid;
}
