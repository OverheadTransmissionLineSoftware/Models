// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#pragma once

#include <list>
#include <vector>
#include <string>

enum class Type_Polynomial
{
    CREEP,
    LOADSTRAIN
};

/**
 * @par COMPONENT CABLE OVERVIEW
 *
 * This class models a transmission cable component.
 *
 * @par VALIDATION
 *
 * This class contains built-in member validation. To avoid runtime errors when using this class,
 * use the validation functionality before extracting information.
 */
class Component_Cable
{
public:
    /**
     * @brief Default constructor.
     */
    Component_Cable();

    /**
     * @brief Destructor.
     */
    ~Component_Cable();

    /**
     * @brief Validate class members.
     * @param is_included_warnings An optional flag that includes warnings when validating class
     *        members. Warnings have a tightened acceptable value range.
     * @param messages_error An optional list that will be appended to with detailed messages of
     *        class validation errors.
     * @return A boolean value indicating status of class data.
     */
    bool Validate(bool is_included_warnings = true,
                  std::list<std::string>* messages_error = nullptr) const;

    // member variables
    double                  m_coefficient_expansion_thermal_linear;
    std::vector<double>     m_coefficients_polynomial_creep;
    std::vector<double>     m_coefficients_polynomial_loadstrain;
    double                  m_load_limit_polynomial_creep;
    double                  m_load_limit_polynomial_loadstrain;
    double                  m_modulus_compression_elastic_area;
    double                  m_modulus_tension_elastic_area;
};


/**
 * @par CABLE OVERVIEW
 *
 * This class models a transmission cable.
 *
 * @par VALIDATION
 *
 * This class contains built-in member validation. To avoid runtime errors when using this class,
 * use the validation functionality before extracting information.
 */
class Cable
{
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
     * @brief Validate class members.
     * @param is_included_warnings An optional flag that includes warnings when validating class
     *        members. Warnings have a tightened acceptable value range.
     * @param messages_error An optional list that will be appended to with detailed messages of
     *        class validation errors.
     * @return A boolean value indicating status of class data.
     */
    bool Validate(bool is_included_warnings = true,
                  std::list<std::string>* messages_error = nullptr) const;

    // member variables
    double          m_area_physical;
    double          m_area_electrical;
    Component_Cable m_component_core;
    Component_Cable m_component_shell;
    double          m_diameter;
    std::string     m_name;
    double          m_strength_rated;
    double          m_temperature_component_properties;
    std::string     m_type_construction;
    Type_Polynomial m_type_polynomial_active;
    double          m_weight_unit;
};
