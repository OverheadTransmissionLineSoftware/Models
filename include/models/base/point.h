// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef OTLS_MODELS_BASE_POINT_H_
#define OTLS_MODELS_BASE_POINT_H_

/// \par OVERVIEW
///
/// This is a 2D cartesian point, with members being of type 'double'.
struct Point2d {
 public:
  /// \brief Default constructor.
  Point2d();

  /// \brief Alternate constructor.
  /// \param[in] x_value
  ///   The x value of the point.
  /// \param[in] y_value
  ///   The y value of the point.
  Point2d(const double& x_value, const double& y_value);

  /// \var x
  ///   The x value of the point.
  double x;

  /// \var y
  ///   The y value of the point.
  double y;
};

/// \par OVERVIEW
///
/// This is a 3D cartesian point, with members being of type 'double'.
struct Point3d {
 public:
  /// \brief Default constructor.
  Point3d();

  /// \brief Alternate constructor.
  /// \param[in] x_value
  ///   The x value of the point.
  /// \param[in] y_value
  ///   The y value of the point.
  /// \param[in] z_value
  ///   The z value of the point.
  Point3d(const double& x_value, const double& y_value, const double& z_value);

  /// \var x
  ///   The x value of the point.
  double x;

  /// \var y
  ///   The y value of the point.
  double y;

  /// \var z
  ///   The z value of the point.
  double z;
};

/// \par OVERVIEW
///
/// This is a 3D spherical point, with members being of type 'double'.
struct SphericalPoint3d {
 public:
  /// \brief Default constructor.
  SphericalPoint3d();

  /// \brief Alternate constructor.
  /// \param[in] radius_value
  ///   The radius.
  /// \param[in] angle_x_value
  ///   The angle from the positive x-axis.
  /// \param[in] angle_z_value
  ///   The angle from the positive z-axis.
  SphericalPoint3d(const double& radius_value, const double& angle_x_value,
                   const double& angle_z_value);

  /// \var radius
  ///   The distance from the origin.
  double radius;

  /// \var angle_x
  ///   The angle of rotation from the positive x-axis, in degrees. The range on
  ///   this angle is 0-360.
  double angle_x;

  /// \var angle_z
  ///   The angle of rotation from the z-axis, in degrees. The range on this
  ///   angle is 0-360.
  double angle_z;
};

#endif  // OTLS_MODELS_BASE_POINT_H_
