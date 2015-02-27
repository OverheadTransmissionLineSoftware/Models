// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef TRANSMISSIONLINE_CABLE_H_
#define TRANSMISSIONLINE_CABLE_H_

#include <list>
#include <string>
#include <vector>

/// \par OVERVIEW
///
/// This enum contains types of cable polynomials.
enum class CablePolynomialType {
  kCreep,
  kLoadStrain
};

/// \par OVERVIEW
///
/// This struct is a container for a transmission cable component.
///  (ex: shell, core)
struct CableComponent {
 public:
  /// \brief Default constructor.
  CableComponent();

  /// \brief Destructor.
  ~CableComponent();

  /// \brief Validates member variables.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages_error
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings = true,
                std::list<std::string>* messages_error = nullptr) const;

  /// \var coefficient_expansion_linear_thermal
  ///   The coefficient that determines how the component linearly elongates
  ///   with changes in temperature.
  double coefficient_expansion_linear_thermal;

  /// \var coefficients_polynomial_creep
  ///   The polynomial coefficients that model how the cable component
  ///   permanently elongates due to creep after a given number of years.
  std::vector<double> coefficients_polynomial_creep;

  /// \var coefficients_polynomial_loadstrain
  ///   The polynomial coefficients that model how the cable component
  ///   elongates due to load.
  std::vector<double> coefficients_polynomial_loadstrain;

  /// \var load_limit_polynomial_creep
  ///   The load where the polynomial modeling creep elongation is no longer
  ///   valid.
  double load_limit_polynomial_creep;

  /// \var load_limit_polynomial_loadstrain
  ///   The load where the polynomial modeling elongation is no longer valid.
  double load_limit_polynomial_loadstrain;

  /// \var modulus_compression_elastic_area
  ///   The elastic area modulus of the cable component when compressed.
  double modulus_compression_elastic_area;

  /// \var modulus_tension_elastic_area
  ///   The elastic area modulus of the cable component when tensioned. This
  ///   is typically used when modeling how a cable unloads when stretched.
  double modulus_tension_elastic_area;
};

/// \par OVERVIEW
///
/// This struct is a container for a transmission cable. This struct can store
/// information for up to two separate cable components, noted as core and
/// shell.
struct Cable {
 public:
  /// \brief Default constructor.
  Cable();

  /// \brief Destructor.
  ~Cable();

  /// \brief Validates member variables.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages_error
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings = true,
                std::list<std::string>* messages_error = nullptr) const;

  /// \var area_physical
  ///   The physical cross section area of the entire cable.
  double area_physical;

  /// \var area_electrical
  ///   The electrical area of the entire cable, usually restricted to the
  ///   electrical current carrying area of the cable.
  double area_electrical;

  /// \var component_core
  ///   The central cable component which typically reinforces the cable. Not
  ///   all cables contain a core.
  CableComponent component_core;

  /// \var component_shell
  ///   The outer cable component which typically carries the electrical
  ///   current.
  CableComponent component_shell;

  /// \var diameter
  ///   The nominal diameter of the entire cable.
  double diameter;

  /// \var name
  ///   The shorthand name for the cable, often determined by the cable
  ///   construction type.
  std::string name;

  /// \var strength_rated
  ///   The maximum load the cable can withstand.
  double strength_rated;

  /// \var temperature_properties_components
  ///   The reference temperature for the cable component properties, which can
  ///   be temperature-dependent.
  double temperature_properties_components;

  /// \var type_construction
  ///   The type of construction, usually abbreviated, which typically
  ///   indicates which materials the cable has, and if it contains a
  ///   reinforcing core component.
  std::string type_construction;

  /// \var type_polynomial_active
  ///   A switch that activates a specific polynomial to describe the cable
  ///   elongation characteristics.
  CablePolynomialType type_polynomial_active;

  /// \var weight_unit
  ///   The weight per unit length of the entire cable.
  double weight_unit;
};

#endif // TRANSMISSIONLINE_CABLE_H_
