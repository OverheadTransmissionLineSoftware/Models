// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "sagtension/cable_strainer.h"

CableStrainer::CableStrainer() {

  is_stretched_finish_ = -999999;
  is_stretched_start_ = -999999;
  length_start_ = -999999;
  load_finish_ = -999999;
  load_start_ = -999999;
}

CableStrainer::~CableStrainer() {
}

/// Calculates the length by using the starting length, and adjusting for
/// thermal and load based strain transitions.
double CableStrainer::LengthFinish() const {

  double length_finish = 0;

  // get load-based strain transition and length after load-straining
  const double strain_transition_load = StrainTransitionLoad();
  length_finish = LengthAfterStrain(length_start_, strain_transition_load);

  // get thermal-based strain transition and length after thermal straining
  const double strain_transition_thermal = StrainTransitionThermal();
  length_finish = LengthAfterStrain(length_finish, strain_transition_thermal);

  return length_finish;
}

double CableStrainer::LoadFinishCore() const {

  // get strain at finish load
  const double strain_finish =
      model_elongation_finish_.GetStrainTotal(load_finish_,
                                              is_stretched_finish_);

  // get load for core
  return model_elongation_finish_.GetLoadCore(
      strain_finish, is_stretched_finish_);
}

double CableStrainer::LoadFinishShell() const {

  // get strain at finish load
  const double strain_finish =
      model_elongation_finish_.GetStrainTotal(load_finish_,
                                              is_stretched_finish_);

  // get load for core
  return model_elongation_finish_.GetLoadShell(
      strain_finish, is_stretched_finish_);
}

double CableStrainer::LoadStartCore() const {

  // get strain at finish load
  const double strain_start =
      model_elongation_start_.GetStrainTotal(load_start_,
                                              is_stretched_start_);

  // get load for shell
  return model_elongation_start_.GetLoadCore(
      strain_start, is_stretched_start_);
}

double CableStrainer::LoadStartShell() const {

  // get strain at finish load
  const double strain_start =
      model_elongation_start_.GetStrainTotal(load_start_,
                                              is_stretched_start_);

  // get load for shell
  return model_elongation_start_.GetLoadShell(
      strain_start, is_stretched_start_);
}

double CableStrainer::StrainTransitionLoad() const {

  // strain at start
  const double strain_start_unloaded_unstretched =
      model_elongation_start_.GetStrainTotal(0, false);

  const double strain_start_loaded =
      model_elongation_start_.GetStrainTotal(load_start_, is_stretched_start_);

  // strain at finish
  const double strain_finish_unloaded_unstretched =
      model_elongation_finish_.GetStrainTotal(0, false);

  const double strain_finish_loaded =
      model_elongation_finish_.GetStrainTotal(load_finish_,
                                              is_stretched_finish_);

  // subtract to get transition strain
  return (strain_finish_loaded - strain_finish_unloaded_unstretched)
      - (strain_start_loaded - strain_start_unloaded_unstretched);
}

double CableStrainer::StrainTransitionThermal() const {

  // strain-unloaded-unstretched at start temperature
  const double strain_unloaded_unstretched_start =
      model_elongation_start_.GetStrainTotal(0, false);

  // strain-unloaded-unstretched at finish temperature
  const double strain_unloaded_unstretched_finish =
      model_elongation_finish_.GetStrainTotal(0, false);

  // subtract to get transition strain
  return strain_unloaded_unstretched_finish- strain_unloaded_unstretched_start;
}

bool CableStrainer::Validate(const bool& is_included_warnings,
                             std::list<std::string>* messages_error) const {

  // initialize
  bool is_valid = true;

  // validate is-stretched-start
  // nothing to validate

  // validate is-stretched-finish
  // nothing to validate

  // validate length - start
  if (length_start_ <= 0) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back(
          "CABLE STRAINER - Invalid length");
    }
  }

  // validate load-finish
  if (load_finish_ < 0) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back(
          "CABLE STRAINER - Invalid finish load");
    }
  }

  // validate load-start
  if (load_start_ < 0) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back(
          "CABLE STRAINER - Invalid start load");
    }
  }

  // further validate, if no errors are present
  if (is_valid == true) {

    // validate load-strain model
    if (model_elongation_start_.Validate(is_included_warnings,
                                         messages_error) == false) {
      is_valid = false;
    }

    model_elongation_finish_.Validate(is_included_warnings, nullptr);
  }

  return is_valid;
}

Cable CableStrainer::cable() const {
  // doesn't matter which model it is grabbed from - both are similar
  return model_elongation_start_.cable();
}

bool CableStrainer::is_stretched_finish() const {
  return is_stretched_finish_;
}

bool CableStrainer::is_stretched_start() const {
  return is_stretched_start_;
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

double CableStrainer::load_stretch() const {
  // doesn't matter which model it is grabbed from - both are similar
  return model_elongation_start_.load_stretch();
}

void CableStrainer::set_cable(const Cable& cable) {
  model_elongation_start_.set_cable(cable);
  model_elongation_finish_.set_cable(cable);
}

void CableStrainer::set_is_stretched_finish(const bool& is_stretched_finish) {
  is_stretched_finish_ = is_stretched_finish;
}

void CableStrainer::set_is_stretched_start(const bool& is_stretched_start) {
  is_stretched_start_ = is_stretched_start;
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

void CableStrainer::set_load_stretch(const double& load_stretch) {
  model_elongation_start_.set_load_stretch(load_stretch);
  model_elongation_finish_.set_load_stretch(load_stretch);
}

void CableStrainer::set_temperature_finish(const double& temperature_finish) {
  model_elongation_finish_.set_temperature(temperature_finish);
}

void CableStrainer::set_temperature_start(const double& temperature_start) {
  model_elongation_start_.set_temperature(temperature_start);
}

void CableStrainer::set_temperature_stretch(const double& temperature_stretch) {
  model_elongation_start_.set_temperature_stretch(temperature_stretch);
  model_elongation_finish_.set_temperature_stretch(temperature_stretch);
}

double CableStrainer::temperature_finish() const {
  return model_elongation_finish_.temperature();
}

double CableStrainer::temperature_start() const {
  return model_elongation_start_.temperature();
}

double CableStrainer::temperature_stretch() const {
  // doesn't matter which model it is grabbed from - both are similar
  return model_elongation_start_.temperature_stretch();
}

/// This function uses the the generic strain equation. For positive strain
/// transitions, the equation solves for the final length. For negative strain
/// transitions, the equation solves for the initial length. This is done so
/// that if a cable is strained positively and then negatively by the exact
/// same transition strain, the final length would be equal to the initial
/// length.
double CableStrainer::LengthAfterStrain(const double& length_before_strain,
                                        const double& strain_transition) const {

  // negative strain transition
  if (strain_transition < 0) {

    return length_before_strain / (1 + abs(strain_transition));

  // positive strain transition
  } else {

    return length_before_strain * (1 + strain_transition);

  }
}
