// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/thermalrating/thermal_rating_cable.h"

#include "models/base/helper.h"

ThermalRatingCableComponent::ThermalRatingCableComponent() {
  component_base_ = nullptr;
}

ThermalRatingCableComponent::~ThermalRatingCableComponent() {
}

bool ThermalRatingCableComponent::Validate(
    const bool& is_included_warnings,
    std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "THERMAL RATING CABLE COMPONENT";

  // validates base component
  if (component_base_ == nullptr) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid base cable";
      messages->push_back(message);
    }
    return is_valid;
  } else {
    if (component_base_->Validate(is_included_warnings, messages) == false) {
      is_valid = false;
    }
  }

  // validates capacity-heat
  if (component_base_->capacity_heat < 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid heat capacity";
      messages->push_back(message);
    }
  }

  return is_valid;
}

const double* ThermalRatingCableComponent::capacity_heat() const {
  return &component_base_->capacity_heat;
}

const CableComponent* ThermalRatingCableComponent::component_base() const {
  return component_base_;
}

void ThermalRatingCableComponent::set_component_base(
    const CableComponent* component_base) {
  component_base_ = component_base;
}


ThermalRatingCable::ThermalRatingCable() {
  cable_base_ = nullptr;
}

ThermalRatingCable::~ThermalRatingCable() {
}

double ThermalRatingCable::Resistance(const double& temperature) const {
  // checks that at least two resistance values are specified
  if (cable_base_->resistances_ac.size() <= 1) {
    return -999999;
  }

  // selects the boundary points based on cable temperature
  const std::list<Cable::ResistancePoint>& resistances =
      cable_base_->resistances_ac;
  auto iter = resistances.cbegin();
  const Cable::ResistancePoint* point_low = &(*iter);
  iter++;
  const Cable::ResistancePoint* point_high = &(*iter);

  // searches list of points for the closest ones to target temp, starting
  // from the lowest
  while (true) {
    // stops searching if resistance points bound the target temp
    if ((point_low->temperature < temperature)
         && (temperature < point_high->temperature)) {
      break;
    }

    // stops searching if both resistance points are greater than target temp
    if ((point_low->temperature < temperature)
         && (point_high->temperature < temperature)) {
      break;
    }

    // increments and stops searching if the end is found
    iter++;
    if (iter == resistances.cend()) {
      break;
    }

    // adjusts points for next iteration
    point_low = point_high;
    point_high = &(*iter);
  }

  // determines the resistance of the cable at a specific temperature
  // uses linear interpolation/extrapolation
  return helper::LinearY(point_low->temperature, point_low->resistance,
                         point_high->temperature, point_high->resistance,
                         temperature);
}

bool ThermalRatingCable::Validate(const bool& is_included_warnings,
                                  std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "THERMAL RATING CABLE";

  // validates base cable
  if (cable_base_ == nullptr) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid base cable";
      messages->push_back(message);
    }
    return is_valid;
  } else {
    if (cable_base_->Validate(is_included_warnings, messages) == false) {
      is_valid = false;
    }
  }

  // validates absorptivity
  if (cable_base_->absorptivity < 0 || 1 < cable_base_->absorptivity) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid absorptivity";
      messages->push_back(message);
    }
  }

  // validates emissivity
  if (cable_base_->emissivity < 0 || 1 < cable_base_->emissivity) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid emissivity";
      messages->push_back(message);
    }
  }

  // validates resistances-ac
  if (cable_base_->resistances_ac.size() < 2) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid ac resistances";
      messages->push_back(message);
    }
  } else {
    // further checks resistances
    for (auto iter = cable_base_->resistances_ac.cbegin();
         iter != cable_base_->resistances_ac.cend(); iter++) {
      // skips first iteration
      if (iter == cable_base_->resistances_ac.cbegin()) {
        continue;
      }

      // checks if that temperature is ascending
      const Cable::ResistancePoint& point_prev = *std::prev(iter, 1);
      const Cable::ResistancePoint& point = *iter;

      if (point.temperature <= point_prev.temperature) {
        is_valid = false;
        if (messages != nullptr) {
          message.description = "Resistances are not ordered by increasing "
                                "temperature";
          messages->push_back(message);
        }
      }
    }
  }

  // validates component-core
  if (component_thermalrating_core_.Validate(is_included_warnings,
                                             messages) == false) {
    is_valid = false;
  }

  // validates component-shell
  if (component_thermalrating_shell_.Validate(is_included_warnings,
                                              messages) == false) {
    is_valid = false;
  }

  return is_valid;
}

const double* ThermalRatingCable::absorptivity() const {
  return &cable_base_->absorptivity;
}

const Cable* ThermalRatingCable::cable_base() const {
  return cable_base_;
}

const ThermalRatingCableComponent* ThermalRatingCable::component_core() const {
  return &component_thermalrating_core_;
}

const ThermalRatingCableComponent* ThermalRatingCable::component_shell() const {
  return &component_thermalrating_shell_;
}

const double* ThermalRatingCable::diameter() const {
  return &cable_base_->diameter;
}

const double* ThermalRatingCable::emissivity() const {
  return &cable_base_->emissivity;
}

const std::list<Cable::ResistancePoint>* ThermalRatingCable::resistances_ac()
    const {
  return &cable_base_->resistances_ac;
}

void ThermalRatingCable::set_cable_base(const Cable* cable_base) {
  cable_base_ = cable_base;

  if (cable_base_ == nullptr) {
    component_thermalrating_core_.set_component_base(nullptr);
    component_thermalrating_shell_.set_component_base(nullptr);
  } else {
    component_thermalrating_core_.set_component_base(
        &cable_base_->component_core);
    component_thermalrating_shell_.set_component_base(
        &cable_base_->component_shell);
  }
}
