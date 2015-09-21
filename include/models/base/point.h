// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef OTLS_MODELS_BASE_POINT_H_
#define OTLS_MODELS_BASE_POINT_H_

/// \par OVERVIEW
///
/// This is a 2D point, with members being of type 'double'.
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
/// This is a 3D point, with members being of type 'double'.
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

#endif // OTLS_MODELS_BASE_POINT_H_
