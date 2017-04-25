// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/transmissionline/hardware.h"

Hardware::Hardware() {
  area_cross_section = -999999;
  length = -999999;
  name = "";
  weight = -999999;
}

Hardware::~Hardware() {
}

bool Hardware::Validate(
    const bool& is_included_warnings,
    std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "HARDWARE";

  // validates area-cross-section
  if (area_cross_section < 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid cross sectional area";
      messages->push_back(message);
    }
  }

  // validates length
  if (length < 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid length";
      messages->push_back(message);
    }
  }

  // validates type
  if ((type != Hardware::HardwareType::kDeadEnd)
      && (type != Hardware::HardwareType::kSuspension)) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid type";
      messages->push_back(message);
    }
  }

  // validates weight
  if (weight < 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid weight";
      messages->push_back(message);
    }
  }

  return is_valid;
}
