// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/transmissionline/cable_constraint.h"

CableConstraint::CableConstraint() {
  case_weather = nullptr;
  condition = CableConditionType::kNull;
  limit = -999999;
  type_limit = CableConstraint::LimitType::kNull;
}

CableConstraint::~CableConstraint() {}

bool CableConstraint::Validate(
    const bool& is_included_warnings,
    std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "CABLE CONSTRAINT";

  // validates case-weather
  if (case_weather == nullptr) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid weather case";
      messages->push_back(message);
    }
  } else {
    if (case_weather->Validate(is_included_warnings, messages) == false) {
      is_valid = false;
    }
  }

  // validates condition
  if (condition == CableConditionType::kNull) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid condition";
      messages->push_back(message);
    }
  }

  // validates limit
  if (limit < 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid limit";
      messages->push_back(message);
    }
  }

  // validates type-limit
  if (type_limit == CableConstraint::LimitType::kNull) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid limit type";
      messages->push_back(message);
    }
  }

  return is_valid;
}
