// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/thermalrating/cable_heat_transfer_state.h"

CableHeatTransferState::CableHeatTransferState() {
  heat_convection = 999999;
  heat_radiation = 999999;
  heat_resistance = -999999;
  heat_solar = -999999;
  heat_storage = -999999;
}

CableHeatTransferState::~CableHeatTransferState() {
}

bool CableHeatTransferState::Validate(
    const bool& /*is_included_warnings*/,
    std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "CABLE HEAT TRANSFER STATE";

  // validates heat-convection
  if (0 < heat_convection) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid convection heat transfer";
      messages->push_back(message);
    }
  }

  // validates heat-radiation
  if (0 < heat_radiation) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid radiation heat transfer";
      messages->push_back(message);
    }
  }

  // validates heat-resistance
  if (heat_resistance < 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid resistance heat transfer";
      messages->push_back(message);
    }
  }

  // validates heat-solar
  if (heat_solar < 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid solar heat transfer";
      messages->push_back(message);
    }
  }

  // validates heat-storage
  if ((heat_storage <= -999999) || (999999 <= heat_storage)) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid stored heat transfer";
      messages->push_back(message);
    }
  }

  return is_valid;
}
