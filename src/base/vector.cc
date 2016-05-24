// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/base/vector.h"

#include <cmath>

#include "models/base/units.h"

Vector2d::Vector2d() {
  x_ = -999999;
  y_ = -999999;
}

Vector2d::Vector2d(const double& x, const double& y) {
  x_ = x;
  y_ = y;
}

Vector2d::~Vector2d() {}

/// Determines which quadrant (I,II, III, IV) the vector is in, and uses the
/// arc tangent function to calculate the angle.
double Vector2d::Angle(const bool& is_enabled_negative_angles) const {
  double angle = -999999;

  // uses trigonometry to solve for angle of vector
  if (x_ == 0 || y_ == 0) {

    // The vector lies on an axis, so this returns an angle constant.
    // Ex: For values on the positive horizontal axis (0 degrees), this helps
    // return 0 instead of 359.99999....
    if (x_ == 0) {
      if (0 < y_) {
        angle = units::ConvertAngle(
            units::kPi / 2,
            units::AngleConversionType::kRadiansToDegrees);
      } else if (y_ < 0) {
        angle = units::ConvertAngle(
            3 * (units::kPi / 2),
            units::AngleConversionType::kRadiansToDegrees);
      } else {
        return angle; // both x_ and y_ are zero, returns invalid answer
      }
    } else {  // y_ == 0

      if (0 < x_) {
        angle = 0;
      } else if (x_ < 0) {
        angle = units::ConvertAngle(
            2 * (units::kPi / 2),
            units::AngleConversionType::kRadiansToDegrees);
      } else {
        return angle; // both x_ and y_ are zero, returns invalid answer
      }
    }
  } else {
    // selects the quadrant and calculate using arc tangent function
    if (0 <= y_) {

      if (0 <= x_) {  // quadrant I
        angle = units::ConvertAngle(
            atan(y_ / x_),
            units::AngleConversionType::kRadiansToDegrees);
      } else {  // quadrant II
        angle = 180 - units::ConvertAngle(
            std::abs(atan(y_ / x_)),
            units::AngleConversionType::kRadiansToDegrees);
      }
    } else {
      if (x_ <= 0) {  // quadrant III
        angle = 180 + units::ConvertAngle(
            std::abs(atan(y_ / x_)),
            units::AngleConversionType::kRadiansToDegrees);
      } else {  // quadrant IV
        angle = 360 - units::ConvertAngle(
            std::abs(atan(y_ / x_)),
            units::AngleConversionType::kRadiansToDegrees);
      }
    }
  }

  // adjusts angle if flag is true and angle is negative
  if (180 < angle && is_enabled_negative_angles == true) {
    angle = angle -360;
  }

  return angle;
}

double Vector2d::Magnitude() const {
  return sqrt( pow(x_, 2) + pow(y_, 2) );
}

/// Converts the vector into the radial coordinate system (magnitude and angle)
/// applies the angle rotation, and then converts back into cartesian
/// components.
void Vector2d::Rotate(const double& angle_rotation) {
  // checks if rotation angle is zero (or near zero) and exits if so
  if (std::abs(angle_rotation) < 0.00005)
    return;

  // defines in terms of radial coordinate system (magnitude and angle)
  const double magnitude = Magnitude();
  const double angle = Angle();

  // calculates new angle (rotation included)
  const double angle_new = angle + angle_rotation;

  // resolves back to new x and y cartesian components
  const double angle_radians =
      units::ConvertAngle(angle_new,
                          units::AngleConversionType::kDegreesToRadians);
  x_ = magnitude * cos(angle_radians);
  y_ = magnitude * sin(angle_radians);
}

void Vector2d::Scale(const double& factor_scale) {
  x_ = x_ * factor_scale;
  y_ = y_ * factor_scale;
}

void Vector2d::set_x(const double& x) {
  x_ = x;
}

void Vector2d::set_y(const double& y) {
  y_ = y;
}

double Vector2d::x() const {
  return x_;
}

double Vector2d::y() const {
  return y_;
}

Vector3d::Vector3d() {
  x_ = -999999;
  y_ = -999999;
  z_ = -999999;
}

Vector3d::Vector3d(const double& x, const double& y, const double& z) {
  x_ = x;
  y_ = y;
  z_ = z;
}

Vector3d::~Vector3d() {}

/// The planar vector components are assigned to a 2D vector. The angle is
/// calculated using the Vector2d::Angle function.
/// \see Vector2d
double Vector3d::Angle(const Plane2dType& plane,
                       const bool& is_enabled_negative_angles) const {
  // creates a 2D vector of planar components
  Vector2d vector_2d;

  // sets vector components based on selected plane
  if (plane == Plane2dType::kXy) {
    vector_2d.set_x(x_);
    vector_2d.set_y(y_);
  } else if (plane == Plane2dType::kXz) {
    vector_2d.set_x(x_);
    vector_2d.set_y(z_);
  } else if (plane == Plane2dType::kYx) {
    vector_2d.set_x(y_);
    vector_2d.set_y(x_);
  } else if (plane == Plane2dType::kYz) {
    vector_2d.set_x(y_);
    vector_2d.set_y(z_);
  } else if (plane == Plane2dType::kZx) {
    vector_2d.set_x(z_);
    vector_2d.set_y(x_);
  } else if (plane == Plane2dType::kZy) {
    vector_2d.set_x(z_);
    vector_2d.set_y(y_);
  }

  return vector_2d.Angle(is_enabled_negative_angles);
}

double Vector3d::Magnitude() const {
  return sqrt( pow(x_, 2) + pow(y_, 2) + pow(z_, 2) );
}

/// The planar vector components are assigned to a 2D vector. The rotation is
/// done using the Vector2d::Rotate function.
/// \see Vector2d
void Vector3d::Rotate(const Plane2dType& plane, const double& angle_rotation) {
  // checks if rotation angle is zero
  if (std::abs(angle_rotation) < 0.00005) {
    return;
  }

  // creates a 2D vector of the planar components
  Vector2d vector_2d;

  // sets vector components based on selected plane
  if (plane == Plane2dType::kXy) {
    vector_2d.set_x(x_);
    vector_2d.set_y(y_);
  } else if (plane == Plane2dType::kXz) {
    vector_2d.set_x(x_);
    vector_2d.set_y(z_);
  } else if (plane == Plane2dType::kYx) {
    vector_2d.set_x(y_);
    vector_2d.set_y(x_);
  } else if (plane == Plane2dType::kYz) {
    vector_2d.set_x(y_);
    vector_2d.set_y(z_);
  } else if (plane == Plane2dType::kZx) {
    vector_2d.set_x(z_);
    vector_2d.set_y(x_);
  } else if (plane == Plane2dType::kZy) {
    vector_2d.set_x(z_);
    vector_2d.set_y(y_);
  }

  // rotates the plane
  vector_2d.Rotate(angle_rotation);

  // assigns rotated 2D vector components to new planar components
  if (plane == Plane2dType::kXy) {
    x_ = vector_2d.x();
    y_ = vector_2d.y();
  } else if (plane == Plane2dType::kXz) {
    x_ = vector_2d.x();
    z_ = vector_2d.y();
  } else if (plane == Plane2dType::kYx) {
    y_ = vector_2d.x();
    x_ = vector_2d.y();
  } else if (plane == Plane2dType::kYz) {
    y_ = vector_2d.x();
    z_ = vector_2d.y();
  } else if (plane == Plane2dType::kZx) {
    z_ = vector_2d.x();
    x_ = vector_2d.y();
  } else if (plane == Plane2dType::kZy) {
    z_ = vector_2d.x();
    y_ = vector_2d.y();
  }
}

/// The vector components are multiplied by the scale factor.
void Vector3d::Scale(const double& factor_scale) {
  x_ = x_ * factor_scale;
  y_ = y_ * factor_scale;
  z_ = z_ * factor_scale;
}

void Vector3d::set_x(const double& x) {
  x_ = x;
}

void Vector3d::set_y(const double& y) {
  y_ = y;
}

void Vector3d::set_z(const double& z) {
  z_ = z;
}

double Vector3d::x() const {
  return x_;
}

double Vector3d::y() const {
  return y_;
}

double Vector3d::z() const {
  return z_;
}
