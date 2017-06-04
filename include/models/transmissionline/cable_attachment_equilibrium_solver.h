// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef OTLS_MODELS_TRANSMISSIONLINE_CABLEATTACHMENTEQUILIBRIUMSOLVER_H_
#define OTLS_MODELS_TRANSMISSIONLINE_CABLEATTACHMENTEQUILIBRIUMSOLVER_H_

#include <list>

#include "models/base/error_message.h"
#include "models/base/vector.h"
#include "models/transmissionline/catenary.h"

/// \par OVERVIEW
///
/// This class solves for the cable attachment position by finding the static
/// force balance equilibrium.
///
/// \par STATIC EQUILIBRIUM
///
/// This class uses three vectors to model static equilibrium:
/// - Cable: The combined (back and ahead) force of the cable, modeled using
///   catenaries. The orientation of this vector is defined by the catenary
///   tensions. The horizontal tension of the back and ahead catenaries must
///   be equal.
/// - Hardware: The reactive force of the hardware string. The orientation of
///   this is vector is defined by the angle of the hardware assembly.
/// - Imbalance: The difference between the cable and hardware force vectors.
///   This helps determine how close the cable attachment is to static
///   equilibrium.
/// 
/// \par COORDINATE SYSTEM
///
/// The origin coincides with the cable attachment, which is where static
/// equilibrium is determined. The coordinate system xy plane is oriented such
/// that the forces along the x-axis sum to zero. The x-axis bisects the xy
/// angle between the back and and ahead catenaries. For a tangent xy angle
/// (180 degrees), the coordinate system would be as follows:
///  - x = longitudinal
///  - y = transverse
///  - z = vertical
///
/// \par CATENARY DIRECTION
///
/// The catenary direction defines how the back and ahead catenaries extend from
/// the origin, relative to the y-axis.
class CableAttachmentEquilibriumSolver {
 public:
  /// \brief Default constructor.
  CableAttachmentEquilibriumSolver();

  /// \brief Destructor.
  ~CableAttachmentEquilibriumSolver();

  /// \brief Gets the hardware equilibrium angle (in the y-z plane).
  /// \return The hardware equilibrium angle (in the y-z plane).
  /// This angle is measured from the positive z-axis, and will be in the range
  /// of 90-270. If the angle is between 90-180, the equilibrium angle is in the
  /// same side of the y-axis as the catenaries. If between 180-270, the angle
  /// is in the opposite side of the y-axis.
  double AngleEquilibrium() const;

  /// \brief Gets the tension imbalance.
  /// \return The tension imbalance.
  double TensionImbalance() const;

  /// \brief Validates member variables.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings = true,
                std::list<ErrorMessage>* messages = nullptr) const;

  /// \brief Gets the angle between catenaries.
  /// \return The angle between catenaries.
  double angle_catenaries() const;

  /// \brief Gets the angle of the hardware assembly.
  /// \return The angle of the hardware assembly.
  double angle_hardware() const;

  /// \brief Gets the ahead-on-line catenary.
  /// \return The ahead-on-line catenary.
  const Catenary3d* catenary_ahead() const;

  /// \brief Gets the back-on-line catenary.
  /// \return The back-on-line catenary.
  const Catenary3d* catenary_back() const;

  /// \brief Gets the y-axis direction of the catenaries.
  /// \return The y-axis direction of the catenaries.
  AxisDirectionType direction_catenaries() const;

  /// \brief Sets the angle between catenaries.
  /// \param[in] angle
  ///   The angle between the catenaries.
  void set_angle_catenaries(const double& angle);

  /// \brief Sets the hardware angle.
  /// \param[in] angle
  ///   The hardware angle.
  void set_angle_hardware(const double& angle);

  /// \brief Sets the ahead-on-line catenary.
  /// \param[in] catenary
  ///   The catenary.
  void set_catenary_ahead(const Catenary3d* catenary);

  /// \brief Sets the back-on-line catenary.
  /// \param[in] catenary
  ///   The catenary.
  void set_catenary_back(const Catenary3d* catenary);

  /// \brief Sets the y-axis direction of the catenaries.
  /// \param[in] direction_catenaries
  ///   The y-axis direction of the catenaries.
  void set_direction_catenaries(const AxisDirectionType& direction_catenaries);

 private:
  /// \brief Determines if class is updated.
  /// \return A boolean indicating if class is updated.
  bool IsUpdated() const;

  /// \brief Updates cached member variables and modifies control variables if
  ///    update is required.
  /// \return A boolean indicating if class updates completed successfully.
  bool Update() const;

  /// \var angle_catenaries_
  ///   The angle between the back and ahead catenary in the x-y plane, in
  ///   degrees.
  double angle_catenaries_;

  /// \var angle_hardware_
  ///   The y-z plane angle between the vertical z axis and the hardware. The
  ///   angle is in degrees.
  double angle_hardware_;

  /// \var angle_hardware_equilibrium_
  ///   The y-z plane angle between the vertical z axis and the hardware that
  ///   would result in static equilibrium. The angle is in degrees.
  mutable double angle_hardware_equilibrium_;

  /// \var catenary_ahead_
  ///   The ahead-on-line catenary. This catenary produces a tension that has a
  ///   positive x component.
  const Catenary3d* catenary_ahead_;

  /// \var catenary_back_
  ///   The back-on-line catenary. This catenary produces a tension that has a
  ///   negative x component.
  const Catenary3d* catenary_back_;

  /// \var direction_catenaries_
  ///   The y-axis direction of the catenaries, as they extend from the origin.
  AxisDirectionType direction_catenaries_;

  /// \var is_updated_
  ///   An indicator that tells if the class is updated.
  mutable bool is_updated_;

  /// \var tension_cable_
  ///   The combined tension of the cable. This is calculated using the
  ///   catenaries.
  mutable Vector3d tension_cable_;

  /// \var tension_hardware_
  ///   The tension of the hardware using the hardware angle.
  mutable Vector3d tension_hardware_;

  /// \var tension_imbalance_
  ///   The tension imbalance between the cable and hardware.
  mutable Vector3d tension_imbalance_;
};

#endif  // OTLS_MODELS_TRANSMISSIONLINE_CABLEATTACHMENTEQUILIBRIUMSOLVER_H_
