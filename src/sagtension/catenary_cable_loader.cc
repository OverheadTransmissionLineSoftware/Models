// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "sagtension/catenary_cable_loader.h"

CatenaryCableLoader::CatenaryCableLoader() {
  is_updated_catenary_ = false;
  load_start_ = 0;
}

CatenaryCableLoader::~CatenaryCableLoader() {
}

double CatenaryCableLoader::LengthFinish() const {

  // update class, if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return CableStrainer::LengthFinish();
}

double CatenaryCableLoader::LoadFinishCore() const {

  // update class, if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return CableStrainer::LoadFinishCore();
}

double CatenaryCableLoader::LoadFinishShell() const {

  // update class, if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return CableStrainer::LoadFinishShell();
}

double CatenaryCableLoader::LoadStartCore() const {

  // update class, if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return CableStrainer::LoadStartCore();
}

double CatenaryCableLoader::LoadStartShell() const {

  // update class, if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return CableStrainer::LoadStartShell();
}

double CatenaryCableLoader::StrainTransitionLoad() const {

  // update class, if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return CableStrainer::StrainTransitionLoad();
}

double CatenaryCableLoader::StrainTransitionThermal() const {

  // update class, if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return CableStrainer::StrainTransitionThermal();
}

void CatenaryCableLoader::set_spacing_endpoints_catenary(
    const Vector3d& spacing_endpoints) {

  catenary_.set_spacing_endpoints(spacing_endpoints);

  is_updated_catenary_ = false;
}

void CatenaryCableLoader::set_weight_unit_catenary(
    const Vector3d& weight_unit) {

  catenary_.set_weight_unit(weight_unit);

  is_updated_catenary_ = false;
}

Vector3d CatenaryCableLoader::spacing_endpoints_catenary() const {
  return catenary_.spacing_endpoints();
}

Vector3d CatenaryCableLoader::weight_unit_catenary() const {
  return catenary_.weight_unit();
}

bool CatenaryCableLoader::IsUpdated() const {

  if (is_updated_catenary_ == true) {
    return true;
  } else {
    return false;
  }
}

double CatenaryCableLoader::LengthDifference(
    const double& tension_horizontal) const {

  // update the catenary and cable strainer load
  catenary_.set_tension_horizontal(tension_horizontal);
  load_finish_ = catenary_.TensionAverage();

  // catenary length
  const double length_catenary = catenary_.Length();

  // cable length
  const double length_cable = CableStrainer::LengthFinish();

  return length_catenary - length_cable;
}

bool CatenaryCableLoader::Update() const {

  // update catenary
  if (is_updated_catenary_ == false) {

    is_updated_catenary_ = UpdateTensionHorizontal();
    if (is_updated_catenary_ == false) {
      return false;
    }
  }

  // if it reaches this point, update was successful
  return true;
}

bool CatenaryCableLoader::UpdateTensionHorizontal() const {

  // x = horizontal tension
  // y = length difference  i.e.(catenary length - cable length)

  // iterative routine to determine solution
  // solution reached when y = 0
  const double target_solution = 0;

  // declare and initialize left point
  // lowest acceptable value for catenary
  Point2d point_left;
  point_left.x = catenary_.ConstantMinimum(
      catenary_.spacing_endpoints().Magnitude());
  point_left.y = LengthDifference(point_left.x);

  // declare and initialize right point
  // 2x value of lowest point
  Point2d point_right;
  point_right.x = 2 * point_left.x;
  point_right.y = LengthDifference(point_right.x);

  // declare and initialize current point
  Point2d point_current;

  // iteration
  int iter = 0;
  const int iter_max = 100;
  while (0.01 < abs(point_left.x - point_right.x) && (iter < iter_max)) {

    // current point horizontal tension using left and right points
    if ((point_left.y > target_solution)
        && (target_solution > point_right.y)) {

      // average of left and right points
      point_current.x = (point_left.x + point_right.x) / 2;

    } else {

      // calculate tangent line between points, extrapolate using line
      double slope_line = (point_right.y - point_left.y)
          / (point_right.x - point_left.x);

      point_current.x = point_left.x + ((target_solution - point_left.y)
          / slope_line);

    }

    // current point length difference
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

  // return success status and cache result
  if (iter < iter_max) {
    return true;
  } else {
    return false;
  }
}
