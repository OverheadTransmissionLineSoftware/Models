// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef MODELS_BASE_POINT_H_
#define MODELS_BASE_POINT_H_

/// \par OVERVIEW
///
/// This is a 2D cartesian point.
template <class T>
struct Point2d {
 public:
  /// \brief Default constructor.
  Point2d() {
    x = -999999;
    y = -999999;
  }

  /// \brief Alternate constructor.
  /// \param[in] x_value
  ///   The x value of the point.
  /// \param[in] y_value
  ///   The y value of the point.
  Point2d(const T& x_value, const T& y_value) {
    x = x_value;
    y = y_value;
  }

  /// \var x
  ///   The x value of the point.
  T x;

  /// \var y
  ///   The y value of the point.
  T y;
};

/// \par OVERVIEW
///
/// This is a 3D cartesian point.
template <class T>
struct Point3d {
 public:
  /// \brief Default constructor.
  Point3d() {
    x = -999999;
    y = -999999;
    z = -999999;
  }

  /// \brief Alternate constructor.
  /// \param[in] x_value
  ///   The x value of the point.
  /// \param[in] y_value
  ///   The y value of the point.
  /// \param[in] z_value
  ///   The z value of the point.
  Point3d(const T& x_value, const T& y_value, const T& z_value) {
    x = x_value;
    y = y_value;
    z = z_value;
  }

  /// \var x
  ///   The x value of the point.
  T x;

  /// \var y
  ///   The y value of the point.
  T y;

  /// \var z
  ///   The z value of the point.
  T z;
};

/// \par OVERVIEW
///
/// This is a 3D spherical point.
template <class T>
struct SphericalPoint3d {
 public:
  /// \brief Default constructor.
  SphericalPoint3d() {
    radius = -999999;
    angle_x = -999999;
    angle_z = -999999;
  }

  /// \brief Alternate constructor.
  /// \param[in] radius_value
  ///   The radius.
  /// \param[in] angle_x_value
  ///   The angle from the positive x-axis.
  /// \param[in] angle_z_value
  ///   The angle from the positive z-axis.
  SphericalPoint3d(const T& radius_value, const T& angle_x_value,
                   const T& angle_z_value) {
    radius = radius_value;
    angle_x = angle_x_value;
    angle_z = angle_z_value;
  }

  /// \var radius
  ///   The distance from the origin.
  T radius;

  /// \var angle_x
  ///   The angle of rotation from the positive x-axis, in degrees. The range on
  ///   this angle is 0-360.
  T angle_x;

  /// \var angle_z
  ///   The angle of rotation from the z-axis, in degrees. The range on this
  ///   angle is 0-360.
  T angle_z;
};

#endif  // MODELS_BASE_POINT_H_
