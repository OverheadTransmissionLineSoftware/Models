// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/sagtension/cable_state.h"

CableState::CableState() {
  temperature = -999999;
  type_polynomial = SagTensionCableComponent::PolynomialType::kNull;
}

CableState::~CableState() {
}

bool CableState::Validate(const bool& /*is_included_warnings*/,
                          std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "CABLE STATE";

  // validates temperature
  if (temperature < -100) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid temperature";
      messages->push_back(message);
    }
  }

  // validates type-polynomial
  if (type_polynomial == SagTensionCableComponent::PolynomialType::kNull) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid polynomial type";
      messages->push_back(message);
    }
  }

  return is_valid;
}

CableStretchState::CableStretchState() {
  load = -999999;
}

CableStretchState::~CableStretchState() {
}

bool CableStretchState::Validate(const bool& is_included_warnings,
                                 std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "CABLE STRETCH STATE";

  // validates parent struct
  CableState::Validate(is_included_warnings, messages);

  // validates load
  if (load < 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid stretch load";
      messages->push_back(message);
    }
  }

  return is_valid;
}

