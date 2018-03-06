// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef MODELS_SAGGING_TRANSIT_SAGGER_H_
#define MODELS_SAGGING_TRANSIT_SAGGER_H_

#include <list>

#include "models/base/point.h"
#include "models/transmissionline/catenary.h"

/// \par OVERVIEW
///
/// This class solves for the transit angle to be used for sagging a
/// transmission cable.
///
/// \par COORDINATE SYSTEM
///
/// This class uses the Catenary3d coordinate system. The coordinate system
/// origin (0,0,0) is the the left catenary endpoint.
///  x = horizontal spacing along the catenary (along survey centerline)
///  y = transverse spacing from the catenary (perpendicular to centerline)
///  z = vertical
///
/// \par CABLE / CATENARY
///
/// The transmission cable position is modeled with a catenary. The catenary
/// can model the cable with inclination elevation as well as transverse wind
/// loading.
///
/// \par TRANSIT
///
/// The transit is positioned in 3D space, but must be positioned below the
/// cable to have a valid solution.
///
/// \par ANGLE
///
/// The transit angle is datumed from the horizontal (x-y) plane. The solution
/// is the LOWEST vertical angle from the perspective of the transit to any
/// point on the cable.
///
/// This angle can be used to sag a transmission cable by setting the view of
/// the transit at the specified angle, and then varying the cable tension until
/// the cable position intersects the transit view.
///
/// \par TARGET POINT
///
/// The target point is an extension of the transit angle. It lies along the
/// transit-to-low angle line, at the opposite catenary end point. The target
/// position can only be calculated if the transit is positioned along the x-z
/// plane (i.e. y coordinate = 0).
class TransitSagger {
 public:
  /// \brief Default constructor.
  TransitSagger();

  /// \brief Destructor.
  ~TransitSagger();

  /// \brief Gets the lowest angle from the transit to the catenary.
  /// \return The lowest angle from the transit to the catenary.
  double AngleLow() const;

  /// \brief Gets the control factor for the low angle point.
  /// \return The control factor for the low angle point.
  double FactorControl() const;

  /// \brief Gets the catenary position for the low angle point.
  /// \return The catenary position of the low angle point.
  Point3d<double> PointCatenaryLow() const;

  /// \brief Gets the target point.
  /// \return The target point.
  /// The method only returns a valid result if the transit is located along the
  /// x-z plane (i.e. zero y coordinate).
  Point3d<double> PointTarget() const;

  /// \brief Validates member variables.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings = true,
                std::list<ErrorMessage>* messages = nullptr) const;

  /// \brief Gets the catenary.
  /// \return The catenary.
  Catenary3d catenary() const;

  /// \brief Gets the transit position.
  /// \return The transit position.
  Point3d<double> point_transit() const;

  /// \brief Sets the catenary.
  /// \param[in] catenary
  ///   The catenary.
  void set_catenary(const Catenary3d& catenary);

  /// \brief Sets the transit position.
  /// \param[in] point_transit
  ///   The transit point.
  void set_point_transit(const Point3d<double>& point_transit);

 private:
  /// \brief Gets the vertical angle between the specified points, from the
  ///   horizontal plane.
  /// \param[in] point_from
  ///   The from point.
  /// \param[in] point_to
  ///   The to point.
  /// \return The vertical angle between the specified points, datumed from the
  ///   horizontal plane. This value can be positive or negative.
  double AngleVertical(const Point3d<double>& point_from,
                       const Point3d<double>& point_to) const;

  /// \brief Determines if class is updated.
  /// \return A boolean indicating if class is updated.
  bool IsUpdated() const;

  /// \brief Searches for the catenary position with the lowest angle.
  /// \param[in] position_back
  ///   The back catenary position boundary.
  /// \param[in] position_ahead
  ///   The ahead catenary position boundary.
  /// \param[in] num_points
  ///   The number of points to model within the specified range.
  /// \return The catenary position with the lowest angle.
  double PositionCatenaryLowAngle(const double& position_back,
                                  const double& position_ahead,
                                  const int& num_points) const;

  /// \brief Updates cached member variables and modifies control variables if
  ///    update is required.
  /// \return A boolean indicating if class updates completed successfully.
  bool Update() const;

  /// \brief Updates the lowest angle between the transit and catenary.
  /// \return If the low angle has been successfully updated.
  bool UpdateAngleLow() const;

  /// \brief Updates the lowest angle catenary point.
  /// \return If the lowest angle catenary point has been successfully solved.
  bool UpdatePointCatenaryLow() const;

  /// \var angle_low_
  ///   The lowest angle between the transit and catenary, datumed from the
  ///   horizontal plane. Positive values are above horizontal, while negative
  ///   is below.
  mutable double angle_low_;

  /// \var catenary_
  ///   The hyperbolic curve that models the cable position and tension.
  Catenary3d catenary_;

  /// \var is_updated_
  ///   An indicator that tells if the class has been updated.
  mutable bool is_updated_;

  /// \var point_catenary_low_
  ///   The catenary point that has the lowest angle, from the perspective of
  ///   the transit.
  mutable Point3d<double> point_catenary_low_;

  /// \var point_transit_
  ///   The position of the transit.
  Point3d<double> point_transit_;

  /// \var position_low_
  ///   The catenary position fraction that has the lowest angle, from the
  ///   perspective of the transit.
  mutable double position_low_;
};

#endif  // MODELS_SAGGING_TRANSIT_SAGGER_H_
