// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef TLSLIBRARIES_SAGTENSION_LINECABLERELOADER_H_
#define TLSLIBRARIES_SAGTENSION_LINECABLERELOADER_H_

#include <list>
#include <string>

#include "sagtension/cable_elongation_model.h"
#include "sagtension/catenary_cable.h"
#include "sagtension/catenary_cable_component_tension_solver.h"
#include "transmissionline/line_cable.h"

/// \par OVERVIEW
///
/// This class reloads a line cable to a specified load case and condition. The
/// basic process is as follows:
///   - create a catenary cable based on the line cable constraint
///   - determine the stretch at the reloaded condition
///   - reload the catenary cable (based on the line cable constraint) at the
///     reloaded state and loading parameters
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
  /// \return The load the cable was stretched to.
  double LoadStretch() const;

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
  /// \param[in,out] messages_error
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings = true,
                std::list<std::string>* messages_error = nullptr) const;

  /// \brief Gets the reloaded load case.
  /// \return The reloaded load case.
  const WeatherLoadCase* case_reloaded() const;

  /// \brief Gets the stretch load case.
  /// \return The stretch load case.
  const WeatherLoadCase* case_stretch() const;

  /// \brief Gets the reloaded cable condition.
  /// \return The reloaded cable condition.
  CableConditionType condition_reloaded() const;

  /// \brief Gets the unloaded unstretched cable length adjustment.
  /// \return The unloaded unstretched cable length adjustment.
  double length_unloaded_unstretched_adjustment() const;

  /// \brief Gets the line cable.
  /// \return The line cable.
  const LineCable* line_cable() const;

  /// \brief Sets the reloaded load case.
  /// \param[in] case_reloaded
  ///   The reloaded load case.
  void set_case_reloaded(const WeatherLoadCase* case_reloaded);

  /// \brief Sets the stretch load case.
  /// \param[in] case_stretch
  ///   The stretch load case.
  void set_case_stretch(const WeatherLoadCase* case_stretch);

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

  /// \brief Sets the type of stretch.
  /// \param[in] type_stretch
  ///   The type of stretch.
  void set_type_stretch(const CableConditionType& type_stretch);

  /// \brief Gets the type of stretch.
  /// \return The type of stretch.
  CableConditionType type_stretch() const;

 private:
  /// \brief Determines if class is updated.
  /// \return A boolean indicating if class is updated.
  bool IsUpdated() const;

  /// \brief Calculates the unit load of the catenary.
  /// \param[in] case_weather
  ///   The weather case.
  /// \return The unit loading of the cable.
  Vector3d UnitLoad(const WeatherLoadCase& case_weather) const;

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

  /// \var case_reloaded_
  ///   The load case that the cable is being reloaded to.
  const WeatherLoadCase* case_reloaded_;

  /// \var case_stretch_
  ///   The load case that defines the cable stretch.
  const WeatherLoadCase* case_stretch_;

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

  /// \var is_updated_load_stretch_
  ///   An indicator that tells if the stretch load is updated.
  mutable bool is_updated_load_stretch_;

  /// \var length_unloaded_unstretched_adjustment_
  ///   The adjustment to the unloaded unstretched length when the cable is
  ///   reloaded.
  mutable double length_unloaded_unstretched_adjustment_;

  /// \var line_cable_
  ///   The line cable that is being reloaded.
  const LineCable* line_cable_;

  /// \var load_stretch_
  ///   The load that the cable is stretched to.
  mutable double load_stretch_;

  /// \var solver_component_tension_
  ///   The catenary cable component tension solver.
  mutable CatenaryCableComponentTensionSolver solver_component_tension_;

  /// \var type_stretch_
  ///   The type of stretch the cable has experienced.
  CableConditionType type_stretch_;
};

#endif  // TLSLIBRARIES_SAGTENSION_LINECABLERELOADER_H_
