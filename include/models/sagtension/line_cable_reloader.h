// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef OTLS_MODELS_SAGTENSION_LINECABLERELOADER_H_
#define OTLS_MODELS_SAGTENSION_LINECABLERELOADER_H_

#include <list>

#include "models/base/error_message.h"
#include "models/sagtension/cable_elongation_model.h"
#include "models/sagtension/catenary_cable.h"
#include "models/sagtension/catenary_cable_component_tension_solver.h"
#include "models/sagtension/sag_tension_cable.h"
#include "models/transmissionline/line_cable.h"

/// \par OVERVIEW
///
/// This class reloads a line cable to a specified weather case and condition.
/// The process is as follows:
///   - solve for a catenary cable based on the line cable information
///   - determine the stretch load at all conditions
///   - reload the catenary cable (based on the line cable) at the reloaded
///     state
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

  /// \brief Gets the load the cable was stretched to.
  /// \param[in] condition
  ///   The cable stretch condition.
  /// \return The load the cable was stretched to.
  double LoadStretch(const CableConditionType& condition) const;

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

  /// \brief Gets the unloaded unstretched cable length adjustment.
  /// \return The unloaded unstretched cable length adjustment.
  double length_unloaded_unstretched_adjustment() const;

  /// \brief Gets the line cable.
  /// \return The line cable.
  const LineCable* line_cable() const;

  /// \brief Sets the reloaded cable condition.
  /// \param[in] condition_reloaded
  ///   The reloaded cable condition.
  void set_condition_reloaded(const CableConditionType& condition_reloaded);

  /// \brief Sets the unloaded unstretched cable length adjustment.
  /// \param[in] length_unloaded_unstretched_adjustment
  ///   The unloaded unstretched cable length adjustment.
  void set_length_unloaded_unstretched_adjustment(
      const double& length_unloaded_unstretched_adjustment);

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

  /// \brief Updates catenary cable tension solver.
  /// \return The success status of the update.
  bool UpdateCatenaryCableComponentTensionSolver() const;

  /// \brief Updates the constraint catenary cable.
  /// \return The success status of the update.
  bool UpdateConstraintCatenaryCable() const;

  /// \brief Updates the reloaded state.
  /// \return The success status of the update.
  bool UpdateLoadStretch() const;

  /// \brief Updates the reloaded catenary cable.
  /// \return The success status of the update.
  bool UpdateReloadedCatenaryCable() const;

  /// \var cable_sagtension_
  ///   The cable, which is referenced for sag-tension methods.
  SagTensionCable cable_sagtension_;

  /// \var catenarycable_constraint_
  ///   The catenary cable for the line cable constraint.
  mutable CatenaryCable catenarycable_constraint_;

  /// \var catenarycable_reloaded_
  ///   The catenary cable that is reloaded.
  mutable CatenaryCable catenarycable_reloaded_;

  /// \var condition_reloaded_
  ///   The condition of the cable when loaded at the query load case.
  CableConditionType condition_reloaded_;

  /// \var is_updated_catenarycable_constraint_
  ///   An indicator that tells if the constraint catenary cable is updated.
  mutable bool is_updated_catenarycable_constraint_;

  /// \var is_updated_catenarycable_reloaded_
  ///   An indicator that tells if the query catenary cable is updated.
  mutable bool is_updated_catenarycable_reloaded_;

  /// \var is_updated_stretch_
  ///   An indicator that tells if the stretch load is updated.
  mutable bool is_updated_stretch_;

  /// \var length_unloaded_unstretched_adjustment_
  ///   The adjustment to the unloaded unstretched length when the cable is
  ///   reloaded.
  mutable double length_unloaded_unstretched_adjustment_;

  /// \var line_cable_
  ///   The line cable that is being reloaded.
  const LineCable* line_cable_;

  /// \var load_stretch_creep_
  ///   The stretch load for the creep condition.
  mutable double load_stretch_creep_;

  /// \var load_stretch_load_;
  ///   The stretch load for the load condition.
  mutable double load_stretch_load_;

  /// \var solver_component_tension_
  ///   The catenary cable component tension solver.
  mutable CatenaryCableComponentTensionSolver solver_component_tension_;

  /// \var weathercase_reloaded_
  ///   The load case that the cable is being reloaded to.
  const WeatherLoadCase* weathercase_reloaded_;
};

#endif  // OTLS_MODELS_SAGTENSION_LINECABLERELOADER_H_
