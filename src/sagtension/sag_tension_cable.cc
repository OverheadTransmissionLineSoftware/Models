// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/sagtension/sag_tension_cable.h"

SagTensionCableComponent::SagTensionCableComponent() {
  component_base_ = nullptr;
}

SagTensionCableComponent::~SagTensionCableComponent() {
}

bool SagTensionCableComponent::IsEnabled() const {
  return is_enabled_;
}

bool SagTensionCableComponent::Validate(
    const bool& is_included_warnings,
    std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "SAG TENSION CABLE COMPONENT";

  // validates base component
  if (component_base_ == nullptr) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid base component";
      messages->push_back(message);
    }
    return is_valid;
  } else {
    component_base_->Validate(is_included_warnings, messages);
  }

  // validates coefficient-expansion-thermal-linear
  if (component_base_->coefficient_expansion_linear_thermal <= -0.005
      || 0.005 < component_base_->coefficient_expansion_linear_thermal) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid thermal expansion coefficient";
      messages->push_back(message);
    }
  }

  // validates coefficients-polynomial-creep
  if (component_base_->coefficients_polynomial_creep.size() != 5) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid creep coefficients";
      messages->push_back(message);
    }
  }

  // validates coefficients-polynomial-loadstrain
  if (component_base_->coefficients_polynomial_loadstrain.size() != 5) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid load-strain coefficients";
      messages->push_back(message);
    }
  }

  // validates load-limit-polynomial-creep
  if (component_base_->load_limit_polynomial_creep < 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid creep polynomial limit";
      messages->push_back(message);
    }
  }

  // validates load-limit-polynomial-loadstrain
  if (component_base_->load_limit_polynomial_loadstrain < 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid load-strain polynomial limit";
      messages->push_back(message);
    }
  }

  // validates modulus-compression-elastic-area
  if (component_base_->modulus_compression_elastic_area < 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid compression elastic area modulus";
      messages->push_back(message);
    }
  }

  // validates modulus-tension-elastic-area
  if (component_base_->modulus_tension_elastic_area < 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid tension elastic area modulus";
      messages->push_back(message);
    }
  }

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

  if (component_base_ == nullptr) {
    is_enabled_ = false;
  } else {
    UpdateIsEnabled();
  }
}

/// The routine searches the component polynomial coefficients. If non-zero
/// values are present for both polynomials the component is flagged as enabled.
void SagTensionCableComponent::UpdateIsEnabled() {
  is_enabled_ = false;
  const std::vector<double>* coefficients = nullptr;

  // gets creep polynomials and scans coefficients for a non-zero value
  bool is_valid_creep = false;
  coefficients = &component_base_->coefficients_polynomial_creep;
  for (auto iter = coefficients->cbegin(); iter != coefficients->cend();
       iter++) {
    const double& coefficient = *iter;
    if (coefficient != 0) {
      is_valid_creep = true;
      break;
    }
  }

  // gets loadstrain polynomial and scans coefficients for a non-zero value
  bool is_valid_loadstrain = false;
  coefficients = &component_base_->coefficients_polynomial_loadstrain;
  for (auto iter = coefficients->cbegin(); iter != coefficients->cend();
       iter++) {
    const double& coefficient = *iter;
    if (coefficient != 0) {
      is_valid_loadstrain = true;
      break;
    }
  }

  // updates enabled status
  if ((is_valid_creep == true) && (is_valid_loadstrain == true)) {
    is_enabled_ = true;
  } else {
    is_enabled_ = false;
  }
}

SagTensionCable::SagTensionCable() {
  cable_base_ = nullptr;
}

SagTensionCable::~SagTensionCable() {
}

bool SagTensionCable::IsEnabled(
    const SagTensionCable::ComponentType& type_component) const {
  // selects based on component type
  if (type_component == SagTensionCable::ComponentType::kCombined) {
    const bool& is_enabled_core = component_sagtension_core_.IsEnabled();
    const bool& is_enabled_shell = component_sagtension_shell_.IsEnabled();

    if ((is_enabled_core == true) || (is_enabled_shell == true)) {
      return true;
    } else {
      return false;
    }
  } else if (type_component == SagTensionCable::ComponentType::kCore) {
    return component_sagtension_core_.IsEnabled();
  } else if (type_component == SagTensionCable::ComponentType::kShell) {
    return component_sagtension_shell_.IsEnabled();
  } else {
    return false;
  }
}

bool SagTensionCable::Validate(const bool& is_included_warnings,
                              std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "SAG TENSION CABLE";

  // validates base cable
  if (cable_base_ == nullptr) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid base cable";
      messages->push_back(message);
    }
    return is_valid;
  } else {
    cable_base_->Validate(is_included_warnings, messages);
  }

  // validates strength-rated
  if (cable_base_->strength_rated < 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid rated strength";
      messages->push_back(message);
    }
  }

  // validates temperature-component-properties
  if (cable_base_->temperature_properties_components < 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid component properties temperature";
      messages->push_back(message);
    }
  }

  // validates component-core
  if (component_sagtension_core_.IsEnabled() == true) {
    if (component_sagtension_core_.Validate(is_included_warnings,
                                            messages) == false) {
      is_valid = false;
    }
  }

  // validates component-shell
  if (component_sagtension_shell_.IsEnabled() == true) {
    if (component_sagtension_shell_.Validate(is_included_warnings,
                                             messages) == false) {
      is_valid = false;
    }
  }

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

  if (cable_base_ == nullptr) {
    component_sagtension_core_.set_component_base(nullptr);
    component_sagtension_shell_.set_component_base(nullptr);
  } else {
    component_sagtension_core_.set_component_base(&cable_base_->component_core);
    component_sagtension_shell_.set_component_base(
        &cable_base_->component_shell);
  }
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