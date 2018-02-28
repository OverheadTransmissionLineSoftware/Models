// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef MODELS_TRANSMISSIONLINE_CATENARY_SOLVER_H_
#define MODELS_TRANSMISSIONLINE_CATENARY_SOLVER_H_

#include <list>

#include "models/base/error_message.h"
#include "models/transmissionline/cable.h"
#include "models/transmissionline/cable_constraint.h"
#include "models/transmissionline/catenary.h"

/// \par OVERVIEW
///
/// This class solves for a catenary.
///
/// \par HORIZONTAL TENSION
///
/// Horizontal tension is the value that is solved for. The remaining catenary
/// parameters (i.e. spacing, unit weight) must be known.
///
/// \par TARGET TYPE/POSITION/VALUE
///
/// The target is the desired result for the catenary. The target type can be:
/// - constant
/// - length
/// - sag
/// - tension
///
/// Some of the results vary along the catenary, so the position must also be
/// specified.
///
/// The target value is directly converted to horizontal tension if possible. If
/// not, the horizontal tension is iterated until it produces the target value.
class CatenarySolver {
 public:
  /// \par OVERVIEW
  ///
  /// This enum contains types of CatenarySolver targets.
  enum class TargetType {
    kConstant,
    kLength,
    kNull,
    kSag,
    kTension
  };

  /// \brief Default constructor.
  CatenarySolver();

  /// \brief Destructor.
  ~CatenarySolver();

  /// \brief Gets the catenary.
  /// \return The catenary.
  Catenary3d Catenary() const;

  /// \brief Gets the catenary horizontal tension.
  /// \return The catenary horizontal tension.
  double TensionHorizontal() const;

  /// \brief Validates member variables.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings = true,
                std::list<ErrorMessage>* messages = nullptr) const;

  /// \brief Gets the target position.
  /// \return The target position.
  double position_target() const;

  /// \brief Sets the target position.
  /// \param[in] position
  ///   The catenary position. If the target type applies to the entire curve,
  ///   and not just a specific point, leave this to the default value.
  void set_position_target(const double& position = -1);

  /// \brief Sets the end point spacing.
  /// \param[in] spacing_endpoints
  ///   The endpoint spacing.
  void set_spacing_endpoints(const Vector3d& spacing_endpoints);

  /// \brief Sets the target type.
  /// \param[in] type
  ///   The target type.
  void set_type_target(const TargetType& type);

  /// \brief Sets the target value.
  /// \param[in] value
  ///   The target value.
  void set_value_target(const double& value);

  /// \brief Sets the unit weight.
  /// \param[in] weight_unit
  ///   The unit weight.
  void set_weight_unit(const Vector3d& weight_unit);

  /// \brief Gets the end point spacing.
  /// \return The end point spacing.
  const Vector3d spacing_endpoints() const;

  /// \brief Gets the target type.
  /// \return The target type.
  TargetType type_target() const;

  /// \brief Gets the target value.
  /// \return The target value.
  double value_target() const;

  /// \brief Gets the unit weight.
  /// \return The unit weight.
  const Vector3d weight_unit() const;

 private:
  /// \brief Determines if class is updated.
  /// \return A boolean indicating if class is updated.
  bool IsUpdated() const;

  /// \brief Solves and updates catenary horizontal tension by converting from
  ///   the catenary constant.
  /// \return A boolean indicating the success status of the update.
  bool SolveHorizontalTensionFromConstant() const;

  /// \brief Solves and updates catenary horizontal tension by converting from
  ///   a length.
  /// \return A boolean indicating the success status of the update.
  bool SolveHorizontalTensionFromLength() const;

  /// \brief Solves and updates catenary horizontal tension by converting from
  ///   sag at a specific position on the catenary.
  /// \return A boolean indicating the success status of the update.
  bool SolveHorizontalTensionFromSag() const;

  /// \brief Solves and updates catenary horizontal tension by converting from
  ///   a tension at a specific position on the catenary.
  /// \return A boolean indicating the success status of the update.
  bool SolveHorizontalTensionFromTension() const;

  /// \brief Updates cached member variables and modifies control variables if
  ///   update is required.
  /// \return A boolean indicating if class updates completed successfully.
  bool Update() const;

  /// \brief Updates catenary horizontal tension and calculates new length.
  /// \param[in] tension_horizontal
  ///   The new horizontal tension for the catenary.
  /// \return The length correlating to the new horizontal tension of the
  ///   catenary.
  double UpdateCatenaryLength(const double& tension_horizontal) const;

  /// \brief Updates catenary horizontal tension and calculates new sag at the
  ///   specified position.
  /// \param[in] tension_horizontal
  ///   The new horizontal tension for the catenary.
  /// \param[in] position_fraction
  ///   The position fraction to calculate the catenary tension at.
  /// \return The sag correlating to the new horizontal tension of the catenary.
  double UpdateCatenarySag(const double& tension_horizontal,
                           const double& position_fraction) const;

  /// \brief Updates catenary horizontal tension and calculates new tension at
  ///   the specified position.
  /// \param[in] tension_horizontal
  ///   The new horizontal tension for the catenary.
  /// \param[in] position_fraction
  ///   The position fraction to calculate the catenary tension at.
  /// \return The tension correlating to the new horizontal tension of the
  ///   catenary.
  double UpdateCatenaryTension(const double& tension_horizontal,
                               const double& position_fraction) const;

  /// \var catenary_
  ///   The catenary being solved for.
  mutable Catenary3d catenary_;

  /// \var is_updated_
  ///   An indicator that tells if the class is updated.
  mutable bool is_updated_;

  /// \var position_target_
  ///   The target position fraction along the catenary curve. If the position
  ///   isn't needed or the maximum value is to be used, set to -1.
  double position_target_;

  /// \var type_target_
  ///   The target type.
  TargetType type_target_;

  /// \var value_target_
  ///   The target value.
  double value_target_;
};

#endif  // MODELS_TRANSMISSIONLINE_CATENARY_SOLVER_H_
