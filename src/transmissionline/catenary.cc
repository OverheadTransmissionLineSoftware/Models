// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "transmissionline/catenary.h"

#include <cmath>

#include "base/units.h"

Catenary2d::Catenary2d() {
  tension_horizontal_ = -999999;
  weight_unit_ = -999999;

  is_updated_points_end_ = true;
}

Catenary2d::~Catenary2d() {
}

/// \f[ \frac{H}{w} = \frac{HorizontalTension}{UnitWeight} \f]
double Catenary2d::Constant() const {

  return tension_horizontal_ / weight_unit_;
}

double Catenary2d::ConstantMinimum(const double& spacing_endpoints) {

  return spacing_endpoints / 2;
}

/// Solves for length and direction from origin, and then converts to
/// coordinates.
Point2d Catenary2d::Coordinate(const double& position_fraction,
                               const bool& is_shifted_origin) const {
  Point2d coordinate;

  if (IsUpdated() == false) {
    if (Update() == false) {
      return coordinate;
    }
  }

  // calculates length from left end to position
  double length_left_to_position = position_fraction * Length();

  // calculates length from origin to left endpoint
  const double length_origin_to_left = LengthFromOrigin(point_end_left_);

  // solves for length from origin and direction from origin
  double length_origin_to_position;
  AxisDirectionType direction_origin_to_position;

  // select how to calculate length and position from origin based on
  // endpoint coordinates

  // both endpoints are left from origin
  if (point_end_left_.x < 0 && point_end_right_.x < 0) {
    length_origin_to_position = length_origin_to_left -
                                length_left_to_position;
    direction_origin_to_position = AxisDirectionType::kPositive;
  }
  // one endpoint left from origin, one endpoint right from origin
  else if (point_end_left_.x < 0 && 0 < point_end_right_.x) {

    // left from origin
    if (length_left_to_position < length_origin_to_left) {
      length_origin_to_position = length_origin_to_left -
                                  length_left_to_position;
      direction_origin_to_position = AxisDirectionType::kNegative;
    }
    // at origin
    else if (length_left_to_position == length_origin_to_left) {
      length_origin_to_position = 0;
      direction_origin_to_position = AxisDirectionType::kPositive;
    }
    // right from origin
    else if (length_origin_to_left < length_left_to_position) {
      length_origin_to_position = length_left_to_position -
                                  length_origin_to_left;
      direction_origin_to_position = AxisDirectionType::kPositive;
    }
  }
  // both endpoints are AOL of origin
  else if (0 < point_end_left_.x && 0 < point_end_right_.x) {
    length_origin_to_position = length_origin_to_left +
                                length_left_to_position;
    direction_origin_to_position = AxisDirectionType::kPositive;
  }

  // solves for catenary coordinate
  coordinate.x = CoordinateX(length_origin_to_position,
                             direction_origin_to_position);
  coordinate.y = CoordinateY(length_origin_to_position,
                             direction_origin_to_position);

  return coordinate;
}

Point2d Catenary2d::CoordinateChord(const double& position_fraction,
                                    const bool& is_shifted_origin) const {
  Point2d coordinate_chord;

  if (IsUpdated() == false) {
    if (Update() == false) {
      return coordinate_chord;
    }
  }

  // gets a catenary coordinate
  Point2d coordinate_catenary = Coordinate(position_fraction,
                                is_shifted_origin);

  // calculates a chord coordinate
  coordinate_chord.x = coordinate_catenary.x;
  coordinate_chord.y = point_end_left_.y
                       + ((coordinate_catenary.x - point_end_left_.x)
                          * (spacing_endpoints_.y() / spacing_endpoints_.x()));

  // checks if shifted coordinate is requested, modifies if necessary
  if (is_shifted_origin == true) {
    coordinate_chord.x = coordinate_chord.x - point_end_left_.x;
    coordinate_chord.y = coordinate_chord.y
                         - point_end_left_.y;
  }

  return coordinate_chord;
}

/// If the lowpoint is within the curve section being used, the length from the
/// origin to the left endpoint and the length from the origin to the right
/// endpoint are summed. If the origin is beyond an endpoint, the length to the
/// closest endpoint is treated as negative.
/// \see Catenary2d::LengthFromOrigin
double Catenary2d::Length() const {

  double length = -999999;

  if (IsUpdated() == false) {
    if (Update() == false) {
      return length;
    }
  }

  // length from origin to left end
  double length_origin_to_left = -999999;
  if (point_end_left_.x < 0) {
    length_origin_to_left = LengthFromOrigin(point_end_left_);
  } else if (0 <= point_end_left_.x) {
    length_origin_to_left = -LengthFromOrigin(point_end_left_);
  }

  // length from origin to right end
  double length_origin_to_right = -999999;
  if (point_end_right_.x < 0) {
    length_origin_to_right = -LengthFromOrigin(point_end_right_);
  } else if (0 <= point_end_right_.x) {
    length_origin_to_right = LengthFromOrigin(point_end_right_);
  }

  length = length_origin_to_left + length_origin_to_right;
  return length;
}

double Catenary2d::LengthSlack() const {

  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return Length() - spacing_endpoints().Magnitude();
}

double Catenary2d::PositionFractionOrigin() const {

  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  // gets position along curve of low point, where tangent angle is zero
  return PositionFraction(0);
}

double Catenary2d::PositionFractionSagPoint() const {

  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  double tangent_angle = spacing_endpoints_.Angle(true);

  // gets position fraction of chord tangent line
  return PositionFraction(tangent_angle);
}

double Catenary2d::Sag() const {

  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  // gets sag position
  const double position_fraction_sagpoint = PositionFractionSagPoint();

  // gets catenary coordinate at sag position
  const Point2d coordinate_catenary = Coordinate(position_fraction_sagpoint);

  // gets a chord coordinate at sag position
  const Point2d coordinate_chord = CoordinateChord(position_fraction_sagpoint);

  // gets height difference between chord and catenary
  return coordinate_chord.y - coordinate_catenary.y;
}

double Catenary2d::TangentAngle(const double& position_fraction,
                                const AxisDirectionType& direction) const {
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  // gets coordinate at position
  const Point2d coordinate = Coordinate(position_fraction);

  // calculates slope at position
  const double slope = sinh(coordinate.x / (tension_horizontal_/weight_unit_));

  // converts to degrees
  double tangent_angle = units::Convert(
      atan(slope),
      units::ConversionType::kRadiansToDegrees);

  // adjusts if direction is negative
  if (direction == AxisDirectionType::kNegative) {
    tangent_angle = tangent_angle * -1;
  }

  return tangent_angle;
}

/// The slope at the position is calculated, and then adjusted to a unit vector
/// based on the direction the vector is pointing.
/// Slope equation is:
/// \f[ Slope = sinh \left( \frac{x}{\frac{H}{w}} \right) \f]
Vector2d Catenary2d::TangentVector(const double& position_fraction,
                                   const AxisDirectionType& direction) const {
  Vector2d tangent_vector;

  if (IsUpdated() == false) {
    if (Update() == false) {
      return tangent_vector;
    }
  }

  // gets tangent angle
  const double angle_tangent = TangentAngle(position_fraction, direction);

  // resolves to a unit vector
  const double angle_radians = units::Convert(
      angle_tangent,
      units::ConversionType::kDegreesToRadians);
  if (direction == AxisDirectionType::kNegative) {
    tangent_vector.set_x( -(1 * cos(angle_radians)) );
    tangent_vector.set_y( sin(angle_radians) );
  } else if (direction == AxisDirectionType::kPositive) {
    tangent_vector.set_x( cos(angle_radians) );
    tangent_vector.set_y( sin(angle_radians) );
  }

  return tangent_vector;
}

double Catenary2d::Tension(const double& position_fraction) const {

  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  // gets coordinate
  Point2d coordinate = Coordinate(position_fraction);

  return tension_horizontal_
         * cosh( coordinate.x / (tension_horizontal_/weight_unit_));
}

/// A tangent unit vector is calculated and then multiplied by the tension magnitude.
/// The tension magnitude equation is:
/// \f[ Tension = H cosh \left( \frac{x}{\frac{H}{w}} \right) \f]
/// @see Catenary2d::TangentVector
Vector2d Catenary2d::Tension(const double& position_fraction,
                             const AxisDirectionType& direction) const {
  Vector2d vector_tangent;

  if (IsUpdated() == false) {
    if (Update() == false) {
      return vector_tangent;
    }
  }

  // gets a unit vector and scale according to tension magnitude
  vector_tangent = TangentVector(position_fraction, direction);
  const double magnitude = Tension(position_fraction);
  vector_tangent.Scale(magnitude);

  return vector_tangent;
}

double Catenary2d::TensionAverage(const int& num_points) const {

  double tension_average = -999999;

  if (IsUpdated() == false) {
    if (Update() == false) {
      return tension_average;
    }
  }

  if (num_points == 0) {

    // calculates using Ehrenburg approximation
    const double h = tension_horizontal_;
    const double w = weight_unit_;
    const double l = Length();

    const double term_1 = (pow(h, 2) / (2 * w * l));
    const double term_2 = sinh(point_end_right_.x / (h / w))
                          * cosh(point_end_right_.x / (h / w));
    const double term_3 = sinh(point_end_left_.x / (h / w))
                          * cosh(point_end_left_.x / (h / w));
    const double term_4 = (point_end_right_.x - point_end_left_.x)
                          / (h / w);

    tension_average = term_1 * (term_2 - term_3 + term_4);

  } else if (0 < num_points) {

    // calculates average tension using evenly spaced points
    double sum = 0;
    const double kNumPoints = num_points;  // convert int to double

    for (int iter = 0; iter <= kNumPoints; iter++) {

      const double position_fraction = iter / kNumPoints;
      const double tension_magnitude = Tension(position_fraction);
      sum = sum + tension_magnitude;
    }

    tension_average = sum / (kNumPoints + 1);
  }

  return tension_average;
}

double Catenary2d::TensionMax() const {

  double tension_max = -999999;

  if (IsUpdated() == false) {
    if (Update() == false) {
      return tension_max;
    }
  }

  // selects highest end point and calculate tension
  if (spacing_endpoints_.y() <= 0) {
    return Tension(0);
  } else {
    return Tension(1);
  }
}

bool Catenary2d::Validate(const bool& is_included_warnings,
                          std::list<std::string>* messages_error) const {
  bool is_valid = true;

  // validates tension-horizontal
  if (tension_horizontal_ <= 0
      || (100000 < tension_horizontal_ && is_included_warnings == true)) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CATENARY - Invalid horizontal tension");
    }
  }

  // validates weight-unit
  if (weight_unit_ <= 0
      || (15 < weight_unit_ && is_included_warnings == true)) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CATENARY - Invalid unit weight");
    }
  }

  // validates spacing-endpoints-horizontal
  if (spacing_endpoints_.x() <= 0
      || (5000 < spacing_endpoints_.x() && is_included_warnings == true)) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CATENARY - Invalid horizontal endpoint spacing");
    }
  }

  // validates spacing-endpoint-vertical
  if (10000 < abs(spacing_endpoints_.y())
      || 2000 <= abs(spacing_endpoints_.y()) ) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CATENARY - Invalid vertical endpoint spacing");
    }
  }

  return is_valid;
}

void Catenary2d::set_spacing_endpoints(const Vector2d& spacing_endpoints) {

  spacing_endpoints_ = spacing_endpoints;

  is_updated_points_end_ = false;
}

void Catenary2d::set_tension_horizontal(const double& tension_horizontal) {

  tension_horizontal_ = tension_horizontal;

  is_updated_points_end_ = false;
}

void Catenary2d::set_weight_unit(const double& weight_unit) {

  weight_unit_ = weight_unit;

  is_updated_points_end_ = false;
}

Vector2d Catenary2d::spacing_endpoints() const {
  return spacing_endpoints_;
}

double Catenary2d::tension_horizontal() const {
  return tension_horizontal_;
}

double Catenary2d::weight_unit() const {
  return weight_unit_;
}

/// The function is a derivation of the equation for curve length.
/// @see Catenary2d::CurveLengthFromOrigin
/// \f[ x = \frac{H}{w} sinh^{-1} \left(\frac{L}{\frac{H}{w}}\right) \f]
double Catenary2d::CoordinateX(
    const double& length_origin_to_position,
    const AxisDirectionType& direction_origin_to_position) const {

  const double l = length_origin_to_position;
  const double h = tension_horizontal_;
  const double w = weight_unit_;

  double coordinate_x = -999999;

  // BOL from origin - negative x coordinate
  if (direction_origin_to_position == AxisDirectionType::kNegative) {
    coordinate_x = -(h/w) * (asinh(l / (h/w)));
  }
  // AOL from origin - positive x coordinate
  else if (direction_origin_to_position == AxisDirectionType::kPositive) {
    coordinate_x = (h/w) * (asinh(l / (h/w)));
  }

  return coordinate_x;
}

/// \f[ y = \frac{H}{w} \cosh \left(\frac{x}{\frac{H}{w}-1}\right) \f]
double Catenary2d::CoordinateY(
    const double& length_origin_to_position,
    const AxisDirectionType& direction_origin_to_position) const {

  const double x = CoordinateX(length_origin_to_position,
                               direction_origin_to_position);
  const double h = tension_horizontal_;
  const double w = weight_unit_;

  return (h/w) * (cosh(x / (h/w)) - 1);
}

bool Catenary2d::IsUpdated() const {

  if (is_updated_points_end_ == true) {
    return true;
  } else {
    return false;
  }
}

/// \f[ CurveLength = \left| \frac{H}{w} sinh^{-1} \left(\frac{x}{\frac{H}{w}}\right) \right| \f]
double Catenary2d::LengthFromOrigin(const Point2d& coordinate) const {

  const double x = coordinate.x;
  const double h = tension_horizontal_;
  const double w = weight_unit_;

  return std::abs((h/w) * sinh(x / (h/w)));
}

double Catenary2d::PositionFraction(const double& tangent_angle) const {

  double position_fraction_lower = 0;
  double position_fraction_upper = 1;
  double position_fraction_target = -999999;
  double tangent_angle_position = -999999;
  int iter = 0;
  const int kIterMax = 100;

  while ((0.001 < abs(tangent_angle - tangent_angle_position)
      || (0.0001 < position_fraction_upper - position_fraction_lower))
      && (iter < kIterMax)) {

    // updates position along curve
    position_fraction_target = (position_fraction_upper +
                                position_fraction_lower) / 2;

    // gets a tangent angle at the position along curve
    tangent_angle_position = TangentAngle(position_fraction_target,
                                          AxisDirectionType::kPositive);

    // compares against attachment spacing, update limits
    if (tangent_angle_position == tangent_angle) {
      break;
    } else if (tangent_angle_position < tangent_angle) {
      position_fraction_lower = position_fraction_target;
    } else if (tangent_angle < tangent_angle_position) {
      position_fraction_upper = position_fraction_target;
    }

    // checks iterator, increment or exit
    if (iter < kIterMax) {
      iter++;
    } else {
      break;
    }
  }

  if (iter < kIterMax) {
    return position_fraction_target;
  } else {
    return -999999;
  }
}

bool Catenary2d::Update() const {

  if (is_updated_points_end_ == false) {

    is_updated_points_end_ = UpdateEndPoints();
    if (is_updated_points_end_ == false) {
      return false;
    }
  }

  // if it reaches this point, update was successful
  return true;
}

/// Hyperbolic identity equations are used to solve for the endpoint coordinates:
/// \f[ xBOL = \frac{A}{2} - \frac{H}{w} sinh^{-1} \left( \frac{\frac{B}{2}}{ \frac{H}{w} sinh \left( \frac{\frac{A}{2}}{\frac{H}{w}} \right)} \right) \f]
/// \f[ xAOL = \frac{A}{2} + \frac{H}{w} sinh^{-1} \left( \frac{\frac{B}{2}}{ \frac{H}{w} sinh \left( \frac{\frac{A}{2}}{\frac{H}{w}} \right)} \right) \f]
bool Catenary2d::UpdateEndPoints() const {

  const double h = tension_horizontal_;
  const double w = weight_unit_;
  const double a = spacing_endpoints_.x();
  const double b = spacing_endpoints_.y();
  const double z = (a/2) / (h/w);

  // solves for left endpoint coordinate
  point_end_left_.x = (h/w) * (asinh((b * z) / (a * sinh(z))) - z);
  point_end_left_.y = CoordinateY(LengthFromOrigin(point_end_left_),
                                  AxisDirectionType::kNegative);

  // solves for right endpoint coordinate
  point_end_right_.x = (h/w) * (asinh((b * z) / (a * sinh(z))) + z);
  point_end_right_.y = CoordinateY(LengthFromOrigin(point_end_right_),
                                   AxisDirectionType::kPositive);

  return true;
}

bool Catenary2d::ValidateCurveAndSpacing(
    const bool& is_included_warnings,
    std::list<std::string>* messages_error) const {

  bool validate = true;

  const double constant_minimum =
      ConstantMinimum(spacing_endpoints_.Magnitude());

  const double constant = Constant();

  if (constant < constant_minimum) {

    validate = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CATENARY 2D - Invalid H/w and endpoint spacing"
                                "combination");
    }
  }

  return validate;
}

Catenary3d::Catenary3d() {
  is_updated_catenary_2d_ = false;
}

Catenary3d::~Catenary3d() {}

double Catenary3d::Constant() const {

  double constant = -999999;

  if (IsUpdated() == false) {
    if (Update() == false) {
      return constant;
    }
  }

  return catenary_2d_.Constant();
}

double Catenary3d::ConstantMinimum(const double& spacing_endpoints) {

  return Catenary2d::ConstantMinimum(spacing_endpoints);
}

Point3d Catenary3d::Coordinate(const double& position_fraction,
                               const bool& is_shifted_origin) const {
  Point3d coordinate_catenary;

  if (IsUpdated() == false) {
    if (Update() == false) {
      return coordinate_catenary;
    }
  }

  // gets a 2D chord coordinate
  Point2d coordinate_2d_chord = catenary_2d_.CoordinateChord(position_fraction,
                                is_shifted_origin);

  // gets a 2D curve coordinate
  Point2d coordinate_2d_curve = catenary_2d_.Coordinate(position_fraction,
                                is_shifted_origin);

  // creates a vector between chord coordinate and curve coordinate
  // rotates vector transversely according to unit loading
  Vector3d vector_chord_to_curve;
  vector_chord_to_curve.set_x(0);
  vector_chord_to_curve.set_y(0);
  vector_chord_to_curve.set_z(coordinate_2d_curve.y - coordinate_2d_chord.y);
  vector_chord_to_curve.Rotate(Plane2dType::kZy,
                               weight_unit_.Angle(Plane2dType::kZy));


  // uses 2D chord coordinate and chord-to-curve vector to solve for 3D curve
  // coordinate
  coordinate_catenary.x = coordinate_2d_chord.x;
  coordinate_catenary.y = 0 + vector_chord_to_curve.y();
  coordinate_catenary.z = coordinate_2d_chord.y + vector_chord_to_curve.z();

  return coordinate_catenary;
}

Point3d Catenary3d::CoordinateChord(const double& position_fraction,
                                    const bool& is_shifted_origin) const {
  Point3d coordinate_chord;

  if (IsUpdated() == false) {
    if (Update() == false) {
      return coordinate_chord;
    }
  }

  // gets a 2d chord coordinate from 2D catenary
  Point2d coordinate_2d_chord = catenary_2d_.CoordinateChord(position_fraction,
                                is_shifted_origin);

  // converts to 3D coordinate system
  coordinate_chord.x = coordinate_2d_chord.x;
  coordinate_chord.y = 0;
  coordinate_chord.z = coordinate_2d_chord.y;

  return coordinate_chord;
}

double Catenary3d::Length() const {

  double length = -999999;

  if (IsUpdated() == false) {
    if (Update() == false) {
      return length;
    }
  }

  return catenary_2d_.Length();
}

double Catenary3d::LengthSlack() const {

  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return catenary_2d_.LengthSlack();
}

double Catenary3d::PositionFractionOrigin() const {

  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return catenary_2d_.PositionFractionOrigin();
}

double Catenary3d::PositionFractionSagPoint() const {

  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return catenary_2d_.PositionFractionSagPoint();
}

double Catenary3d::Sag() const {

  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return catenary_2d_.Sag();
}

double Catenary3d::SwingAngle() const {

  double angle_swing = -999999;

  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  angle_swing = units::Convert(
      atan(weight_unit_.y() / weight_unit_.z()),
      units::ConversionType::kRadiansToDegrees);
  return angle_swing;
}

double Catenary3d::TangentAngleTransverse(
    const double& position_fraction,
    const AxisDirectionType& direction) const {

  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  // gets the 3D tangent vector
  Vector3d tangent_vector = TangentVector(position_fraction, direction);

  // adjusts components to use only positive values in unit circle
  tangent_vector.set_y( abs(tangent_vector.y()) );
  tangent_vector.set_z( abs(tangent_vector.z()) );

  // returns transverse angle
  return tangent_vector.Angle(Plane2dType::kZy, true);
}

double Catenary3d::TangentAngleVertical(
    const double& position_fraction,
    const AxisDirectionType& direction) const {

  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  // gets the 3D tangent vector
  Vector3d tangent_vector = TangentVector(position_fraction, direction);

  // adjusts x component to use only positive values in unit circle
  tangent_vector.set_x( abs(tangent_vector.x()) );

  // returns vertical angle
  return tangent_vector.Angle(Plane2dType::kXz, true);
}

Vector3d Catenary3d::TangentVector(const double& position_fraction,
                                   const AxisDirectionType& direction) const {
  Vector3d tangent_vector;

  if (IsUpdated() == false) {
    if (Update() == false) {
      return tangent_vector;
    }
  }

  // gets 2D tangent vector
  Vector2d tangent_vector_2d = catenary_2d_.TangentVector(position_fraction,
                               direction);

  // sets initial 3D vector values
  tangent_vector.set_x(tangent_vector_2d.x());
  tangent_vector.set_y(0);
  tangent_vector.set_z(tangent_vector_2d.y());

  // rotates 3D vector due to different endpoint spacing of 2D and 3D catenary.
  // only applicable when endpoint spacing has a vertical component, and
  // transverse load is present
  if (spacing_endpoints_.z() != 0 && weight_unit_.y() != 0) {

    Vector2d spacing_endpoints_2d = catenary_2d_.spacing_endpoints();
    double angle_endpoints_2d = spacing_endpoints_2d.Angle(true);
    double angle_endpoints_3d = spacing_endpoints_.Angle(Plane2dType::kXz, true);

    tangent_vector.Rotate(Plane2dType::kXz,
                          angle_endpoints_3d - angle_endpoints_2d);
  }

  // rotates 3D vector due to transverse loading
  if (weight_unit_.y() != 0) {

    double angle_rotation = units::Convert(
        atan(weight_unit_.y() / weight_unit_.z()),
        units::ConversionType::kRadiansToDegrees);

    if (0 < weight_unit_.y()) {
      angle_rotation = angle_rotation * -1;
    }

    tangent_vector.Rotate(Plane2dType::kYz,
                          angle_rotation);
  }

  return tangent_vector;
}

double Catenary3d::Tension(const double& position_fraction) const {

  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return catenary_2d_.Tension(position_fraction);
}

Vector3d Catenary3d::Tension(const double& position_fraction,
                             const AxisDirectionType& direction) const {
  Vector3d tension_vector;

  if (IsUpdated() == false) {
    if (Update() == false) {
      return tension_vector;
    }
  }

  // gets tension magnitude
  const double tension = catenary_2d_.Tension(position_fraction);

  // gets tangent vector
  Vector3d tangent_vector = TangentVector(position_fraction, direction);

  // scales vector
  tangent_vector.Scale(tension);

  return tangent_vector;
}

double Catenary3d::TensionAverage(const int& num_points) const {

  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return catenary_2d_.TensionAverage(num_points);
}

double Catenary3d::TensionMax() const {

  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return catenary_2d_.TensionMax();
}

bool Catenary3d::Validate(const bool& is_included_warnings,
                          std::list<std::string>* messages_error) const {
  bool is_valid = true;

  // validates spacing-endpoints-horizontal
  if (spacing_endpoints_.x() <= 0
      || (5000 < spacing_endpoints_.x() && is_included_warnings == true)) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CATENARY - Invalid horizontal endpoint spacing");
    }
  }

  // validates spacing-endpoints-transverse
  if (spacing_endpoints_.y() != 0) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CATENARY - Transverse endpoint spacing must "
                                "equal zero");
    }
  }

  // validates spacing-endpoint-vertical
  if (10000 < abs(spacing_endpoints_.z())
      || 2000 <= abs(spacing_endpoints_.z()) ) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CATENARY - Invalid vertical endpoint spacing");
    }
  }

  // validates weight-unit-horizontal
  if (weight_unit_.x() != 0) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CATENARY - Horizontal unit weight must"
                                "equal zero");
    }
  }

  // validates weight-unit-transverse
  if (weight_unit_.y() < 0
      || (15 < weight_unit_.y() && is_included_warnings == true)) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CATENARY - Invalid transverse unit weight");
    }
  }

  // validates weight-unit-vertical
  if (weight_unit_.z() <= 0
      || (25 < weight_unit_.z() && is_included_warnings == true)) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CATENARY - Invalid vertical unit weight");
    }
  }

  return is_valid;
}

void Catenary3d::set_spacing_endpoints(const Vector3d& spacing_endpoints) {

  spacing_endpoints_ = spacing_endpoints;

  is_updated_catenary_2d_ = false;
}

void Catenary3d::set_tension_horizontal(const double& tension_horizontal) {
  catenary_2d_.set_tension_horizontal(tension_horizontal);
}

void Catenary3d::set_weight_unit(const Vector3d& weight_unit) {

  weight_unit_ = weight_unit;

  is_updated_catenary_2d_ = false;
}

Vector3d Catenary3d::spacing_endpoints() const {
  return spacing_endpoints_;
}

double Catenary3d::tension_horizontal() const {
  return catenary_2d_.tension_horizontal();
}

Vector3d Catenary3d::weight_unit() const {
  return weight_unit_;
}

bool Catenary3d::IsUpdated() const {

  if (is_updated_catenary_2d_ == true) {
    return true;
  } else {
    return false;
  }
}

bool Catenary3d::Update() const {

  if (is_updated_catenary_2d_ == false) {

    is_updated_catenary_2d_ = UpdateCatenary2d();
    if (is_updated_catenary_2d_ == false) {
      return false;
    }
  }

  // if it reaches this point, update was successful
  return true;
}

bool Catenary3d::UpdateCatenary2d() const {

  const double b = spacing_endpoints_.z();
  const double c = spacing_endpoints_.Magnitude();
  const double v = abs(weight_unit_.z());
  const double w = weight_unit_.Magnitude();

  // solves for 2D catenary endpoint spacing
  Vector2d spacing_endpoints_2d;
  spacing_endpoints_2d.set_y(b * (v/w));
  spacing_endpoints_2d.set_x(sqrt( pow(c,2)-pow(spacing_endpoints_2d.y(), 2)));

  // sets catenary spacing and unit weight
  catenary_2d_.set_spacing_endpoints(spacing_endpoints_2d);
  catenary_2d_.set_weight_unit(weight_unit_.Magnitude());

  return true;
}
