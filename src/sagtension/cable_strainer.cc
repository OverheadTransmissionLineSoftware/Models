// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/sagtension/cable_strainer.h"

#include <cmath>

CableStrainer::CableStrainer() {
  length_start_ = -999999;
  load_finish_ = -999999;
  load_start_ = -999999;
}

CableStrainer::~CableStrainer() {
}

/// This function uses the the generic strain equation. For positive strain
/// transitions, the equation solves for the final length. For negative strain
/// transitions, the equation solves for the initial length. This is done so
/// that if a cable is strained positively and then negatively by the exact
/// same transition strain, the final length would be equal to the initial
/// length.
double CableStrainer::LengthFinish() const {
  // gets the start and finish strains
  const double strain_start = model_elongation_start_.Strain(
      CableElongationModel::ComponentType::kCombined, load_start_);
  const double strain_finish = model_elongation_finish_.Strain(
      CableElongationModel::ComponentType::kCombined, load_finish_);

  // solves for finish length
  const double strain_transition = strain_finish - strain_start;

  // selects variant of strain equation depending on whether transition
  // strain is positive or negative
  if (strain_transition < 0) {
    return length_start_ / (1 + std::abs(strain_transition));
  } else {
    return length_start_ * (1 + strain_transition);
  }
}

bool CableStrainer::Validate(const bool& is_included_warnings,
                             std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "CABLE STRAINER";

  // validates length-start
  if (length_start_ <= 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid length";
      messages->push_back(message);
    }
  }

  // validates load-finish
  if (load_finish_ < 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid finish load";
      messages->push_back(message);
    }
  }

  // validates load-start
  if (load_start_ < 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid start load";
      messages->push_back(message);
    }
  }

  // returns if errors are present
  if (is_valid == false) {
    return is_valid;
  }

  // validates elongation model
  if (model_elongation_start_.Validate(is_included_warnings,
                                        messages) == false) {
    is_valid = false;
    return is_valid;
  }

  if (model_elongation_finish_.Validate(is_included_warnings,
                                        messages) == false) {
    is_valid = false;
    return is_valid;
  }

  return is_valid;
}

const SagTensionCable* CableStrainer::cable() const {
  // doesn't matter which model it is grabbed from - both are similar
  return model_elongation_start_.cable();
}

double CableStrainer::length_start() const {
  return length_start_;
}

double CableStrainer::load_finish() const {
  return load_finish_;
}

double CableStrainer::load_start() const {
  return load_start_;
}

void CableStrainer::set_cable(const SagTensionCable* cable) {
  model_elongation_start_.set_cable(cable);
  model_elongation_finish_.set_cable(cable);
}

void CableStrainer::set_length_start(const double& length_start) {
  length_start_ = length_start;
}

void CableStrainer::set_load_finish(const double& load_finish) {
  load_finish_ = load_finish;
}

void CableStrainer::set_load_start(const double& load_start) {
  load_start_ = load_start;
}

void CableStrainer::set_state_finish(const CableState* state_finish) {
  model_elongation_finish_.set_state(state_finish);
}

void CableStrainer::set_state_start(const CableState* state_start) {
  model_elongation_start_.set_state(state_start);
}

const CableState* CableStrainer::state_finish() const {
  return model_elongation_finish_.state();
}

const CableState* CableStrainer::state_start() const {
  return model_elongation_start_.state();
}
