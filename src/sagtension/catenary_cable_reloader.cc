// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/sagtension/catenary_cable_reloader.h"

#include <cmath>

#include "models/sagtension/catenary_cable_unloader.h"

CatenaryCableReloader::CatenaryCableReloader() {
  catenary_ = nullptr;
  model_reference_ = nullptr;
  model_reloaded_ = nullptr;
  weight_unit_reloaded_ = nullptr;

  is_updated_catenary_reloaded_ = false;
}

CatenaryCableReloader::~CatenaryCableReloader() {
}

Catenary3d CatenaryCableReloader::CatenaryReloaded() const {
  // updates class if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return Catenary3d();
    }
  }

  return catenary_reloaded_;
}

double CatenaryCableReloader::LengthUnloaded() const {
  // updates class if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  // creates an unloader using the reference cable models
  CatenaryCableUnloader unloader;
  unloader.set_catenary(catenary_);
  unloader.set_model_reference(model_reference_);
  unloader.set_model_unloaded(model_reference_);

  return unloader.LengthUnloaded();
}

double CatenaryCableReloader::TensionHorizontal() const {
  // updates class if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return catenary_reloaded_.tension_horizontal();
}

bool CatenaryCableReloader::Validate(
    const bool& is_included_warnings,
    std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "CATENARY CABLE RELOADER";

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

  // validates model-reference
  if (model_reference_ == nullptr) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid reference cable model";
      messages->push_back(message);
    }
  } else {
    if (model_reference_->Validate(is_included_warnings,
                                   messages) == false) {
      is_valid = false;
    }
  }

  // validates model-reloaded
  if (model_reloaded_ == nullptr) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid reloaded cable model";
      messages->push_back(message);
    }
  } else {
    if (model_reloaded_->Validate(is_included_warnings,
                                  messages) == false) {
      is_valid = false;
    }
  }

  // validates reloaded unit weight
  if (weight_unit_reloaded_ == nullptr) {


  } else {
    if (weight_unit_reloaded_->x() != 0) {
      is_valid = false;
      if (messages != nullptr) {
        message.description = "Invalid longitudinal unit weight";
        messages->push_back(message);
      }
    }

    if (weight_unit_reloaded_->y() < 0) {
      is_valid = false;
      if (messages != nullptr) {
        message.description = "Invalid transverse unit weight";
        messages->push_back(message);
      }
    }

    if (weight_unit_reloaded_->z() <= 0) {
      is_valid = false;
      if (messages != nullptr) {
        message.description = "Invalid vertical unit weight";
        messages->push_back(message);
      }
    }
  }

  // returns if errors are present
  if (is_valid == false) {
    return is_valid;
  }

  // validates update process
  if (Update() == false) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Error updating class. Could not solve for "
                            "reloaded catenary cable.";
      messages->push_back(message);
    }
  }

  return is_valid;
}

const Catenary3d* CatenaryCableReloader::catenary() const {
  return catenary_;
}

const CableElongationModel* CatenaryCableReloader::model_reference() const {
  return model_reference_;
}

const CableElongationModel* CatenaryCableReloader::model_reloaded() const {
  return model_reloaded_;
}

void CatenaryCableReloader::set_catenary(const Catenary3d* catenary) {
  catenary_ = catenary;

  is_updated_catenary_reloaded_ = false;
}

void CatenaryCableReloader::set_model_reference(
    const CableElongationModel* model_reference) {
  model_reference_ = model_reference;

  is_updated_catenary_reloaded_ = false;
}

void CatenaryCableReloader::set_model_reloaded(
    const CableElongationModel* model_reloaded) {
  model_reloaded_ = model_reloaded;

  is_updated_catenary_reloaded_ = false;
}

void CatenaryCableReloader::set_weight_unit_reloaded(
    const Vector3d* weight_unit_reloaded) {
  weight_unit_reloaded_ = weight_unit_reloaded;

  is_updated_catenary_reloaded_ = false;
}

const Vector3d* CatenaryCableReloader::weight_unit_reloaded() const {
  return weight_unit_reloaded_;
}

bool CatenaryCableReloader::InitializeReloadedCatenary() const {
  catenary_reloaded_ = Catenary3d();
  catenary_reloaded_.set_spacing_endpoints(catenary_->spacing_endpoints());
  catenary_reloaded_.set_tension_horizontal(catenary_->tension_horizontal());
  catenary_reloaded_.set_weight_unit(*weight_unit_reloaded_);

  return true;
}

bool CatenaryCableReloader::InitializeStrainer() const {
  strainer_.set_length_start(catenary_->Length());
  strainer_.set_load_start(catenary_->TensionAverage());
  strainer_.set_model_finish(model_reloaded_);
  strainer_.set_model_start(model_reference_);

  return true;
}

bool CatenaryCableReloader::IsUpdated() const {
  if (is_updated_catenary_reloaded_ == true) {
    return true;
  } else {
    return false;
  }
}

double CatenaryCableReloader::LengthDifference(
    const double& tension_horizontal) const {
  UpdateReloadedCatenaryAndStrainer(tension_horizontal);

  const double length_catenary = catenary_reloaded_.Length();
  const double length_cable = strainer_.LengthFinish();

  return length_catenary - length_cable;
}

/// This function solves for the reloaded catenary horizontal tension by
/// comparing the loaded length of the catenary and the cable strainer. The
/// reloaded catenary horizontal tension is iterated until a solution is found.
/// The lengths aren't held to a specific precision due to convergence accuracy
/// and consistency problems, so the range between horizontal tension boundary
/// points is tracked instead and is assumed solved when the range is small
/// enough.
bool CatenaryCableReloader::SolveReloadedCatenaryTension() const {
  // x = horizontal tension
  // y = length difference  i.e.(catenary length - cable length)

  InitializeReloadedCatenary();
  InitializeStrainer();

  // uses an iterative routine to determine solution
  // target y = 0, but is not tracked during iterations
  const double target_solution = 0;

  // declares and initializes left point
  // lowest acceptable value for catenary
  Point2d point_left;
  point_left.x = catenary_reloaded_.ConstantMinimum(
      catenary_reloaded_.spacing_endpoints().Magnitude())
      * catenary_reloaded_.weight_unit().Magnitude();
  point_left.y = LengthDifference(point_left.x);

  // declares and initializes right point
  // 2x value of lowest point
  Point2d point_right;
  point_right.x = 2 * point_left.x;
  point_right.y = LengthDifference(point_right.x);

  // declares and initializes current point
  Point2d point_current;

  // iterates until range between x values is small enough
  int iter = 0;
  const int iter_max = 100;
  const double precision = 0.01;
  while (precision < std::abs(point_left.x - point_right.x)
         && (iter < iter_max)) {

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
    iter++;
  }

  // returns success status
  if (iter < iter_max) {
    // does one last sanity check to see if lengths match
    if (point_current.y < 0.1) {
      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
}

bool CatenaryCableReloader::Update() const {
  // updates catenary-cable-reloaded
  if (is_updated_catenary_reloaded_ == false) {
    is_updated_catenary_reloaded_ = SolveReloadedCatenaryTension();
    if (is_updated_catenary_reloaded_ == false) {
      return false;
    }
  }

  // if it reaches this point, update was successful
  return true;
}

bool CatenaryCableReloader::UpdateReloadedCatenaryAndStrainer(
    const double& tension_horizontal) const {
  // updates the reloaded catenary and strainer
  catenary_reloaded_.set_tension_horizontal(tension_horizontal);
  strainer_.set_load_finish(catenary_reloaded_.TensionAverage());

  return true;
}
