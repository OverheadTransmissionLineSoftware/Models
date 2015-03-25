// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "sagtension/catenary_cable_unloader.h"

CatenaryCableUnloader::CatenaryCableUnloader() {
  is_updated_load_ = false;
  load_finish_ = 0;
}

CatenaryCableUnloader::~CatenaryCableUnloader() {
}

double CatenaryCableUnloader::LengthFinish() const {

  // update class, if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return CableStrainer::LengthFinish();
}

double CatenaryCableUnloader::LoadFinishCore() const {

  // update class, if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return CableStrainer::LoadFinishCore();
}

double CatenaryCableUnloader::LoadFinishShell() const {

  // update class, if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return CableStrainer::LoadFinishShell();
}

double CatenaryCableUnloader::LoadStartCore() const {

  // update class, if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return CableStrainer::LoadStartCore();
}

double CatenaryCableUnloader::LoadStartShell() const {

  // update class, if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return CableStrainer::LoadStartShell();
}

double CatenaryCableUnloader::StrainTransitionLoad() const {

  // update class, if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return CableStrainer::StrainTransitionLoad();
}

double CatenaryCableUnloader::StrainTransitionThermal() const {

  // update class, if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return CableStrainer::StrainTransitionThermal();
}

void CatenaryCableUnloader::set_spacing_endpoints_catenary(
    const Vector3d& spacing_endpoints) {

  catenary_.set_spacing_endpoints(spacing_endpoints);

  is_updated_load_ = false;
}

void CatenaryCableUnloader::set_tension_horizontal_catenary(
    const double& tension_horizontal) {

  catenary_.set_tension_horizontal(tension_horizontal);

  is_updated_load_ = false;
}

void CatenaryCableUnloader::set_weight_unit_catenary(
    const Vector3d& weight_unit) {

  catenary_.set_weight_unit(weight_unit);

  is_updated_load_ = false;
}

Vector3d CatenaryCableUnloader::spacing_endpoints_catenary() const {
  return catenary_.spacing_endpoints();
}

double CatenaryCableUnloader::tension_horizontal() const {
  return catenary_.tension_horizontal();
}

Vector3d CatenaryCableUnloader::weight_unit_catenary() const {
  return catenary_.weight_unit();
}

bool CatenaryCableUnloader::IsUpdated() const {

  if (is_updated_load_ == true) {
    return true;
  } else {
    return false;
  }
}

bool CatenaryCableUnloader::Update() const {

  // update catenary
  if (is_updated_load_ == false) {

    is_updated_load_ = UpdateLoadEffective();
    if (is_updated_load_ == false) {
      return false;
    }
  }

  // if it reaches this point, update was successful
  return true;
}

bool CatenaryCableUnloader::UpdateLoadEffective() const {

  // update unload cable parameters
  length_start_ = catenary_.Length();
  load_start_ = catenary_.TensionAverage();

  return true;
}
