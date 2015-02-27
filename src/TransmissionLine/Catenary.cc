// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "include/TransmissionLine/Catenary.h"

#include <cmath>

#include "include/Base/SupportFile.h"

Catenary2D::Catenary2D() {
  tension_horizontal_ = -999999;
  weight_unit_ = -999999;

  is_updated_points_end_ = true;
}

Catenary2D::~Catenary2D() {
}

/// \f[ \frac{H}{w} = \frac{HorizontalTension}{UnitWeight} \f]
double Catenary2D::Constant() const {

  double constant = -999999;

  if (IsUpdated() == false) {
    if (Update() == false) {
      return constant;
    }
  }

  return tension_horizontal_ / weight_unit_;
}

/// Solves for length and direction from origin, and then converts to
/// coordinates.
Point2D Catenary2D::Coordinate(const double& position_fraction,
                               const bool& is_shifted_origin) const {
  Point2D coordinate;

  if (IsUpdated() == false) {
    if (Update() == false) {
      return coordinate;
    }
  }

  // length from left end to position
  double length_left_to_position = position_fraction * Length();

  // length from origin to left endpoint
  const double length_origin_to_left = LengthFromOrigin(point_end_left_);

  // solve for length from origin and direction from origin
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

  // solve for catenary coordinate
  coordinate.x = CoordinateX(length_origin_to_position,
                             direction_origin_to_position);
  coordinate.y = CoordinateY(length_origin_to_position,
                             direction_origin_to_position);

  return coordinate;
}

Point2D Catenary2D::CoordinateChord(const double& position_fraction,
                                    const bool& is_shifted_origin) const {
  Point2D coordinate_chord;

  if (IsUpdated() == false) {
    if (Update() == false) {
      return coordinate_chord;
    }
  }

  // get a catenary coordinate
  Point2D coordinate_catenary = Coordinate(position_fraction,
                                is_shifted_origin);

  // calculate a chord coordinate
  coordinate_chord.x = coordinate_catenary.x;
  coordinate_chord.y = point_end_left_.y
                       + ((coordinate_catenary.x - point_end_left_.x)
                          * (spacing_endpoints_.y() / spacing_endpoints_.x()));

  // check if shifted coordinate is requested, modify if necessary
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
/// @see Catenary2D::LengthFromOrigin
double Catenary2D::Length() const {

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

double Catenary2D::LengthSlack() const {

  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return Length() - spacing_endpoints().Magnitude();
}

double Catenary2D::PositionFractionOrigin() const {

  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  // get position along curve of low point, where tangent angle is zero
  return PositionFraction(0);
}

double Catenary2D::PositionFractionSagPoint() const {

  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  double tangent_angle = spacing_endpoints_.Angle(true);

  // get position fraction of chord tangent line
  return PositionFraction(tangent_angle);
}

double Catenary2D::Sag() const {

  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  // get sag position
  const double position_fraction_sagpoint = PositionFractionSagPoint();

  // get catenary coordinate at sag position
  const Point2D coordinate_catenary = Coordinate(position_fraction_sagpoint);

  // get a chord coordinate at sag position
  const Point2D coordinate_chord = CoordinateChord(position_fraction_sagpoint);

  // get height difference between chord and catenary
  return coordinate_chord.y - coordinate_catenary.y;
}

double Catenary2D::TangentAngle(const double& position_fraction,
                                const AxisDirectionType& direction) const {
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  // get coordinate at position
  const Point2D coordinate = Coordinate(position_fraction);

  // calculate slope at position
  const double slope = sinh(coordinate.x / (tension_horizontal_/weight_unit_));

  // convert to degrees
  double tangent_angle = atan(slope) * (kRadiansToDegrees);

  // adjust if direction is negative
  if (direction == AxisDirectionType::kNegative) {
    tangent_angle = tangent_angle * -1;
  }

  return tangent_angle;
}

/// The slope at the position is calculated, and then adjusted to a unit vector
/// based on the direction the vector is pointing.
/// Slope equation is:
/// \f[ Slope = sinh \left( \frac{x}{\frac{H}{w}} \right) \f]
Vector2D Catenary2D::TangentVector(const double& position_fraction,
                                   const AxisDirectionType& direction) const {
  Vector2D tangent_vector;

  if (IsUpdated() == false) {
    if (Update() == false) {
      return tangent_vector;
    }
  }

  // get tangent angle
  const double angle_tangent = TangentAngle(position_fraction, direction);

  // resolve to a unit vector
  if (direction == AxisDirectionType::kNegative) {
    tangent_vector.set_x( -(1 * cos(angle_tangent * kRadiansToDegrees)) );
    tangent_vector.set_y( sin(angle_tangent * kRadiansToDegrees) );
  } else if (direction == AxisDirectionType::kPositive) {
    tangent_vector.set_x( cos(angle_tangent * kRadiansToDegrees) );
    tangent_vector.set_y( sin(angle_tangent * kRadiansToDegrees) );
  }

  return tangent_vector;
}

double Catenary2D::Tension(const double& position_fraction) const {

  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  // get coordinate
  Point2D coordinate = Coordinate(position_fraction);

  return tension_horizontal_
         * cosh( coordinate.x / (tension_horizontal_/weight_unit_));
}

/// A tangent unit vector is calculated and then multiplied by the tension magnitude.
/// The tension magnitude equation is:
/// \f[ Tension = H cosh \left( \frac{x}{\frac{H}{w}} \right) \f]
/// @see Catenary2D::TangentVector
Vector2D Catenary2D::Tension(const double& position_fraction,
                             const AxisDirectionType& direction) const {
  Vector2D vector_tangent;

  if (IsUpdated() == false) {
    if (Update() == false) {
      return vector_tangent;
    }
  }

  // get a unit vector and scale according to tension magnitude
  vector_tangent = TangentVector(position_fraction, direction);
  const double magnitude = Tension(position_fraction);
  vector_tangent.Scale(magnitude);

  return vector_tangent;
}

double Catenary2D::TensionAverage(const int& num_points) const {

  double tension_average = -999999;

  if (IsUpdated() == false) {
    if (Update() == false) {
      return tension_average;
    }
  }

  if (num_points == 0) {

    // calculate using Ehrenburg approximation
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

    // claculate average tension using evenly spaced points
    double sum = 0;

    for (int iter = 0; iter <= num_points; iter++) {

      const double position_fraction = iter / num_points;
      const double tension_magnitude = Tension(position_fraction);
      sum = sum + tension_magnitude;
    }

    tension_average = sum / num_points;
  }

  return tension_average;
}

double Catenary2D::TensionMax() const {

  double tension_max = -999999;

  if (IsUpdated() == false) {
    if (Update() == false) {
      return tension_max;
    }
  }

  // select highest end point and calculate tension
  if (spacing_endpoints_.y() <= 0) {
    return Tension(0);
  } else {
    return Tension(1);
  }
}

bool Catenary2D::Validate(const bool& is_included_warnings,
                          std::list<std::string>* messages_error) const {
  bool is_valid = true;

  // validate tension-horizontal
  if (tension_horizontal_ <= 0
      || (100000 < tension_horizontal_ && is_included_warnings == true)) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CATENARY - Invalid horizontal tension");
    }
  }

  // validate weight-unit
  if (weight_unit_ <= 0
      || (15 < weight_unit_ && is_included_warnings == true)) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CATENARY - Invalid unit weight");
    }
  }

  // validate spacing-endpoints-horizontal
  if (spacing_endpoints_.x() <= 0
      || (5000 < spacing_endpoints_.x() && is_included_warnings == true)) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CATENARY - Invalid horizontal endpoint spacing");
    }
  }

  // validate spacing-endpoint-vertical
  if (10000 < abs(spacing_endpoints_.y())
      || 2000 <= abs(spacing_endpoints_.y()) ) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CATENARY - Invalid vertical endpoint spacing");
    }
  }

  return is_valid;
}

void Catenary2D::set_spacing_endpoints(const Vector2D& spacing_endpoints) {

  spacing_endpoints_ = spacing_endpoints;

  is_updated_points_end_ = false;
}

void Catenary2D::set_tension_horizontal(const double& tension_horizontal) {

  tension_horizontal_ = tension_horizontal;

  is_updated_points_end_ = false;
}

void Catenary2D::set_weight_unit(const double& weight_unit) {

  weight_unit_ = weight_unit;

  is_updated_points_end_ = false;
}

Vector2D Catenary2D::spacing_endpoints() const {
  return spacing_endpoints();
}

double Catenary2D::tension_horizontal() const {
  return tension_horizontal_;
}

double Catenary2D::weight_unit() const {
  return weight_unit_;
}

/// The function is a derivation of the equation for curve length.
/// @see Catenary2D::CurveLengthFromOrigin
/// \f[ x = \frac{H}{w} sinh^{-1} \left(\frac{L}{\frac{H}{w}}\right) \f]
double Catenary2D::CoordinateX(
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
double Catenary2D::CoordinateY(
    const double& length_origin_to_position,
    const AxisDirectionType& direction_origin_to_position) const {

  const double x = CoordinateX(length_origin_to_position,
                               direction_origin_to_position);
  const double h = tension_horizontal_;
  const double w = weight_unit_;

  return (h/w) * (cosh(x / (h/w)) - 1);
}

bool Catenary2D::IsUpdated() const {

  if (is_updated_points_end_ == true) {
    return true;
  } else {
    return false;
  }
}

/// \f[ CurveLength = \left| \frac{H}{w} sinh^{-1} \left(\frac{x}{\frac{H}{w}}\right) \right| \f]
double Catenary2D::LengthFromOrigin(const Point2D& coordinate) const {

  const double x = coordinate.x;
  const double h = tension_horizontal_;
  const double w = weight_unit_;

  return std::abs((h/w) * sinh(x / (h/w)));
}

double Catenary2D::PositionFraction(const double& tangent_angle) const {

  double position_fraction_lower = 0;
  double position_fraction_upper = 1;
  double position_fraction_target = -999999;
  double tangent_angle_position = -999999;
  int iter = 0;
  const int kIterMax = 100;

  while ((0.001 < abs(tangent_angle - tangent_angle_position)
      || (0.0001 < position_fraction_upper - position_fraction_lower))
      && (iter < kIterMax)) {

    // update position along curve
    position_fraction_target = (position_fraction_upper +
                                position_fraction_lower) / 2;

    // get a tangent angle at the position along curve
    tangent_angle_position = TangentAngle(position_fraction_target,
                                          AxisDirectionType::kPositive);

    // compare against attachment spacing, update limits
    if (tangent_angle_position == tangent_angle) {
      break;
    } else if (tangent_angle_position < tangent_angle) {
      position_fraction_lower = position_fraction_target;
    } else if (tangent_angle < tangent_angle_position) {
      position_fraction_upper = position_fraction_target;
    }

    // check iterator, increment or exit
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

bool Catenary2D::Update() const {

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
bool Catenary2D::UpdateEndPoints() const {

  const double h = tension_horizontal_;
  const double w = weight_unit_;
  const double a = spacing_endpoints_.x();
  const double b = spacing_endpoints_.y();
  const double z = (a/2) / (h/w);

  // solve for left endpoint coordinate
  point_end_left_.x = (h/w) * (asinh(b * z) / (a * sinh(z)) - z);
  point_end_left_.y = CoordinateY(LengthFromOrigin(point_end_left_),
                                  AxisDirectionType::kNegative);

  // solve for right endpoint coordinate
  point_end_right_.x = (h/w) * (asinh(b * z) / (a * sinh(z)) + z);
  point_end_right_.y = CoordinateY(LengthFromOrigin(point_end_right_),
                                   AxisDirectionType::kPositive);

  return true;
}

Catenary3D::Catenary3D() {
  is_updated_catenary_2d_ = false;
}

Catenary3D::~Catenary3D() {}

double Catenary3D::Constant() const {

  double constant = -999999;

  if (IsUpdated() == false) {
    if (Update() == false) {
      return constant;
    }
  }

  return catenary_2d_.Constant();
}

Point3D Catenary3D::Coordinate(const double& position_fraction,
                               const bool& is_shifted_origin) const {
  Point3D coordinate_catenary;

  if (IsUpdated() == false) {
    if (Update() == false) {
      return coordinate_catenary;
    }
  }

  // get a 2D chord coordinate
  Point2D coordinate_2d_chord = catenary_2d_.CoordinateChord(position_fraction,
                                is_shifted_origin);

  // get a 2D curve coordinate
  Point2D coordinate_2d_curve = catenary_2d_.Coordinate(position_fraction,
                                is_shifted_origin);

  // create a vector between chord coordinate and curve coordinate
  // rotate vector transversely according to unit loading
  Vector3D vector_chord_to_curve;
  vector_chord_to_curve.set_x(0);
  vector_chord_to_curve.set_y(0);
  vector_chord_to_curve.set_z(coordinate_2d_curve.y - coordinate_2d_chord.y);
  vector_chord_to_curve.Rotate(Plane2dType::kZy,
                               weight_unit_.Angle(Plane2dType::kZy));


  // use 2D chord coordinate and chord-to-curve vector to solve for 3D curve
  // coordinate
  coordinate_catenary.x = coordinate_2d_chord.x;
  coordinate_catenary.y = 0 + vector_chord_to_curve.y();
  coordinate_catenary.z = coordinate_2d_chord.y + vector_chord_to_curve.z();

  return coordinate_catenary;
}

Point3D Catenary3D::CoordinateChord(const double& position_fraction,
                                    const bool& is_shifted_origin) const {
  Point3D coordinate_chord;

  if (IsUpdated() == false) {
    if (Update() == false) {
      return coordinate_chord;
    }
  }

  // get a 2d chord coordinate from 2D catenary
  Point2D coordinate_2d_chord = catenary_2d_.CoordinateChord(position_fraction,
                                is_shifted_origin);

  // convert to 3D coordinate system
  coordinate_chord.x = coordinate_2d_chord.x;
  coordinate_chord.y = 0;
  coordinate_chord.z = coordinate_2d_chord.y;

  return coordinate_chord;
}

double Catenary3D::Length() const {

  double length = -999999;

  if (IsUpdated() == false) {
    if (Update() == false) {
      return length;
    }
  }

  return catenary_2d_.Length();
}

double Catenary3D::LengthSlack() const {

  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return catenary_2d_.LengthSlack();
}

double Catenary3D::PositionFractionOrigin() const {

  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return catenary_2d_.PositionFractionOrigin();
}

double Catenary3D::PositionFractionSagPoint() const {

  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return catenary_2d_.PositionFractionSagPoint();
}

double Catenary3D::Sag() const {

  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return catenary_2d_.Sag();
}

double Catenary3D::SwingAngle() const {

  double angle_swing = -999999;

  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  angle_swing = atan(weight_unit_.y() / weight_unit_.z()) * kRadiansToDegrees;
  return angle_swing;
}

double Catenary3D::TangentAngleTransverse(
    const double& position_fraction,
    const AxisDirectionType& direction) const {

  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  // get the 3D tangent vector
  Vector3D tangent_vector = TangentVector(position_fraction, direction);

  // adjust components to use only positive values in unit circle
  tangent_vector.set_y( abs(tangent_vector.y()) );
  tangent_vector.set_z( abs(tangent_vector.z()) );

  // return transverse angle
  return tangent_vector.Angle(Plane2dType::kZy, true);
}

double Catenary3D::TangentAngleVertical(
    const double& position_fraction,
    const AxisDirectionType& direction) const {

  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  // get the 3D tangent vector
  Vector3D tangent_vector = TangentVector(position_fraction, direction);

  // adjust x component to use only positive values in unit circle
  tangent_vector.set_x( abs(tangent_vector.x()) );

  // return vertical angle
  return tangent_vector.Angle(Plane2dType::kXz, true);
}

Vector3D Catenary3D::TangentVector(const double& position_fraction,
                                   const AxisDirectionType& direction) const {
  Vector3D tangent_vector;

  if (IsUpdated() == false) {
    if (Update() == false) {
      return tangent_vector;
    }
  }

  // get 2D tangent vector
  Vector2D tangent_vector_2d = catenary_2d_.TangentVector(position_fraction,
                               direction);

  // set initial 3D vector values
  tangent_vector.set_x(tangent_vector_2d.x());
  tangent_vector.set_y(0);
  tangent_vector.set_z(tangent_vector_2d.y());

  // rotate 3D vector due to different endpoit spacing of 2D and 3D catenary
  // only applicable when endpoint spacing has a vertical component, and
  // transverse load is present
  if (spacing_endpoints_.z() != 0 && weight_unit_.y() != 0) {

    Vector2D spacing_endpoints_2d = catenary_2d_.spacing_endpoints();
    double angle_endpoints_2d = spacing_endpoints_2d.Angle(true);
    double angle_endpoints_3d = spacing_endpoints_.Angle(Plane2dType::kXz, true);

    tangent_vector.Rotate(Plane2dType::kXz,
                          angle_endpoints_3d - angle_endpoints_2d);
  }

  // rotate 3D vector due to transverse loading
  if (weight_unit_.y() != 0) {

    if (weight_unit_.y() < 0) {
      tangent_vector.Rotate(Plane2dType::kYz,
                            (atan(weight_unit_.y()/weight_unit_.z())
                             * kRadiansToDegrees));
    } else if (0 < weight_unit_.y()) {
      tangent_vector.Rotate(Plane2dType::kYz,
                            -(atan(weight_unit_.y()/weight_unit_.z())
                              * kRadiansToDegrees));
    }
  }

  return tangent_vector;
}

double Catenary3D::Tension(const double& position_fraction) const {

  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return catenary_2d_.Tension(position_fraction);
}

Vector3D Catenary3D::Tension(const double& position_fraction,
                             const AxisDirectionType& direction) const {
  Vector3D tension_vector;

  if (IsUpdated() == false) {
    if (Update() == false) {
      return tension_vector;
    }
  }

  // get tension magnitude
  const double tension = catenary_2d_.Tension(position_fraction);

  // get tangent vector
  Vector3D tangent_vector = TangentVector(position_fraction, direction);

  // scale vector
  tangent_vector.Scale(tension);

  return tangent_vector;
}

double Catenary3D::TensionAverage(const int& num_points) const {

  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return catenary_2d_.TensionAverage(num_points);
}

double Catenary3D::TensionMax() const {

  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return catenary_2d_.TensionMax();
}

bool Catenary3D::Validate(const bool& is_included_warnings,
                          std::list<std::string>* messages_error) const {
  bool is_valid = true;

  // validate spacing-endpoints-horizontal
  if (spacing_endpoints_.x() <= 0
      || (5000 < spacing_endpoints_.x() && is_included_warnings == true)) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CATENARY - Invalid horizontal endpoint spacing");
    }
  }

  // validate spacing-endpoints-transverse
  if (spacing_endpoints_.y() != 0) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CATENARY - Transverse endpoint spacing must "
                                "equal zero");
    }
  }

  // validate spacing-endpoint-vertical
  if (10000 < abs(spacing_endpoints_.z())
      || 2000 <= abs(spacing_endpoints_.z()) ) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CATENARY - Invalid vertical endpoint spacing");
    }
  }

  // validate weight-unit-horizontal
  if (weight_unit_.x() != 0) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CATENARY - Horizontal unit weight must"
                                "equal zero");
    }
  }

  // validate weight-unit-transverse
  if (0 < weight_unit_.y()
      || (15 < weight_unit_.y() && is_included_warnings == true)) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CATENARY - Invalid transverse unit weight");
    }
  }


  // validate weight-unit-vertical
  if (0 < weight_unit_.z()
      || (25 < weight_unit_.z() && is_included_warnings == true)) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CATENARY - Invalid vertical unit weight");
    }
  }

  return is_valid;
}

void Catenary3D::set_spacing_endpoints(const Vector3D& spacing_endpoints) {

  spacing_endpoints_ = spacing_endpoints;

  is_updated_catenary_2d_ = false;
}

void Catenary3D::set_tension_horizontal(const double& tension_horizontal) {
  catenary_2d_.set_tension_horizontal(tension_horizontal);
}

void Catenary3D::set_weight_unit(const Vector3D& weight_unit) {

  weight_unit_ = weight_unit;

  is_updated_catenary_2d_ = false;
}

Vector3D Catenary3D::spacing_endpoints() const {
  return spacing_endpoints_;
}

double Catenary3D::tension_horizontal() const {
  return catenary_2d_.tension_horizontal();
}

Vector3D Catenary3D::weight_unit() const {
  return weight_unit_;
}

bool Catenary3D::IsUpdated() const {

  if (is_updated_catenary_2d_ == true) {
    return true;
  } else {
    return false;
  }
}

bool Catenary3D::Update() const {

  if (is_updated_catenary_2d_ == false) {

    is_updated_catenary_2d_ = UpdateCatenary2D();
    if (is_updated_catenary_2d_ == false) {
      return false;
    }
  }

  // if it reaches this point, update was successful
  return true;
}

bool Catenary3D::UpdateCatenary2D() const {

  const double b = spacing_endpoints_.z();
  const double c = spacing_endpoints_.Magnitude();
  const double v = abs(weight_unit_.z());
  const double w = weight_unit_.Magnitude();

  // 2D catenary endpoint spacing
  Vector2D spacing_endpoints_2d;
  spacing_endpoints_2d.set_y(b * (v/w));
  spacing_endpoints_2d.set_x(sqrt( pow(c,2)-pow(spacing_endpoints_2d.y(), 2)));

  // set catenary spacing and unit weight
  catenary_2d_.set_spacing_endpoints(spacing_endpoints_2d);
  catenary_2d_.set_weight_unit(weight_unit_.Magnitude());

  // return update success status
  return true;
}
