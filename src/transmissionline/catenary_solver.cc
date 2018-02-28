// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/transmissionline/catenary_solver.h"

#include <cmath>

CatenarySolver::CatenarySolver() {
  catenary_ = Catenary3d();
  position_target_ = -1;
  type_target_ = TargetType::kNull;
  value_target_ = -999999;

  is_updated_ = false;
}

CatenarySolver::~CatenarySolver() {
}

Catenary3d CatenarySolver::Catenary() const {
  // updates class if necessary
  if ((IsUpdated() == false) && (Update() == false)) {
    return Catenary3d();
  }

  return catenary_;
}

double CatenarySolver::TensionHorizontal() const {
  // updates class if necessary
  if ((IsUpdated() == false) && (Update() == false)) {
    return -999999;
  }

  return catenary_.tension_horizontal();
}

bool CatenarySolver::Validate(
    const bool& /*is_included_warnings*/,
    std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "CATENARY SOLVER";

  // validates position-target
  if (position_target_ == -1) {
    // indicates position isn't needed or to use maximum - no errors
  } else if ((position_target_ < 0) || (1 < position_target_)) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid target position";
      messages->push_back(message);
    }
  }

  // validates spacing-endpoints
  Vector3d spacing_endpoints = catenary_.spacing_endpoints();
  if (spacing_endpoints.x() <= 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid horizontal end point spacing";
      messages->push_back(message);
    }
  }

  if (spacing_endpoints.y() != 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid end point spacing";
      messages->push_back(message);
    }
  }

  if (2000 < std::abs(spacing_endpoints.z())) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid vertical end point spacing";
      messages->push_back(message);
    }
  }

  // validates type-target
  if (type_target_ == CatenarySolver::TargetType::kNull) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid target type";
      messages->push_back(message);
    }
  }

  // validates value-target
  if (value_target_ <= 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid target value";
      messages->push_back(message);
    }
  }

  // validates weight-unit-horizontal
  Vector3d weight_unit = catenary_.weight_unit();
  if (weight_unit.x() != 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid horizontal unit weight. It must "
                            "equal zero";
      messages->push_back(message);
    }
  }

  // validates weight-unit-transverse
  if (weight_unit.y() < 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid transverse unit weight";
      messages->push_back(message);
    }
  }

  // validates weight-unit-vertical
  if (weight_unit.z() <= 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid vertical unit weight";
      messages->push_back(message);
    }
  }

  // returns if errors are present
  if (is_valid == false) {
    return false;
  }

  // validates update process
  if (Update() == false) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Error updating class. Could not solve for "
                            "catenary";
      messages->push_back(message);
    }
  }

  // returns validation status
  return is_valid;
}

double CatenarySolver::position_target() const {
  return position_target_;
}

void CatenarySolver::set_position_target(const double& position) {
  position_target_ = position;
  is_updated_ = false;
}

void CatenarySolver::set_spacing_endpoints(const Vector3d& spacing_endpoints) {
  catenary_.set_spacing_endpoints(spacing_endpoints);
  is_updated_ = false;
}

void CatenarySolver::set_type_target(const TargetType& type) {
  type_target_ = type;
  is_updated_ = false;
}

void CatenarySolver::set_value_target(const double& value) {
  value_target_ = value;
  is_updated_ = false;
}

void CatenarySolver::set_weight_unit(const Vector3d& weight_unit) {
  catenary_.set_weight_unit(weight_unit);
  is_updated_ = false;
}

const Vector3d CatenarySolver::spacing_endpoints() const {
  return catenary_.spacing_endpoints();
}

CatenarySolver::TargetType CatenarySolver::type_target() const {
  return type_target_;
}

double CatenarySolver::value_target() const {
  return value_target_;
}

const Vector3d CatenarySolver::weight_unit() const {
  return catenary_.weight_unit();
}

bool CatenarySolver::IsUpdated() const {
  return is_updated_ == true;
}

bool CatenarySolver::SolveHorizontalTensionFromConstant() const {
  // the catenary unit weight should already be updated, so this multiplies
  // catenary constant by w to get horizontal tension
  catenary_.set_tension_horizontal(
      value_target_ * catenary_.weight_unit().Magnitude());

  return true;
}

/// This is done iteratively by adjusting the horizontal tension until the
/// length is matched.
bool CatenarySolver::SolveHorizontalTensionFromLength() const {
  // x = tension-horizontal
  // y = length

  // initializes target
  double target_solution = value_target_;

  // declares and initializes left point
  // lowest acceptable value for catenary
  Point2d<double> point_left;
  point_left.x = Catenary2d::ConstantMinimum(
      catenary_.spacing_endpoints().Magnitude())
      * catenary_.weight_unit().Magnitude();
  point_left.y = UpdateCatenaryLength(point_left.x);

  // checks if target length is less than straight line distance
  if (target_solution <= catenary_.spacing_endpoints().Magnitude()) {
    return false;
  }

  // checks if target length is greater than lowest acceptable catenary value
  if (point_left.y < target_solution) {
    return false;
  }

  // declares and initializes right point to 10,000 H/w
  Point2d<double> point_right;
  point_right.x = point_left.x * 1.10;
  point_right.y = UpdateCatenaryLength(point_right.x);

  // declares and initializes current point
  Point2d<double> point_current;

  // iterates
  unsigned int iter = 0;
  const int iter_max = 100;
  while (0.01 < std::abs(point_left.x - point_right.x) && (iter <= iter_max)) {
    // gets current point x value using left and right points
    // calculates tangent line between points, extrapolates using line
    double slope_line = (point_right.y - point_left.y)
                        / (point_right.x - point_left.x);

    point_current.x = point_left.x
                      + ((target_solution - point_left.y) / slope_line);

    // gets current point y value
    point_current.y = UpdateCatenaryLength(point_current.x);

    // updates either left or right point based on current point
    if (point_current.x < point_left.x) {
      point_right.x = point_left.x;
      point_right.y = point_left.y;
      point_left.x = point_current.x;
      point_left.y = point_current.y;

    } else if ((point_left.x < point_current.x)
        && (point_current.x < point_right.x)) {
      if (point_current.y < target_solution) {
        point_right.x = point_current.x;
        point_right.y = point_current.y;
      } else if (target_solution < point_current.y) {
        point_left.x = point_current.x;
        point_left.y = point_current.y;
      }

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
    catenary_.set_tension_horizontal(point_current.x);
    return true;
  } else {
    catenary_.set_tension_horizontal(-999999);
    return false;
  }
}

/// This is done iteratively by adjusting the horizontal tension until the
/// sag is matched.
bool CatenarySolver::SolveHorizontalTensionFromSag() const {
  // x = tension-horizontal
  // y = sag

  // initializes target
  double target_solution = value_target_;

  // declares and initializes left point
  // lowest acceptable value for catenary
  Point2d<double> point_left;
  point_left.x = Catenary2d::ConstantMinimum(
      catenary_.spacing_endpoints().Magnitude())
      * catenary_.weight_unit().Magnitude();
  point_left.y = UpdateCatenarySag(point_left.x, position_target_);

  // target sag is greater than lowest acceptable catenary value
  if (point_left.y < target_solution) {
    return false;
  }

  // declares and initializes right point to 10,000 H/w
  Point2d<double> point_right;
  point_right.x = point_left.x * 1.10;
  point_right.y = UpdateCatenarySag(point_right.x, position_target_);

  // declares and initializes current point
  Point2d<double> point_current;

  // iterates
  unsigned int iter = 0;
  const int iter_max = 100;
  while (0.01 < std::abs(point_left.x - point_right.x) && (iter <= iter_max)) {
    // gets current point x value using left and right points
    // calculates tangent line between points, extrapolates using line
    double slope_line = (point_right.y - point_left.y)
                        / (point_right.x - point_left.x);

    point_current.x = point_left.x
                      + ((target_solution - point_left.y) / slope_line);

    // gets current point y value
    point_current.y = UpdateCatenarySag(point_current.x, position_target_);

    // updates either left or right point based on current point
    if (point_current.x < point_left.x) {
      point_right.x = point_left.x;
      point_right.y = point_left.y;
      point_left.x = point_current.x;
      point_left.y = point_current.y;

    } else if ((point_left.x < point_current.x)
        && (point_current.x < point_right.x)) {
      if (point_current.y < target_solution) {
        point_right.x = point_current.x;
        point_right.y = point_current.y;
      } else if (target_solution < point_current.y) {
        point_left.x = point_current.x;
        point_left.y = point_current.y;
      }

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
    catenary_.set_tension_horizontal(point_current.x);
    return true;
  } else {
    catenary_.set_tension_horizontal(-999999);
    return false;
  }
}

/// This is done iteratively by adjusting the horizontal tension until the
/// support tension is matched.
bool CatenarySolver::SolveHorizontalTensionFromTension() const {
  // x = tension-horizontal
  // y = tension-support

  // initializes target
  double target_solution = value_target_;

  // declares and initializes left point
  // lowest acceptable value for catenary
  Point2d<double> point_left;
  point_left.x = Catenary2d::ConstantMinimum(
      catenary_.spacing_endpoints().Magnitude())
      * catenary_.weight_unit().Magnitude();
  point_left.y = UpdateCatenaryTension(point_left.x, position_target_);

  // target is less than lowest acceptable catenary value
  if (target_solution < point_left.y) {
    return false;
  }

  // declares and initializes right point
  // highest value (horizontal tension cannot exceed support tension)
  Point2d<double> point_right;
  point_right.x = value_target_;
  point_right.y = UpdateCatenaryTension(point_right.x, position_target_);

  // declares and initializes current point
  Point2d<double> point_current;

  // iterates
  unsigned int iter = 0;
  const int iter_max = 100;
  while (0.01 < std::abs(point_left.x - point_right.x) && (iter <= iter_max)) {
    // gets current point x value using left and right points
    // calculates tangent line between points, extrapolates using line
    double slope_line = (point_right.y - point_left.y)
                        / (point_right.x - point_left.x);

    point_current.x = point_left.x
                      + ((target_solution - point_left.y) / slope_line);

    // gets current point y value
    point_current.y = UpdateCatenaryTension(point_current.x, position_target_);

    // updates either left or right point based on current point
    if (point_current.x < point_left.x) {
      point_right.x = point_left.x;
      point_right.y = point_left.y;
      point_left.x = point_current.x;
      point_left.y = point_current.y;

    } else if ((point_left.x < point_current.x)
        && (point_current.x < point_right.x)) {
      if (point_current.y < target_solution) {
        point_right.x = point_current.x;
        point_right.y = point_current.y;
      } else if (target_solution < point_current.y) {
        point_left.x = point_current.x;
        point_left.y = point_current.y;
      }

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
    catenary_.set_tension_horizontal(point_current.x);
    return true;
  } else {
    catenary_.set_tension_horizontal(-999999);
    return false;
  }
}

bool CatenarySolver::Update() const {
  // updates catenary
  if (type_target_ == CatenarySolver::TargetType::kConstant) {
    // solves for horizontal tension based on constant
    is_updated_ = SolveHorizontalTensionFromConstant();
    if (is_updated_ == false) {
      return false;
    }
  } else if (type_target_ == CatenarySolver::TargetType::kLength) {
    // solves for horizontal tension based on length
    is_updated_ = SolveHorizontalTensionFromLength();
    if (is_updated_ == false) {
      return false;
    }
  } else if (type_target_ == CatenarySolver::TargetType::kSag) {
    // solves for horizontal tension based on sag
    is_updated_ = SolveHorizontalTensionFromSag();
    if (is_updated_ == false) {
      return false;
    }
  } else if (type_target_ == CatenarySolver::TargetType::kTension) {
    // solves for horizontal tension based on tension
    is_updated_ = SolveHorizontalTensionFromTension();
    if (is_updated_ == false) {
      return false;
    }
  } else {
    return false;
  }

  // validates catenary to make sure that H/w is valid
  if (catenary_.Validate(false, nullptr) == false) {
    return false;
  }

  // if it reaches this point, update was successful
  return true;
}

double CatenarySolver::UpdateCatenaryLength(
    const double& tension_horizontal) const {
  // updates catenary
  catenary_.set_tension_horizontal(tension_horizontal);

  // returns length
  return catenary_.Length();
}

double CatenarySolver::UpdateCatenarySag(
    const double& tension_horizontal,
    const double& position_fraction) const {
  // updates catenary
  catenary_.set_tension_horizontal(tension_horizontal);

  // returns sag at specified position
  if (position_target_ == -1) {
    return catenary_.Sag();
  } else {
    return catenary_.Sag(position_fraction);
  }
}

double CatenarySolver::UpdateCatenaryTension(
    const double& tension_horizontal,
    const double& position_fraction) const {
  // updates catenary
  catenary_.set_tension_horizontal(tension_horizontal);

  // returns tension at specified position
  if (position_target_ == -1) {
    return catenary_.TensionMax();
  } else {
    return catenary_.Tension(position_fraction);
  }
}
