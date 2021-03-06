// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef MODELS_SAGTENSION_CABLE_ELONGATION_MODEL_H_
#define MODELS_SAGTENSION_CABLE_ELONGATION_MODEL_H_

#include <list>
#include <vector>

#include "models/base/error_message.h"
#include "models/base/point.h"
#include "models/sagtension/cable_component_elongation_model.h"
#include "models/sagtension/cable_state.h"
#include "models/sagtension/sag_tension_cable.h"

/// \par OVERVIEW
///
/// This class models the elongation of a cable.
///
/// The primary features are:
///   - multiple components
///   - non-linear elongation (Experimental Plastic Elongation)
///   - stretch
///   - temperature shift
///
/// \par COMPONENTS
///
/// This class supports up to two cable components, and each is handled
/// indentically. To determine the behavior of the entire cable, the components
/// are summed.
///
/// \par STRETCH
///
/// This class uses components that support a stretched condition. The stretch
/// parameters are defined for the entire cable, and then the stretch is solved
/// individually for each component.
///
/// \par TEMPERATURE
///
/// This class models the cable as being of uniform temperature. The core and
/// shell component temperatures are equal.
///
/// \par COMPRESSION
///
/// This class models a cable where the core and shell components are always at
/// a similar strain. At temperatures other than the component reference
/// temperature and when the coefficient of thermal expansion is different for
/// core and shell components, this may cause compression for a component.
///
/// \par REGIONS
///
/// The cable elongation model may have abrupt changes in elongation behavior.
/// This may be caused by the interaction two different components, if each has
/// significantly different thermal elongation and load-based elongation
/// behavior. This may also be caused by the elongation behavior as the
/// component transitions from stretched to non-stretched, or as the component
/// polynomial becomes invalid. It is necessary to capture the locations where
/// elongation may become abrupt, and doing this allows more easily allows the
/// strain function to determine which region a given load value is in. This
/// makes the numerical solution converge more quickly and consistently.
class CableElongationModel {
 public:
  /// \par OVERVIEW
  ///
  /// This enum contains types of cable elongation model components.
  enum class ComponentType {
    kNull,
    kCombined,
    kCore,
    kShell
  };

  /// \brief Default constructor.
  CableElongationModel();

  /// \brief Destructor.
  ~CableElongationModel();

  /// \brief Gets the load.
  /// \param[in] type_component
  ///   The model component type.
  /// \param[in] strain
  ///   The strain value (x-axis)
  /// \return The load (y-axis).
  double Load(const CableElongationModel::ComponentType& type_component,
              const double& strain) const;

  /// \brief Gets the slope.
  /// \param[in] type_component
  ///   The model component type.
  /// \param[in] strain
  ///   The strain value (x-axis)
  /// \return The slope.
  double Slope(const CableElongationModel::ComponentType& type_component,
                const double& strain) const;

  /// \brief Gets the strain.
  /// \param[in] type_component
  ///   The model component type.
  /// \param[in] load
  ///   The load value (y-axis)
  /// \return The strain (x-axis).
  double Strain(const CableElongationModel::ComponentType& type_component,
                const double& load) const;

  /// \brief Validates member variables.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings,
                std::list<ErrorMessage>* messages = nullptr) const;

  /// \brief Gets the cable.
  /// \return A copy of the cable.
  const SagTensionCable* cable() const;

  /// \brief Sets the cable.
  /// \param[in] cable
  ///   The cable.
  void set_cable(const SagTensionCable* cable);

  /// \brief Sets the state.
  /// \param[in] state
  ///   The state.
  void set_state(const CableState& state);

  /// \brief Sets the stretch state.
  /// \param[in] state_stretch
  ///   The stretch state.
  void set_state_stretch(const CableStretchState& state_stretch);

  /// \brief Gets the state.
  /// \return A copy of the state.
  CableState state() const;

  /// \brief Gets the stretch state.
  /// \return The stretch state.
  CableStretchState state_stretch() const;

 private:
  /// \brief Determines if class is updated.
  /// \return A boolean indicating if class is updated.
  bool IsUpdated() const;

  /// \brief Gets the load of the combined components (entire cable).
  /// \param[in] strain
  ///   The strain value (x-axis).
  /// \return The load of the total cable.
  /// If the component is inactive, the returned load is zero.
  double LoadCombined(const double& strain) const;

  /// \brief Gets the load of the core component.
  /// \param[in] strain
  ///   The strain value (x-axis).
  /// \return The load of the core component.
  /// If the component is inactive, the returned load is zero.
  double LoadCore(const double& strain) const;

  /// \brief Gets the load of the shell component.
  /// \param[in] strain
  ///   The strain value (x-axis).
  /// \return The load of the shell component.
  /// If the component is inactive, the returned load is zero.
  double LoadShell(const double& strain) const;

  /// \brief Gets the slope of the combined components (entire cable).
  /// \param[in] strain
  ///   The strain value (x-axis).
  /// \return The slope of the combined components.
  double SlopeCombined(const double& strain) const;

  /// \brief Gets the slope of the core component.
  /// \param[in] strain
  ///   The strain value (x-axis)
  /// \return The slope of the core component.
  /// If the component is inactive, the returned slope is zero.
  double SlopeCore(const double& strain) const;

  /// \brief Gets the slope of the shell component.
  /// \param[in] strain
  ///   The strain value (x-axis)
  /// \return The slope of the shell component.
  /// If the component is inactive, the returned load is zero.
  double SlopeShell(const double& strain) const;

  /// \brief Gets the strain of the entire cable.
  /// \param[in] load
  ///   The load value (y-axis).
  /// \param[in] precision_decimal_load
  ///   The decimal precision of the numerical solution to the target load.
  /// \return The strain of the combined components (entire cable).
  double StrainCombined(const double& load,
                        const int& precision_decimal_load = 2) const;

  /// \brief Gets the strain of the core component.
  /// \param[in] load
  ///   The load value (y-axis).
  /// \return The strain of the core component.
  /// If the component is inactive, the returned strain is zero.
  double StrainCore(const double& load) const;

  /// \brief Gets the strain of the shell component.
  /// \param[in] load
  ///   The load value (y-axis).
  /// \return The strain of the shell component.
  /// If the component is inactive, the returned strain is zero.
  double StrainShell(const double& load) const;

  /// \brief Updates cached member variables and modifies control variables if
  ///   update is required.
  /// \return A boolean indicating if class updates completed successfully.
  bool Update() const;

  /// \brief Updates the component models to reflect the cable state.
  /// \param[in] state
  ///   The state to update the components to.
  /// \return A boolean indicating the success status of the update.
  /// This function will also call the region points to be updated.
  bool UpdateComponentsState(const CableState& state) const;

  /// \brief Updates the component models to reflect the stretch state, and
  ///   then solves for component stretch.
  /// \return A boolean indicating the success status of the update.
  bool UpdateComponentsStretch() const;

  /// \brief Updates the cached points for the regions in the component models.
  /// \return A boolean indicating the success status of the update.
  bool UpdatePointsRegions() const;

  /// \brief Validates class by checking if conponent strain limits are greater
  ///   than cable rated strength.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool ValidateComponentsStrainLimit(
      const bool& is_included_warnings = true,
      std::list<ErrorMessage>* messages = nullptr) const;

  /// \brief Validates class by checking if unloaded unstretched strain of core
  ///   and shell are similar.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool ValidateComponentsStrainUnloaded(
      const bool& is_included_warnings = true,
      std::list<ErrorMessage>* messages = nullptr) const;

  /// \var cable_
  ///  The cable that is modeled.
  const SagTensionCable* cable_;

  /// \var model_core_
  ///   The elongation model for the core cable component.
  mutable CableComponentElongationModel model_core_;

  /// \var model_shell_
  ///   The elongation model for the shell cable component.
  mutable CableComponentElongationModel model_shell_;

  /// \var is_updated_state_
  ///   An indicator that tells if the state for the component elongation
  ///   models have been updated.
  mutable bool is_updated_state_;

  /// \var is_updated_stretch_
  ///   An indicator that tells if the stretch for the component elongation
  ///   models have been updated.
  mutable bool is_updated_stretch_;

  /// \var points_regions_
  ///   The cached points from the component elongation models that show where
  ///   elongation behavior may be abrupt.
  mutable std::vector<Point2d<double>> points_regions_;

  /// \var state_
  ///   The cable state parameters that are used for calculating load/strain
  ///   values.
  CableState state_;

  /// \var state_stretch_
  ///   The cable state parameters that are used for solving the component
  ///   stretch.
  CableStretchState state_stretch_;

  /// \var state_stretch_core_;
  ///   The cable state parameters that define the core component stretch.
  mutable CableStretchState state_stretch_core_;

  /// \var state_stretch_shell_;
  ///   The cable state parameters that define the shell component stretch.
  mutable CableStretchState state_stretch_shell_;
};

#endif  // MODELS_SAGTENSION_CABLE_ELONGATION_MODEL_H_
