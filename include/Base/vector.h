// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef BASE_VECTOR_H_
#define BASE_VECTOR_H_

/// \par OVERVIEW
///
/// This enum contains types of 2D planes.
enum class Plane2dType {
  kXy,  // XY plane
  kXz,  // XZ plane
  kYx,  // YX plane
  kYz,  // YZ plane
  kZx,  // ZX plane
  kZy,  // ZY plane
};

/// \par OVERVIEW
///
/// This enum contains types of axis directions.
enum class AxisDirectionType {
  kNegative,
  kPositive,
};

/// \par OVERVIEW
///
/// This class is a mathematical vector in a 2D Cartesian coordinate system.
/// Although the members of this class are defined using the Cartesian
/// coordinate system, components of the radial coordinate system (magnitude
/// and angle) are member functions.
///
/// The vector can be manipulated by rotating and scaling.
class Vector2d {
 public:
  /// \brief Default constructor.
  Vector2d();

  /// \brief Alternate constructor.
  /// \param[in] x
  ///   The x component.
  /// \param[in] y
  ///   The y component.
  Vector2d(const double& x, const double& y);

  /// \brief Destructor.
  ~Vector2d();

  /// \brief Gets the angle from the positive x axis, in degrees.
  /// \param[in] is_enabled_negative_angles
  ///   A boolean that indicates whether negative angles are returned when the
  ///   positive angle is between 180-360 degrees (quadrants III and IV).
  /// \return The angle between the positive horizontal axis and the vector, in
  ///   degrees, defined using a counter-clockwise orientation.
  double Angle(const bool& is_enabled_negative_angles = false) const;

  /// \brief Gets the magnitude.
  /// \return Magnitude of the vector.
  double Magnitude() const;

  /// \brief Rotates the vector.
  /// \param[in] angle_rotation
  ///   The angle to rotate the vector, in degrees. Positive values rotate
  ///   counter-clockwise, while negative values rotate clockwise.
  void Rotate(const double& angle_rotation);

  /// \brief Scales the vector by increasing or decreasing the magnitude.
  /// \param[in] factor_scale
  ///   The scaling factor for adjusting the magnitude.
  void Scale(const double& factor_scale);

  /// \brief Sets the x component.
  /// \param[in] x
  ///   The x component.
  void set_x(const double& x);

  /// \brief Sets the y component.
  /// \param[in] y
  ///   The y component.
  void set_y(const double& y);

  /// \brief Gets the x component.
  /// \return The x component.
  double x() const;

  /// \brief Gets the y component.
  /// \return The y component.
  double y() const;

 private:
  /// \var x_
  ///   The x Cartesian component.
  double x_;

  /// \var y_
  ///   The y Cartesian component.
  double y_;
};

/// \par OVERVIEW
///
/// This class is a mathematical vector in a 2D Cartesian coordinate system.
/// Although the members of this class are defined using the Cartesian
/// coordinate system, components of the radial coordinate system (magnitude
/// and angle) are member functions.
///
/// The vector can be manipulated by rotating and scaling.
class Vector3d {
 public:
  /// \brief Default constructor.
  Vector3d();

  /// \brief Alternate constructor.
  /// \param[in] x
  ///   The x component.
  /// \param[in] y
  ///   The y component.
  /// \param[in] z
  ///   The z component.
  Vector3d(const double& x, const double& y, const double& z);

  /// \brief Destructor.
  ~Vector3d();

  /// \brief Gets the angle in a specific plane, in degrees.
  /// \param[in] plane
  ///   The 2D plane which is referenced. The first axis is the horizontal
  ///   axis.
  /// \param[in] is_enabled_negative_angles
  ///   A boolean that indicates whether negative angles are returned when the
  ///   positive angle is between 180-360 degrees (quadrants III and IV).
  /// \return The angle from the 2D plane positive horizontal axis.
  double Angle(const Plane2dType& plane,
               const bool& is_enabled_negative_angle = false) const;

  /// \brief Gets the magnitude.
  /// \return The magnitude, or vector sum of all components.
  double Magnitude() const;

  /// \brief Rotates the vector in a specific plane.
  /// \param[in] plane
  ///   The 2D plane which is referenced. The first axis is the horizontal
  ///   axis.
  /// \param angle_rotation
  ///   The angle to rotate the vector, in degrees. Positive values rotate
  ///   counter-clockwise, while negative values rotate clockwise.
  void Rotate(const Plane2dType& plane, const double& angle_rotation);

  /// \brief Scales the vector by increasing or decreasing the magnitude.
  /// \param[in] factor_scale
  ///   The scaling factor for adjusting the magnitude.
  void Scale(const double& factor_scale);

  /// \brief Sets the x component.
  /// \param[in] x
  ///   The x component.
  void set_x(const double& x);

  /// \brief Sets the y component.
  /// \param[in] y
  ///   The y component.
  void set_y(const double& y);

  /// \brief Sets the z component.
  /// \param[in] z
  ///   The z component.
  void set_z(const double& z);

  /// \brief Gets the x component.
  /// \return The x component.
  double x() const;

  /// \brief Gets the y component.
  /// \return The y component.
  double y() const;

  /// \brief Gets the z component.
  /// \return The z component.
  double z() const;

 private:
  /// \var x_
  ///   The x Cartesian component.
  double x_;

  /// \var y_
  ///   The y Cartesian component.
  double y_;

  /// \var z_
  ///   The z Cartesian component.
  double z_;
};

#endif // BASE_VECTOR_H_
