// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/transmissionline/line_cable.h"

#include <cmath>

#include "models/transmissionline/catenary_solver.h"

LineCable::LineCable() {
  cable_ = nullptr;
  weathercase_stretch_creep_ = nullptr;
  weathercase_stretch_load_ = nullptr;
}

LineCable::~LineCable() {
}

bool LineCable::Validate(const bool& is_included_warnings,
                         std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "LINE CABLE";

  // validates cable
  if (cable_ == nullptr) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid cable";
      messages->push_back(message);
    }
  } else {
    if (cable_->Validate(is_included_warnings, messages) == false) {
      is_valid = false;
    }
  }

  // validates constraint
  if (constraint_.Validate(is_included_warnings, messages) == false) {
    is_valid = false;
  }

  // validates spacing-attachments-rulingspan
  if (spacing_attachments_ruling_span_.x() <= 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid horizontal ruling span attachment spacing";
      messages->push_back(message);
    }
  }

  if (spacing_attachments_ruling_span_.y() != 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid transverse ruling span attachment spacing";
      messages->push_back(message);
    }
  }

  if (2000 < std::abs(spacing_attachments_ruling_span_.z())) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid vertical ruling span attachment spacing";
      messages->push_back(message);
    }
  }

  // validates weathercase-stretch-creep
  if (weathercase_stretch_creep_ == nullptr) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid creep stretch weathercase";
      messages->push_back(message);
    }
  } else {
    if (weathercase_stretch_creep_->Validate(is_included_warnings,
                                             messages) == false) {
      is_valid = false;
    }
  }

  // validates weathercase-stretch-load
  if (weathercase_stretch_load_ == nullptr) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid load stretch weathercase";
      messages->push_back(message);
    }
  } else {
    if (weathercase_stretch_load_->Validate(is_included_warnings,
                                            messages) == false) {
      is_valid = false;
    }
  }

  // returns if errors are present
  if (is_valid == false) {
    return is_valid;
  }

  // validates if catenary can be solved for with contraint and ruling
  // span geometry
  CatenarySolver solver;
  solver.set_cable(cable_);
  solver.set_constraint(&constraint_);
  solver.set_spacing_attachments(&spacing_attachments_ruling_span_);
  if (solver.Validate(is_included_warnings, messages) == false) {
    is_valid = false;
  }

  // returns validation status
  return is_valid;
}

const Cable* LineCable::cable() const {
  return cable_;
}

CableConstraint LineCable::constraint() const {
  return constraint_;
}

void LineCable::set_cable(const Cable* cable) {
  cable_ = cable;
}

void LineCable::set_constraint(const CableConstraint& constraint) {
  constraint_ = constraint;
}

void LineCable::set_spacing_attachments_ruling_span(
    const Vector3d& spacing_attachments_ruling_span) {
  spacing_attachments_ruling_span_ = spacing_attachments_ruling_span;
}

void LineCable::set_weathercase_stretch_creep(
    const WeatherLoadCase* weathercase) {
  weathercase_stretch_creep_ = weathercase;
}

void LineCable::set_weathercase_stretch_load(
    const WeatherLoadCase* weathercase) {
  weathercase_stretch_load_ = weathercase;
}

const Vector3d LineCable::spacing_attachments_ruling_span() const {
  return spacing_attachments_ruling_span_;
}

const WeatherLoadCase* LineCable::weathercase_stretch_creep() const {
  return weathercase_stretch_creep_;
}

const WeatherLoadCase* LineCable::weathercase_stretch_load() const {
  return weathercase_stretch_load_;
}
