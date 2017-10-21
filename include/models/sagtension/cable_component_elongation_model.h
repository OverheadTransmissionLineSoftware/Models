// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef OTLS_MODELS_SAGTENSION_CABLECOMPONENTELONGATIONMODEL_H_
#define OTLS_MODELS_SAGTENSION_CABLECOMPONENTELONGATIONMODEL_H_

#include <list>

#include "models/base/error_message.h"
#include "models/base/point.h"
#include "models/base/polynomial.h"
#include "models/sagtension/cable_state.h"
#include "models/sagtension/sag_tension_cable.h"

/// \par OVERVIEW
///
/// This class models the elongation of a cable component.
///
/// \par POLYNOMIALS
///
/// Polynomials are the foundation for the cable component elongation model.
/// They can be manipulated by either shifting or extrapolating as a means to
/// describe the cable elongation under various thermal and stretch parameters.
///
/// The polynomials can model the non-linear elongation of the component, and
/// are based on empirical test data. The polynomial coefficients must be
/// provided using the following units:
///   - X axis = percent strain
///   - Y axis = load
///
/// Cable components have two polynomials: creep and load-strain. The
/// load-strain polynomial describes how the component elongates under load. The
/// creep polynomial describes the amount of stretch the component has after a
/// fixed period of time. See following sections for how these two polynomials
/// are used to determine stretch.
///
/// The polynomials can become erratic at ranges beyond the curve-fitted region.
/// To account for this, the cable component must have polynomial limits that
/// restricts the polynomial use only to the valid region.
///
/// \par THERMAL STRAIN SHIFTING
///
/// Thermal strain is modeled for the component by shifting along the strain
/// (x) axis. The temperature difference from reference temperature and the
/// thermal expansion coefficient determine the thermal strain shift.
/// The polynomial coefficients are not adjusted for thermal strain shifts.
/// Instead, strain values are converted to percent strain values at the
/// polynomial reference temperature whenever a polynomial calculation is
/// needed.
///
/// \par STRETCH
///
/// Stretch is defined by the load that the component was stretched to. When the
/// component is stretched:
///  - for loads below the stretch load, the elastic tension area modulus is
///    extended to from the stretch load to zero
///  - for loads above the stretch load, the cable is effectively unstretched
///    and the polynomial is referenced
///
/// The stretch load must eventually be solved for on the load-strain
/// polynomial, but both polynomial types can be used to define the amount of
/// stretch the component has.
///
/// \par REGIONS
///
/// The elongation of the cable component can be described by polynomials,
/// or by means of extrapolating from polynomials. This can cause irregular
/// (non-smooth) elongation behavior. To account for this, the elongation
/// model is broken into discrete regions which capture the specific
/// elongation characteristics of the component at various loads/strains. The
/// specific regions are as follows:
///   - Compressed: The load/strain of the component is less than the unloaded
///     point. In this region, the component elongates from the unloaded point
///     according to the component compression modulus.
///   - Stretched: The load/strain of the component is between the unloaded
///     point and the polynomial start point. In this region, the component
///     elongates from the unloaded point to the stretch point (polynomial start
///     point) according the component tension modulus.
///   - Polynomial: The load/strain of the component is between the polynomial
///     start and end points. The polynomial start point is the point the
///     component was stretched to, and the end point is the limit where the
///     polynomial is no longer valid at describing the component elongation.
///   - Extrapolated: The load/strain of the component is beyond the polynomial
///     end point. The modulus used to extrapolate is tangent to the polynomial
///     limit point.
class CableComponentElongationModel {
 public:
  /// \brief Default constructor.
  CableComponentElongationModel();

  /// \brief Destructor.
  ~CableComponentElongationModel();

  /// \brief Gets the load.
  /// \param[in] strain
  ///   The strain value (x-axis)
  /// \return A load value.
  double Load(const double& strain) const;

  /// \brief Gets the region boundary points for the component state.
  /// \return A vector of points at the region boundaries.
  /// The region boundaries are at the following locations:
  ///  - unloaded
  ///  - polynomial start
  ///  - polynomial end
  std::vector<Point2d<double>> PointsRegions() const;

  /// \brief Gets the slope of a tangent line.
  /// \param[in] strain
  ///   The strain value (x-axis)
  /// \return The slope of a tangent line.
  double Slope(const double& strain) const;

  /// \brief Gets the strain.
  /// \param[in] load
  ///   The load value (y-axis).
  /// \return A strain value..
  double Strain(const double& load) const;

  /// \brief Gets the thermal strain.
  /// \return The thermal strain.
  double StrainThermal() const;

  /// \brief Validates member variables.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings = true,
                std::list<ErrorMessage>* messages = nullptr) const;

  /// \brief Gets the cable component.
  /// \return A copy of the cable component.
  const SagTensionCableComponent* component_cable() const;

  /// \brief Sets the cable component.
  /// \param[in] component_cable
  ///   The cable component.
  void set_component_cable(const SagTensionCableComponent* component_cable);

  /// \brief Sets the state.
  /// \param[in] state
  ///   The state.
  void set_state(const CableState* state);

  /// \brief Sets the stretch state.
  /// \param[in] state
  ///   The state.
  void set_state_stretch(const CableStretchState* state);

  /// \brief Sets the reference temperature of the polynomial.
  /// \param[in] temperature_reference
  ///   The reference temperature of the polynomial.
  void set_temperature_reference(const double* temperature_reference);

  /// \brief Gets the state.
  /// \return The state.
  const CableState* state() const;

  /// \brief Gets the stretch state.
  /// \return The stretch state.
  const CableStretchState* state_stretch() const;

  /// \brief Gets the reference temperature.
  /// \return The reference temperature.
  const double* temperature_reference() const;

 private:
  /// \brief Converts to a percent strain value for use with the thermally
  ///   unshifted polynomial at reference temperature.
  /// \param[in] strain
  ///   The strain value (x-axis)
  /// \param[in] strain_thermal
  ///   The thermal strain to reference temperature.
  /// \return A percent strain value for use with the polynomial.
  double ConvertToPercentStrainPolynomial(const double& strain,
                                          const double& strain_thermal) const;

  /// \brief Converts to a strain value.
  /// \param[in] percent_strain_polynomial
  ///   The percent strain value (x-axis) of the polynomial at reference
  ///   temperature.
  /// \param[in] strain_thermal
  ///   The thermal strain from reference temperature.
  /// \return A strain value at component temperature.
  double ConvertToStrain(const double& percent_strain_polynomial,
                         const double& strain_thermal) const;

  /// \brief Determines if class is updated.
  /// \return A boolean indicating if class is updated.
  bool IsUpdated() const;

  /// \brief Gets the load for the specified parameters.
  /// \param[in] type_polynomial
  ///   The polynomial type.
  /// \param[in] points
  ///   The region points.
  /// \param[in] strain_thermal
  ///   The thermal strain.
  /// \param[in] strain
  ///   The strain.
  /// \return The load.
  double Load(const SagTensionCableComponent::PolynomialType& type_polynomial,
              const std::vector<Point2d<double>>& points,
              const double& strain_thermal,
              const double& strain) const;

  /// \brief Gets the load by extending a line from a point.
  /// \param[in] point
  ///   The reference point.
  /// \param[in] slope
  ///   The slope of the line.
  /// \param[in] strain
  ///   The strain value (x-axis).
  /// \return The load (y-axis) on the line.
  double LoadLinearExtension(const Point2d<double>& point,
                             const double& slope,
                             const double& strain) const;

  /// \brief Gets the load from the polynomial.
  /// \param[in] polynomial
  ///   The polynomial to use.
  /// \param[in] strain_thermal
  ///   The thermal strain from reference temperature.
  /// \param[in] strain
  ///   The strain value (x-axis).
  /// \return The load from the polynomial.
  /// The thermal strain is needed for this function because the polynomials
  /// are not shifted along the x-axis. It is easier to use the thermal strain
  /// to convert to the polynomial x-axis reference as needed.
  double LoadPolynomial(const Polynomial& polynomial,
                        const double& strain_thermal,
                        const double& strain) const;

  /// \brief Gets the region points for the specified model.
  /// \param[in] polynomial
  ///   The polynomial.
  /// \param[in] temperature
  ///   The temperature.
  /// \param[in] load_limit_polynomial
  ///   The polynomial limit.
  /// \param[in] load_stretch
  ///   The stretch load.
  /// \param[out] points
  ///   The points vector to populate. The points will be ordered as follows:
  ///     - unloaded
  ///     - polynomial start (stretch)
  ///     - polynomial end
  void PointsRegions(const Polynomial& polynomial,
                     const double& temperature,
                     const double& load_limit_polynomial,
                     const double& load_stretch,
                     std::vector<Point2d<double>>& points) const;

  /// \brief Gets the slope for the specified parameters.
  /// \param[in] type_polynomial
  ///   The polynomial type.
  /// \param[in] points
  ///   The region points.
  /// \param[in] strain_thermal
  ///   The thermal strain from reference temperature.
  /// \param[in] strain
  ///   The strain value (x-axis).
  /// \return The slope.
  double Slope(const SagTensionCableComponent::PolynomialType& type_polynomial,
               const std::vector<Point2d<double>>& points,
               const double& strain_thermal,
               const double& strain) const;

  /// \brief Gets the slope from the polynomial.
  /// \param[in] polynomial
  ///   The polynomial to use.
  /// \param[in] strain_thermal
  ///   The thermal strain from reference temperature.
  /// \param[in] strain
  ///   The strain value (x-axis).
  /// \return The slope from the polynomial.
  /// The thermal strain is needed for this function because the polynomials
  /// are not shifted along the x-axis. It is easier to use the thermal strain
  /// to convert to the polynomial x-axis reference as needed.
  double SlopePolynomial(const Polynomial& polynomial,
                         const double& strain_thermal,
                         const double& strain) const;

  /// \brief Gets the strain for the specified parameters.
  /// \param[in] type_polynomial
  ///   The polynomial type.
  /// \param[in] points
  ///   The region points.
  /// \param[in] strain_thermal
  ///   The thermal strain.
  /// \param[in] load
  ///   The load.
  /// \return The strain.
  double Strain(const SagTensionCableComponent::PolynomialType& type_polynomial,
                const std::vector<Point2d<double>>& points,
                const double& strain_thermal,
                const double& load) const;

  /// \brief Gets the strain by extending a line from a point.
  /// \param[in] point
  ///   The reference point.
  /// \param[in] slope
  ///   The slope of the line.
  /// \param[in] load
  ///   The load value (y-axis).
  /// \return The strain (x-axis) on the line.
  double StrainLinearExtension(const Point2d<double>& point,
                               const double& slope,
                               const double& load) const;

  /// \brief Gets the strain from the polynomial.
  /// \param[in] polynomial
  ///   The polynomial to use.
  /// \param[in] strain_thermal
  ///   The thermal strain from reference temperature.
  /// \param[in] load
  ///   The load value (y-axis).
  /// \return The strain from the polynomial.
  double StrainPolynomial(const Polynomial& polynomial,
                          const double& strain_thermal,
                          const double& load) const;

  /// \brief Calculates the thermal strain from reference temperature.
  /// \param[in] temperature
  ///   The temperature of the cable component.
  /// \return The thermal strain from reference temperature.
  double StrainThermal(const double& temperature) const;

  /// \brief Updates cached member variables and modifies control variables if
  ///   update is required.
  /// \return A boolean indicating if class updates completed successfully.
  bool Update() const;

  /// \brief Updates the slope used to extrapolate from the creep and
  ///   load-strain polynomials.
  void UpdateModulusExtrapolate() const;

  /// \brief Updates the component state.
  /// \param[in] state
  ///   The cable state to update to.
  /// \return The success status of the update.
  bool UpdateState(const CableState& state) const;

  /// \brief Updates the stretch of the component.
  /// \return The success status of the update.
  bool UpdateStretch() const;

  /// \brief Validates member variables by checking polynomial origin.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool ValidatePolynomialOrigin(
      const bool& is_included_warnings = false,
      std::list<ErrorMessage>* messages = nullptr) const;

  /// \brief Validates member variables by checking polynomial shape.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool ValidatePolynomialShape(
      const bool& is_included_warnings = false,
      std::list<ErrorMessage>* messages = nullptr) const;

  /// \var component_
  ///   The cable component that is being modeled.
  const SagTensionCableComponent* component_;

  /// \var is_updated_state_
  ///   An indicator that tells if the state has been updated.
  mutable bool is_updated_state_;

  /// \var is_updated_stretch_
  ///   An indicator that tells if the stretch has been updated.
  mutable bool is_updated_stretch_;

  /// \var load_stretch_
  ///   The amount of load that the cable state polynomial has been stretched
  ///   to. The stretch load will match the stretch defined in the cable stretch
  ///   state if the stretch polynomial and the state polynomial types are the
  ///   same.
  ///   Plastic strain is an option to replace this variable, but if the initial
  ///   (unstretched) and modulus (stretched) functions intersect prior to the
  ///   stretch load, the model will erroneously show less stretch.
  mutable double load_stretch_;

  /// \var modulus_extrapolate_creep_;
  ///   The elongation modulus used to extrapolate from the creep polynomial.
  mutable double modulus_extrapolate_creep_;

  /// \var modulus_extrapolate_loadstrain_;
  ///   The elongation modulus used to extrapolate from the load-strain
  ///   polynomial.
  mutable double modulus_extrapolate_loadstrain_;

  /// \var points_state_
  ///   The region points for the state model. These points are in increasing
  ///   order.
  mutable std::vector<Point2d<double>> points_state_;

  /// \var points_stretch_creep_
  ///   The region points for the creep stretch model. These points are only
  ///   solved for if the stretch polynomial is different than the state
  ///   polynomial. These points are in increasing order.
  mutable std::vector<Point2d<double>> points_stretch_creep_;

  /// \var points_stretch_load_
  ///   The region points for the loadstrain stretch model. These points are
  ///   only solved for if the stretch polynomial is different than the state
  ///   polynomial. These points are in increasing order.
  mutable std::vector<Point2d<double>> points_stretch_load_;

  /// \var polynomial_creep_
  ///   The polynomial that describes how the component stretches after an
  ///   extended period of time.
  Polynomial polynomial_creep_;

  /// \var polynomial_loadstrain_
  ///   The polynomial that describes the cable component elongation.
  Polynomial polynomial_loadstrain_;

  /// \var state_
  ///   The state of the component.
  const CableState* state_;

  /// \var state_stretch_
  ///   The state used to stretch the component.
  const CableStretchState* state_stretch_;

  /// \var strain_thermal_state_
  ///   The amount of strain due to shifting from reference temperature to the
  ///   state temperature.
  mutable double strain_thermal_state_;

  /// \var strain_thermal_stretch_
  ///   The amount of strain due to shifting from reference temperature to the
  ///   stretch temperature.
  mutable double strain_thermal_stretch_;

  /// \var temperature_reference_
  ///   The base reference temperature for the cable component. The polynomial
  ///   is defined at this temperature.
  const double* temperature_reference_;
};

#endif  // OTLS_MODELS_SAGTENSION_CABLECOMPONENTELONGATIONMODEL_H_
