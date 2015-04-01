// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef TLSLIBRARIES_SAGTENSION_CABLEELONGATIONMODEL_H_
#define TLSLIBRARIES_SAGTENSION_CABLEELONGATIONMODEL_H_

#include <list>
#include <string>
#include <vector>

#include "base/point.h"
#include "sagtension/cable_component_elongation_model.h"
#include "transmissionline/cable.h"

/// \par OVERVIEW
///
/// This class models the elongation of a cable.
///
/// The primary features are:
///   - Multiple components
///   - Non-linear elongation (Experimental Plastic Elongation)
///   - Stretch
///   - Temperature shift
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
/// load and temperature are defined for the entire cable, and then the stretch
/// is solved individually for each component.
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
/// \par DISCRETE REGIONS
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
  /// \brief Default constructor.
  CableElongationModel();

  /// \brief Destructor.
  ~CableElongationModel();

  /// \brief Gets the load of the core component.
  /// \param[in] strain
  ///   The strain value (x-axis)
  /// \param[in] is_stretched
  ///   An indicator that tells if the cable is stretched.
  /// \return The load of the core component.
  double GetLoadCore(const double& strain, const bool& is_stretched) const;

  /// \brief Gets the load of the shell component.
  /// \param[in] strain
  ///   The strain value (x-axis)
  /// \param[in] is_stretched
  ///   An indicator that tells if the cable is stretched.
  /// \return The load of the shell component.
  double GetLoadShell(const double& strain, const bool& is_stretched) const;

  /// \brief Gets the load of the entire cable.
  /// \param[in] strain
  ///   The strain value (x-axis)
  /// \param[in] is_stretched
  ///   An indicator that tells if the cable is stretched.
  /// \return The load of the entire cable.
  double GetLoadTotal(const double& strain, const bool& is_stretched) const;

  /// \brief Gets the strain of the core component.
  /// \param[in] load
  ///   The load value (y-axis)
  /// \param[in] is_stretched
  ///   An indicator that tells if the cable is stretched.
  /// \return The strain of the core component.
  double GetStrainCore(const double& load, const bool& is_stretched) const;

  /// \brief Gets the strain of the shell component.
  /// \param[in] load
  ///   The load value (y-axis)
  /// \param[in] is_stretched
  ///   An indicator that tells if the cable is stretched.
  /// \return The strain of the shell component.
  double GetStrainShell(const double& load, const bool& is_stretched) const;

  /// \brief Gets the strain of the entire cable.
  /// \param[in] load
  ///   The load value (y-axis)
  /// \param[in] is_stretched
  ///   An indicator that tells if the cable is stretched.
  /// \return The strain of the entire cable.
  double GetStrainTotal(const double& load, const bool& is_stretched) const;

  /// \brief Validates member variables.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages_error
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings,
                std::list<std::string>* messages_error) const;

  /// \brief Gets the cable.
  /// \return A copy of the cable.
  Cable cable() const;

  /// \brief Gets the stretch load.
  /// \return The stretch load.
  double load_stretch() const;

  /// \brief Sets the cable.
  /// \param[in] cable
  ///   The cable.
  void set_cable(const Cable& cable);

  /// \brief Sets the stretch load.
  /// \param[in] load_stretch
  ///   The stretch load.
  void set_load_stretch(const double& load_stretch);

  /// \brief Sets the temperature.
  /// \param[in] temperature
  ///   The temperature of the components.
  void set_temperature(const double& temperature);

  /// \brief Sets the stretch temperature.
  /// \param[in] temperature_stretch
  ///   The stretch temperature.
  void set_temperature_stretch(const double& temperature_stretch);

  /// \brief Gets the temperature.
  /// \return The temperature.
  double temperature() const;

  /// \brief Gets the stretch temperature.
  /// \return The stretch temperature.
  double temperature_stretch() const;

 private:
  /// \brief Determines if class is updated.
  /// \return A boolean indicating if class is updated.
  bool IsUpdated() const;

  /// \brief Gets the load of the core component.
  /// \param[in] strain
  ///   The strain value (x-axis).
  /// \param[in] is_stretched
  ///   A boolean indicating whether the cable condition is unstretched, or
  ///   stretched.
  /// \return The load of the core component.
  /// If the component is inactive, the returned load is zero.
  double LoadCore(const double& strain, const bool& is_stretched) const;

  /// \brief Gets the load of the shell component.
  /// \param[in] strain
  ///   The strain value (x-axis).
  /// \param[in] is_stretched
  ///   A boolean indicating whether the cable condition is unstretched, or
  ///   stretched.
  /// \return The load of the shell component.
  /// If the component is inactive, the returned load is zero.
  double LoadShell(const double& strain, const bool& is_stretched) const;

  /// \brief Gets the load of the total cable.
  /// \param[in] strain
  ///   The strain value (x-axis).
  /// \param[in] is_stretched
  ///   A boolean indicating whether the cable condition is unstretched, or
  ///   stretched.
  /// \return The load of the total cable.
  /// If the component is inactive, the returned load is zero.
  double LoadTotal(const double& strain, const bool& is_stretched) const;

  /// \brief Gets the points for the discrete regions from the component
  ///   elongation models.
  /// \param[in] is_stretched
  ///   An indicator that tells if the cable is stretched.
  /// \return The points for the discrete regions from the component elongation
  ///   models.
  std::vector<Point2d> PointsDiscreteRegions(const bool& is_stretched) const;

  /// \brief Gets the strain of the core component.
  /// \param[in] load
  ///   The load value (y-axis).
  /// \param[in] is_stretched
  ///   A boolean indicating whether the cable condition is unstretched, or
  ///   stretched.
  /// \return The strain of the core component.
  /// If the component is inactive, the returned strain is zero.
  double StrainCore(const double& load, const bool& is_stretched) const;

  /// \brief Gets the strain of the shell component.
  /// \param[in] load
  ///   The load value (y-axis).
  /// \param[in] is_stretched
  ///   A boolean indicating whether the cable condition is unstretched, or
  ///   stretched.
  /// \return The strain of the shell component.
  /// If the component is inactive, the returned strain is zero.
  double StrainShell(const double& load, const bool& is_stretched) const;

  /// \brief Gets the strain of the entire cable.
  /// \param[in] load
  ///   The load value (y-axis).
  /// \param[in] is_stretched
  ///   A boolean indicating whether the cable condition is unstretched, or
  ///   stretched.
  /// \param[in] precision_decimal_load
  ///   The decimal precision of the numerical solution to the target load.
  /// \return The strain of the shell component.
  double StrainTotal(const double& load,
                     const bool& is_stretched,
                     const int& precision_decimal_load = 2) const;

  /// \brief Updates cached member variables and modifies control variables if
  ///   update is required.
  /// \return A boolean indicating if class updates completed successfully.
  bool Update() const;

/// \todo
/// This function may need addressed. Active polynomial does not belong
/// in the Cable struct. May need to develop an inherited SagTensionCable
/// which adds on this member.

/// \todo
/// The whole concept of active components was developed to avoid unnecessary
/// component queries, but also to stop a runtime error of dividing by zero
/// when the modulus (tension/compression) was zero. This should be addressed in
/// the CableComponentElongationModel class. So basically, tagging component
/// models as active may not be required.

  /// \brief Updates which component models are enabled, and what polynomial
  ///   types they are using.
  /// \return A boolean indicating the success status of the update.
/// Based on the active polynomial, the routine searches the component
/// polynomial coefficients. If non-zero values are present, the component
/// is flagged as enabled.
  bool UpdateComponentsEnabled() const;

  /// \brief Updates the stretch load for the components.
  /// \return A boolean indicating the success status of the update.
/// The component models are set to zero stretch load, and are shifted to the
/// stretch temperature. The strain for the entire cable is calculated at the
/// stretch load for the entire cable. The load for each component is
/// calculated and updated.
  bool UpdateComponentsLoadStretch() const;

  /// \brief Updates the cable component struct for each component elongation
  ///   model.
  /// \return A boolean indicating the success status of the update.
  bool UpdateComponentsProperties() const;

  /// \brief Updates the temperature for the component models.
  /// \param[in] temperature
  ///   The temperature to update the component models to.
  /// \return A boolean indicating the success status of the update.
  bool UpdateComponentsTemperature(const double& temperature) const;

  /// \brief Updates the cached points for the discrete regions in the
  ///    component models.
  /// \return A boolean indicating the success status of the update.
  bool UpdatePointsDiscreteRegions() const;

  /// \brief Validates class by checking if conponent strain limits are greater
  ///   than cable rated strength.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages_error
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool ValidateComponentsStrainLimit(
      const bool& is_included_warnings = true,
      std::list<std::string>* messages_error = nullptr) const;

  /// \brief Validates class by checking if unloaded unstretched strain of core
  ///   and shell are similar.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages_error
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool ValidateComponentsStrainUnloaded(
      const bool& is_included_warnings = true,
      std::list<std::string>* messages_error = nullptr) const;

  /// \var cable_
  ///  The cable which will be modeled.
  Cable cable_;

  /// \var component_core_
  ///   The elongation model for the core cable component.
  mutable CableComponentElongationModel component_core_;

  /// \var component_shell_
  ///   The elongation model for the shell cable component.
  mutable CableComponentElongationModel component_shell_;

  /// \var is_enabled_core_
  ///   An indicator that tells if the core elongation model is enabled.
  mutable bool is_enabled_core_;

  /// \var is_enabled_shell_
  ///   An indicator that tells if the shell elongation model is enabled.
  mutable bool is_enabled_shell_;

  /// \todo This variable needs renamed.
  /// \var is_updated_components_properties_
  ///   An indicator that tells if the properties for the component elongation
  ///   models have been updated.
  mutable bool is_updated_components_properties_;

  /// \var is_updated_components_load_stretch_
  ///   An indicator that tells if the stretch load for the component
  ///   elongation models has been updated.
  mutable bool is_updated_components_load_stretch_;

  /// \var is_updated_components_temperature_
  ///   An indicator that tells if the temperature for the component elongation
  ///   models has been updated.
  mutable bool is_updated_components_temperature_;

  /// \var load_stretch_
  ///   The load that the entire cable has been stretched to.
  double load_stretch_;

  /// \var points_discrete_regions_stretched_
  ///   The cached points from the component elongation models that show where
  ///   elongation behavior may be abrupt for a stretched cable.
  mutable std::vector<Point2d> points_discrete_regions_stretched_;

  /// \var points_discrete_regions_unstretched_
  ///   The cached points from the component elongation models that show where
  ///   elongation behavior may be abrupt for an unstretched cable.
  mutable std::vector<Point2d> points_discrete_regions_unstretched_;

  /// \var temperature_
  ///   The temperature of the component elongation models.
  double temperature_;

  /// \var temperature_stretch_
  ///   The temperature that the entire cable was stretched at.
  double temperature_stretch_;
};

#endif // TLSLIBRARIES_SAGTENSION_CABLEELONGATIONMODEL_H_
