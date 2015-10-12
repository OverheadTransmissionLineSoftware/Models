// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/sagtension/sag_tension_cable.h"

SagTensionCableComponent::SagTensionCableComponent() {
  is_updated_limit_polynomial_ = false;
  load_limit_polynomial_ = -999999;
}

SagTensionCableComponent::~SagTensionCableComponent() {
}

bool SagTensionCableComponent::Validate(
    const bool& is_included_warnings,
    std::list<std::string>* messages_error) const {
  bool is_valid = true;
  /// \todo need to finish this
  ///   should pull some of the validation away from the Cable class

  return is_valid;
}

const double* SagTensionCableComponent::coefficient_expansion_linear_thermal()
    const {
  return &component_base_->coefficient_expansion_linear_thermal;
}

const std::vector<double>* SagTensionCableComponent::coefficients_polynomial(
    const PolynomialType& type_polynomial) const {
  if (type_polynomial == PolynomialType::kCreep) {
    return &component_base_->coefficients_polynomial_creep;
  } else if (type_polynomial == PolynomialType::kLoadStrain) {
    return &component_base_->coefficients_polynomial_loadstrain;
  } else {
    return nullptr;
  }
}

const CableComponent* SagTensionCableComponent::component_base() const {
  return component_base_;
}

const double* SagTensionCableComponent::load_limit_polynomial(
    const PolynomialType& type_polynomial) const {
  /// \todo implement update process
  //if (IsUpdated() == false) {
  //  return nullptr;
  //}
  
  if (type_polynomial == PolynomialType::kCreep) {
    return &component_base_->load_limit_polynomial_creep;
  } else if (type_polynomial == PolynomialType::kLoadStrain) {
    return &component_base_->load_limit_polynomial_loadstrain;
  } else {
    return nullptr;
  }
}

const double* SagTensionCableComponent::modulus_compression_elastic_area()
    const {
  return &component_base_->modulus_compression_elastic_area;
}

const double* SagTensionCableComponent::modulus_tension_elastic_area() const {
  return &component_base_->modulus_tension_elastic_area;
}

void SagTensionCableComponent::set_component_base(
    const CableComponent* component_base) {
  component_base_ = component_base;
  is_updated_limit_polynomial_ = false;
}

bool SagTensionCableComponent::IsUpdated() const {
  if (is_updated_limit_polynomial_ == true) {
    return true;
  } else {
    return false;
  }
}

bool SagTensionCableComponent::UpdateLimitPolynomial() const {
  /// \todo need to finish this

  return true;
}

SagTensionCable::SagTensionCable() {
}

SagTensionCable::~SagTensionCable() {
}

bool SagTensionCable::Validate(const bool& is_included_warnings,
                              std::list<std::string>* messages_error) const {
  bool is_valid = true;

  /// \todo need to do this
  ///   should pull some of the validation away from the Cable class

  // returns validation status
  return is_valid;
}

const Cable* SagTensionCable::cable_base() const {
  return cable_base_;
}

const SagTensionCableComponent* SagTensionCable::component_core() const {
  return &component_sagtension_core_;
}

const SagTensionCableComponent* SagTensionCable::component_shell() const {
  return &component_sagtension_shell_;
}

const double* SagTensionCable::diameter() const {
  return &cable_base_->diameter;
}

void SagTensionCable::set_cable_base(const Cable* cable_base) {
  cable_base_ = cable_base;
  component_sagtension_core_.set_component_base(&cable_base_->component_core);
  component_sagtension_shell_.set_component_base(&cable_base_->component_shell);
}

const double* SagTensionCable::strength_rated() const {
    return &cable_base_->strength_rated;
}

const double* SagTensionCable::temperature_properties_components() const {
  return &cable_base_->temperature_properties_components;
}

const double* SagTensionCable::weight_unit() const {
  return &cable_base_->weight_unit;
}