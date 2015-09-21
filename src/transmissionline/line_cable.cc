// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/transmissionline/line_cable.h"

LineCable::LineCable() {
  cable = nullptr;
}

LineCable::~LineCable() {
}

bool LineCable::Validate(const bool& is_included_warnings,
                         std::list<std::string>* messages_error) const {

  bool is_valid = true;

  // validates cable
  if (cable == nullptr) {
    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("LINE CABLE - Invalid cable");
    }
  } else {
    if (cable->Validate(is_included_warnings, messages_error) == false) {
      is_valid = false;
    }
  }

  // validates constraint
  if (constraint.Validate(is_included_warnings, messages_error) == false) {
    is_valid = false;
  }

  // validates spacing-attachments-rulingspan
  if (spacing_attachments_ruling_span.x() <= 0) {
    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("LINE CABLE - Invalid horizontal ruling span "
                                 "attachment spacing");
    }
  }

  if (spacing_attachments_ruling_span.y() != 0) {
    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("LINE CABLE - Invalid ruling span attachment"
                                "spacing");
    }
  }

  if (2000 < abs(spacing_attachments_ruling_span.z())) {
    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("LINE CABLE - Invalid vertical ruling span"
                                "attachment spacing");
    }
  }

  // returns validation status
  return is_valid;
}
