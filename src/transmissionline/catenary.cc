// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/transmissionline/catenary.h"

#include <cmath>

#include "models/base/units.h"

Catenary2d::Catenary2d() {
  tension_horizontal_ = -999999;
  weight_unit_ = -999999;

  length_ = -999999;
  length_left_ = -999999;

  is_updated_ = true;
}

Catenary2d::~Catenary2d() {
}

double Catenary2d::Constant() const {
  return tension_horizontal_ / weight_unit_;
}

double Catenary2d::ConstantMinimum(const double& spacing_endpoints) {
  return spacing_endpoints / 2;
}

Point2d<double> Catenary2d::Coordinate(const double& position_fraction,
                                       const bool& is_shifted_origin) const {
  Point2d<double> coordinate;

  if ((IsUpdated() == false) && (Update() == false)) {
    return coordinate;
  }

  // solves for catenary coordinate
  coordinate.x = CoordinateX(position_fraction);
  coordinate.y = CoordinateY(coordinate.x);

  // shifts coordinate origin if requested
  if (is_shifted_origin == true) {
    coordinate.x = coordinate.x - point_end_left_.x;
    coordinate.y = coordinate.y - point_end_left_.y;
  }

  return coordinate;
}

Point2d<double> Catenary2d::CoordinateChord(
    const double& position_fraction,
    const bool& is_shifted_origin) const {
  Point2d<double> coordinate_chord;

  if ((IsUpdated() == false) && (Update() == false)) {
    return coordinate_chord;
  }

  // gets a catenary coordinate to compare to left end point
  Point2d<double> coordinate_catenary = Coordinate(position_fraction, false);

  // calculates a chord coordinate
  coordinate_chord.x = coordinate_catenary.x;
  coordinate_chord.y = point_end_left_.y
                       + ((coordinate_catenary.x - point_end_left_.x)
                       * (spacing_endpoints_.y() / spacing_endpoints_.x()));

  // checks if shifted coordinate is requested, modifies if necessary
  if (is_shifted_origin == true) {
    coordinate_chord.x = coordinate_chord.x - point_end_left_.x;
    coordinate_chord.y = coordinate_chord.y - point_end_left_.y;
  }

  return coordinate_chord;
}

double Catenary2d::Length() const {
  double length = -999999;

  if ((IsUpdated() == false) && (Update() == false)) {
    return length;
  }

  return length_;
}

double Catenary2d::LengthSlack() const {
  if ((IsUpdated() == false) && (Update() == false)) {
    return -999999;
  }

  return length_ - spacing_endpoints().Magnitude();
}

double Catenary2d::PositionFraction(const double& tangent_angle) const {
  if ((IsUpdated() == false) && (Update() == false)) {
    return -999999;
  }

  // converts angle to radians and solves for slope
  const double angle_radians =
      units::ConvertAngle(tangent_angle,
                          units::AngleConversionType::kDegreesToRadians);
  const double& slope = std::tan(angle_radians);

  // uses inverse slope function to get x coordinate
  const double& h = tension_horizontal_;
  const double& w = weight_unit_;
  const double x = (h / w) * std::asinh(slope);

  // returns position fraction
  return ConvertToPositionFraction(x);
}

double Catenary2d::PositionFractionOrigin() const {
  if ((IsUpdated() == false) && (Update() == false)) {
    return -999999;
  }

  // gets position along curve of low point, where tangent angle is zero
  return PositionFraction(0);
}

double Catenary2d::PositionFractionSagPoint() const {
  if ((IsUpdated() == false) && (Update() == false)) {
    return -999999;
  }

  double tangent_angle = spacing_endpoints_.Angle(true);

  // gets position fraction of chord tangent line
  return PositionFraction(tangent_angle);
}

double Catenary2d::Sag() const {
  if ((IsUpdated() == false) && (Update() == false)) {
    return -999999;
  }

  // gets sag position
  const double position_fraction_sagpoint = PositionFractionSagPoint();

  // calculates and returns
  return Sag(position_fraction_sagpoint);
}

double Catenary2d::Sag(const double& position_fraction) const {
  // gets catenary coordinate at the position
  const Point2d<double> coordinate_catenary = Coordinate(position_fraction);

  // gets a chord coordinate at the position
  const Point2d<double> coordinate_chord = CoordinateChord(position_fraction);

  // gets height difference between chord and catenary
  return coordinate_chord.y - coordinate_catenary.y;
}

double Catenary2d::TangentAngle(const double& position_fraction,
                                const AxisDirectionType& direction) const {
  if ((IsUpdated() == false) && (Update() == false)) {
    return -999999;
  }

  // gets x coordinate at position
  const double x = CoordinateX(position_fraction);

  // calculates slope at position
  const double slope = std::sinh(x / (tension_horizontal_/weight_unit_));

  // converts to degrees
  double tangent_angle = units::ConvertAngle(
      std::atan(slope),
      units::AngleConversionType::kRadiansToDegrees);

  // adjusts if direction is negative
  if (direction == AxisDirectionType::kNegative) {
    tangent_angle = tangent_angle * -1;
  }

  return tangent_angle;
}

Vector2d Catenary2d::TangentVector(const double& position_fraction,
                                   const AxisDirectionType& direction) const {
  Vector2d tangent_vector;

  if ((IsUpdated() == false) && (Update() == false)) {
    return tangent_vector;
  }

  // gets tangent angle
  const double angle_tangent = TangentAngle(position_fraction, direction);

  // resolves to a unit vector
  const double angle_radians = units::ConvertAngle(
      angle_tangent,
      units::AngleConversionType::kDegreesToRadians);
  if (direction == AxisDirectionType::kNegative) {
    tangent_vector.set_x(-(1 * std::cos(angle_radians)));
    tangent_vector.set_y(std::sin(angle_radians));
  } else if (direction == AxisDirectionType::kPositive) {
    tangent_vector.set_x(std::cos(angle_radians));
    tangent_vector.set_y(std::sin(angle_radians));
  }

  return tangent_vector;
}

double Catenary2d::Tension(const double& position_fraction) const {
  if ((IsUpdated() == false) && (Update() == false)) {
    return -999999;
  }

  // gets x coordinate
  const double x = CoordinateX(position_fraction);

  return tension_horizontal_
         * std::cosh(x / (tension_horizontal_ / weight_unit_));
}

Vector2d Catenary2d::Tension(const double& position_fraction,
                             const AxisDirectionType& direction) const {
  Vector2d vector_tangent;

  if ((IsUpdated() == false) && (Update() == false)) {
    return vector_tangent;
  }

  // gets a unit vector and scales according to tension magnitude
  vector_tangent = TangentVector(position_fraction, direction);
  const double magnitude = Tension(position_fraction);
  vector_tangent.Scale(magnitude);

  return vector_tangent;
}

double Catenary2d::TensionAverage(const int& num_points) const {
  double tension_average = -999999;

  if ((IsUpdated() == false) && (Update() == false)) {
    return tension_average;
  }

  if (num_points == 0) {
    // calculates using Ehrenburg approximation
    const double& h = tension_horizontal_;
    const double& w = weight_unit_;
    const double& l = length_;

    const double k1 = std::pow(h, 2) / (2 * w * l);
    const double k2 = std::sinh(point_end_right_.x / (h / w))
                      * std::cosh(point_end_right_.x / (h / w));
    const double k3 = std::sinh(point_end_left_.x / (h / w))
                      * std::cosh(point_end_left_.x / (h / w));
    const double k4 = (point_end_right_.x - point_end_left_.x) / (h / w);

    tension_average = k1 * (k2 - k3 + k4);

  } else if (0 < num_points) {
    // calculates average tension using evenly spaced points
    double sum = 0;
    const double kNumPoints = num_points;

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

  if ((IsUpdated() == false) && (Update() == false)) {
    return tension_max;
  }

  // selects highest end point and calculate tension
  if (spacing_endpoints_.y() <= 0) {
    return Tension(0);
  } else {
    return Tension(1);
  }
}

bool Catenary2d::Validate(const bool& is_included_warnings,
                          std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "CATENARY 2D";

  // validates tension-horizontal
  if (tension_horizontal_ <= 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid horizontal tension";
      messages->push_back(message);
    }
  }

  // validates weight-unit
  if (weight_unit_ <= 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid unit weight";
      messages->push_back(message);
    }
  }

  // validates spacing-endpoints-horizontal
  if (spacing_endpoints_.x() <= 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid horizontal endpoint spacing";
      messages->push_back(message);
    }
  }

  // validates spacing-endpoint-vertical
  if (10000 < std::abs(spacing_endpoints_.y())
      || 2000 <= std::abs(spacing_endpoints_.y()) ) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid vertical endpoint spacing";
      messages->push_back(message);
    }
  }

  // returns if errors are present
  if (is_valid == false) {
    return is_valid;
  }

  // validates H/w and spacing combination
  if (ValidateCurveAndSpacing(is_included_warnings,
                              messages) == false) {
    is_valid = false;
  }

  return is_valid;
}

void Catenary2d::set_spacing_endpoints(const Vector2d& spacing_endpoints) {
  spacing_endpoints_ = spacing_endpoints;

  is_updated_ = false;
}

void Catenary2d::set_tension_horizontal(const double& tension_horizontal) {
  tension_horizontal_ = tension_horizontal;

  is_updated_ = false;
}

void Catenary2d::set_weight_unit(const double& weight_unit) {
  weight_unit_ = weight_unit;

  is_updated_ = false;
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

double Catenary2d::ConvertToPositionFraction(const double& x) const {
  // calculates length from origin
  const double length_from_origin = LengthFromOrigin(x);

  // determines length from left end point
  const double length_left_to_position = length_from_origin - length_left_;

  // converts to fraction and returns
  return length_left_to_position / length_;
}

double Catenary2d::CoordinateX(const double& position_fraction) const {
  // converts position fraction to length
  const double length_left_to_position = position_fraction * length_;

  // determines length and direction relative to origin
  // this may seem like the wrong operator (+), but it's not
  // the left length may already be negative based on direction from origin
  const double length_from_origin = length_left_to_position + length_left_;

  // converts to x coordinate and returns
  // uses inverse of the LengthFromOrigin method
  const double& h = tension_horizontal_;
  const double& w = weight_unit_;

  return (h / w) * (std::asinh(length_from_origin / (h / w)));
}

double Catenary2d::CoordinateY(const double& x) const {
  const double& h = tension_horizontal_;
  const double& w = weight_unit_;

  return (h / w) * (std::cosh(x / (h / w)) - 1);
}

bool Catenary2d::IsUpdated() const {
  return is_updated_ == true;
}

double Catenary2d::LengthFromOrigin(const double& x) const {
  const double& h = tension_horizontal_;
  const double& w = weight_unit_;

  return (h / w) * std::sinh(x / (h / w));
}

bool Catenary2d::Update() const {
  // updates end points
  is_updated_ = UpdateEndPoints();
  if (is_updated_ == false) {
    return false;
  }

  // updates lengths
  is_updated_ = UpdateLengths();
  if (is_updated_ == false) {
    return false;
  }

  // if it reaches this point, update was successful
  return true;
}

bool Catenary2d::UpdateEndPoints() const {
  const double& h = tension_horizontal_;
  const double& w = weight_unit_;
  const double& a = spacing_endpoints_.x();
  const double& b = spacing_endpoints_.y();
  const double& z = (a / 2) / (h / w);

  // solves for left endpoint coordinate (Ehrenburg)
  point_end_left_.x = (h / w) * (std::asinh((b * z) / (a * std::sinh(z))) - z);
  point_end_left_.y = CoordinateY(point_end_left_.x);

  // solves for right endpoint coordinate (Ehrenburg)
  point_end_right_.x = (h / w) * (std::asinh((b * z) / (a * std::sinh(z))) + z);
  point_end_right_.y = CoordinateY(point_end_right_.x);

  return true;
}

bool Catenary2d::UpdateLengths() const {
  // solves for length and direction from origin to left end point
  length_left_ = LengthFromOrigin(point_end_left_.x);

  // solves for the length from left to right end point
  length_ = LengthFromOrigin(point_end_right_.x) - length_left_;

  return true;
}

bool Catenary2d::ValidateCurveAndSpacing(
    const bool& /*is_included_warnings*/,
    std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "CATENARY 2D";

  const double constant_minimum =
      ConstantMinimum(spacing_endpoints_.Magnitude());
  const double constant = Constant();

  if (constant < constant_minimum) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid H/w and endpoint spacing combination. "
                            "The H/w must be increased for this spacing";
      messages->push_back(message);
    }
  }

  return is_valid;
}

Catenary3d::Catenary3d() {
  direction_transverse_ = AxisDirectionType::kPositive;
  is_updated_catenary_2d_ = false;
}

Catenary3d::~Catenary3d() {}

double Catenary3d::Constant() const {
  double constant = -999999;

  if ((IsUpdated() == false) && (Update() == false)) {
    return constant;
  }

  return catenary_2d_.Constant();
}

double Catenary3d::ConstantMinimum(const double& spacing_endpoints) {
  return Catenary2d::ConstantMinimum(spacing_endpoints);
}

Point3d<double> Catenary3d::Coordinate(const double& position_fraction) const {
  Point3d<double> coordinate;

  if ((IsUpdated() == false) && (Update() == false)) {
    return coordinate;
  }

  // initializes
  double angle = -999999;

  // gets a 2D curve coordinate
  Point2d<double> coord_2d_curve = catenary_2d_.Coordinate(position_fraction,
                                                           true);

  // gets a 2D chord coordiante
  Point2d<double> coord_2d_chord = catenary_2d_.CoordinateChord(
                                                    position_fraction,
                                                    true);

  // gets a 3D chord coordinate
  Point3d<double> coord_3d_chord = CoordinateChord(position_fraction);

  // creates a vector between 2d chord coordinate and 2d curve coordinate
  Vector3d vector;
  vector.set_x(coord_2d_curve.x - coord_2d_chord.x);
  vector.set_y(0);
  vector.set_z(coord_2d_curve.y - coord_2d_chord.y);

  // rotates vector along xz axis
  const double kAngleXz2d = catenary_2d_.spacing_endpoints().Angle();
  const double kAngleXz3d = spacing_endpoints_.Angle(Plane2dType::kXz);
  angle = kAngleXz3d - kAngleXz2d;
  vector.Rotate(Plane2dType::kXz, angle);

  // rotates vector along yz axis
  angle = weight_unit_.Angle(Plane2dType::kZy);
  if (direction_transverse_ == AxisDirectionType::kNegative) {
    angle = angle * -1;
  }
  vector.Rotate(Plane2dType::kYz, angle);

  // adds rotated vector components to chord coordinate
  coordinate.x = coord_3d_chord.x + vector.x();
  coordinate.y = coord_3d_chord.y + vector.y();
  coordinate.z = coord_3d_chord.z + vector.z();

  return coordinate;
}

Point3d<double> Catenary3d::CoordinateChord(
    const double& position_fraction) const {
  Point3d<double> coordinate;

  if ((IsUpdated() == false) && (Update() == false)) {
    return coordinate;
  }

  // initializes
  double angle = -999999;

  // gets a 2D chord coordiante
  Point2d<double> coord_2d_chord = catenary_2d_.CoordinateChord(
                                                    position_fraction,
                                                    true);

  // creates a vector between BOL coordinate (0,0) and 2d chord coordinate
  Vector3d vector;
  vector.set_x(coord_2d_chord.x - 0);
  vector.set_y(0);
  vector.set_z(coord_2d_chord.y - 0);

  // rotates vector along xz axis
  const double kAngleXz2d = catenary_2d_.spacing_endpoints().Angle();
  const double kAngleXz3d = spacing_endpoints_.Angle(Plane2dType::kXz);
  angle = kAngleXz3d - kAngleXz2d;
  vector.Rotate(Plane2dType::kXz, angle);

  // returns
  coordinate.x = vector.x();
  coordinate.y = vector.y();
  coordinate.z = vector.z();

  return coordinate;
}

double Catenary3d::Length() const {
  double length = -999999;

  if ((IsUpdated() == false) && (Update() == false)) {
    return length;
  }

  return catenary_2d_.Length();
}

double Catenary3d::LengthSlack() const {
  if ((IsUpdated() == false) && (Update() == false)) {
    return -999999;
  }

  return catenary_2d_.LengthSlack();
}

double Catenary3d::PositionFractionOrigin() const {
  if ((IsUpdated() == false) && (Update() == false)) {
    return -999999;
  }

  return catenary_2d_.PositionFractionOrigin();
}

double Catenary3d::PositionFractionSagPoint() const {
  if ((IsUpdated() == false) && (Update() == false)) {
    return -999999;
  }

  return catenary_2d_.PositionFractionSagPoint();
}

double Catenary3d::Sag() const {
  if ((IsUpdated() == false) && (Update() == false)) {
    return -999999;
  }

  return catenary_2d_.Sag();
}

double Catenary3d::Sag(const double& position_fraction) const {
  if ((IsUpdated() == false) && (Update() == false)) {
    return -999999;
  }

  return catenary_2d_.Sag(position_fraction);
}

double Catenary3d::SwingAngle() const {
  double angle_swing = -999999;

  if ((IsUpdated() == false) && (Update() == false)) {
    return -999999;
  }

  angle_swing = units::ConvertAngle(
      std::atan(weight_unit_.y() / weight_unit_.z()),
      units::AngleConversionType::kRadiansToDegrees);
  return angle_swing;
}

double Catenary3d::TangentAngleTransverse(
    const double& position_fraction,
    const AxisDirectionType& direction) const {
  if ((IsUpdated() == false) && (Update() == false)) {
    return -999999;
  }

  // gets the 3D tangent vector
  Vector3d tangent_vector = TangentVector(position_fraction, direction);

  // adjusts components to use only positive values in unit circle
  tangent_vector.set_y(std::abs(tangent_vector.y()));
  tangent_vector.set_z(std::abs(tangent_vector.z()));

  // returns transverse angle
  return tangent_vector.Angle(Plane2dType::kZy, true);
}

double Catenary3d::TangentAngleVertical(
    const double& position_fraction,
    const AxisDirectionType& direction) const {
  if ((IsUpdated() == false) && (Update() == false)) {
    return -999999;
  }

  // gets the 3D tangent vector
  Vector3d tangent_vector = TangentVector(position_fraction, direction);

  // adjusts x component to use only positive values in unit circle
  tangent_vector.set_x(std::abs(tangent_vector.x()));

  // returns vertical angle
  return tangent_vector.Angle(Plane2dType::kXz, true);
}

Vector3d Catenary3d::TangentVector(const double& position_fraction,
                                   const AxisDirectionType& direction) const {
  Vector3d tangent_vector;

  if ((IsUpdated() == false) && (Update() == false)) {
    return tangent_vector;
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
    double angle_endpoints_3d = spacing_endpoints_.Angle(Plane2dType::kXz,
                                                         true);

    tangent_vector.Rotate(Plane2dType::kXz,
                          angle_endpoints_3d - angle_endpoints_2d);
  }

  // rotates 3D vector due to transverse loading
  if (weight_unit_.y() != 0) {
    double angle_rotation = SwingAngle();
    if (direction_transverse_ == AxisDirectionType::kNegative) {
      angle_rotation = angle_rotation * -1;
    }

    tangent_vector.Rotate(Plane2dType::kYz, angle_rotation);
  }

  return tangent_vector;
}

double Catenary3d::Tension(const double& position_fraction) const {
  if ((IsUpdated() == false) && (Update() == false)) {
    return -999999;
  }

  return catenary_2d_.Tension(position_fraction);
}

Vector3d Catenary3d::Tension(const double& position_fraction,
                             const AxisDirectionType& direction) const {
  Vector3d tension_vector;

  if ((IsUpdated() == false) && (Update() == false)) {
    return tension_vector;
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
  if ((IsUpdated() == false) && (Update() == false)) {
    return -999999;
  }

  return catenary_2d_.TensionAverage(num_points);
}

double Catenary3d::TensionMax() const {
  if ((IsUpdated() == false) && (Update() == false)) {
    return -999999;
  }

  return catenary_2d_.TensionMax();
}

bool Catenary3d::Validate(const bool& is_included_warnings,
                          std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "CATENARY 3D";

  // validates spacing-endpoints-horizontal
  if (spacing_endpoints_.x() <= 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid horizontal endpoint spacing";
      messages->push_back(message);
    }
  }

  // validates spacing-endpoints-transverse
  if (spacing_endpoints_.y() != 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid transverse endpoint spacing. It must "
                            "equal zero.";
      messages->push_back(message);
    }
  }

  // validates spacing-endpoint-vertical
  if (10000 < std::abs(spacing_endpoints_.z())
      || 2000 <= std::abs(spacing_endpoints_.z()) ) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid vertical endpoint spacing";
      messages->push_back(message);
    }
  }

  // validates weight-unit-horizontal
  if (weight_unit_.x() != 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid horizontal unit weight. It must "
                            "equal zero";
      messages->push_back(message);
    }
  }

  // validates weight-unit-transverse
  if (weight_unit_.y() < 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid transverse unit weight";
      messages->push_back(message);
    }
  }

  // validates weight-unit-vertical
  if (weight_unit_.z() <= 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid vertical unit weight";
      messages->push_back(message);
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
                            "Catenary2d.";
      messages->push_back(message);
    }
  } else {
    // validates 2D catenary
    if (catenary_2d_.Validate(is_included_warnings, messages) == false) {
      is_valid = false;
    }
  }

  return is_valid;
}

AxisDirectionType Catenary3d::direction_transverse() const {
  return direction_transverse_;
}

void Catenary3d::set_direction_transverse(const AxisDirectionType& direction) {
  direction_transverse_ = direction;
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
  return is_updated_catenary_2d_ == true;
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
  const double v = std::abs(weight_unit_.z());
  const double w = weight_unit_.Magnitude();

  // solves for 2D catenary endpoint spacing
  Vector2d spacing_endpoints_2d;
  spacing_endpoints_2d.set_y(b * (v / w));
  spacing_endpoints_2d.set_x(
      std::sqrt(std::pow(c, 2) - std::pow(spacing_endpoints_2d.y(), 2)));

  // sets catenary spacing and unit weight
  catenary_2d_.set_spacing_endpoints(spacing_endpoints_2d);
  catenary_2d_.set_weight_unit(weight_unit_.Magnitude());

  return true;
}
