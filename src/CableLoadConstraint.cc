// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "include/CableLoadConstraint.h"

CableLoadConstraint::CableLoadConstraint() {
  limit = -999999;
}

CableLoadConstraint::~CableLoadConstraint() {}

bool CableLoadConstraint::Validate(
    const bool& is_included_warnings,
    std::list<std::string>* messages_error) const {
  
  bool is_valid = true;

  // validate case-load
  if (case_load.Validate(is_included_warnings, messages_error) == false) {
    is_valid = false;
  }

  // validate limit
  if (limit < 0
      || ((limit < 100) && (is_included_warnings == true))
      || ((100000 < limit) && (is_included_warnings == true))) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CABLE LOAD CONSTRAINT - Invalid limit");
    }
  }

  return is_valid;
}
