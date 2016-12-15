// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef OTLS_MODELS_SAGTENSION_LINECABLERELOADER_H_
#define OTLS_MODELS_SAGTENSION_LINECABLERELOADER_H_

#include <list>

#include "models/base/error_message.h"
#include "models/sagtension/cable_elongation_model.h"
#include "models/sagtension/sag_tension_cable.h"
#include "models/transmissionline/catenary.h"
#include "models/transmissionline/line_cable.h"

/// \par OVERVIEW
///
/// This class reloads a line cable to a specified weather case and condition.
/// The process is as follows:
///   - solve for a catenary based on the line cable constraint
///   - build cable models for the constraint, creep-stretch, load-stretch, and
///     reloaded weathercases
///   - determine the stretch load for creep and load conditions
///   - reload the catenary (based on the line cable constraint) at the reloaded
///     weathercase
///
/// \par STRETCH
///
/// This class supports multiple forms of stretch, including:
///   - short term, heavy load stretch
///   - long term, creep stretch
///
/// \par LINE CABLE GEOMETRY
///
/// This class uses the ruling span geometry defined in the line cable.
class LineCableReloader {
 public:
  /// \brief Default constructor.
  LineCableReloader();

  /// \brief Destructor.
  ~LineCableReloader();

  /// \brief Gets a reloaded catenary.
  /// \return A catenary at the reloaded load case.
  Catenary3d CatenaryReloaded() const;

  /// \brief Gets the average load of the cable component.
  /// \param[in] type_component
  ///   The model component type.
  /// \return The average load of the cable component.
  double TensionAverageComponent(
      const CableElongationModel::ComponentType& type_component) const;

  /// \brief Gets the horizontal tension.
  /// \return The horizontal tension.
  double TensionHorizontal() const;

  /// \brief Gets the horizontal tension of the cable component.
  /// \param[in] type_component
  ///   The model component type.
  /// \return The horizontal tension of the cable component.
  double TensionHorizontalComponent(
      const CableElongationModel::ComponentType& type_component) const;

  /// \brief Validates member variables.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings = true,
                std::list<ErrorMessage>* messages = nullptr) const;

  /// \brief Gets the reloaded cable condition.
  /// \return The reloaded cable condition.
  CableConditionType condition_reloaded() const;

  /// \brief Gets the line cable.
  /// \return The line cable.
  const LineCable* line_cable() const;

  /// \brief Sets the reloaded cable condition.
  /// \param[in] condition_reloaded
  ///   The reloaded cable condition.
  void set_condition_reloaded(const CableConditionType& condition_reloaded);

  /// \brief Sets the line cable.
  /// \param[in] line_cable
  ///   The line cable.
  void set_line_cable(const LineCable* line_cable);

  /// \brief Sets the reloaded load case.
  /// \param[in] weathercase_reloaded
  ///   The reloaded weathercase.
  void set_weathercase_reloaded(const WeatherLoadCase* weathercase_reloaded);

  /// \brief Gets the reloaded load case.
  /// \return The reloaded load case.
  const WeatherLoadCase* weathercase_reloaded() const;

 private:
  /// \brief Initializes the models defined in the line cable.
  /// \return The success status of the update.
  /// This function initializes the cable models with the correct temperature
  /// and polynomial, but does not solve for any stretch.
  bool InitializeLineCableModels() const;

  /// \brief Determines if class is updated.
  /// \return A boolean indicating if class is updated.
  bool IsUpdated() const;

  /// \brief Calculates the unit load of the catenary.
  /// \param[in] weathercase
  ///   The weather case.
  /// \return The unit loading of the cable.
  Vector3d UnitLoad(const WeatherLoadCase& weathercase) const;

  /// \brief Updates cached member variables and modifies control variables if
  ///   update is required.
  /// \return A boolean indicating if class updates completed successfully.
  bool Update() const;

  /// \brief Updates the constraint cable model.
  /// \return The success status of the update.
  /// This function further updates the constraint cable model to include any
  /// stretch.
  bool UpdateConstraintCableModel() const;

  /// \brief Updates the constraint catenary.
  /// \return The success status of the update.
  bool UpdateConstraintCatenary() const;

  /// \brief Updates the stretch due to load and creep.
  /// \return The success status of the update.
  bool UpdateLoadStretch() const;

  /// \brief Updates the reloaded cable model.
  /// \return The success status of the update.
  bool UpdateReloadedCableModel() const;

  /// \brief Updates the reloaded catenary.
  /// \return The success status of the update.
  bool UpdateReloadedCatenary() const;

  /// \var cable_sagtension_
  ///   The cable, which is referenced for sag-tension methods.
  SagTensionCable cable_sagtension_;

  /// \var catenary_constraint_
  ///   The catenary for the line cable constraint.
  mutable Catenary3d catenary_constraint_;

  /// \var catenary_reloaded_
  ///   The catenary that is reloaded.
  mutable Catenary3d catenary_reloaded_;

  /// \var condition_reloaded_
  ///   The condition of the cable when loaded at the query load case.
  CableConditionType condition_reloaded_;

  /// \var is_updated_catenary_constraint_
  ///   An indicator that tells if the constraint catenary cable is updated.
  mutable bool is_updated_catenary_constraint_;

  /// \var is_updated_catenary_reloaded_
  ///   An indicator that tells if the query catenary cable is updated.
  mutable bool is_updated_catenary_reloaded_;

  /// \var is_updated_stretch_
  ///   An indicator that tells if the stretch load is updated.
  mutable bool is_updated_stretch_;

  /// \var line_cable_
  ///   The line cable that is being reloaded.
  const LineCable* line_cable_;

  /// \var model_constraint_
  ///   The cable model for the constraint weathercase. This model may contain
  ///   stretch, depending on the constraint condition.
  mutable CableElongationModel model_constraint_;

  /// \var model_creep_
  ///   The cable model used for solving creep stretch. This model will always
  ///   contain zero stretch to ensure that only the polynomial is referenced.
  mutable CableElongationModel model_creep_;

  /// \var model_load_
  ///   The cable model used for solving heavy load stretch. This model will
  ///   always contain zero stretch to ensure that only the polynomial is
  ///   referenced.
  mutable CableElongationModel model_load_;

  /// \var model_reloaded_
  ///   The cable model for the reloaded weathercase. This model may contain
  ///   stretch, depending on the reloaded condition.
  mutable CableElongationModel model_reloaded_;

  /// \var state_stretch_creep_
  ///   The stretch state for the creep condition.
  mutable CableStretchState state_stretch_creep_;

  /// \var state_stretch_load_;
  ///   The stretch state for the load condition.
  mutable CableStretchState state_stretch_load_;

  /// \var weathercase_reloaded_
  ///   The load case that the cable is being reloaded to.
  const WeatherLoadCase* weathercase_reloaded_;
};

#endif  // OTLS_MODELS_SAGTENSION_LINECABLERELOADER_H_
