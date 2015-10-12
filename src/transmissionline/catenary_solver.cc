// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/transmissionline/catenary_solver.h"

#include "models/transmissionline/cable_unit_load_calculator.h"

CatenarySolver::CatenarySolver() {
  cable_ = nullptr;
  constraint_ = nullptr;
  spacing_attachments_ = nullptr;

  is_updated_catenary_ = false;
}

CatenarySolver::~CatenarySolver() {
}

Catenary3d CatenarySolver::Catenary() const {
  // updates class if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return Catenary3d();
    }
  }

  return catenary_;
}

bool CatenarySolver::Validate(
    const bool& is_included_warnings,
    std::list<std::string>* messages_error) const {
  bool is_valid = true;

  // validates cable
  if (cable_ == nullptr) {
    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back(
        "CATENARY CABLE SOLVER - Invalid cable");
    }
  } else {
    if (cable_->Validate(is_included_warnings, messages_error) == false) {
      is_valid = false;
    }
  }

  // validates constraint
  if (constraint_ == nullptr) {
    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back(
          "CATENARY CABLE SOLVER - Invalid constraint");
    }
  } else {
    if (cable_->Validate(is_included_warnings, messages_error) == false) {
      is_valid = false;
    }
  }

  // validates spacing-attachments
  if (spacing_attachments_->x() <= 0) {
    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back(
          "CATENARY CABLE SOLVER - Invalid horizontal attachment spacing");
    }
  }

  if (spacing_attachments_->y() != 0) {
    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back(
          "CATENARY CABLE SOLVER - Invalid attachment spacing");
    }
  }

  if (2000 < abs(spacing_attachments_->z())) {
    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back(
          "CATENARY CABLE SOLVER - Invalid vertical attachment spacing");
    }
  }

  // validates update process
  if (is_valid == true) {
    if (Update() == false) {
      is_valid = false;
      if (messages_error != nullptr) {
        messages_error->push_back("LINE CABLE TO CATENARY CONVERTER - Error "
                                  "updating class");
      }
    }
  }

  // returns validation status
  return is_valid;
}

const Cable* CatenarySolver::cable() const {
  return cable_;
}

const CableConstraint* CatenarySolver::constraint() const {
  return constraint_;
}

void CatenarySolver::set_cable(const Cable* cable) {
  cable_ = cable;
  is_updated_catenary_ = false;
}

void CatenarySolver::set_constraint(const CableConstraint* constraint) {
  constraint_ = constraint;
  is_updated_catenary_ = false;
}

void CatenarySolver::set_spacing_attachments(
    const Vector3d* spacing_attachments) {
  spacing_attachments_ = spacing_attachments;
  is_updated_catenary_ = false;
}

const Vector3d* CatenarySolver::spacing_attachments() const {
  return spacing_attachments_;
}

bool CatenarySolver::IsUpdated() const {
  if (is_updated_catenary_ == true) {
    return true;
  } else {
    return false;
  }
}

bool CatenarySolver::SolveHorizontalTensionFromConstant() const {
  // the catenary unit weight should already be updated, so this multiplies
  // catenary constant by w to get horizontal tension
  catenary_.set_tension_horizontal(constraint_->limit
                                   * catenary_.weight_unit().Magnitude());

  return true;
}

/// This is done iteratively by adjusting the horizontal tension until the
/// support tension is matched.
bool CatenarySolver::SolveHorizontalTensionFromSupportTension() const {
  // x = tension-horizontal
  // y = tension-support

  // initializes target
  double target_solution = constraint_->limit;

  // declares and initializes left point
  // lowest acceptable value for catenary
  Point2d point_left;
  point_left.x = 0.5 * catenary_.weight_unit().Magnitude()
                     * catenary_.spacing_endpoints().Magnitude();
  point_left.y = UpdateCatenaryMaxTension(point_left.x);

  // target is less than lowest acceptable catenary value
  if (target_solution < point_left.y) {
    return false;
  }

  // declares and initializes right point
  // highest value (horizontal tension cannot exceed support tension)
  Point2d point_right;
  point_right.x = constraint_->limit;
  point_right.y = UpdateCatenaryMaxTension(point_right.x);

  // declares and initializes current point
  Point2d point_current;

  // iterates
  unsigned int iter = 0;
  const int iter_max = 100;
  while (0.5 < abs(point_left.x - point_right.x) && (iter <= iter_max)) {

    // gets current point x value using left and right points
    // calculates tangent line between points, extrapolates using line
    double slope_line = (point_right.y - point_left.y)
                        / (point_right.x - point_left.x);

    point_current.x = point_left.x
                      + ((target_solution - point_left.y) / slope_line);

    // gets current point y value
    point_current.y = UpdateCatenaryMaxTension(point_current.x);

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

bool CatenarySolver::SolveWeightUnit() const {
  // creates a calculator based on the line cable
  CableUnitLoadCalculator calculator;
  calculator.set_diameter_cable(&cable_->diameter);
  calculator.set_weight_unit_cable(&cable_->weight_unit);

  // calculates the unit load and updates catenary
  Vector3d load_unit = calculator.UnitCableLoad(*constraint_->case_weather);
  catenary_.set_weight_unit(load_unit);

  return true;
}

bool CatenarySolver::Update() const {
  // updates catenary
  if (is_updated_catenary_ == false) {

    // updates spacing
    catenary_.set_spacing_endpoints(*spacing_attachments_);

    // solves for the unit weight
    is_updated_catenary_ = SolveWeightUnit();
    if (is_updated_catenary_ == false) {
      return false;
    }

    // solves for the horizontal tension
    if (constraint_->type_limit
        == CableConstraint::LimitType::kHorizontalTension) {

      catenary_.set_tension_horizontal(constraint_->limit);

    } else if (constraint_->type_limit
               == CableConstraint::LimitType::kCatenaryConstant) {

      is_updated_catenary_ = SolveHorizontalTensionFromConstant();
      if (is_updated_catenary_ == false) {
        return false;
      }

    } else if (constraint_->type_limit
               == CableConstraint::LimitType::kSupportTension) {

      is_updated_catenary_ = SolveHorizontalTensionFromSupportTension();
      if (is_updated_catenary_ == false) {
        return false;
      }

    } else {
      return false;
    }
  }

  // if it reaches this point, update was successful
  return true;
}

double CatenarySolver::UpdateCatenaryMaxTension(
    const double& tension_horizontal) const {
  // updates catenary
  catenary_.set_tension_horizontal(tension_horizontal);

  // returns support tension
  return catenary_.TensionMax();
}
