// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/sagtension/catenary_cable.h"

CatenaryCable::CatenaryCable() {
  cable_ = nullptr;
}

CatenaryCable::~CatenaryCable() {
}

bool CatenaryCable::Validate(const bool& is_included_warnings,
                             std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "CATENARY CABLE";

  // validates cable
  if (cable_ == nullptr) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid cable";
      messages->push_back(message);
    }
  } else {
    if (cable_->Validate(is_included_warnings, messages) == false) {
      is_valid = false;
    }
  }

  // validates state
  if (state_.Validate(is_included_warnings, messages) == false) {
    is_valid = false;
  }

  // validates catenary
  if (Catenary3d::Validate(is_included_warnings, messages) == false) {
    is_valid = false;
  }

  // validates catenary-cable weight
  if (ValidateCatenaryCableWeight(is_included_warnings,
                                  messages) == false) {
    is_valid = false;
  }

  return is_valid;
}

const SagTensionCable* CatenaryCable::cable() const {
  return cable_;
}

void CatenaryCable::set_cable(const SagTensionCable* cable) {
  cable_ = cable;
}

void CatenaryCable::set_state(const CableState& state) {
  state_ = state;
}

const CableState* CatenaryCable::state() const {
  return &state_;
}

bool CatenaryCable::ValidateCatenaryCableWeight(
    const bool& is_included_warnings,
    std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "CATENARY CABLE";

  // validates that catenary unit weight is greater than cable unit weight
  if (weight_unit_.Magnitude() < *cable_->weight_unit()) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Catenary weight is less than cable unit weight";
      messages->push_back(message);
    }
  }
  return is_valid;
}
