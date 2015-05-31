// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "sagtension/catenary_cable_reloader.h"

#include "sagtension/catenary_cable_unloader.h"

CatenaryCableReloader::CatenaryCableReloader() {
  is_updated_catenary_cable_reloaded_ = false;
  is_updated_length_unloaded_unstretched_ = false;

  length_unloaded_unstretched_ = -999999;

  state_unloaded_.load_stretch = 0;
  state_unloaded_.temperature = 32;
  state_unloaded_.temperature_stretch = 0;

  strainer_.set_load_start(0);
  strainer_.set_state_start(state_unloaded_);
}

CatenaryCableReloader::~CatenaryCableReloader() {
}

CatenaryCable CatenaryCableReloader::CatenaryCableReloaded() const {

  // updates class if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return CatenaryCable();
    }
  }

  return catenary_cable_reloaded_;
}

double CatenaryCableReloader::LengthUnloadedUnstretched() const {

  // updates class if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return length_unloaded_unstretched_;
}

double CatenaryCableReloader::TensionHorizontal() const {

  // updates class if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return catenary_cable_reloaded_.tension_horizontal();
}

bool CatenaryCableReloader::Validate(
    const bool& is_included_warnings,
    std::list<std::string>* messages_error) const {

  bool is_valid = true;

  /// \todo implement this

  return is_valid;
}

CatenaryCable CatenaryCableReloader::catenary_cable() const {
  return catenary_cable_;
}

void CatenaryCableReloader::set_catenary_cable(
    const CatenaryCable& catenary_cable) {

  catenary_cable_ = catenary_cable;

  is_updated_length_unloaded_unstretched_ = false;
  is_updated_catenary_cable_reloaded_ = false;
}

void CatenaryCableReloader::set_state_reloaded(
    const CableState& state_reloaded) {

  state_reloaded_ = state_reloaded;

  is_updated_catenary_cable_reloaded_ = false;
}

void CatenaryCableReloader::set_weight_unit_reloaded(
    const Vector3d& weight_unit_reloaded) {

  weight_unit_reloaded_ = weight_unit_reloaded;

  is_updated_catenary_cable_reloaded_ = false;
}

CableState CatenaryCableReloader::state_reloaded() const {
  return state_reloaded_;
}

Vector3d CatenaryCableReloader::weight_unit_reloaded() const {
  return weight_unit_reloaded_;
}

bool CatenaryCableReloader::InitializeReloadedCatenaryCable() const {

  catenary_cable_reloaded_ = catenary_cable_;
  catenary_cable_reloaded_.set_state(state_reloaded_);
  catenary_cable_reloaded_.set_weight_unit(weight_unit_reloaded_);

  return true;
}

bool CatenaryCableReloader::InitializeStrainer() const {

  strainer_.set_cable(catenary_cable_.cable());
  strainer_.set_length_start(length_unloaded_unstretched_);
  strainer_.set_load_start(0);
  strainer_.set_state_finish(state_reloaded_);

  return true;
}

bool CatenaryCableReloader::IsUpdated() const {

  if (is_updated_catenary_cable_reloaded_ == true) {
    return true;
  } else {
    return false;
  }
}

double CatenaryCableReloader::LengthDifference(
    const double& tension_horizontal) const {

  UpdatedReloadedCatenaryCableAndStrainer(tension_horizontal);

  const double length_catenary = catenary_cable_reloaded_.Length();
  const double length_cable = strainer_.LengthFinish();

  return length_catenary - length_cable;
}

/// This function solves for the loaded length of the catenary-cable by
/// iterating the catenary horizontal tension and strainer average tension
/// until the loaded length of both is within tolerance.
bool CatenaryCableReloader::SolveReloadedCatenaryCableTension() const {

  // x = horizontal tension
  // y = length difference  i.e.(catenary length - cable length)

  InitializeReloadedCatenaryCable();
  InitializeStrainer();

  // uses an iterative routine to determine solution
  // solution is reached when y = 0
  const double target_solution = 0;

  // declares and initializes left point
  // lowest acceptable value for catenary
  Point2d point_left;
  point_left.x = catenary_cable_reloaded_.ConstantMinimum(
      catenary_cable_reloaded_.spacing_endpoints().Magnitude());
  point_left.y = LengthDifference(point_left.x);

  // declares and initializes right point
  // 2x value of lowest point
  Point2d point_right;
  point_right.x = 2 * point_left.x;
  point_right.y = LengthDifference(point_right.x);

  // declares and initializes current point
  Point2d point_current;

  // iterates until target is reached
  int iter = 0;
  const int iter_max = 100;
  while (0.01 < abs(point_left.x - point_right.x) && (iter < iter_max)) {

    // gets current point horizontal tension using left and right points
    if ((point_left.y > target_solution)
        && (target_solution > point_right.y)) {

      // gets average of left and right points
      point_current.x = (point_left.x + point_right.x) / 2;

    } else {

      // calculates tangent line between points, extrapolate using line
      double slope_line = (point_right.y - point_left.y)
          / (point_right.x - point_left.x);

      point_current.x = point_left.x + ((target_solution - point_left.y)
          / slope_line);
    }

    // gets current point length difference
    point_current.y = LengthDifference(point_current.x);

    // current point is left of left/right points
    if (point_current.x < point_left.x) {

      point_right.x = point_left.x;
      point_right.y = point_left.y;

      point_left.x = point_current.x;
      point_left.y = point_current.y;

    // current point is between left/right points
    } else if ((point_left.x < point_current.x) &&
        (point_current.x < point_right.x)) {

      if (point_current.y < target_solution) {
        point_right.x = point_current.x;
        point_right.y = point_current.y;
      } else if (target_solution < point_current.y) {
        point_left.x = point_current.x;
        point_left.y = point_current.y;
      }

    // current point is right of left/right points
    } else if (point_right.x < point_current.x) {

      point_left.x = point_right.x;
      point_left.y = point_right.y;

      point_right.x = point_current.x;
      point_right.y = point_current.y;

    }
  }

  // returns success status
  if (iter < iter_max) {
    return true;
  } else {
    return false;
  }
}

bool CatenaryCableReloader::Update() const {

  // updates length-unloaded-unstretch
  if (is_updated_length_unloaded_unstretched_ == false) {

    is_updated_length_unloaded_unstretched_ = UpdateLengthUnloadedUnstretched();
    if (is_updated_length_unloaded_unstretched_ == false) {
      return false;
    }
  }

  // updates catenary-cable-reloaded
  if (is_updated_catenary_cable_reloaded_ == false) {

    is_updated_catenary_cable_reloaded_ = SolveReloadedCatenaryCableTension();
    if (is_updated_catenary_cable_reloaded_ == false) {
      return false;
    }
  }

  // if it reaches this point, update was successful
  return true;
}

bool CatenaryCableReloader::UpdateLengthUnloadedUnstretched() const {

  length_unloaded_unstretched_ = -999999;

  // builds unloader that solves for unloaded cable length
  CatenaryCableUnloader unloader;
  unloader.set_catenary_cable(catenary_cable_);
  unloader.set_state_unloaded(state_unloaded_);

  // returns success status
  if (unloader.Validate(false, nullptr) == false) {
    return false;
  } else {
    length_unloaded_unstretched_ = unloader.LengthUnloaded();
    return true;
  }
}

bool CatenaryCableReloader::UpdatedReloadedCatenaryCableAndStrainer(
    const double& tension_horizontal) const {

  // updates the reloaded catenary-cable and strainer
  catenary_cable_reloaded_.set_tension_horizontal(tension_horizontal);
  strainer_.set_load_finish(catenary_cable_reloaded_.TensionAverage());

  return true;
}
