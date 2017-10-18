// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/transmissionline/cable_unit_load_solver.h"

#include "models/base/geometric_shapes.h"

CableUnitLoadSolver::CableUnitLoadSolver() {
  diameter_cable_ = nullptr;
  weight_unit_cable_ = nullptr;
}

CableUnitLoadSolver::~CableUnitLoadSolver() {
}

bool CableUnitLoadSolver::Validate(
    const bool& is_included_warnings,
    std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "CABLE UNIT LOAD CALCULATOR";

  // validates diameter-cable
  if ((diameter_cable_ == nullptr) ||  (*diameter_cable_ <= 0)) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid cable diameter";
      messages->push_back(message);
    }
  }

  // validates weight-unit-cable
  if ((weight_unit_cable_ == nullptr) || (*weight_unit_cable_ <= 0)) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid cable unit weight";
      messages->push_back(message);
    }
  }

  return is_valid;
}

/// The vertical load on the cable is solved using the following formula:
/// \f[ V = w + \left(dv) \f]
///
/// The transverse load on the cable is solved using the following formula:
/// \f[ T = AP \f]
Vector3d CableUnitLoadSolver::UnitCableLoad(
    const WeatherLoadCase& case_load_weather) const {
  // creates bare and iced cylinders
  Cylinder cylinder_bare;
  cylinder_bare.set_diameter(*diameter_cable_);
  cylinder_bare.set_length(1);

  Cylinder cylinder_iced;
  cylinder_iced.set_diameter(*diameter_cable_
                             + (2 * case_load_weather.thickness_ice));
  cylinder_iced.set_length(1);

  const double volume_ice = cylinder_iced.Volume() - cylinder_bare.Volume();
  const double density_ice = case_load_weather.density_ice;
  const double weight_ice = volume_ice * density_ice;

  // solves for unit load
  Vector3d load_unit_cable;
  load_unit_cable.set_x(0);
  load_unit_cable.set_y(cylinder_iced.diameter() * cylinder_iced.length()
                        * case_load_weather.pressure_wind);
  load_unit_cable.set_z(*weight_unit_cable_ + weight_ice);

  return load_unit_cable;
}

const double* CableUnitLoadSolver::diameter_cable() const {
  return diameter_cable_;
}

const double* CableUnitLoadSolver::weight_unit_cable() const {
  return weight_unit_cable_;
}

void CableUnitLoadSolver::set_diameter_cable(const double* diameter_cable) {
  diameter_cable_ = diameter_cable;
}

void CableUnitLoadSolver::set_weight_unit_cable(
  const double* weight_unit_cable) {
  weight_unit_cable_ = weight_unit_cable;
}
