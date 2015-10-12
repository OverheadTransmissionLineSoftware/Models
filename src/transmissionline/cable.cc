// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/transmissionline/cable.h"

CableComponent::CableComponent() {
  coefficient_expansion_linear_thermal = -999999;
  coefficients_polynomial_creep = std::vector<double>(5, 0);
  coefficients_polynomial_loadstrain = std::vector<double>(5, 0);
  load_limit_polynomial_creep = -999999;
  load_limit_polynomial_loadstrain = -999999;
  modulus_compression_elastic_area = -999999;
  modulus_tension_elastic_area = -999999;
}

CableComponent::~CableComponent() {}

bool CableComponent::Validate(const bool& is_included_warnings,
                              std::list<std::string>* messages_error) const {
  bool is_valid = true;

  // validates coefficient-expansion-thermal-linear
  if (coefficient_expansion_linear_thermal <= -0.005
      || 0.005 < coefficient_expansion_linear_thermal) {
    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CABLE COMPONENT - Invalid coefficient of "
                                "thermal expansion");
    }
  }

  // validates coefficients-polynomial-creep
  if (coefficients_polynomial_creep.size() != 5) {
    messages_error->push_back("CABLE COMPONENT - Invalid creep coefficients");
  }

  // validates coefficients-polynomial-loadstrain
  if (coefficients_polynomial_loadstrain.size() != 5) {
    messages_error->push_back("CABLE COMPONENT - Invalid load-strain "
                              "coefficients");
  }

  // validates load-limit-polynomial-creep
  if (load_limit_polynomial_creep < 0) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CABLE COMPONENT - Invalid creep polynomial "
                                "limit");
    }
  }

  // validates load-limit-polynomial-loadstrain
  if (load_limit_polynomial_loadstrain < 0) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CABLE COMPONENT - Invalid load-strain "
                                "polynomial limit");
    }
  }

  // validates modulus-compression-elastic-area
  if (modulus_compression_elastic_area < 0) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CABLE COMPONENT- Invalid compression elastic "
                                "area modulus");
    }
  }

  // validates modulus-tension-elastic-area
  if (modulus_tension_elastic_area < 0) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("COMPONENT | CABLE - Invalid tension elastic "
                                "area modulus");
    }
  }

  // returns validation status
  return is_valid;
}

Cable::Cable() {
  area_electrical = -999999;
  area_physical = -999999;
  diameter = -999999;
  name = "";
  strength_rated = -999999;
  temperature_properties_components = -999999;
  type_construction = "";
  weight_unit = -999999;
}

Cable::~Cable() {}

bool Cable::Validate(const bool& is_included_warnings,
                     std::list<std::string>* messages_error) const {
  bool is_valid = true;

  // validates area-electrical
  if (area_electrical < 0) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CABLE - Invalid electrical area");
    }
  }

  // validates area-physical
  if (area_physical < 0) {
    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CABLE - Invalid physical area");
    }
  }

  // validates component-core
  component_core.Validate(is_included_warnings, messages_error);

  // validates component-shell
  component_shell.Validate(is_included_warnings, messages_error);

  // validates diameter
  if (diameter <= 0
      || (3 < diameter && is_included_warnings == true)) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CABLE - Invalid diameter");
    }
  }

  // validates name
  // nothing to validate

  // validates strength-rated
  if (strength_rated < 0) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CABLE - Invalid rated strength");
    }
  }

  // validates temperature-component-properties
  if (temperature_properties_components < 0) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CABLE - Invalid component properties "
                                "temperature");
    }
  }

  // validates type-construction
  // nothing to validate

  // validates weight-unit
  if (weight_unit <= 0
      || (10 < weight_unit && is_included_warnings == true)) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CABLE - Invalid unit weight");
    }
  }

  return is_valid;
}
