// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/sagging/stopwatch_sagger.h"

#include <cmath>

StopwatchSagger::StopwatchSagger() {
  catenary_ = Catenary3d();
  sag_ = -999999;
}

StopwatchSagger::~StopwatchSagger() {
}

double StopwatchSagger::TimeReturn(const int& wave) const {
  return wave * std::sqrt(sag_ / 1.00625);
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
  sag_ = catenary_.Sag();
}
