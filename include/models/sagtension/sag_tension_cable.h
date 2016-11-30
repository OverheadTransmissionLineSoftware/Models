// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef OTLS_MODELS_SAGTENSION_SAGTENSIONCABLE_H_
#define OTLS_MODELS_SAGTENSION_SAGTENSIONCABLE_H_

#include <list>

#include "models/base/error_message.h"
#include "models/transmissionline/cable.h"

/// \par OVERVIEW
///
/// This class provides an interface for the cable component for use in the
/// sag-tension library.
class SagTensionCableComponent {
 public:
  /// \par OVERVIEW
  ///
  /// This enum contains types of cable component polynomials.
  enum class PolynomialType {
    kCreep,
    kLoadStrain
  };

  /// \brief Constructor.
  SagTensionCableComponent();

  /// \brief Destructor.
  ~SagTensionCableComponent();

  /// \brief Determines if the component is enabled.
  /// \return If the component is enabled.
  bool IsEnabled() const;

  /// \brief Validates member variables.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings = true,
                std::list<ErrorMessage>* messages = nullptr) const;

  /// \brief Gets the coefficient of linear thermal expansion.
  /// \return The coefficient of linear thermal expansion.
  const double* coefficient_expansion_linear_thermal() const;

  /// \brief Gets the polynomial coefficients.
  /// \param[in] type_polynomial
  ///   The polynomial type.
  /// \return The polynomial coefficients.
  const std::vector<double>* coefficients_polynomial(
      const PolynomialType& type_polynomial) const;

  /// \brief Gets the base component.
  /// \return The base component.
  const CableComponent* component_base() const;

  /// \brief Gets the load limit of the active polynomial.
  /// \param[in] type_polynomial
  ///   The polynomial type.
  /// \return The load limit of the polynomial.
  const double* load_limit_polynomial(const PolynomialType& type_polynomial)
      const;

  /// \brief Gets the compression elastic area modulus of the cable component.
  /// \return The compression elastic area modulus of the cable component.
  const double* modulus_compression_elastic_area() const;

  /// \brief Gets the tension elastic area modulus of the cable component.
  /// \return The tension elastic area modulus of the cable component.
  const double* modulus_tension_elastic_area() const;

  /// \brief Sets the base cable component.
  /// \param[in] component_base
  ///   The base cable component.
  void set_component_base(const CableComponent* component_base);

 private:
  /// \brief Updates if the component is enabled.
  void UpdateIsEnabled();

  /// \var component_base_
  ///   The cable component that is interfaced.
  const CableComponent* component_base_;

  /// \var is_enabled_
  ///   An indicator that determines if the component is enabled.
  bool is_enabled_;
};

/// \par OVERVIEW
///
/// This class provides an interface for the cable for use in the sag-tension
/// library.
class SagTensionCable {
 public:
  /// \par OVERVIEW
  ///
  /// This enum contains types of cable elongation model components.
  enum class ComponentType {
    kCombined,
    kCore,
    kShell
  };

  /// \brief Constructor.
  SagTensionCable();

  /// \brief Destructor.
  ~SagTensionCable();

  /// \brief Determines if portions of the cable are enabled.
  /// \param[in] type_component
  ///   The component type.
  /// \return If the cable component is enabled. If the combined enumeration is
  /// specified, true will be returned if any single component is enabled.
  bool IsEnabled(const SagTensionCable::ComponentType& type_component) const;

  /// \brief Validates member variables.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings = true,
                std::list<ErrorMessage>* messages = nullptr) const;

  /// \brief Gets the base cable.
  /// \return The base cable.
  const Cable* cable_base() const;

  /// \brief Gets the core component.
  /// \return The core component.
  const SagTensionCableComponent* component_core() const;

  /// \brief Gets the core component.
  /// \return The core component.
  const SagTensionCableComponent* component_shell() const;

  /// \brief Gets the diameter.
  /// \return The diameter.
  const double* diameter() const;

  /// \brief Sets the base cable.
  /// \param[in] cable_base
  ///   The base cable.
  void set_cable_base(const Cable* cable_base);

  /// \brief Gets the rated strength.
  /// \return The rated strength.
  const double* strength_rated() const;

  /// \brief Gets the temperature of the component properties.
  /// \return The temperature of the component properties.
  const double* temperature_properties_components() const;

  /// \brief Gets the unit weight.
  /// \return The unit weight.
  const double* weight_unit() const;

 private:
  /// \var cable_base_
  ///   The cable that is interfaced.
  const Cable* cable_base_;

  /// \var component_sagtension_core_
  ///   The sagtension core cable component.
  SagTensionCableComponent component_sagtension_core_;

  /// \var component_sagtension_shell_
  ///   The sag-tension shell cable component.
  SagTensionCableComponent component_sagtension_shell_;
};

#endif  // OTLS_MODELS_SAGTENSION_SAGTENSIONCABLE_H_
