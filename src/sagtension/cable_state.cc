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
                          std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "CABLE STATE";

  // validates load-stretch
  if (load_stretch < 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid stretch load";
      messages->push_back(message);
    }
  }

  // validates temperature
  if (temperature < -100) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid temperature";
      messages->push_back(message);
    }
  }

  // validates temperature - stretch
  if (temperature_stretch < -100) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid stretch temperature";
      messages->push_back(message);
    }
  }

  // validates type-polynomial
  if ((type_polynomial != SagTensionCableComponent::PolynomialType::kCreep)
      && (type_polynomial !=
          SagTensionCableComponent::PolynomialType::kLoadStrain)) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid polynomial type";
      messages->push_back(message);
    }
  }

  // validates load-stretch and type-polynomial
  if ((type_polynomial == SagTensionCableComponent::PolynomialType::kCreep)
      && (load_stretch != 0)) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid stretch load and polynomial type "
                            "combination. The creep polynomial must not have "
                            "any stretch load.";
      messages->push_back(message);
    }
  }

  return is_valid;
}
