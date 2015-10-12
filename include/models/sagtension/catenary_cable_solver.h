// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef OTLS_MODELS_SAGTENSION_CATENARYCABLESOLVER_H_
#define OTLS_MODELS_SAGTENSION_CATENARYCABLESOLVER_H_

#include <list>
#include <string>

#include "models/sagtension/catenary_cable.h"
#include "models/sagtension/catenary_cable_reloader.h"
#include "models/transmissionline/cable_constraint.h"
#include "models/transmissionline/weather_load_case.h"

/// \par OVERVIEW
///
/// This class solves for a catenary cable.
///
/// \par STRETCHED CONSTRAINT CONDITION
///
/// When the constraint has a stretched condition, the catenary cable must be
/// solved for iteratively.
class CatenaryCableSolver {
 public:
  /// \brief Default constructor.
  CatenaryCableSolver();

  /// \brief Destructor.
  ~CatenaryCableSolver();

  /// \brief Gets the catenary cable with the updated state.
  /// \return The catenary cable with the updated state.
  CatenaryCable GetCatenaryCable() const;

  /// \brief Validates member variables.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages_error
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings = true,
                std::list<std::string>* messages_error = nullptr) const;

  /// \brief Gets the cable.
  /// \return The cable.
  const SagTensionCable* cable() const;

  /// \brief Gets the constraint.
  /// \return The constraint.
  const CableConstraint* constraint() const;

  /// \brief Sets the cable.
  /// \param[in] cable
  ///   The cable.
  void set_cable(const SagTensionCable* cable);

  /// \brief Gets the constraint.
  /// \param[in] constraint
  ///   The constraint.
  void set_constraint(const CableConstraint* constraint);

  /// \brief Sets the attachment spacing.
  /// \param[in] spacing_attachments
  void set_spacing_attachments(const Vector3d* spacing_attachments);

  /// \brief Sets the creep stretch weathercase.
  /// \param[in] weathercase_stretch_creep
  ///   The creep stretch weathercase.
  void set_weathercase_stretch_creep(
      const WeatherLoadCase* weathercase_stretch_creep);

  /// \brief Sets the load stretch weathercase.
  /// \param[in] weathercase_stretch_load
  ///   The load stretch weathercase.
  void set_weathercase_stretch_load(
      const WeatherLoadCase* weathercase_stretch_load);

  /// \brief Gets the attachment spacing.
  /// \return The attachment spacing.
  const Vector3d* spacing_attachments() const;

  /// \brief Gets the creep stretch weather case.
  /// \return The creep stretch weather case.
  const WeatherLoadCase* weathercase_stretch_creep() const;

  /// \brief Gets the load stretch weather case.
  /// \return The load stretch weather case.
  const WeatherLoadCase* weathercase_stretch_load() const;

 private:
  /// \brief Gets the difference between the reloaded and defined stretch load.
  /// \param[in] load_stretch
  ///   The defined stretch load.
  /// \return The difference between the reloaded and defined stretch load.
  /// difference = reloaded - defined
  double LoadStretchDifference(const double& load_stretch) const;

  /// \brief Intializes the reloader.
  /// \return The success status of the update.
  bool InitializeReloader() const;

  /// \brief Determines if class is updated.
  /// \return A boolean indicating if class is updated.
  bool IsUpdated() const;

  /// \brief Solves the stretch load for the catenary cable state.
  /// \return The success status of converging on the stretch load.
  bool SolveStateLoadStretch() const;

  /// \brief Updates cached member variables and modifies control variables if
  ///   update is required.
  /// \return A boolean indicating if class updates completed successfully.
  bool Update() const;

  /// \brief Updates catenary cable.
  /// \return The success status of the update.
  bool UpdateCatenaryCable() const;

  /// \brief Updates the catenary cable state.
  bool UpdateCatenaryCableState(const double& load_stretch) const;

  /// \var cable_
  ///   The cable.
  const SagTensionCable* cable_;

  /// \var constraint_
  ///   The constraint.
  const CableConstraint* constraint_;

  /// \var catenary_cable_
  ///   The catenary cable which contains the solved for state.
  mutable CatenaryCable catenary_cable_;

  /// \var is_updated_catenarycable_
  ///   An indicator that tells if the catenary cable is updated.
  mutable bool is_updated_catenarycable_;

  /// \var reloader_
  ///   The catenary cable reloader.
  mutable CatenaryCableReloader reloader_;

  /// \var spacing_attachments_
  ///   The attachment spacing for the catenary.
  const Vector3d* spacing_attachments_;

  /// \var weathercase_creep_
  ///   The weather case that defines the creep-based stretch.
  const WeatherLoadCase* weathercase_stretch_creep_;

  /// \var weathercase_stretch_
  ///   The weather case that defines the load-based stretch.
  const WeatherLoadCase* weathercase_stretch_load_;
};

#endif  // OTLS_MODELS_SAGTENSION_CATENARYCABLESOLVER_H_
