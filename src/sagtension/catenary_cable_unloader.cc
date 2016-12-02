// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/sagtension/catenary_cable_unloader.h"

CatenaryCableUnloader::CatenaryCableUnloader() {
  catenary_ = nullptr;

  strainer_.set_load_finish(0);
}

CatenaryCableUnloader::~CatenaryCableUnloader() {
}

double CatenaryCableUnloader::LengthUnloaded() const {
  return strainer_.LengthFinish();
}

bool CatenaryCableUnloader::Validate(
    const bool& is_included_warnings,
    std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "CATENARY CABLE UNLOADER";

  // validates catenary
  if (catenary_ == nullptr) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid catenary";
      messages->push_back(message);
    }
  } else {
    if (catenary_->Validate(is_included_warnings,
                            messages) == false) {
      is_valid = false;
    }
  }

  // validates strainer
  if (strainer_.Validate(is_included_warnings,
                         messages) == false) {
    is_valid = false;
  }

  return is_valid;
}

const Catenary3d* CatenaryCableUnloader::catenary() const {
  return catenary_;
}

const CableElongationModel* CatenaryCableUnloader::model_reference() const {
  return strainer_.model_start();
}

const CableElongationModel* CatenaryCableUnloader::model_unloaded() const {
  return strainer_.model_finish();
}

void CatenaryCableUnloader::set_catenary(const Catenary3d* catenary) {
  catenary_ = catenary;

  strainer_.set_length_start(catenary_->Length());
  strainer_.set_load_start(catenary_->TensionAverage());
}

void CatenaryCableUnloader::set_model_reference(
    const CableElongationModel* model_reference) {
  strainer_.set_model_start(model_reference);
}

void CatenaryCableUnloader::set_model_unloaded(
    const CableElongationModel* model_unloaded) {
  strainer_.set_model_finish(model_unloaded);
}