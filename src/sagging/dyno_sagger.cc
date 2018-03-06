// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/sagging/dyno_sagger.h"

DynoSagger::DynoSagger() {
  catenary_ = Catenary3d();
  location_ = DynoSagger::SpanEndLocation::kNull;
}

DynoSagger::~DynoSagger() {
}

double DynoSagger::Tension() const {
  if (location_ == DynoSagger::SpanEndLocation::kBack) {
    return catenary_.Tension(0);
  } else if (location_ == DynoSagger::SpanEndLocation::kAhead) {
    return catenary_.Tension(1);
  } else {
    return -999999;
  }
}

bool DynoSagger::Validate(const bool& is_included_warnings,
                          std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "DYNO SAGGER";

  // validates catenary
  if (catenary_.Validate(is_included_warnings, messages) == false) {
    is_valid = false;
  }

  // validates location
  if (location_ == DynoSagger::SpanEndLocation::kNull) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid dyno span end location";
      messages->push_back(message);
    }
  }

  return is_valid;
}

Catenary3d DynoSagger::catenary() const {
  return catenary_;
}

DynoSagger::SpanEndLocation DynoSagger::location() const {
  return location_;
}

void DynoSagger::set_catenary(const Catenary3d& catenary) {
  catenary_ = catenary;
}

void DynoSagger::set_location(const SpanEndLocation& location) {
  location_ = location;
}
