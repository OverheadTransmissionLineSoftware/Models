// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef TRANSMISSIONLINE_CATENARY_H_
#define TRANSMISSIONLINE_CATENARY_H_

#include <list>
#include <string>

#include "include/Base/point.h"
#include "include/Base/vector.h"

/// \par OVERVIEW
///
/// This class models a 2D catenary.
///
/// The shape of the catenary curve is defined by the horizontal tension and
/// unit weight, while the endpoint spacing defines the portion of the curve
/// that is used.
///
/// \par COORDINATE SYSTEM
///
/// The coordinate system origin (0,0) is the catenary lowpoint, unless
/// shifted. A shifted system uses the left endpoint as the origin.
///  x = horizontal
///  y = vertical
class Catenary2D {
 public:
  /// \brief Default constructor.
  Catenary2D();

  /// \brief Destructor.
  ~Catenary2D();

  /// \brief Gets the constant (H/w), which determines the shape of the curve.
  /// \return The constant of the curve.
  double Constant() const;

  /// \brief Gets a coordinate point at a location on the curve.
  /// \param[in] position_fraction
  ///   A decimal ranging from 0 and 1  that describes position on the curve by
  ///   the fraction of curve length from the left end point.
  /// \param[in] is_shifted_origin
  ///   A flag that shifts the coordinate system origin to the left end point.
  /// \return A 2D coordinate point containing the x and y position of the
  ///   catenary curve.
  Point2d Coordinate(const double& position_fraction,
                     const bool& is_shifted_origin = false) const;

  /// \brief Gets a coordinate point at a location on the straight line
  ///   connecting the end points.
  /// \param[in] position_fraction
  ///   A decimal ranging from 0 and 1  that describes position on the curve by
  ///   the fraction of curve length from the left end point.
  /// \param[in] is_shifted_origin
  ///   A flag that shifts the coordinate system origin to the left end point.
  /// \return A 2D coordinate point containing the x and y position of the
  ///   straight line connecting the end points.
  Point2d CoordinateChord(const double& position_fraction,
                          const bool& is_shifted_origin = false) const;

  /// \brief Gets the curve length.
  /// \return The curve length between endpoints.
  double Length() const;

  /// \brief Gets the length of catenary slack.
  /// \return The difference in length between the curve length and straight
  ///   line length.
  double LengthSlack() const;

  /// \brief Gets the position fraction of the origin.
  /// \return The position fraction of the origin, or the lowpoint.
  double PositionFractionOrigin() const;

  /// \brief Gets the position fraction of the sag point.
  /// \return The position fraction of the sag point, or where the tangent line
  ///   of the catenary is parallel with the straight line between endpoints.
  double PositionFractionSagPoint() const;

  /// \brief Gets the sag of the catenary.
  /// \return The vertical distance bewteen the straight line and the catenary
  ///   the sag point.
  double Sag() const;

  /// \brief Gets a tangent angle from the horizontal axis to the curve tangent
  ///   line.
  /// \param[in] position_fraction
  ///   A decimal ranging from 0 and 1  that describes position on the curve by
  ///   the fraction of curve length from the left end point.
  /// \param[in] direction
  ///   The axis direction of the tangent line.
  /// \return A tangent angle.
  double TangentAngle(const double& position_fraction,
                      const AxisDirectionType& direction) const;

  /// \brief Gets a tangent unit vector.
  /// \param[in] position_fraction
  ///   A decimal ranging from 0 and 1  that describes position on the curve by
  ///   the fraction of curve length from the left end point.
  /// \param[in] direction
  ///   The axis direction of the tangent line.
  /// \return A tangent unit vector.
  Vector2d TangentVector(const double& position_fraction,
                         const AxisDirectionType& direction) const;

  /// \brief Gets the tension.
  /// \param[in] position_fraction
  ///   A decimal ranging from 0 and 1  that describes position on the curve by
  ///   the fraction of curve length from the left end point.
  /// \return The tension at the location on the catenary.
  double Tension(const double& position_fraction) const;

  /// \brief Gets the tension.
  /// \param[in] position_fraction
  ///   A decimal ranging from 0 and 1  that describes position on the curve by
  ///   the fraction of curve length from the left end point.
  /// \param[in] direction
  ///   The axis direction of the tangent line.
  /// \return A tension vector.
  Vector2d Tension(const double& position_fraction,
                   const AxisDirectionType& direction) const;

  /// \brief Gets the average catenary tension.
  /// \param[in] num_points
  ///   The number of points used to calculate average tension. If zero is
  ///   specified, the calculation technique will use the Ehrenburg
  ///   approximation method.
  /// \return The average tension of the catenary.
  double TensionAverage(const int& num_points = 100) const;

  /// \brief Gets the maximum tension of the catenary.
  /// \return The maximum tension, which is at the highest end point.
  double TensionMax() const;

  /// \brief Validates member variables.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages_error
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings = true,
                std::list<std::string>* messages_error = nullptr) const;

  /// \brief Sets the endpoint spacing.
  /// \param[in] spacing_endpoints
  ///   The vector spacing.
  void set_spacing_endpoints(const Vector2d& spacing_endpoints);

  /// \brief Sets the horizontal tension.
  /// \param[in] tension_horizontal
  ///   The horizontal tension.
  void set_tension_horizontal(const double& tension_horizontal);

  /// \brief Sets the unit weight.
  /// \param[in] weight_unit
  ///   The unit weight.
  void set_weight_unit(const double& weight_unit);

  /// \brief Gets the endpoint spacing vector.
  /// \return A copy of the endpoint spacing vector.
  Vector2d spacing_endpoints() const;

  /// \brief Gets the horizontal tension.
  /// \return A copy of the horizontal tension.
  double tension_horizontal() const;

  /// \brief Gets the unit weight.
  /// \return The copy of the unit weight.
  double weight_unit() const;

 private:
  /// \brief Gets the x coordinate of the catenary.
  /// \param[in] length_origin_to_position
  ///   The length of the curve from the origin to the position.
  /// \param[in] direction_origin_to_position
  ///   The direction from the origin to the position.
  /// \return The x coordinate for the specified position.
  /// This function removes the H/w constant ordinate shift.
  double CoordinateX(
      const double& length_origin_to_position,
      const AxisDirectionType& direction_origin_to_position) const;

  /// \brief Gets the y coordinate of the catenary.
  /// \param[in] length_origin_to_position
  ///   The length of the curve from the origin to the position.
  /// \param[in] direction_origin_to_position
  ///   The direction from the origin to the position.
  /// \return The y coordinate for the specified position.
  ///  This function removes the H/w constant ordinate shift.
  double CoordinateY(
      const double& length_origin_to_position,
      const AxisDirectionType& direction_origin_to_position) const;

  /// \brief Determines if class is updated.
  /// \return A boolean indicating if class is updated.
  bool IsUpdated() const;

  /// \brief Gets the length from the origin for the specified position.
  /// \param[in] coordinate
  ///   The coordinate for the specified position.
  /// \return The curve length from the catenary origin to the coordinate.
  double LengthFromOrigin(const Point2d& coordinate) const;

  /// \brief Gets the fraction fo the length that relates to a specific
  ///   position on the catenary. This is referenced from the left end point.
  /// \param[in] tangent_angle
  ///   The tangent angle of a speicifc position on the catenary.
  /// \return The fraction of the length for the catenary position, from the
  ///   left end point.
  double PositionFraction(const double& tangent_angle) const;

  /// \brief Updates cached member variables and modifies control variables if
  ///   update is required.
  /// \return A boolean indicating if class updates completed successfully.
  bool Update() const;

  /// \brief Updates the endpoint coordinates.
  /// \return A boolean indicating the success status of the update.
  bool UpdateEndPoints() const;

  /// \var is_updated_points_end_
  ///   An indicator that tells if the end point coordinates are updated.
  mutable bool is_updated_points_end_;

  /// \var point_end_left_
  ///   The coordinate point for the left end of the catenary.
  mutable Point2d point_end_left_;

  /// \var point_end_right_
  ///   The coordinate point for the right end of the catenary.
  mutable Point2d point_end_right_;

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
/// endpoint spacing for inclined spans with transverse loading.
///
/// \par COORDINATE SYSTEM
///
/// The coordinate system origin (0,0) is the catenary lowpoint, unless
/// shifted. A shifted system uses the left endpoint as the origin.
///  x = horizontal
///  y = transverse
///  z = vertical
class Catenary3D {
 public:
  /// @brief Constructor.
  Catenary3D();

  /// @brief Destructor.
  ~Catenary3D();

  /// \brief Gets the constant (H/w), which determines the shape of the curve.
  /// \return The constant of the curve.
  double Constant() const;

  /// \brief Gets a coordinate point at a location on the curve.
  /// \param[in] position_fraction
  ///   A decimal ranging from 0 and 1  that describes position on the curve by
  ///   the fraction of curve length from the left end point.
  /// \param[in] is_shifted_origin
  ///   A flag that shifts the coordinate system origin to the left end point.
  /// \return A 3D coordinate point containing the x, y, and z position of the
  ///   catenary curve.
  Point3d Coordinate(const double& position_fraction,
                     const bool& is_shifted_origin = false) const;

  /// \brief Gets a coordinate point at a location on the straight line
  ///   connecting the end points.
  /// \param[in] position_fraction
  ///   A decimal ranging from 0 and 1  that describes position on the curve by
  ///   the fraction of curve length from the left end point.
  /// \param[in] is_shifted_origin
  ///   A flag that shifts the coordinate system origin to the left end point.
  /// \return A 3D coordinate point containing the x, y, and z position of the
  ///   straight line connecting the end points.
  Point3d CoordinateChord(const double& position_fraction,
                          const bool& is_shifted_origin = false) const;

  /// \brief Gets the curve length.
  /// \return The curve length between endpoints.
  double Length() const;

  /// \brief Gets the length of catenary slack.
  /// \return The difference in length between the curve length and straight-line
  ///    length.
  double LengthSlack() const;

  /// \brief Gets the position fraction of the origin.
  /// \return The position fraction of the origin, or the lowpoint.
  double PositionFractionOrigin() const;

  /// \brief Gets the position fraction of the sag point.
  /// \return The position fraction of the sag point, or where the tangent line
  ///   of the catenary is parallel with the straight line between endpoints.
  double PositionFractionSagPoint() const;

  /// \brief Gets the sag of the catenary.
  /// \return The vertical distance bewteen the straight line and the catenary
  ///   the sag point.
  double Sag() const;

  /// \brief Gets the swing angle of the catenary due to transverse loading.
  /// \return The swing angle, which is referenced from the vertical plane.
  double SwingAngle() const;

  /// \brief Gets a tangent angle from the vertical axis to the curve tangent
  ///   line.
  /// \param[in] position_fraction
  ///   A decimal ranging from 0 and 1  that describes position on the curve by
  ///   the fraction of curve length from the left end point.
  /// \param[in] direction
  ///   The axis direction of the tangent line.
  /// \return A tangent angle.
  double TangentAngleTransverse(const double& position_fraction,
                                const AxisDirectionType& direction) const;

  /// \brief Gets a tangent angle from the horizontal axis to the curve tangent
  ///   line.
  /// \param[in] position_fraction
  ///   A decimal ranging from 0 and 1  that describes position on the curve by
  ///   the fraction of curve length from the left end point.
  /// \param[in] direction
  ///   The axis direction of the tangent line.
  /// \return A tangent angle.
  double TangentAngleVertical(const double& position_fraction,
                              const AxisDirectionType& direction) const;

  /// \brief Gets a tangent unit vector.
  /// \param[in] position_fraction
  ///   A decimal ranging from 0 and 1  that describes position on the curve by
  ///   the fraction of curve length from the left end point.
  /// \param[in] direction
  ///   The axis direction of the tangent line.
  /// \return A tangent unit vector.
  Vector3d TangentVector(const double& position_fraction,
                         const AxisDirectionType& direction) const;

  /// \brief Gets the tension.
  /// \param[in] position_fraction
  ///   A decimal ranging from 0 and 1  that describes position on the curve by
  ///   the fraction of curve length from the left end point.
  /// \return The tension at the location on the catenary.
  double Tension(const double& position_fraction) const;

  /// \brief Gets the tension.
  /// \param[in] position_fraction
  ///   A decimal ranging from 0 and 1  that describes position on the curve by
  ///   the fraction of curve length from the left end point.
  /// \param[in] direction
  ///   The axis direction of the tangent line.
  /// \return A tension vector.
  Vector3d Tension(const double& position_fraction,
                   const AxisDirectionType& direction) const;

  /// \brief Gets the average catenary tension.
  /// \param[in] num_points
  ///   The number of points used to calculate average tension. If zero is
  ///   specified, the calculation technique will use the Ehrenburg
  ///   approximation method.
  /// \return The average tension of the catenary.
  double TensionAverage(const int& num_points = 100) const;

  /// \brief Gets the maximum tension of the catenary.
  /// \return The maximum tension, which is at the highest end point.
  double TensionMax() const;

  /// \brief Validates member variables.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages_error
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings = true,
                std::list<std::string>* messages_error = nullptr) const;

  /// \brief Sets the endpoint spacing.
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

  /// \brief Gets the endpoint spacing vector.
  /// \return A copy of the endpoint spacing vector.
  Vector3d spacing_endpoints() const;

  /// \brief Gets the horizontal tension.
  /// \return A copy of the horizontal tension.
  double tension_horizontal() const;

  /// \brief Gets the unit weight.
  /// \return A copy of the unit weight.
  Vector3d weight_unit() const;

 private:
  /// \brief Determines if class is updated.
  /// \return A boolean indicating if class is updated.
  bool IsUpdated() const;

  /// \brief Updates cached member variables and modifies control variables if
  ///   update is required.
  /// \return A boolean indicating if class updates completed successfully.
  bool Update() const;

  /// \brief Updates the 2D catenary.
  /// \return A boolean indicating the success status of the update.
  bool UpdateCatenary2D() const;

  /// \var catenary_2d_
  ///   A 2D catenary, with only vertical and horizontal axes.
  mutable Catenary2D  catenary_2d_;

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

#endif // TRANSMISSIONLINE_CATENARY_H_
