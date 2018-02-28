// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef MODELS_TRANSMISSIONLINE_CABLE_POSITION_LOCATOR_H_
#define MODELS_TRANSMISSIONLINE_CABLE_POSITION_LOCATOR_H_

#include <list>
#include <vector>

#include "models/base/point.h"
#include "models/base/vector.h"
#include "models/transmissionline/catenary.h"
#include "models/transmissionline/line_cable.h"
#include "models/transmissionline/transmission_line.h"

/// \par OVERVIEW
///
/// This class solves for the position of a line cable.
///
/// \par COORDINATE SYSTEM
///
/// The coordinate system for this class matches the transmission line
/// coordinate system.
///
/// \par CABLE ATTACHMENT POINTS
///
/// Cable attachment points depend on the connected structure attachment point
/// and hardware orientation. The structure attachment point is determined by
/// the transmission line, while the hardware orientation is solved by finding
/// the static equilibrium of the back and ahead cable loading. The hardware
/// orientation is solved iteratively for all suspension type hardware, and the
/// solution is defined with a spherical coordinate point with an origin that
/// coincides with the structure attachment. The cable attachment points can
/// then be determined by combining the structure attachment point and the
/// hardware spherical point.
///
/// \par CABLE POSITION
///
/// Once the cable attachment points are determined, the cable position is
/// modeled using a catenary. The wind direction and transverse unit loading is
/// assumed perpendicular to the catenary.
class CablePositionLocator {
 public:
  /// \brief Default constructor.
  CablePositionLocator();

  /// \brief Destructor.
  ~CablePositionLocator();

  /// \brief Gets the cable xyz points.
  /// \param[in] index_span
  ///   The span index.
  /// \param[in] num
  ///   The number of points to calculate.
  /// \return A list of xyz points at the cable position.
  std::list<Point3d<double>> PointsCable(const int& index_span,
                                         const int& num) const;

  /// \brief Gets the cable attachment xyz points.
  /// \return The cable attachment points. If the class does not update, a
  ///   nullptr is returned.
  const std::vector<Point3d<double>>* PointsCableAttachment() const;

  /// \brief Validates member variables.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings = true,
                std::list<ErrorMessage>* messages = nullptr) const;

  /// \brief Gets the direction of the wind.
  /// \return The direction of the wind.
  AxisDirectionType direction_wind() const;

  /// \brief Gets the line cable index.
  /// \return The line cable index.
  int index_line_cable() const;

  /// \brief Sets the direction of the wind.
  /// \param[in] direction_wind
  ///   The direction of the wind.
  void set_direction_wind(const AxisDirectionType& direction_wind);

  /// \brief Sets the line cable index.
  /// \param[in] index_line_cable
  ///   The line cable index.
  void set_index_line_cable(const int& index_line_cable);

  /// \brief Sets the horizontal tension.
  /// \param[in] tension_horizontal
  ///   The horizontal tension.
  void set_tension_horizontal(const double& tension_horizontal);

  /// \brief Sets the transmission line.
  /// \param[in] line
  ///   The transmission line.
  void set_transmission_line(const TransmissionLine* line);

  /// \brief Sets the unit weight.
  /// \param[in] weight_unit
  ///   The unit weight.
  void set_weight_unit(const Vector3d* weight_unit);

  /// \brief Gets the horizontal tension.
  /// \return The horizontal tension.
  double tension_horizontal() const;

  /// \brief Gets the transmission line.
  /// \return The transmission line.
  const TransmissionLine* transmission_line() const;

  /// \brief Gets the unit weight.
  /// \return The unit weight.
  const Vector3d* weight_unit() const;

 private:
  /// \brief Updates the containers with connection data.
  /// \return The success status of the update.
  /// This will update the attachment points and hardware datasets.
  bool InitializeConnectionData() const;

  /// \brief Initializes containers that hold the cached data.
  /// \return The success status of the initialization.
  /// This function will clear current containers and then re-allocate memory
  /// for them.
  bool InitializeContainers() const;

  /// \brief Initializes the hardware points at each attachment.
  /// \return The success status of the initialization.
  bool InitializePointsHardware() const;

  /// \brief Determines if class is updated.
  /// \return A boolean indicating if class is updated.
  bool IsUpdated() const;

  /// \brief Solves for the cable position.
  /// \return If the cable position was solved for successfully.
  bool SolveCablePosition() const;

  /// \brief Updates cached member variables and modifies control variables if
  ///    update is required.
  /// \return A boolean indicating if class updates completed successfully.
  bool Update() const;

  /// \brief Updates the cable xyz points.
  /// \return If the update completed successfully.
  bool UpdatePointsCable() const;

  /// \brief Updates the hardware points.
  /// \return The maximum imbalance tension.
  double UpdatePointsHardware() const;

  /// \var direction_wind_
  ///   The wind axis direction, relative to the ahead alignment path. Positive
  ///   indicates from right to left, while negative is the opposite.
  AxisDirectionType direction_wind_;

  /// \var hardwares_
  ///   The hardware assemblies at each attachment/connection.
  mutable std::vector<const Hardware*> hardwares_;

  /// \var index_cable_
  ///   The index of the line cable. The line cable is owned by the transmission
  ///   line.
  int index_cable_;

  /// \var is_updated_
  ///   An indicator that tells if the class is updated or not.
  mutable bool is_updated_;

  /// \var line_
  ///   The transmission line.
  const TransmissionLine* line_;

  /// \var points_cable_
  ///   The cable attachment points, in xyz coordinates.
  mutable std::vector<Point3d<double>> points_cable_;

  /// \var points_hardware_
  ///   The hardware points that define orientation, in spherical coordinates.
  ///   The the origin coincides with the corresponding structure attachment
  ///   point. The radius is the hardware length, the angle from the positive
  ///   x-axis is defined by the bisect angle between the back and ahead
  ///   structure attachments, and the angle from the positive z-axis is defined
  ///   by the static equilibrium solution. The first and last points for the
  ///   dead-end hardware contain all zeroes, but are kept in to align with
  ///   indexes from other containers.
  mutable std::vector<SphericalPoint3d<double>> points_hardware_;

  /// \var points_structure_
  ///   The structure attachment points, in xyz coordinates.
  mutable std::vector<Point3d<double>> points_structure_;

  /// \var size_connections_
  ///   The number of connections on the line cable.
  mutable int size_connections_;

  /// \var tension_horizontal_
  ///   The horizontal tension of the catenary/cable.
  double tension_horizontal_;

  /// \var weight_unit_
  ///   The unit weight of the cable. The x component is not used, the y
  ///   component is transverse weight, and z component is vertical weight.
  const Vector3d* weight_unit_;
};

#endif  // MODELS_TRANSMISSIONLINE_CABLE_POSITION_LOCATOR_H_
