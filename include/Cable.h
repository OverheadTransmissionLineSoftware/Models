// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef TRANSMISSIONLINE_CABLE_H_
#define TRANSMISSIONLINE_CABLE_H_

#include <list>
#include <string>
#include <vector>

/**
 * @par CABLE POLYNOMIAL TYPE
 *
 * This enum contains types of cable polynomials.
 */
enum class CablePolynomialType {
  CREEP,
  LOADSTRAIN
};

/**
 * @par COMPONENT CABLE OVERVIEW
 *
 * This class contains information for a transmission cable component.
 * (ex: shell, core)
 *
 */
struct CableComponent {
public:
  /**
   * @brief Default constructor.
   */
  CableComponent();

  /**
   * @brief Destructor.
   */
  ~CableComponent();

  /**
   * @brief Validates class data.
   * @param is_included_warnings A flag that tightens the acceptable value
   *        range.
   * @param messages_error A list of detailed error messages. If this is
   *        provided, any validation errors will be appended to the list.
   * @return A boolean value indicating status of class data.
   */
  bool Validate(bool is_included_warnings = true,
                std::list<std::string>* messages_error = nullptr) const;

  // member variables
  double                  coefficient_expansion_linear_thermal;
  std::vector<double>     coefficients_polynomial_creep;
  std::vector<double>     coefficients_polynomial_loadstrain;
  double                  load_limit_polynomial_creep;
  double                  load_limit_polynomial_loadstrain;
  double                  modulus_compression_elastic_area;
  double                  modulus_tension_elastic_area;
};

/**
 * @par CABLE OVERVIEW
 *
 * This class contains information for a transmission cable.
 *
 * This class can store information for up to two separate cable components,
 * noted as core and shell.
 */
struct Cable {
public:
  /**
   * @brief Default constructor.
   */
  Cable();

  /**
   * @brief Destructor.
   */
  ~Cable();

  /**
   * @brief Validates class data.
   * @param is_included_warnings A flag that tightens the acceptable value
   *        range.
   * @param messages_error A list of detailed error messages. If this is
   *        provided, any validation errors will be appended to the list.
   * @return A boolean value indicating status of class data.
   */
  bool Validate(bool is_included_warnings = true,
                std::list<std::string>* messages_error = nullptr) const;

  // member variables
  double              area_physical;
  double              area_electrical;
  CableComponent      component_core;
  CableComponent      component_shell;
  double              diameter;
  std::string         name;
  double              strength_rated;
  double              temperature_properties_components;
  std::string         type_construction;
  CablePolynomialType type_polynomial_active;
  double              weight_unit;
};

#endif // TRANSMISSIONLINE_CABLE_H_
