// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef TRANSMISSIONLINE_SUPPORTLIBRARY_H_
#define TRANSMISSIONLINE_SUPPORTLIBRARY_H_

const double kPi = 3.14159265358979;
const double kRadiansToDegrees = (180 / kPi);
const double kDegreesToRadians = (kPi / 180);

double Round(const double& number, const int& precision);

/**
 * @par POINT 2D OVERVIEW
 *
 * This is a 2D point, with members being of type 'double'.
 */
struct Point2D {
  double x;
  double y;
};

/**
 * @par POINT 3D OVERVIEW
 *
 * This is a 3D point, with members being of type 'double'.
 */
struct Point3D {
  double x;
  double y;
  double z;
};

enum class Plane2D {
  XY,
  XZ,
  YX,
  YZ,
  ZX,
  ZY,
};

enum class AxisDirection {
  NEGATIVE,
  POSITIVE,
};

/**
 * @par VECTOR 2D OVERVIEW
 *
 * This class is a mathematical vector in a 2D Cartesian coordinate system.
 * Although the members of this class are defined using the Cartesian coordinate
 * system, components of the radial coordinate system (magnitude and angle) are
 * member functions.
 *
 * The vector can be manipulated by rotating and scaling.
 */
class Vector2D {
public:
  /**
   * @brief Default constructor.
   */
  Vector2D();

  /**
   * @brief Alternate constructor.
   * @param x vector component
   * @param y vector component
   */
  Vector2D(const double& X, const double& Y);

  /**
   * @brief Destructor.
   */
  ~Vector2D();

  /**
   * @brief Gets the angle from the positive x axis, in degrees.
   * @param is_enabled_negative_angles A boolean that indicates whether
   *        negative angles are returned when the positive angle is between
   *        180-360 degrees (quadrants III and IV).
   * @return The angle between the positive horizontal axis and the vector, in
   *         degrees, defined using a counter-clockwise orientation.
   */
  double Angle(const bool& is_enabled_negative_angles = false) const;

  /**
   * @brief Gets the magnitude.
   * @return Magnitude of the vector.
   */
  double Magnitude() const;

  /**
   * @brief Rotates the vector, in degrees.
   * @param angle_rotation The angle to rotate the vector, in degrees. Positive
   *        values rotate counter-clockwise, while negative values rotate
   *        clockwise.
   */
  void Rotate(const double& angle_rotation);

  /**
   * @brief Scales the vector by increasing or decreasing the magnitude.
   * @param factor_scale The scaling factor for adjusting the magnitude.
   */
  void Scale(const double& factor_scale);

  /**
   * @brief Sets the x component of the vector.
   * @param The x component of the vector.
   */
  void set_x(const double& x);

  /**
   * @brief Sets the y component of the vector.
   * @param The y component of the vector.
   */
  void set_y(const double& y);

  /**
   * @brief Gets the x component of the vector.
   * @return The x component of the vector.
   */
  double x() const;

  /**
   * @brief Gets the y component of the vector.
   * @return The y component of the vector.
   */
  double y() const;

private:
  // member variables
  double x_;
  double y_;
};

/**
 * @par VECTOR 3D OVERVIEW
 *
 * This class is a mathematical vector in a 3D Cartesian coordinate system.
 * Although the members of this class are defined using the Cartesian coordinate
 * system, components of the radial coordinate system (magnitude and angle) are
 * member functions.
 *
 * The vector can be manipulated by rotating and scaling.
 */
class Vector3D {
public:
  /**
   * @brief Default constructor.
   */
  Vector3D();

  /**
   * @brief Alternate constructor.
   * @param x vector component
   * @param y vector component
   * @param z vector component
   */
  Vector3D(const double& x, const double& y, const double& z);

  /**
   * @brief Destructor.
   */
  ~Vector3D();

  /**
   * @brief Get the angle in a specific plane, in degrees.
   * @param plane The 2D plane which is referenced. The first axis is the
   *        horizontal axis.
   * @return The angle from the 2D plane positive horizontal axis.
   */
  double Angle(const Plane2D& plane) const;

  /**
   * @brief Get the magnitude.
   * @return Vector length.
   */
  double Magnitude() const;

  /**
   * @brief Rotate the vector in a specific plane, in degrees.
   * @param plane The 2D plane which is referenced. The first axis is the
   *        horizontal axis.
   * @param angle_rotation The angle to rotate the vector, in degrees. Positive
   *        values rotate counter-clockwise, while negative values rotate
   *        clockwise.
   */
  void Rotate(const Plane2D& plane, const double& angle_rotation);

  /**
   * @brief Scales the vector by increasing or decreasing the magnitude.
   * @param factor_scale The scaling factor for adjusting the magnitude.
   */
  void Scale(const double& factor_scale);

  /**
   * @brief Sets the x component of the vector.
   * @param The x component of the vector.
   */
  void set_x(const double& x);

  /**
   * @brief Sets the y component of the vector.
   * @param The y component of the vector.
   */
  void set_y(const double& y);

  /**
   * @brief Sets the z component of the vector.
   * @param The z component of the vector.
   */
  void set_z(const double& z);

  /**
   * @brief Gets the x component of the vector.
   * @return The x component of the vector.
   */
  double x() const;

  /**
   * @brief Gets the y component of the vector.
   * @return The y component of the vector.
   */
  double y() const;

  /**
   * @brief Gets the z component of the vector.
   * @return The z component of the vector.
   */
  double z() const;

private:
  // member variables
  double x_;
  double y_;
  double z_;

};

#endif // TRANSMISSIONLINE_SUPPORTLIBRARY_H_
