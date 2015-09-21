// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef OTLS_MODELS_SAGTENSION_CABLECOMPONENTELONGATIONMODEL_H_
#define OTLS_MODELS_SAGTENSION_CABLECOMPONENTELONGATIONMODEL_H_

#include <list>
#include <string>

#include "models/base/point.h"
#include "models/base/polynomial.h"
#include "models/transmissionline/cable.h"

/// \par OVERVIEW
///
/// This class models the elongation of a cable component.
///
/// \par POLYNOMIAL
///
/// The polynomial is the foundation for the cable component elongation model.
/// It is often manipulated by either shifting or extrapolating as a means to
/// describe the cable elongation under various thermal and stretch parameters.
///
/// The polynomial can model the non-linear elongation of the component, and is
/// based on empirical test data. The polynomial coefficients must be provided
/// using the following units:
///   - X axis = percent strain
///   - Y axis = load
///
/// The polynomial can become erratic at ranges beyond the curve-fitted region.
/// To account for this, the cable component must have a polynomial limit that
/// restricts the polynomial use only to the valid region of the polynomial.
///
/// \par THERMAL STRAIN SHIFTING
///
/// Thermal strain is modeled for the component by shifting along the strain
/// (x) axis. The temperature difference from reference temperature and the
/// thermal expansion coefficient determine the thermal strain shift.
/// The polynomial coefficients are not adjusted for thermal strain shifts.
/// Instead, strain values are converted to percent strain values at the
/// polynomial reference temperature.
///
/// \par STRETCH
///
/// Stretch is defined by the load that the cable was stretched to. When the
/// cable is stretched:
///  - for loads below the stretch load, the elastic tension area modulus is
///    extended to from the stretch load to zero
///  - for loads above the stretch load, the cable is effectively unstretched
///    and the polynomial is referenced
///
/// \par REGIONS
///
/// The elongation of the cable component can be described by the polynomial,
/// or by means of extrapolating from the polynomial. This can cause irregular
/// (non-smooth) elongation behavior. To account for this, the elongation
/// model is broken into discrete regions, which capture the specific
/// elongation characteristics of the component at various loads/strains. The
/// specific regions are as follows, but not always in this order:
///   - Compressed: The load/strain of the component is less than the unloaded
///     point. In this region, the component elongates from the unloaded point
///     according to the component compression modulus.
///   - Stretched: The load/strain of the component is between the unloaded
///     point and the polynomial start point. In this region, the component
///     elongates from the unloaded point to the stretch point according the
///     component tension modulus.
///   - Polynomial: The load/strain of the component is between the polynomial
///     start and end points. The polynomial start point is the point the
///     component was stretched to, and the end point is the limit where the
///     polynomial is no longer valid at describing the component elongation.
///   - Extrapolated: The load/strain of the component is beyond the polynomial
///     end point. All elongation is assumed elastic, and the component
///     elongates according to the tension elastic area modulus.
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

  /// \brief Gets the polynomial end point.
  /// \return The polynomial end point.
  Point2d PointPolynomialEnd() const;

  /// \brief Gets the polynomial start point.
  /// \return The polynomial start point.
  Point2d PointPolynomialStart() const;

  /// \brief Gets the points for the region boundaries.
  /// \return A vector of points at the region boundaries.
  /// The region boundaries are at the following locations:
  ///  - unloaded
  ///  - polynomial start
  ///  - polynomial end
  std::vector<Point2d> PointsRegions() const;

  /// \brief Gets the unloaded point.
  /// \return The unloaded point.
  Point2d PointUnloaded() const;

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
  /// \param[in,out] messages_error
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings = true,
                std::list<std::string>* messages_error = nullptr) const;

  /// \brief Gets the cable component.
  /// \return A copy of the cable component.
  const CableComponent* component_cable() const;

  /// \brief Gets the stretch load.
  /// \return The stretch load.
  double load_stretch() const;

  /// \brief Sets the cable component.
  /// \param[in] component_cable
  ///   The cable component.
  void set_component_cable(const CableComponent* component_cable);

  /// \brief Sets the stretch load.
  /// \param[in] load_stretch
  ///   The stretch load.
  void set_load_stretch(const double& load_stretch);

  /// \brief Sets the temperature of the component.
  /// \param[in] temperature
  ///   The temperature of the component.
  void set_temperature(const double* temperature);

  /// \brief Sets the reference temperature of the polynomial.
  /// \param[in] temperature_reference
  ///   The reference temperature of the polynomial.
  void set_temperature_reference(const double* temperature_reference);

  /// \brief Sets the type of active polynomial.
  /// \param[in] type_polynomial_active
  ///   The type of active polynomial.
  void set_type_polynomial_active(
      const CableComponent::PolynomialType* type_polynomial_active);

  /// \brief Gets the temperature.
  /// \return The temperature.
  const double* temperature() const;

  /// \brief Gets the reference temperature.
  /// \return The reference temperature.
  const double* temperature_reference() const;

  /// \brief Gets the type of active polynomial.
  /// \return The cable polynomial type.
  const CableComponent::PolynomialType* type_polynomial_active() const;

 private:
  /// \brief Converts to a percent strain value for use with the thermally
  ///   unshifted polynomial at reference temperature.
  /// \param[in] strain
  ///   The strain value (x-axis)
  /// \return A percent strain value for use with the polynomial.
  double ConvertToPercentStrainPolynomial(const double& strain) const;

  /// \brief Converts to a strain value.
  /// \param[in] percent_strain_polynomial
  ///   The percent strain value (x-axis) of the polynomial at reference
  ///   temperature.
  /// \return A strain value at component temperature.
  double ConvertToStrain(const double& percent_strain_polynomial) const;

  /// \brief Determines if class is updated.
  /// \return A boolean indicating if class is updated.
  bool IsUpdated() const;

  /// \brief Gets the load due to compression.
  /// \param[in] strain
  ///   The strain value (x-axis)
  /// \return The load in the compressed region.
  double LoadCompressed(const double& strain) const;

  /// \brief Gets the load in the extrapolated region.
  /// \param[in] strain
  ///   The strain value (x-axis)
  /// \return The load in the extrapolated region.
  double LoadExtrapolated(const double& strain) const;

  /// \brief Gets the load in the polynomial region.
  /// \param[in] strain
  ///   The strain value (x-axis)
  /// \return The load in the polynomial region.
  double LoadPolynomial(const double& strain) const;

  /// \brief Gets the load in the stretched region.
  /// \param[in] strain
  ///   The strain value (x-axis)
  /// \return The load in the stretched region.
  double LoadStretched(const double& strain) const;

  /// \brief Gets the slope in the compressed region.
  /// \param[in] strain
  ///   The strain value (x-axis)
  /// \return The slope in the compressed region.
  double SlopeCompressed(const double& strain) const;

  /// \brief Gets the slope in the extrapolated region.
  /// \param[in] strain
  ///   The strain value (x-axis)
  /// \return The slope in the extrapolated region.
  double SlopeExtrapolated(const double& strain) const;

  /// \brief Gets the slope in the extrapolated region.
  /// \param[in] strain
  ///   The strain value (x-axis)
  /// \return The slope in the extrapolated region.
  double SlopePolynomial(const double& strain) const;

  /// \brief Gets the slope in the stretched region.
  /// \param[in] strain
  ///   The strain value (x-axis)
  /// \return The slope in the stretched region.
  double SlopeStretched(const double& strain) const;

  /// \brief Gets the strain in the compressed region.
  /// \param[in] load
  ///   The load value (y-axis)
  /// \return The strain in the compression region.
  double StrainCompressed(const double& load) const;

  /// \brief Gets the strain in the extrapolated region.
  /// \param[in] load
  ///   The load value (y-axis)
  /// \return The strain in the extrapolated region.
  double StrainExtrapolated(const double& load) const;

  /// \brief Gets the strain in the polynomial region.
  /// \param[in] load
  ///   The load value (y-axis)
  /// \return The strain in the polynomial region.
  double StrainPolynomial(const double& load) const;

  /// \brief Gets the strain in the stretched region.
  /// \param[in] load
  ///   The load value (y-axis)
  /// \return The strain in the stretched region.
  double StrainStretched(const double& load) const;

  /// \brief Updates cached member variables and modifies control variables if
  ///   update is required.
  /// \return A boolean indicating if class updates completed successfully.
  bool Update() const;

  /// \brief Updates the polynomial end point.
  /// \return A boolean indicating the success status of the update.
  bool UpdatePointPolynomialEnd() const;

  /// \brief Updates the polynomial start point.
  /// \return A boolean indicating the success status of the update.
  bool UpdatePointPolynomialStart() const;

  /// \brief Updates the unloaded point.
  /// \return A boolean indicating the success status of the update.
  bool UpdatePointUnloaded() const;

  /// \brief Updates the polynomial.
  /// \return A boolean indicating the success status of the update.
  bool UpdatePolynomial() const;

  /// \brief Updates the thermal strain.
  /// \param[in] temperature
  ///   The temperature of the cable component.
  /// \return A boolean indicating the success status of the update.
  bool UpdateStrainThermal(const double& temperature) const;

  /// \brief Updates the thermal shift and cached points for the regions.
  /// \param[in] temperature
  ///   The temperature of the cable component.
  /// \return The success status of the update.
  bool UpdateRegions(const double& temperature) const;

  /// \brief Validates member variables by checking polynomial origin.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages_error
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool ValidatePolynomialOrigin(
      const bool& is_included_warnings = false,
      std::list<std::string>* messages_error = nullptr) const;

  /// \brief Validates member variables by checking polynomial shape.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages_error
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool ValidatePolynomialShape(
      const bool& is_included_warnings = false,
      std::list<std::string>* messages_error = nullptr) const;

  /// \var component_cable_
  ///   The cable component that is being modeled.
  const CableComponent* component_cable_;

  /// \var load_stretch_
  ///   The load the component has been stretched to.
  double load_stretch_;

  /// \var is_updated_polynomial_
  ///   An indicator that tells if the polynomial has been updated.
  mutable bool is_updated_polynomial_;

  /// \var is_updated_regions_
  ///   An indicator that tells if the points of the various region boundaries
  ///   have been updated.
  mutable bool is_updated_regions_;

  /// \var point_polynomial_end_
  ///   The point beyond which the polynomial is no longer valid.
  mutable Point2d point_polynomial_end_;

  /// \var point_polynomial_start_
  ///   The point at which the polynomial is valid. This is the same as the
  ///   point the the cable component has been stretched to.
  mutable Point2d point_polynomial_start_;

  /// \var point_unloaded_
  ///   The point where the cable is unloaded.
  mutable Point2d point_unloaded_;

  /// \var polynomial_
  ///   The polynomial that describes the cable component elongation.
  mutable Polynomial polynomial_;

  /// \var strain_thermal_
  ///   The amount of strain due to shifting from reference temperature to the
  ///   specified temperature.
  mutable double strain_thermal_;

  /// \var temperature_
  ///   The specified temperature of the cable component.
  const double* temperature_;

  /// \var temperature_reference_
  ///   The base reference temperature for the cable component. The polynomial
  ///   is defined at this temperature.
  const double* temperature_reference_;

  /// \var type_polynomial_active_
  ///   The type of polynomial to model. The cable component can have multiple
  ///   polynomials to describe its elongation.
  const CableComponent::PolynomialType* type_polynomial_active_;
};

#endif // OTLS_MODELS_SAGTENSION_CABLECOMPONENTELONGATIONMODEL_H_
