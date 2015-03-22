// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef TLSLIBRARIES_SAGTENSION_CABLECOMPONENTELONGATIONMODEL_H_
#define TLSLIBRARIES_SAGTENSION_CABLECOMPONENTELONGATIONMODEL_H_

#include "base/point.h"
#include "base/polynomial.h"
#include "transmissionline/cable.h"

/// \par OVERVIEW
///
/// This class models the elongation of a cable component.
///
/// \par POLYNOMIAL
///
/// The polynomial can model the non-linear elongation of the component, and is
/// based on empirical test data. The polynomial coefficients must be provided
/// using the following units:
///   X axis - percent strain   ex: (in/in)% or (mm/mm)%
///   Y axis - load             ex: (lbs) or (N)
///
/// \par POLYNOMIAL LIMIT
///
/// Polynomial functions can become erratic at ranges beyond the curve-fitted
/// region. Beyond this point, all elongation is assumed elastic, and the
/// component elongates according to the tension elastic area modulus.
///
/// \par THERMAL STRAIN SHIFTING
///
/// Thermal strain is modeled for the component by shifting along the strain
/// axis. The temperature difference from reference temperature and the thermal
/// expansion coefficient determine the thermal strain shift.
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
/// \par COMPRESSION
///
/// Compression is modeled as elastic, and is defined by the compression
/// elastic area modulus.
class CableComponentElongationModel {
 public:
  /// \brief Default constructor.
  CableComponentElongationModel();

  /// \brief Destructor.
  ~CableComponentElongationModel();

  /// \brief Gets the load.
  /// \param[in] strain
  ///   The strain value (x-axis)
  /// \param[in] is_stretched
  ///   A boolean indicating whether the cable condition is unstretched, or
  ///   stretched.
  /// \return A load value corresponding to the strain and stretch condition.
  double Load(const double& strain, const bool& is_stretched) const;

  /// \brief Gets the polynomial limit point.
  /// \return The point where the polynomial has reached its limit and is no
  ///   longer valid.
  Point2d PointLimitPolynomial() const;

  /// \brief Gets the stretch point.
  /// \return The point where the cable component has been stretched to.
  Point2d PointStretch() const;

  /// \brief Gets the unloaded point.
  /// \param[in] is_stretched
  ///   A boolean indicating whether the cable condition is unstretched, or
  ///   stretched.
  /// \return The point where the cable component is unloaded.
  Point2d PointUnloaded(const bool& is_stretched) const;

  /// \brief Gets the points at the load-strain model boundary regions.
  /// \param[in] is_stretched
  ///   A boolean indicating whether the cable condition is unstretched, or
  ///   stretched.
  /// \return A collection of points at the boundary regions of the load-strain
  ///   model.
  /// The region boundaries are at the following locations, but not necessarily
  /// in this order:
  ///  - unloaded
  ///  - stretched
  ///  - limit
  std::vector<Point2d> PointsDiscreteRegions(const bool& is_stretched) const;

  /// \brief Gets the slope of a tangent line.
  /// \param[in] strain
  ///   The strain value (x-axis)
  /// \return The slope of a tangent line.
  double SlopeTangent(const double& strain, const bool& is_stretched) const;

  /// \brief Gets the strain.
  /// \param[in] load
  ///   The load value (y-axis).
  /// \param[in] is_stretched
  ///   A boolean indicating whether the cable condition is unstretched, or
  ///   stretched.
  double Strain(const double& load, const bool& is_stretched) const;

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
  CableComponent component_cable() const;

  /// \brief Gets the stretch load.
  /// \return The stretch load.
  double load_stretch() const;

  /// \brief Sets the cable component.
  /// \param[in] component_cable
  ///   The cable component.
  void set_component_cable(const CableComponent& component_cable);

  /// \brief Sets the stretch load.
  /// \param[in] load_stretch
  ///   The stretch load.
  void set_load_stretch(const double& load_stretch);

  /// \brief Sets the temperature of the component.
  /// \param[in] temperature
  ///   The temperature of the component.
  void set_temperature(const double& temperature);

  /// \brief Sets the reference temperature of the polynomial.
  /// \param[in] temperature_reference
  ///   The reference temperature of the polynomial.
  void set_temperature_reference(const double& temperature_reference);

  /// \brief Sets the type of active polynomial.
  /// \param[in] type_polynomial_active
  ///   The type of active polynomial.
  void set_type_polynomial_active(
      const CablePolynomialType& type_polynomial_active);

  /// \brief Gets the temperature.
  /// \return The temperature.
  double temperature() const;

  /// \brief Gets the reference temperature.
  /// \return The reference temperature.
  double temperature_reference() const;

  /// \brief Gets the type of active polynomial.
  /// \return The cable polynomial type.
  CablePolynomialType type_polynomial_active() const;

 private:
  /// \brief Converts to a percent strain value for use with the thermally
  ///   unshifted polynomial at reference temperature.
  /// \param[in] strain
  ///   The strain value (x-axis)
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
  /// \param[in] is_stretched
  ///   A boolean indicating whether the cable condition is unstretched, or
  ///   stretched.
  /// \return The load due to compression.
  double LoadCompression(const double& strain, const bool& is_stretched) const;

  /// \brief Gets the load from the polynomial.
  /// \param[in] strain
  ///   The strain value (x-axis)
  /// \return The load from the polynomial.
  double LoadPolynomial(const double& strain) const;

  /// \brief Gets the load when the component is stretched.
  /// \param[in] strain
  ///   The strain value (x-axis)
  /// \return The load of the componet when stretched.
  double LoadStretched(const double& strain) const;

  /// \brief Gets the load when the component is unstretched.
  /// \param[in] strain
  ///   The strain value (x-axis)
  /// \return The load of the componet when unstretched.
  double LoadUnstretched(const double& strain) const;

  /// \brief Gets the strain due to compression.
  /// \param[in] load
  ///   The load value (y-axis)
  /// \param[in] is_stretched
  ///   A boolean indicating whether the cable condition is unstretched, or
  ///   stretched.
  /// \return The strain due to compression.
  double StrainCompression(const double& load,
                           const double& is_stretched) const;

  /// \brief Gets the strain from the polynomial.
  /// \param[in] load
  ///   The load value (y-axis)
  /// \return The strain from the polynomial.
  double StrainPolynomial(const double& load) const;

  /// \brief Gets the strain when the component is stretched.
  /// \param[in] load
  ///   The load value (y-axis)
  /// \return The strain of the componet when stretched.
  double StrainStretched(const double& load) const;

  /// \brief Gets the strain when the component is unstretched.
  /// \param[in] load
  ///   The load value (y-axis)
  /// \return The strain of the componet when unstretched.
  double StrainUnstretched(const double& load) const;

  /// \brief Updates cached member variables and modifies control variables if
  ///   update is required.
  /// \return A boolean indicating if class updates completed successfully.
  bool Update() const;

  /// \brief Updates the polynomial limit point.
  /// \return A boolean indicating the success status of the update.
  bool UpdatePointLimitPolynomial() const;

  /// \brief Updates the stretch point.
  /// \return A boolean indicating the success status of the update.
  bool UpdatePointStretch() const;

  /// \brief Updates the unloaded point when the component is stretched.
  /// \return A boolean indicating the success status of the update.
  bool UpdatePointUnloadedStretched() const;

  /// \brief Updates the unloaded point when the component is unstretched.
  /// \return A boolean indicating the success status of the update.
  bool UpdatePointUnloadedUnstretched() const;

  /// \brief Updates the polynomial.
  /// \return A boolean indicating the success status of the update.
  bool UpdatePolynomial() const;

  /// \brief Updates the thermal strain.
  /// \return A boolean indicating the success status of the update.
  bool UpdateStrainThermal() const;

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
  CableComponent component_cable_;

  /// \var load_stretch_
  ///   The load the component has been stretched to.
  double load_stretch_;

  /// \var is_updated_point_limit_polynomial_
  ///   An indicator that tells if the limit point has been updated.
  mutable bool is_updated_point_limit_polynomial_;

  /// \var is_updated_point_stretch_
  ///   An indicator that tells if the stretch point has been updated.
  mutable bool is_updated_point_stretch_;

  /// \var is_updated_polynomial
  ///   An indicator that tells if the polynomial has been updated.
  mutable bool is_updated_polynomial_;

  /// \var is_updated_strain_thermal_
  ///   An indicator that tells if the thermal strain has been updated.
  mutable bool is_updated_strain_thermal_;

  /// \var point_limit_polynomial_
  ///   The point beyond which the polynomial is no longer valid.
  mutable Point2d point_limit_polynomial_;

  /// \var point_stretch_
  ///   The point where the cable component has been stretched to.
  mutable Point2d point_stretch_;

  /// \var point_unloaded_stretched_
  ///   The point where the cable is unloaded when stretched.
  mutable Point2d point_unloaded_stretched_;

  /// \var point_unloaded_unstretched_
  ///   The point where the cable is unloaded when unstretched.
  mutable Point2d point_unloaded_unstretched_;

  /// \var polynomial_
  ///   The polynomial that describes the cable component elongation.
  mutable Polynomial polynomial_;

  /// \var strain_thermal_
  ///   The amount of strain due to shifting from reference temperature to the
  ///   specified temperature.
  mutable double strain_thermal_;

  /// \var temperature_
  ///   The specified temperature of the cable component.
  double temperature_;

  /// \var temperature_reference_
  ///   The base reference temperature for the cable component. The polynomial
  ///   is defined at this temperature.
  double temperature_reference_;

  /// \var type_polynomial_active_
  ///   The type of polynomial to model. The cable component can have multiple
  ///   polynomials to describe its elongation under.
  CablePolynomialType type_polynomial_active_;
};

#endif // TLSLIBRARIES_SAGTENSION_CABLECOMPONENTELONGATIONMODEL_H_
