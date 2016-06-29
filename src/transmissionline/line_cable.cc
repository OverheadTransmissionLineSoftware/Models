// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/transmissionline/line_cable.h"

#include "models/transmissionline/catenary_solver.h"

LineCable::LineCable() {
  cable = nullptr;
  weathercase_stretch_creep = nullptr;
  weathercase_stretch_load = nullptr;
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
  if (cable == nullptr) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid cable";
      messages->push_back(message);
    }
  } else {
    if (cable->Validate(is_included_warnings, messages) == false) {
      is_valid = false;
    }
  }

  // validates constraint
  if (constraint.Validate(is_included_warnings, messages) == false) {
    is_valid = false;
  }

  // validates spacing-attachments-rulingspan
  if (spacing_attachments_ruling_span.x() <= 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid horizontal ruling span attachment spacing";
      messages->push_back(message);
    }
  }

  if (spacing_attachments_ruling_span.y() != 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid transverse ruling span attachment spacing";
      messages->push_back(message);
    }
  }

  if (2000 < abs(spacing_attachments_ruling_span.z())) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid vertical ruling span attachment spacing";
      messages->push_back(message);
    }
  }

  // validates weathercase-stretch-creep
  if (weathercase_stretch_creep == nullptr) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid creep stretch weathercase";
      messages->push_back(message);
    }
  } else {
    if (weathercase_stretch_creep->Validate(is_included_warnings,
                                            messages) == false) {
      is_valid = false;
    }
  }

  // validates weathercase-stretch-load
  if (weathercase_stretch_load == nullptr) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid load stretch weathercase";
      messages->push_back(message);
    }
  } else {
    if (weathercase_stretch_load->Validate(is_included_warnings,
                                           messages) == false) {
      is_valid = false;
    }
  }

  // validates if catenary can be solved for with contraint and ruling
  // span geometry
  CatenarySolver solver;
  solver.set_cable(cable);
  solver.set_constraint(&constraint);
  solver.set_spacing_attachments(&spacing_attachments_ruling_span);
  if (solver.Validate(is_included_warnings, messages) == false) {
    is_valid = false;
  }

  // returns validation status
  return is_valid;
}
