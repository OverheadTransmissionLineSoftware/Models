// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef MODELS_TRANSMISSIONLINE_CATENARY_H_
#define MODELS_TRANSMISSIONLINE_CATENARY_H_

#include <list>
#include <string>

#include "models/base/error_message.h"
#include "models/base/point.h"
#include "models/base/vector.h"

/// \par OVERVIEW
///
/// This class models a 2D catenary.
///
/// The shape of the catenary curve is defined by the horizontal tension and
/// unit weight, while the end point spacing defines the portion of the curve
/// that is used.
///
/// \par COORDINATE SYSTEM
///
/// The coordinate system origin (0,0) is the catenary lowpoint, unless
/// shifted. A shifted system uses the left end point as the origin.
///  x = horizontal
///  y = vertical
///
/// \par POSITION FRACTION
///
/// The catenary coordinate system can be difficult to use based on how the
/// origin is defined. To make an easier interface, this class uses a position
/// fraction. It is a decimal number ranging from 0 (left end point) to 1
/// (right end point) that describes the position along the catenary length.
///
/// \par CHORD LINE
///
/// The chord line is the straight line between end points. The chord line
/// is used primarily for determining the sag of the catenary.
class Catenary2d {
 public:
  /// \brief Default constructor.
  Catenary2d();

  /// \brief Destructor.
  ~Catenary2d();

  /// \brief Gets the constant (H/w), which determines the shape of the curve.
  /// \return The constant of the curve.
  double Constant() const;

  /// \brief Gets the minimum allowable constant for the provided end point
  ///   spacing.
  /// \param[in] spacing_endpoints
  ///   The vector magnitude of the end point spacing.
  /// \return The minimum allowable constant.
  static double ConstantMinimum(const double& spacing_endpoints);

  /// \brief Gets a coordinate point at a location on the curve.
  /// \param[in] position_fraction
  ///   The position fraction.
  /// \param[in] is_shifted_origin
  ///   A flag that shifts the coordinate system origin to the left end point.
  /// \return A 2D coordinate point containing the x and y position of the
  ///   catenary curve.
  Point2d<double> Coordinate(const double& position_fraction,
                             const bool& is_shifted_origin = false) const;

  /// \brief Gets a coordinate point on the chord line.
  /// \param[in] position_fraction
  ///   The position fraction.
  /// \param[in] is_shifted_origin
  ///   A flag that shifts the coordinate system origin to the left end point.
  /// \return A 2D coordinate point containing the x and y position of the
  ///   chord line.
  Point2d<double> CoordinateChord(const double& position_fraction,
                                  const bool& is_shifted_origin = false) const;

  /// \brief Gets the curve length between end points.
  /// \return The curve length between end points.
  double Length() const;

  /// \brief Gets the length of slack.
  /// \return The length difference between the curve and chord line.
  double LengthSlack() const;

  /// \brief Gets the position fraction.
  /// \param[in] tangent_angle
  ///   The tangent angle of a specific position on the catenary.
  /// \return The position fraction.
  double PositionFraction(const double& tangent_angle) const;

  /// \brief Gets the position fraction of the origin.
  /// \return The position fraction of the origin, or the lowpoint.
  double PositionFractionOrigin() const;

  /// \brief Gets the position fraction of the sag point.
  /// \return The position fraction of the sag point, or where the tangent line
  ///   of the curve is parallel with the chord line.
  double PositionFractionSagPoint() const;

  /// \brief Gets the maximum sag along the curve.
  /// \return The vertical distance between the chord line and the curve at
  ///   the sag point.
  double Sag() const;

  /// \brief Gets the sag at a specific point.
  /// \param[in] position_fraction
  ///   The position fraction.
  /// \return The vertical distance between the chord line and the curve.
  double Sag(const double& position_fraction) const;

  /// \brief Gets a tangent angle from the horizontal axis to the curve tangent
  ///   line.
  /// \param[in] position_fraction
  ///   The position fraction.
  /// \param[in] direction
  ///   The x-axis direction of the tangent line.
  /// \return A tangent angle.
  double TangentAngle(const double& position_fraction,
                      const AxisDirectionType& direction) const;

  /// \brief Gets a tangent unit vector.
  /// \param[in] position_fraction
  ///   The position fraction.
  /// \param[in] direction
  ///   The x-axis direction of the tangent line.
  /// \return A tangent unit vector.
  Vector2d TangentVector(const double& position_fraction,
                         const AxisDirectionType& direction) const;

  /// \brief Gets the tension.
  /// \param[in] position_fraction
  ///   The position fraction.
  /// \return The tension.
  double Tension(const double& position_fraction) const;

  /// \brief Gets the tension.
  /// \param[in] position_fraction
  ///   The position fraction.
  /// \param[in] direction
  ///   The x-axis direction of the tangent line.
  /// \return A tension vector.
  Vector2d Tension(const double& position_fraction,
                   const AxisDirectionType& direction) const;

  /// \brief Gets the average tension.
  /// \param[in] num_points
  ///   The number of points used to calculate average tension. If zero is
  ///   specified, the calculation technique will use the Ehrenburg
  ///   approximation method.
  /// \return The average tension.
  double TensionAverage(const int& num_points = 100) const;

  /// \brief Gets the maximum tension.
  /// \return The maximum tension, which is at the highest end point.
  double TensionMax() const;

  /// \brief Validates member variables.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings = true,
                std::list<ErrorMessage>* messages = nullptr) const;

  /// \brief Sets the end point spacing.
  /// \param[in] spacing_endpoints
  ///   The vector spacing between end points.
  void set_spacing_endpoints(const Vector2d& spacing_endpoints);

  /// \brief Sets the horizontal tension.
  /// \param[in] tension_horizontal
  ///   The horizontal tension.
  void set_tension_horizontal(const double& tension_horizontal);

  /// \brief Sets the unit weight.
  /// \param[in] weight_unit
  ///   The unit weight.
  void set_weight_unit(const double& weight_unit);

  /// \brief Gets the end point spacing.
  /// \return The end point spacing.
  Vector2d spacing_endpoints() const;

  /// \brief Gets the horizontal tension.
  /// \return The horizontal tension.
  double tension_horizontal() const;

  /// \brief Gets the unit weight.
  /// \return The unit weight.
  double weight_unit() const;

 private:
  /// \brief Converts to position fraction.
  /// \param[in] x
  ///   The x coordinate.
  /// \return The position fraction.
  double ConvertToPositionFraction(const double& x) const;

  /// \brief Gets the x coordinate.
  /// \param[in] position_fraction
  ///   The position fraction.
  /// \return The x coordinate.
  double CoordinateX(const double& position_fraction) const;

  /// \brief Gets the y coordinate.
  /// \param[in] x
  ///   The x coordinate.
  /// \return The y coordinate.
  ///  This function removes the H/w constant ordinate shift.
  double CoordinateY(const double& x) const;

  /// \brief Determines if class is updated.
  /// \return A boolean indicating if class is updated.
  bool IsUpdated() const;

  /// \brief Gets the length from the origin.
  /// \param[in] x
  ///   The x coordinate.
  /// \return The length from the origin to the x coordinate. The sign (+/-)
  ///   indicates the x-axis direction.
  double LengthFromOrigin(const double& x) const;

  /// \brief Updates cached member variables and modifies control variables if
  ///   update is required.
  /// \return A boolean indicating if class updates completed successfully.
  bool Update() const;

  /// \brief Updates the end point coordinates.
  /// \return A boolean indicating the success status of the update.
  bool UpdateEndPoints() const;

  /// \brief Updates the cached lengths.
  /// \return A boolean indicating the success status of the update.
  bool UpdateLengths() const;

  /// \brief Validates whether the H/w is appropriate for the end point spacing.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean indicating whether H/w and end point spacing combination
  ///         is valid.
  bool ValidateCurveAndSpacing(
      const bool& is_included_warnings = true,
      std::list<ErrorMessage>* messages = nullptr) const;

  /// \var is_updated_
  ///   An indicator that tells if the class is updated.
  mutable bool is_updated_;

  /// \var length_
  ///   The length from the left end point to the right end point. This should
  ///   always be positive.
  mutable double length_;

  /// \var length_left_
  ///   The length along the curve from the origin to the left end point. The
  ///   sign (+/-) indicates the x-axis direction.
  mutable double length_left_;

  /// \var point_end_left_
  ///   The coordinate point for the left end of the catenary.
  mutable Point2d<double> point_end_left_;

  /// \var point_end_right_
  ///   The coordinate point for the right end of the catenary.
  mutable Point2d<double> point_end_right_;

  /// \var spacing_endpoints_
  ///   The vector spacing between end points of the catenary. The vector
  ///   starts at the left point and ends at the right point.
  Vector2d spacing_endpoints_;

  /// \var tension_horizontal_
  ///   The horizontal component of tension for the catenary. This is constant
  ///   across the catenary.
  double tension_horizontal_;

  /// \var weight_unit_
  ///   The vertical weight per unit length of the catenary. This is constant
  ///   across the catenary.
  double weight_unit_;
};

/// \par OVERVIEW
///
/// This class models a catenary in 3D.
///
/// This class is a wrapper for the 2D catenary, making its functions
/// applicable for modeling transmission cables by adjusting 2D catenary
/// end point spacing for inclined spans with transverse loading.
///
/// \par COORDINATE SYSTEM
///
/// The coordinate system origin (0,0) is the the left end point.
///  x = horizontal
///  y = transverse
///  z = vertical
///
/// \par POSITION FRACTION
///
/// The catenary coordinate system can be difficult to use based on how the
/// origin is defined. To make an easier interface, this class uses a position
/// fraction. It is a decimal number ranging from 0 (left end point) to 1
/// (right end point) that describes the position along the catenary length.
///
/// \par CHORD LINE
///
/// The chord line is the straight line between end points. The chord line
/// is used primarily for determining the sag of the catenary.
///
/// \par TRANSVERSE LOAD
///
/// Transverse load is specified in the unit weight vector, and the direction
/// is specified separately. The transverse load direction will determine if
/// the catenary will be negative/positive of the y-axis.
class Catenary3d {
 public:
  /// \brief Constructor.
  Catenary3d();

  /// \brief Destructor.
  ~Catenary3d();

  /// \brief Gets the constant (H/w), which determines the shape of the curve.
  /// \return The constant of the curve.
  double Constant() const;

  /// \brief Gets the minimum allowable constant for the provided end point
  ///        spacing.
  /// \param[in] spacing_endpoints
  ///   The vector magnitude of the end point spacing.
  /// \return The minimum allowable constant.
  static double ConstantMinimum(const double& spacing_endpoints);

  /// \brief Gets a coordinate point at a location on the curve.
  /// \param[in] position_fraction
  ///   The position fraction.
  /// \return A 3D coordinate point containing the x, y, and z position of the
  ///   catenary curve relative to the left end point (which is the origin).
  Point3d<double> Coordinate(const double& position_fraction) const;

  /// \brief Gets a coordinate point on the chord line.
  /// \param[in] position_fraction
  ///   The position fraction.
  /// \return A 3D coordinate point containing the x, y, and z position of the
  ///   chord line.
  Point3d<double> CoordinateChord(const double& position_fraction) const;

  /// \brief Gets the curve length.
  /// \return The curve length between end points.
  double Length() const;

  /// \brief Gets the length of slack.
  /// \return The length difference between the curve and chord line.
  double LengthSlack() const;

  /// \brief Gets the position fraction of the origin.
  /// \return The position fraction of the origin, or the lowpoint.
  double PositionFractionOrigin() const;

  /// \brief Gets the position fraction of the sag point.
  /// \return The position fraction of the sag point, or where the tangent line
  ///   of the catenary is parallel with the chord line.
  double PositionFractionSagPoint() const;

  /// \brief Gets the sag of the catenary.
  /// \return The distance between the chord line and the curve at the sag
  ///   point.
  double Sag() const;

  /// \brief Gets the sag at a specific point.
  /// \param[in] position_fraction
  ///   The position fraction.
  /// \return The distance between the chord line and the curve.
  double Sag(const double& position_fraction) const;

  /// \brief Gets the swing angle of the catenary due to transverse loading.
  /// \return The swing angle, which is referenced from the vertical plane.
  double SwingAngle() const;

  /// \brief Gets a tangent angle from the vertical axis to the curve tangent
  ///   line.
  /// \param[in] position_fraction
  ///   The position fraction.
  /// \param[in] direction
  ///   The x-axis direction of the tangent line.
  /// \return A tangent angle.
  double TangentAngleTransverse(const double& position_fraction,
                                const AxisDirectionType& direction) const;

  /// \brief Gets a tangent angle from the horizontal axis to the curve tangent
  ///   line.
  /// \param[in] position_fraction
  ///   The position fraction.
  /// \param[in] direction
  ///   The x-axis direction of the tangent line.
  /// \return A tangent angle.
  double TangentAngleVertical(const double& position_fraction,
                              const AxisDirectionType& direction) const;

  /// \brief Gets a tangent unit vector.
  /// \param[in] position_fraction
  ///   The position fraction.
  /// \param[in] direction
  ///   The x-axis direction of the tangent line.
  /// \return A tangent unit vector.
  Vector3d TangentVector(const double& position_fraction,
                         const AxisDirectionType& direction) const;

  /// \brief Gets the tension.
  /// \param[in] position_fraction
  ///   The position fraction.
  /// \return The tension.
  double Tension(const double& position_fraction) const;

  /// \brief Gets the tension.
  /// \param[in] position_fraction
  ///   The position fraction.
  /// \param[in] direction
  ///   The x-axis direction of the tangent line.
  /// \return A tension vector.
  Vector3d Tension(const double& position_fraction,
                   const AxisDirectionType& direction) const;

  /// \brief Gets the average tension.
  /// \param[in] num_points
  ///   The number of points used to calculate average tension. If zero is
  ///   specified, the calculation technique will use the Ehrenburg
  ///   approximation method.
  /// \return The average tension of the catenary.
  double TensionAverage(const int& num_points = 100) const;

  /// \brief Gets the maximum tension.
  /// \return The maximum tension, which is at the highest end point.
  double TensionMax() const;

  /// \brief Validates member variables.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings = true,
                std::list<ErrorMessage>* messages = nullptr) const;

  /// \brief Gets the direction of the transverse load.
  /// \return The direction of the transverse load.
  AxisDirectionType direction_transverse() const;

  /// \brief Sets the direction of the transverse load.
  /// \param[in] direction
  ///   The direction of the transverse load.
  void set_direction_transverse(const AxisDirectionType& direction);

  /// \brief Sets the end point spacing.
  /// \param[in] spacing_endpoints
  ///   The vector spacing.
  void set_spacing_endpoints(const Vector3d& spacing_endpoints);

  /// \brief Sets the horizontal tension.
  /// \param[in] tension_horizontal
  ///   The horizontal tension.
  void set_tension_horizontal(const double& tension_horizontal);

  /// \brief Sets the unit weight.
  /// \param[in] weight_unit
  ///   The unit weight.
  void set_weight_unit(const Vector3d& weight_unit);

  /// \brief Gets the end point spacing.
  /// \return The end point spacing.
  Vector3d spacing_endpoints() const;

  /// \brief Gets the horizontal tension.
  /// \return The horizontal tension.
  double tension_horizontal() const;

  /// \brief Gets the unit weight.
  /// \return The unit weight.
  Vector3d weight_unit() const;

 protected:
  /// \brief Determines if class is updated.
  /// \return A boolean indicating if class is updated.
  bool IsUpdated() const;

  /// \brief Updates cached member variables and modifies control variables if
  ///   update is required.
  /// \return A boolean indicating if class updates completed successfully.
  bool Update() const;

  /// \brief Updates the 2D catenary.
  /// \return A boolean indicating the success status of the update.
  bool UpdateCatenary2d() const;

  /// \var catenary_2d_
  ///   A 2D catenary, with only vertical and horizontal axes.
  mutable Catenary2d  catenary_2d_;

  /// \var direction_transverse_
  ///   The direction of the transverse load. This determines if the catenary
  ///   will be located negative/positive of the y-axis when transverse load
  ///   is specified.
  AxisDirectionType direction_transverse_;

  /// \var is_updated_catenary_2d_
  ///   An indicator that tells if the 2D catenary is updated.
  mutable bool is_updated_catenary_2d_;

  /// \var spacing_endpoints_
  ///   The vector spacing between end points. The vector is defined in 3D to
  ///   align with the coordinate system, but only 2D spacing (y axis, z axis)
  ///   are modeled. The vector starts at the left point and ends at the right
  ///   point.
  Vector3d spacing_endpoints_;

  /// \var weight_unit_
  ///   The vector loading per unit length. The vector is defined in 3D to
  ///   align with the coordinate system, but only 2D loading (x axis, z axis)
  ///   are modeled.
  Vector3d weight_unit_;
};

#endif  // MODELS_TRANSMISSIONLINE_CATENARY_H_
