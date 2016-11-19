// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef OTLS_MODELS_TRANSMISSIONLINE_CATENARYSOLVER_H_
#define OTLS_MODELS_TRANSMISSIONLINE_CATENARYSOLVER_H_

#include <list>

#include "models/base/error_message.h"
#include "models/transmissionline/cable.h"
#include "models/transmissionline/cable_constraint.h"
#include "models/transmissionline/catenary.h"

/// \par OVERVIEW
///
/// This class solves for a catenary.
class CatenarySolver {
 public:
  /// \brief Default constructor.
  CatenarySolver();

  /// \brief Destructor.
  ~CatenarySolver();

  /// \brief Gets the catenary.
  Catenary3d Catenary() const;

  /// \brief Validates member variables.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings = true,
                std::list<ErrorMessage>* messages = nullptr) const;

  /// \brief Gets the cable.
  /// \return The cable.
  const Cable* cable() const;

  /// \brief Gets the constraint.
  /// \return The constraint.
  const CableConstraint* constraint() const;

  /// \brief Sets the cable.
  /// \param[in] cable
  ///   The cable.
  void set_cable(const Cable* cable);

  /// \brief Gets the constraint.
  /// \param[in] constraint
  ///   The constraint.
  void set_constraint(const CableConstraint* constraint);

  /// \brief Sets the attachment spacing.
  /// \param[in] spacing_attachments
  void set_spacing_attachments(const Vector3d* spacing_attachments);

  /// \brief Gets the attachment spacing.
  /// \return The attachment spacing.
  const Vector3d* spacing_attachments() const;

 private:
  /// \brief Determines if class is updated.
  /// \return A boolean indicating if class is updated.
  bool IsUpdated() const;

  /// \brief Updates cached member variables and modifies control variables if
  ///    update is required.
  /// \return A boolean indicating if class updates completed successfully.
  bool Update() const;

  /// \brief Solves and updates catenary horizontal tension by converting from
  ///   the catenary constant.
  /// \return A boolean indicating the success status of the update.
  bool SolveHorizontalTensionFromConstant() const;

  /// \brief Solves and updates catenary horizontal tension by converting from
  ///   sag.
  /// \return A boolean indicating the success status of the update.
  bool SolveHorizontalTensionFromSag() const;

  /// \brief Solves and updates catenary horizontal tension by converting from
  ///   support tension.
  /// \return A boolean indicating the success status of the update.
  bool SolveHorizontalTensionFromSupportTension() const;

  /// \brief Solves and updates the catenary unit weight.
  /// \return A boolean indicating the success status of the update.
  bool SolveWeightUnit() const;

  /// \brief Updates catenary tension and calculates new support tension.
  /// \param[in] tension_horizontal
  ///   The new horizontal tension for the catenary.
  /// \return The support tension correlating to the new horizontal tension of
  ///   the catenary.
  double UpdateCatenaryMaxTension(const double& tension_horizontal) const;

  /// \brief Updates catenary horizontal tension and calculates new sag.
  /// \param[in] tension_horizontal
  ///   The new horizontal tension for the catenary.
  /// \return The sag correlating to the new horizontal tension of the catenary.
  double UpdateCatenarySag(const double& tension_horizontal) const;

  /// \var cable_
  ///   The cable.
  const Cable* cable_;

  /// \var catenary_
  ///   The catenary being solved for.
  mutable Catenary3d catenary_;

  /// \var constraint_
  ///   The constraint.
  const CableConstraint* constraint_;

  /// \var is_updated_catenary_
  ///   An indicator that tells if the catenary is updated.
  mutable bool is_updated_catenary_;

  /// \var spacing_attachments_
  ///   The attachment spacing for the catenary.
  const Vector3d* spacing_attachments_;
};

#endif // OTLS_MODELS_TRANSMISSIONLINE_CATENARYSOLVER_H_
