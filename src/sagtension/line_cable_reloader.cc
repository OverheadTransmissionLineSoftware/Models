// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/sagtension/line_cable_reloader.h"

#include "models/sagtension/catenary_cable_reloader.h"

LineCableReloader::LineCableReloader() {
  condition_reloaded_ = CableConditionType::kNull;
  weathercase_reloaded_ = nullptr;

  is_updated_catenary_reloaded_ = false;
  is_updated_model_reloaded_ = false;
}

LineCableReloader::~LineCableReloader() {
}

Catenary3d LineCableReloader::CatenaryReloaded() const {
  // updates class if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return Catenary3d();
    }
  }

  return catenary_reloaded_;
}

CableState LineCableReloader::StateReloaded() const {
  // updates class if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return CableStretchState();
    }
  }

  return model_reloaded_.state();
}

double LineCableReloader::TensionAverageComponent(
    const CableElongationModel::ComponentType& type_component) const {
  // updates class if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  // gets total strain
  const double strain_total =
      model_reloaded_.Strain(CableElongationModel::ComponentType::kCombined,
                             catenary_reloaded_.TensionAverage());

  return model_reloaded_.Load(type_component, strain_total);
}

double LineCableReloader::TensionHorizontal() const {
  // updates class if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return catenary_reloaded_.tension_horizontal();
}

/// The component horizontal tension cannot be directly solved for. It is
/// calculated by scaling the average component load by the ratio of the entire
/// cable horizontal tension and entire cable average load.
double LineCableReloader::TensionHorizontalComponent(
    const CableElongationModel::ComponentType& type_component) const {
  // updates class if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  // gets average tension of component
  const double tension_average_component =
      TensionAverageComponent(type_component);

  // uses ratio of average tension and horizontal tension to scale value
  return tension_average_component
      * (catenary_reloaded_.tension_horizontal()
      / catenary_reloaded_.TensionAverage());
}

bool LineCableReloader::Validate(
    const bool& is_included_warnings,
    std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "LINE CABLE RELOADER";

  // validates base class
  if (LineCableLoaderBase::Validate(is_included_warnings, messages) == false) {
    is_valid = false;
  }

  // validates condition-reloaded
  if (condition_reloaded_ == CableConditionType::kNull) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid reloaded condition";
      messages->push_back(message);
    }
  }

  // validates weathercase-reloaded
  if (weathercase_reloaded_ == nullptr) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid reloaded weathercase";
      messages->push_back(message);
    }
  } else {
    if (weathercase_reloaded_->Validate(is_included_warnings,
                                        messages) == false) {
      is_valid = false;
    }
  }

  // returns if errors are present
  if (is_valid == false) {
    return is_valid;
  }

  // validates update process
  if (Update() == false) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "";
      if (is_updated_catenary_reloaded_ == false) {
        message.description = "Error updating class. Could not solve for "
                              "reloaded catenary.";
      } else if (is_updated_model_reloaded_ == false) {
        message.description = "Error updating class. Could not solve for "
                              "reloaded cable model.";
      }
      messages->push_back(message);
    }
  }

  return is_valid;
}

CableConditionType LineCableReloader::condition_reloaded() const {
  return condition_reloaded_;
}

void LineCableReloader::set_condition_reloaded(
    const CableConditionType& condition_reloaded) {
  condition_reloaded_ = condition_reloaded;

  is_updated_model_reloaded_ = false;
  is_updated_catenary_reloaded_ = false;
}

void LineCableReloader::set_weathercase_reloaded(
    const WeatherLoadCase* case_reloaded) {
  weathercase_reloaded_ = case_reloaded;

  is_updated_model_reloaded_ = false;
  is_updated_catenary_reloaded_ = false;
}

const WeatherLoadCase* LineCableReloader::weathercase_reloaded() const {
  return weathercase_reloaded_;
}

bool LineCableReloader::IsUpdated() const {
  if ((LineCableLoaderBase::IsUpdated() == true)
      && (is_updated_model_reloaded_ == true)
      && (is_updated_catenary_reloaded_ == true)) {
    return true;
  } else {
    return false;
  }
}

bool LineCableReloader::Update() const {
  // updates base class
  if (LineCableLoaderBase::IsUpdated() == false) {
    // resets downstream update indicators
    is_updated_model_reloaded_ = false;
    is_updated_catenary_reloaded_ = false;

    if (LineCableLoaderBase::Update() == false) {
      return false;
    }
  }

  // updates reloaded cable model
  if (is_updated_model_reloaded_ == false) {
    is_updated_model_reloaded_ = UpdateReloadedCableModel();
    if (is_updated_model_reloaded_ == false) {
      return false;
    }
  }

  // updates reloaded catenary
  if (is_updated_catenary_reloaded_ == false) {
    is_updated_catenary_reloaded_ = UpdateReloadedCatenary();
    if (is_updated_catenary_reloaded_ == false) {
      return false;
    }
  }

  // if it reaches this point, update was successful
  return true;
}

bool LineCableReloader::UpdateReloadedCableModel() const {
  // builds cable states based on reloaded weathercase and stretch
  CableState state;
  state.temperature = weathercase_reloaded_->temperature_cable;
  state.type_polynomial = SagTensionCableComponent::PolynomialType::kLoadStrain;

  CableStretchState state_stretch;
  if (condition_reloaded_ == CableConditionType::kInitial) {
    // stretch parameters don't matter, model isn't stretched
    state_stretch.load = 0;
    state_stretch.temperature = 0;
    state_stretch.type_polynomial =
        SagTensionCableComponent::PolynomialType::kLoadStrain;
  } else if (condition_reloaded_ == CableConditionType::kCreep) {
    state_stretch = state_stretch_creep_;
  } else if (condition_reloaded_ == CableConditionType::kLoad) {
    state_stretch = state_stretch_load_;
  } else {
    return false;
  }

  // updates model
  model_reloaded_.set_cable(&cable_sagtension_);
  model_reloaded_.set_state(state);
  model_reloaded_.set_state_stretch(state_stretch);

  return true;
}

bool LineCableReloader::UpdateReloadedCatenary() const {
  // calculates reloaded unit weight
  Vector3d weight_unit_reloaded = UnitLoad(*weathercase_reloaded_);

  // builds reloader and gets the reloaded catenary
  CatenaryCableReloader reloader;
  reloader.set_catenary(&catenary_constraint_);
  reloader.set_model_reference(&model_constraint_);
  reloader.set_model_reloaded(&model_reloaded_);
  reloader.set_weight_unit_reloaded(&weight_unit_reloaded);

  if (reloader.Validate(false, nullptr) == true) {
    catenary_reloaded_ = reloader.CatenaryReloaded();
    return true;
  } else {
    return false;
  }
}
