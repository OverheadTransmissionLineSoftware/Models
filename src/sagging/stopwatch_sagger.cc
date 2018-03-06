// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/sagging/stopwatch_sagger.h"

#include <cmath>

StopwatchSagger::StopwatchSagger() {
  catenary_ = Catenary3d();
  units_ = units::UnitSystem::kNull;

  length_ = -999999;
  velocity_wave_ = -999999;

  is_updated_ = false;
}

StopwatchSagger::~StopwatchSagger() {
}

double StopwatchSagger::TimeReturn(const int& wave) const {
  // updates class if necessary
  if ((IsUpdated() == false) && (Update() == false)) {
    return -999999;
  }

  return (wave * 2 * length_) / velocity_wave_;
}

bool StopwatchSagger::Validate(const bool& is_included_warnings,
                          std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "STOPWATCH SAGGER";

  // validates catenary
  if (catenary_.Validate(is_included_warnings, messages) == false) {
    is_valid = false;
  }

  return is_valid;
}

Catenary3d StopwatchSagger::catenary() const {
  return catenary_;
}

void StopwatchSagger::set_catenary(const Catenary3d& catenary) {
  catenary_ = catenary;
  is_updated_ = false;
}

void StopwatchSagger::set_units(const units::UnitSystem& units) {
  units_ = units;
  is_updated_ = false;
}

units::UnitSystem StopwatchSagger::units() const {
  return units_;
}

bool StopwatchSagger::IsUpdated() const {
  return is_updated_ == true;
}

bool StopwatchSagger::Update() const {
  // updates the length and velocity
  is_updated_ = UpdateLengthAndVelocity();
  if (is_updated_ == false) {
    return false;
  }

  // if it reaches this point, update was successful
  return true;
}

bool StopwatchSagger::UpdateLengthAndVelocity() const {
  // calculates the length
  length_ = catenary_.Length();

  // calculates the velocity of a traveling wave
  double gravity = 0;
  if (units_ == units::UnitSystem::kImperial) {
    gravity = units::kGravityImperial;
  } else if (units_ == units::UnitSystem::kMetric) {
    gravity = units::kGravityMetric;
  } else {
    return false;
  }

  velocity_wave_ = std::sqrt(gravity * catenary_.Constant());

  return true;
}
