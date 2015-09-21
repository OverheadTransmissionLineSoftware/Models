// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/transmissionline/line_cable_to_catenary_converter.h"

#include "models/transmissionline/cable_unit_load_calculator.h"

LineCableToCatenaryConverter::LineCableToCatenaryConverter() {
  is_updated_catenary_ = false;
  line_cable_ = nullptr;
}

LineCableToCatenaryConverter::~LineCableToCatenaryConverter() {
}

Catenary3d LineCableToCatenaryConverter::Catenary() const {

  // updates class if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return Catenary3d();
    }
  }

  return catenary_;
}

bool LineCableToCatenaryConverter::Validate(
    const bool& is_included_warnings,
    std::list<std::string>* messages_error) const {

  bool is_valid = true;

  // validates line cable
  if (line_cable_ == nullptr) {
    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("LINE CABLE TO CATENARY CONVERTER - Invalid "
                                "line cable");
    }
  } else {
    if (line_cable_->Validate(is_included_warnings, messages_error) == false) {
      is_valid = false;
    }
  }

  // returns validation status
  return is_valid;
}

const LineCable* LineCableToCatenaryConverter::line_cable() const {
  return line_cable_;
}

void LineCableToCatenaryConverter::set_line_cable(const LineCable* line_cable) {

  line_cable_ = line_cable;

  is_updated_catenary_ = false;
}

bool LineCableToCatenaryConverter::IsUpdated() const {

  if (is_updated_catenary_ == true) {
    return true;
  } else {
    return false;
  }
}

bool LineCableToCatenaryConverter::SolveHorizontalTensionFromConstant() const {

  // the catenary unit weight should already be updated, so this multiplies
  // catenary constant by w to get horizontal tension
  catenary_.set_tension_horizontal(
      line_cable_->constraint.limit * catenary_.weight_unit().Magnitude());

  return true;
}

/// This is done iteratively by adjusting the horizontal tension until the
/// support tension is matched.
bool LineCableToCatenaryConverter::SolveHorizontalTensionFromSupportTension()
    const {

  // x = tension-horizontal
  // y = tension-support

  // initializes target
  double target_solution = line_cable_->constraint.limit;

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
  point_right.x = line_cable_->constraint.limit;
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

bool LineCableToCatenaryConverter::SolveWeightUnit() const {

  // creates a calculator based on the line cable
  CableUnitLoadCalculator calculator;
  calculator.set_diameter_cable(&line_cable_->cable->diameter);
  calculator.set_weight_unit_cable(&line_cable_->cable->weight_unit);

  // calculates the unit load and updates catenary
  Vector3d load_unit = calculator.UnitCableLoad(
      *line_cable_->constraint.case_weather);
  catenary_.set_weight_unit(load_unit);

  return true;
}

bool LineCableToCatenaryConverter::Update() const {

  // updates catenary
  if (is_updated_catenary_ == false) {

    // updates spacing
    catenary_.set_spacing_endpoints(
        line_cable_->spacing_attachments_ruling_span);

    // solves for the unit weight
    is_updated_catenary_ = SolveWeightUnit();
    if (is_updated_catenary_ == false) {
      return false;
    }

    // solves for the horizontal tension
    if (line_cable_->constraint.type_limit
        == CableConstraint::LimitType::kHorizontalTension) {

      catenary_.set_tension_horizontal(line_cable_->constraint.limit);

    } else if (line_cable_->constraint.type_limit
               == CableConstraint::LimitType::kCatenaryConstant) {

      is_updated_catenary_ = SolveHorizontalTensionFromConstant();
      if (is_updated_catenary_ == false) {
        return false;
      }

    } else if (line_cable_->constraint.type_limit
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

double LineCableToCatenaryConverter::UpdateCatenaryMaxTension(
    const double& tension_horizontal) const {

  // updates catenary
  catenary_.set_tension_horizontal(tension_horizontal);

  // returns support tension
  return catenary_.TensionMax();
}
