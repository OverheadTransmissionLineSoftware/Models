// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/sagtension/line_cable_unloader.h"

#include <cmath>

#include "models/sagtension/catenary_cable_unloader.h"

LineCableUnloader::LineCableUnloader() {
  condition_unloaded_ = CableConditionType::kNull;
  spacing_attachments_ = Vector3d();
  temperature_unloaded_ = -999999;

  length_unloaded_ = -999999;

  is_updated_length_unloaded_ = false;
  is_updated_model_unloaded_ = false;
}

LineCableUnloader::~LineCableUnloader() {
}

double LineCableUnloader::LengthUnloaded() const {
  // updates class if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return length_unloaded_;
}

bool LineCableUnloader::Validate(
    const bool& is_included_warnings,
    std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "LINE CABLE UNLOADER";

  // validates base class
  if (LineCableLoaderBase::Validate(is_included_warnings, messages) == false) {
    is_valid = false;
  }

  // validates condition-unloaded
  if (condition_unloaded_ == CableConditionType::kNull) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid unloaded condition";
      messages->push_back(message);
    }
  }

  // validates spacing-attachments
  if (spacing_attachments_.x() <= 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid horizontal attachment spacing";
      messages->push_back(message);
    }
  }

  if (spacing_attachments_.y() != 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid transverse attachment spacing";
      messages->push_back(message);
    }
  }

  if (2000 < std::abs(spacing_attachments_.z())) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid vertical attachment spacing";
      messages->push_back(message);
    }
  }

  // validates temperature-unloaded
  if (temperature_unloaded_ < -50) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid unloaded temperature";
      messages->push_back(message);
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
      if (is_updated_length_unloaded_ == false) {
        message.description = "Error updating class. Could not solve for "
                              "unloaded cable length.";
      } else if (is_updated_model_unloaded_ == false) {
        message.description = "Error updating class. Could not solve for "
                              "unloaded cable model.";
      }
      messages->push_back(message);
    }
  }

  return is_valid;
}

CableConditionType LineCableUnloader::condition_unloaded() const {
  return condition_unloaded_;
}

void LineCableUnloader::set_condition_unloaded(
    const CableConditionType& condition_unloaded) {
  condition_unloaded_ = condition_unloaded;

  is_updated_model_unloaded_ = false;
  is_updated_length_unloaded_ = false;
}

void LineCableUnloader::set_spacing_attachments(
    const Vector3d& spacing_attachments) {
  spacing_attachments_ = spacing_attachments;

  is_updated_length_unloaded_ = false;
}

void LineCableUnloader::set_temperature_unloaded(
    const double& temperature_unloaded) {
  temperature_unloaded_ = temperature_unloaded;

  is_updated_model_unloaded_ = false;
  is_updated_length_unloaded_ = false;
}

Vector3d LineCableUnloader::spacing_attachments() const {
  return spacing_attachments_;
}

double LineCableUnloader::temperature_unloaded() const {
  return temperature_unloaded_;
}

bool LineCableUnloader::IsUpdated() const {
  if ((LineCableLoaderBase::IsUpdated() == true)
      && (is_updated_model_unloaded_ == true)
      && (is_updated_length_unloaded_ == true)) {
    return true;
  } else {
    return false;
  }
}

bool LineCableUnloader::Update() const {
  // updates base class
  if (LineCableLoaderBase::IsUpdated() == false) {
    // resets downstream update indicators
    is_updated_model_unloaded_ = false;
    is_updated_length_unloaded_ = false;

    if (LineCableLoaderBase::Update() == false) {
      return false;
    }
  }

  // updates unloaded cable model
  if (is_updated_model_unloaded_ == false) {
    is_updated_model_unloaded_ = UpdateUnloadedCableModel();
    if (is_updated_model_unloaded_ == false) {
      return false;
    }
  }

  // updates unloaded length
  if (is_updated_length_unloaded_ == false) {
    is_updated_length_unloaded_ = UpdateUnloadedLength();
    if (is_updated_length_unloaded_ == false) {
      return false;
    }
  }

  // if it reaches this point, update was successful
  return true;
}

bool LineCableUnloader::UpdateUnloadedCableModel() const {
  // builds cable states based on unloaded condition and temperature
  CableState state;
  state.temperature = temperature_unloaded_;
  state.type_polynomial = SagTensionCableComponent::PolynomialType::kLoadStrain;

  CableStretchState state_stretch;
  if (condition_unloaded_ == CableConditionType::kInitial) {
    // stretch parameters don't matter, model isn't stretched
    state_stretch.load = 0;
    state_stretch.temperature = 0;
    state_stretch.type_polynomial =
        SagTensionCableComponent::PolynomialType::kLoadStrain;
  } else if (condition_unloaded_ == CableConditionType::kCreep) {
    state_stretch = state_stretch_creep_;
  } else if (condition_unloaded_ == CableConditionType::kLoad) {
    state_stretch = state_stretch_load_;
  } else {
    return false;
  }

  // updates model
  model_unloaded_.set_cable(&cable_sagtension_);
  model_unloaded_.set_state(state);
  model_unloaded_.set_state_stretch(state_stretch);

  return true;
}

bool LineCableUnloader::UpdateUnloadedLength() const {
  // builds catenary to unload
  Catenary3d catenary = catenary_constraint_;
  catenary.set_spacing_endpoints(spacing_attachments_);

  // builds unloader and gets the unloaded length
  CatenaryCableUnloader unloader;
  unloader.set_catenary(&catenary);
  unloader.set_model_reference(&model_constraint_);
  unloader.set_model_unloaded(&model_unloaded_);

  if (unloader.Validate(false, nullptr) == true) {
    length_unloaded_ = unloader.LengthUnloaded();
    return true;
  } else {
    return false;
  }
}
