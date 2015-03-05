// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "transmissionline/cable.h"

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

  // validate coefficient-expansion-thermal-linear
  if (coefficient_expansion_linear_thermal <= -0.005
      || 0.005 < coefficient_expansion_linear_thermal) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CABLE COMPONENT - Invalid coefficient of "
                                "thermal expansion");
    }
  }

  // validate coefficients-polynomial-creep
  if (coefficients_polynomial_creep.size() != 5) {
    messages_error->push_back("CABLE COMPONENT - Invalid creep coefficients");
  }

  // validate coefficients-polynomial-loadstrain
  if (coefficients_polynomial_loadstrain.size() != 5) {
    messages_error->push_back("CABLE COMPONENT - Invalid load-strain "
                              "coefficients");
  }

  // validate load-limit-polynomial-creep
  if (load_limit_polynomial_creep < 0) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CABLE COMPONENT - Invalid creep polynomial "
                                "limit");
    }
  }

  // validate load-limit-polynomial-loadstrain
  if (load_limit_polynomial_loadstrain < 0) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CABLE COMPONENT - Invalid load-strain "
                                "polynomial limit");
    }
  }

  // validate modulus-compression-elastic-area
  if (modulus_compression_elastic_area < 0) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CABLE COMPONENT- Invalid compression elastic "
                                "area modulus");
    }
  }

  // validate modulus-tension-elastic-area
  if (modulus_tension_elastic_area < 0) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("COMPONENT | CABLE - Invalid tension elastic "
                                "area modulus");
    }
  }

  //return validation status
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

  // validate area-electrical
  if (area_electrical < 0) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CABLE - Invalid electrical area");
    }
  }

  // validate area-physical
  if (area_physical < 0) {
    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CABLE - Invalid physical area");
    }
  }

  // validate component-core
  component_core.Validate(is_included_warnings, messages_error);

  // validate component-shell
  component_shell.Validate(is_included_warnings, messages_error);

  // validate diameter
  if (diameter <= 0
      || (3 < diameter && is_included_warnings == true)) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CABLE - Invalid diameter");
    }
  }

  // validate name
  // nothing to validate

  // validate strength-rated
  if (strength_rated < 0) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CABLE - Invalid rated strength");
    }
  }

  // validate temperature-component-properties
  if (temperature_properties_components < 0) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CABLE - Invalid component properties "
                                "temperature");
    }
  }

  // validate type-construction
  // nothing to validate

  // validate type-polynomial-active
  if (type_polynomial_active != CablePolynomialType::kCreep
      || type_polynomial_active != CablePolynomialType::kLoadStrain) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CABLE - Invalid active polynomial type");
    }
  }

  // validate weight-unit
  if (weight_unit <= 0
      || (10 < weight_unit && is_included_warnings == true)) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CABLE - Invalid unit weight");
    }
  }

  // return validation status
  return is_valid;
}
