// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "sagtension/catenary_cable_unloader.h"

CatenaryCableUnloader::CatenaryCableUnloader() {
  is_updated_load_ = false;
}

CatenaryCableUnloader::~CatenaryCableUnloader() {
}

double CatenaryCableUnloader::LengthLoaded() const {

  // update class, if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return strainer_cable_.length_start();
}

double CatenaryCableUnloader::LengthUnloaded() const {

  // update class, if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return strainer_cable_.LengthFinish();
}

double CatenaryCableUnloader::StrainTransitionLoad() const {

  // update class, if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return strainer_cable_.StrainTransitionLoad();
}

double CatenaryCableUnloader::StrainTransitionThermal() const {

  // update class, if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return strainer_cable_.StrainTransitionThermal();
}

double CatenaryCableUnloader::load_stretch() const {
  return strainer_cable_.load_stretch();
}

void CatenaryCableUnloader::set_cable(const Cable& cable) {

  strainer_cable_.set_cable(cable);

  is_updated_load_ = false;
}

void CatenaryCableUnloader::set_load_stretch(const double& load_stretch) {

  strainer_cable_.set_load_stretch(load_stretch);

  is_updated_load_ = false;
}

void CatenaryCableUnloader::set_spacing_endpoints_catenary(
    const Vector3d& spacing_endpoints) {

  catenary_.set_spacing_endpoints(spacing_endpoints);

  is_updated_load_ = false;
}

void CatenaryCableUnloader::set_state_loaded(
    const CableStrainerState& state_loaded) {

  strainer_cable_.set_state_start(state_loaded);

  is_updated_load_ = false;
}

void CatenaryCableUnloader::set_state_unloaded(
    const CableStrainerState& state_unloaded) {

  strainer_cable_.set_state_finish(state_unloaded);

  is_updated_load_  = false;
}

void CatenaryCableUnloader::set_temperature_stretch(
    const double& temperature_stretch) {

  strainer_cable_.set_temperature_stretch(temperature_stretch);

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

CableStrainerState CatenaryCableUnloader::state_loaded() const {
  return strainer_cable_.state_start();
}

CableStrainerState CatenaryCableUnloader::state_unloaded() const {
  return strainer_cable_.state_finish();
}

double CatenaryCableUnloader::temperature_stretch() const {
  return strainer_cable_.temperature_stretch();
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
  strainer_cable_.set_load_start(catenary_.Length());
  strainer_cable_.set_load_start(catenary_.TensionAverage());

  return true;
}
