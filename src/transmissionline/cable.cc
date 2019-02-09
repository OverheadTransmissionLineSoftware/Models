// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/transmissionline/cable.h"

CableComponent::CableComponent() {
  capacity_heat = -999999;
  coefficient_expansion_linear_thermal = -999999;
  load_limit_polynomial_creep = -999999;
  load_limit_polynomial_loadstrain = -999999;
  modulus_compression_elastic_area = -999999;
  modulus_tension_elastic_area = -999999;
  scale_polynomial_x = -999999;
  scale_polynomial_y = -999999;
}

CableComponent::~CableComponent() {}

bool CableComponent::Validate(
    const bool& /*is_included_warnings*/,
    std::list<ErrorMessage>* /*messages_error*/) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "CABLE COMPONENT";

  // Most component validation has moved to SagTensionCableComponent.
  // This function will remain as a placeholder for validation when more
  // libraries are developed.

  // returns validation status
  return is_valid;
}

Cable::Cable() {
  absorptivity = -999999;
  area_physical = -999999;
  diameter = -999999;
  emissivity = -999999;
  name = "";
  strength_rated = -999999;
  temperature_properties_components = -999999;
  weight_unit = -999999;
}

Cable::~Cable() {}

bool Cable::Validate(const bool& is_included_warnings,
                     std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "CABLE";

  // validates area-physical
  if (area_physical < 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid physical area";
      messages->push_back(message);
    }
  }

  // validates component-core
  component_core.Validate(is_included_warnings, messages);

  // validates component-shell
  component_shell.Validate(is_included_warnings, messages);

  // validates diameter
  if (diameter <= 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid diameter";
      messages->push_back(message);
    }
  }

  // validates weight-unit
  if (weight_unit <= 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid unit weight";
      messages->push_back(message);
    }
  }

  return is_valid;
}
