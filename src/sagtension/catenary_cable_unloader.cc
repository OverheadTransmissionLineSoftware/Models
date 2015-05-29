// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "sagtension/catenary_cable_unloader.h"

CatenaryCableUnloader::CatenaryCableUnloader() {
  is_updated_strainer_ = false;

  strainer_.set_load_finish(0);
}

CatenaryCableUnloader::~CatenaryCableUnloader() {
}

double CatenaryCableUnloader::LengthUnloaded() const {

  // updates class if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return strainer_.LengthFinish();
}

bool CatenaryCableUnloader::Validate(
    const bool& is_included_warnings,
    std::list<std::string>* messages_error) const {

  bool is_valid = true;

  /// \todo implement this

  return is_valid;
}

CatenaryCable CatenaryCableUnloader::catenary_cable() const {
  return catenary_cable_;
}

void CatenaryCableUnloader::set_catenary_cable(
    const CatenaryCable& catenary_cable) {

  catenary_cable_ = catenary_cable;

  is_updated_strainer_ = false;
}

void CatenaryCableUnloader::set_state_unloaded(
    const CableState& state_unloaded) {

  strainer_.set_state_finish(state_unloaded);
}

CableState CatenaryCableUnloader::state_unloaded() const {
  return strainer_.state_finish();
}

bool CatenaryCableUnloader::IsUpdated() const {

  if (is_updated_strainer_ == true) {
    return true;
  } else {
    return false;
  }
}

bool CatenaryCableUnloader::Update() const {

  // updates strainer
  if (is_updated_strainer_ == false) {

    is_updated_strainer_ = UpdateStrainer();
    if (is_updated_strainer_ == false) {
      return false;
    }
  }

  // if it reaches this point, update was successful
  return true;
}

bool CatenaryCableUnloader::UpdateStrainer() const {

  strainer_.set_cable(catenary_cable_.cable());
  strainer_.set_length_start(catenary_cable_.Length());
  strainer_.set_load_start(catenary_cable_.TensionAverage());
  strainer_.set_state_start(catenary_cable_.state());

  return true;
}
