// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/base/point.h"

Point2d::Point2d() {
  x = -999999;
  y = -999999;
}

Point2d::Point2d(const double& x_value, const double& y_value) {
  x = x_value;
  y = y_value;
}

Point3d::Point3d() {
  x = -999999;
  y = -999999;
  z = -999999;
}

Point3d::Point3d(const double& x_value, const double& y_value,
                 const double& z_value) {
  x = x_value;
  y = y_value;
  z = z_value;
}

SphericalPoint3d::SphericalPoint3d() {
  angle_x = -999999;
  angle_z = -999999;
  radius = -999999;
}

SphericalPoint3d::SphericalPoint3d(const double& radius_value,
                                   const double& angle_x_value,
                                   const double& angle_z_value) {
  angle_x = angle_x_value;
  angle_z = angle_z_value;
  radius = radius_value;
}
