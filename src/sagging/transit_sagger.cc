// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/sagging/transit_sagger.h"

#include <vector>

#include "models/base/helper.h"

TransitSagger::TransitSagger() {
  catenary_ = Catenary3d();
  point_transit_ = Point3d<double>();

  angle_low_ = -999999;
  point_catenary_low_ = Point3d<double>();
  position_low_ = -999999;

  is_updated_ = false;
}

TransitSagger::~TransitSagger() {
}

double TransitSagger::AngleLow() const {
  // updates class if necessary
  if ((IsUpdated() == false) && (Update() == false)) {
    return -999999;
  }

  return angle_low_;
}

double TransitSagger::FactorControl() const {
  // updates class if necessary
  if ((IsUpdated() == false) && (Update() == false)) {
    return -999999;
  }

  // gets the sag at the catenary low point
  const double sag_point_low = catenary_.Sag(position_low_);

  // calculates catenary max sag
  const double sag_max = catenary_.Sag();

  // returns sag fraction percentage
  return sag_point_low / sag_max;
}

Point3d<double> TransitSagger::PointCatenaryLow() const {
  // updates class if necessary
  if ((IsUpdated() == false) && (Update() == false)) {
    return Point3d<double>();
  }

  return point_catenary_low_;
}

Point3d<double> TransitSagger::PointTarget() const {
  // updates class if necessary
  if ((IsUpdated() == false) && (Update() == false)) {
    return Point3d<double>();
  }

  // checks if transit is in x-z plane
  if (point_transit_.y != 0) {
    return Point3d<double>();
  }

  Point3d<double> point;

  // solves x-coordinate by extending to opposite catenary end
  if (point_transit_.x < point_catenary_low_.x) {
    point.x = catenary_.spacing_endpoints().x();
  } else if (point_catenary_low_.x <= point_transit_.x) {
    point.x = 0;
  }

  // no y-coordinate, this is in the x-z plane
  point.y = 0;

  // solves z-coordinate
  point.z = helper::LinearY(point_transit_.x, point_transit_.z,
                            point_catenary_low_.x, point_catenary_low_.z,
                            point.x);

  return point;
}

bool TransitSagger::Validate(const bool& is_included_warnings,
                             std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "TRANSIT SAGGER";

  // validates catenary
  if (catenary_.Validate(is_included_warnings, messages) == false) {
    is_valid = false;
  }

  // validates point-transit-x
  if (point_transit_.x == -999999) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid transit x coordinate";
      messages->push_back(message);
    }
  }

  // validates point-transit-y
  if (point_transit_.y == -999999) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid transit y coordinate";
      messages->push_back(message);
    }
  }

  // validates point-transit-z
  if ((point_transit_.z == -999999)
      || (catenary_.spacing_endpoints().z() < point_transit_.z)) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid transit z coordinate";
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
      message.description = "Error updating class";
      messages->push_back(message);
    }
  }

  return is_valid;
}

Catenary3d TransitSagger::catenary() const {
  return catenary_;
}

Point3d<double> TransitSagger::point_transit() const {
  return point_transit_;
}

void TransitSagger::set_catenary(const Catenary3d& catenary) {
  catenary_ = catenary;
  is_updated_ = false;
}

void TransitSagger::set_point_transit(const Point3d<double>& point_transit) {
  point_transit_ = point_transit;
  is_updated_ = false;
}

double TransitSagger::AngleVertical(const Point3d<double>& point_from,
                                    const Point3d<double>& point_to) const {
  // creates a 2D vector for the horizontal plane
  Vector2d vector_xy;
  vector_xy.set_x(point_to.x - point_from.x);
  vector_xy.set_y(point_to.y - point_from.y);

  // creates a 2D vector to get the vertical angle
  Vector2d vector_vert;
  vector_vert.set_x(vector_xy.Magnitude());
  vector_vert.set_y(point_to.z - point_from.z);

  // returns angle from horizontal plane
  return vector_vert.Angle(true);
}

bool TransitSagger::IsUpdated() const {
  return is_updated_ == true;
}

double TransitSagger::PositionCatenaryLowAngle(
    const double& position_back,
    const double& position_ahead,
    const int& num_points) const {
  // creates a set of position fraction points over the specified range
  const double precision = (position_ahead - position_back) / (num_points - 1);

  std::vector<double> positions(num_points);
  for (int i = 0; i < num_points; i++) {
    positions[i] = position_back  + (i * precision);
  }

  // calculates a set of catenary points for each position
  std::vector<Point3d<double>> points(num_points);
  for (int i = 0; i < num_points; i++) {
    points[i] = catenary_.Coordinate(positions[i]);
  }

  // calculates the angle from horizontal and caches if needed
  double angle_low = 999999;
  int index_low = -9999;
  for (int i = 0; i < num_points; i++) {
    const double angle = AngleVertical(point_transit_, points[i]);
    if (angle < angle_low) {
      angle_low= angle;
      index_low = i;
    }
  }

  return positions[index_low];
}

bool TransitSagger::Update() const {
  // updates the low angle catenary position
  is_updated_ = UpdatePointCatenaryLow();
  if (is_updated_ == false) {
    return false;
  }

  // updates the low angle
  is_updated_ = UpdateAngleLow();
  if (is_updated_ == false) {
    return false;
  }

  // if it reaches this point, update was successful
  return true;
}

bool TransitSagger::UpdateAngleLow() const {
  // updates the angle from transit to catenary
  angle_low_ = AngleVertical(point_transit_, point_catenary_low_);

  // checks angle to make sure transit isn't directly above catenary
  return helper::Round(angle_low_, 0) != -90;
}

bool TransitSagger::UpdatePointCatenaryLow() const {
  // initializes
  const int kNumPoints = 11;
  double position_back = 0.0;
  double position_ahead = 1.0;
  double tolerance = (position_ahead - position_back) / 2;
  double position = -999999;

  int i = 0;
  while ((0.00001 < tolerance) && (i < 10)) {
    // calculates a new position
    position = PositionCatenaryLowAngle(position_back, position_ahead,
                                        kNumPoints);

    // calculates the solution tolerance
    tolerance = (position_ahead - position_back) / (kNumPoints - 1);

    // adjusts the back position
    position_back = position - tolerance;
    if (position_back < 0) {
      position_back = 0;
    }

    // adjusts the ahead position
    position_ahead = position + tolerance;
    if (1 < position_ahead) {
      position_ahead = 1;
    }
  }

  // returns success status
  if ((position == 0) || (position == 1)) {
    // low point wasn't found because the position matches an end point
    position_low_ = -999999;
    point_catenary_low_ = Point3d<double>();
    return false;
  } else {
    position_low_ = position;
    point_catenary_low_ = catenary_.Coordinate(position_low_);
    return true;
  }
}
