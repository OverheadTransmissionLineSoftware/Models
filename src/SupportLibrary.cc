// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>


#include <cmath>
#include <string>

#include "include/SupportLibrary.h"

double Round(const double& number, const int& precision)
{
  return round(number * pow(10, precision)) / pow(10, precision);
}

Vector2D::Vector2D()
{
  x_ = -999999;
  y_ = -999999;
}

Vector2D::Vector2D(const double& x, const double& y)
{
  x_ = x;
  y_ = y;
}

Vector2D::~Vector2D()
{}

/**
 * Determine which quadrant (I, II, III, IV) the vector is in, and use the arc
 * tangent function to calculate the angle.
 */
double Vector2D::Angle(const bool& is_enabled_negative_angles) const
{
  double angle;

  // use standard trigonometry to solve for angle of vector
  if (x_ == 0 && 0 < y_) {
    angle = (kPi / 2) * kRadiansToDegrees;
  } else if (x_ == 0 && y_ < 0) {
    angle = 3 * (kPi / 2) * kRadiansToDegrees;
  } else {
    // select quadrant
    if (0 < y_) {
      if (0 < x_) {        // quadrant I
        angle = atan(y_ / x_) * kRadiansToDegrees;
      } else if (x_ < 0) { // quadrant II
        angle = 180 + atan(y_ / x_) * kRadiansToDegrees;
      }
    } else if (y_ <= 0) {
      if (x_ < 0) {        // quadrant III
        angle = 180 + atan(y_ / x_) * kRadiansToDegrees;
      } else if (0 < x_) { // quadrant IV
        angle = 360 + atan(y_ / x_) * kRadiansToDegrees;
      }
    }
  }

  // adjust angle if flag is true and angle is negative
  if (180 < angle && is_enabled_negative_angles == true) {
    angle = angle -360;
  }

  return angle;
}

/**
 * The square root of the sum of the squares.
 */
double Vector2D::Magnitude() const
{
  return sqrt( pow(x_, 2) + pow(y_, 2) );
}

/**
 * The vector is converted into the radial coordinate system (magnitude and
 * angle), rotated, and then vector components are converted back into
 * Cartesian components.
 */
void Vector2D::Rotate(const double& angle_rotation)
{
  // check if rotation angle is zero and exit if so
  if (std::abs(angle_rotation) < 0.00005)
    return;

  // define in terms of radial coordinate system (magnitude and angle)
  const double magnitude = Magnitude();
  const double angle = Angle();

  // calculate new angle (rotation included)
  const double angle_new = angle + angle_rotation;

  // resolve back to new x and y cartesian components
  x_ = magnitude * cos(angle_new * kDegreesToRadians);
  y_ = magnitude * sin(angle_new * kDegreesToRadians);
}

/**
 * The vector components are multiplied by the scale factor.
 */
void Vector2D::Scale(const double& factor_scale)
{
  x_ = x_ * factor_scale;
  y_ = y_ * factor_scale;
}

void Vector2D::set_x(const double& x)
{
  x_ = x;
}

void Vector2D::set_y(const double& y)
{
  y_ = y;
}

double Vector2D::x() const
{
  return x_;
}

double Vector2D::y() const
{
  return y_;
}

Vector3D::Vector3D()
{
  x_ = -999999;
  y_ = -999999;
  z_ = -999999;
}

Vector3D::Vector3D(const double& x, const double& y, const double& z)
{
  x_ = x;
  y_ = y;
  z_ = z;
}

Vector3D::~Vector3D()
{}

/**
 * The planar vector components are assigned to a 2D vector. The angle is
 * calculated using the Vector2D::Angle function.
 * @see Vector2D
 */
double Vector3D::Angle(const Plane2dType& plane,
                       const bool& is_enabled_negative_angles) const
{
  // create a 2D vector of planar components
  Vector2D vector_2d;

  // set vector components based on selected plane
  if (plane == Plane2dType::XY) {
    vector_2d.set_x(x_);
    vector_2d.set_y(y_);
  } else if (plane == Plane2dType::XZ) {
    vector_2d.set_x(x_);
    vector_2d.set_y(z_);
  } else if (plane == Plane2dType::YX) {
    vector_2d.set_x(y_);
    vector_2d.set_y(x_);
  } else if (plane == Plane2dType::YZ) {
    vector_2d.set_x(y_);
    vector_2d.set_y(z_);
  } else if (plane == Plane2dType::ZX) {
    vector_2d.set_x(z_);
    vector_2d.set_y(x_);
  } else if (plane == Plane2dType::ZY) {
    vector_2d.set_x(z_);
    vector_2d.set_y(y_);
  }

  return vector_2d.Angle(is_enabled_negative_angles);
}

/**
 * The square root of the sum of the squares.
 */
double Vector3D::Magnitude() const
{
  return sqrt( pow(x_, 2) + pow(y_, 2) + pow(z_, 2) );
}

/**
 * The planar vector components are assigned to a 2D vector. The rotation is
 * done using the Vector2D::Rotate function.
 * @see Vector2D
 */
void Vector3D::Rotate(const Plane2dType& plane, const double& angle_rotation)
{
  // check if rotation angle is zero
  if (std::abs(angle_rotation) < 0.00005) {
    return;
  }

  // create a 2D vector of the planar components
  Vector2D vector_2d;

  // set vector components based on selected plane
  if (plane == Plane2dType::XY) {
    vector_2d.set_x(x_);
    vector_2d.set_y(y_);
  } else if (plane == Plane2dType::XZ) {
    vector_2d.set_x(x_);
    vector_2d.set_y(z_);
  } else if (plane == Plane2dType::YX) {
    vector_2d.set_x(y_);
    vector_2d.set_y(x_);
  } else if (plane == Plane2dType::YZ) {
    vector_2d.set_x(y_);
    vector_2d.set_y(z_);
  } else if (plane == Plane2dType::ZX) {
    vector_2d.set_x(z_);
    vector_2d.set_y(x_);
  } else if (plane == Plane2dType::ZY) {
    vector_2d.set_x(z_);
    vector_2d.set_y(y_);
  }

  // rotate the plane
  vector_2d.Rotate(angle_rotation);

  // assign rotated 2D vector components to new planar components
  if (plane == Plane2dType::XY) {
    x_ = vector_2d.x();
    y_ = vector_2d.y();
  } else if (plane == Plane2dType::XZ) {
    x_ = vector_2d.x();
    z_ = vector_2d.y();
  } else if (plane == Plane2dType::YX) {
    y_ = vector_2d.x();
    x_ = vector_2d.y();
  } else if (plane == Plane2dType::YZ) {
    y_ = vector_2d.x();
    z_ = vector_2d.y();
  } else if (plane == Plane2dType::ZX) {
    z_ = vector_2d.x();
    x_ = vector_2d.y();
  } else if (plane == Plane2dType::ZY) {
    z_ = vector_2d.x();
    y_ = vector_2d.y();
  }
}

/**
 * The vector components are multiplied by the scale factor.
 */
void Vector3D::Scale(const double& factor_scale)
{
  x_ = x_ * factor_scale;
  y_ = y_ * factor_scale;
  z_ = z_ * factor_scale;
}

void Vector3D::set_x(const double& x)
{
  x_ = x;
}

void Vector3D::set_y(const double& y)
{
  y_ = y;
}

void Vector3D::set_z(const double& z)
{
  z_ = z;
}

double Vector3D::x() const
{
  return x_;
}

double Vector3D::y() const
{
  return y_;
}

double Vector3D::z() const
{
  return z_;
}
