// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "Cable.hpp"


////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// CONSTRUCTOR / DESTRUCTOR ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////


Component_Cable::Component_Cable()
{
    m_coefficient_expansion_thermal_linear = -999999;
//    m_coefficients_polynomial_creep;
//    m_coefficients_polynomial_loadstrain;
    m_load_limit_polynomial_creep = -999999;
    m_load_limit_polynomial_loadstrain = -999999;
    m_modulus_compression_elastic_area = -999999;
    m_modulus_tension_elastic_area = -999999;
}

Component_Cable::~Component_Cable()
{}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// PUBLIC MEMBER FUNCTIONS /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * This function should be used before extracting any information from the class.
 *
 * Data errors indicate that a class data member contains an invalid value, and that the class will
 * provide unpredictable results and may generate run-time calculation errors.
 * Data warnings indicate that a class data member contains an extreme value that is typically
 * outside of the normal bounds, but will still provide a calculable result.
 */
bool Component_Cable::Validate(bool is_included_warnings,
                               std::list<std::string>* messages_error) const
{
    bool is_valid = true;

    //determine if error messages are to be included
    bool is_included_messages;
    if (messages_error == nullptr)
    {
        is_included_messages = false;
    }
    else
    {
        is_included_messages = true;
    }

    // validate coefficient-expansion-thermal-linear
    if (m_coefficient_expansion_thermal_linear <= -0.005
            || 0.005 < m_coefficient_expansion_thermal_linear)
    {
        is_valid = false;
        if (is_included_messages == true)
        {
            messages_error->push_back("COMPONENT | CABLE - Invalid coefficient of thermal "
                                      "expansion");
        }
    }

//    // check load-strain coefficients
//    if (m_LoadStrainCoefficients.count() != 5) {
//        checkList.push_back("CABLE COMPONENT - Invalid load-strain coefficients")
//    }

//    // check load-strain coefficients
//    if (m_LoadStrainCoefficients.count() != 5) {
//        checkList.push_back("CABLE COMPONENT - Invalid load-strain coefficients")
//    }

    // validate load-limit-polynomial-creep
    if (m_load_limit_polynomial_creep < 0)
    {
        is_valid = false;
        if (is_included_messages == true)
        {
            messages_error->push_back("COMPONENT | CABLE - Invalid creep polynomial limit");
        }
    }

    // validate load-limit-polynomial-loadstrain
    if (m_load_limit_polynomial_loadstrain < 0)
    {
        is_valid = false;
        if (is_included_messages == true)
        {
            messages_error->push_back("COMPONENT | CABLE - Invalid load-strain polynomial limit");
        }
    }

    // validate modulus-compression-elastic-area
    if (m_modulus_compression_elastic_area < 0)
    {
        is_valid = false;
        if (is_included_messages == true)
        {
            messages_error->push_back("COMPONENT | CABLE - Invalid compression elastic area "
                                      "modulus");
        }
    }

    // validate modulus-tension-elastic-area
    if (m_modulus_tension_elastic_area < 0)
    {
        is_valid = false;
        if (is_included_messages == true)
        {
            messages_error->push_back("COMPONENT | CABLE - Invalid tension elastic area modulus");
        }
    }

    //return validation status
    return is_valid;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// CONSTRUCTOR / DESTRUCTOR ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////


Cable::Cable()
{
    m_area_electrical = -999999;
    m_area_physical = -999999;
    m_diameter = -999999;
    m_name = "";
    m_strength_rated = -999999;
    m_temperature_component_properties = -999999;
    m_type_construction = "";
    m_weight_unit = -999999;
}

Cable::~Cable()
{}


////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// PUBLIC MEMBER FUNCTIONS /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////


/**
 * This function should be used before extracting any information from the class.
 *
 * Data errors indicate that a class data member contains an invalid value, and that the class will
 * provide unpredictable results and may generate run-time calculation errors.
 * Data warnings indicate that a class data member contains an extreme value that is typically
 * outside of the normal bounds, but will still provide a calculable result.
 */
bool Cable::Validate(bool is_included_warnings,
                     std::list<std::string>* messages_error) const
{
    bool is_valid = true;

    //determine if error messages are to be included
    bool is_included_messages;
    if (messages_error == nullptr)
    {
        is_included_messages = false;
    }
    else
    {
        is_included_messages = true;
    }

    // validate area-electrical
    if (m_area_electrical < 0)
    {
        is_valid = false;
        if (is_included_messages == true)
        {
            messages_error->push_back("CABLE - Invalid electrical area");
        }
    }

    // validate area-physical
    if (m_area_physical < 0)
    {
        is_valid = false;
        if (is_included_messages == true)
        {
            messages_error->push_back("CABLE - Invalid physical area");
        }
    }

    // validate component-core
    m_component_core.Validate(is_included_warnings, messages_error);

    // validate component-shell
    m_component_shell.Validate(is_included_warnings, messages_error);

    // validate diameter
    if (m_diameter <= 0
            || (3 < m_diameter && is_included_warnings == true))
    {
        is_valid = false;
        if (is_included_messages == true)
        {
            messages_error->push_back("CABLE - Invalid diameter");
        }
    }

    // validate name
    // nothing to validate

    // validate strength-rated
    if (m_strength_rated < 0)
    {
        is_valid = false;
        if (is_included_messages == true)
        {
            messages_error->push_back("CABLE - Invalid rated strength");
        }
    }

    // validate temperature-component-properties
    if (m_temperature_component_properties < 0)
    {
        is_valid = false;
        if (is_included_messages == true)
        {
            messages_error->push_back("CABLE - Invalid component properties temperature");
        }
    }

    // validate type-construction
    // nothing to validate

    // validate type-polynomial-active
    if (m_type_polynomial_active != Type_Polynomial::CREEP
            || m_type_polynomial_active != Type_Polynomial::LOADSTRAIN)
    {
        is_valid = false;
        if (is_included_messages == true)
        {
            messages_error->push_back("CABLE - Invalid active polynomial type");
        }
    }

    // validate weight-unit
    if (m_weight_unit <= 0
            || (10 < m_weight_unit && is_included_warnings == true))
    {
        is_valid = false;
        if (is_included_messages == true)
        {
            messages_error->push_back("CABLE - Invalid unit weight");
        }
    }

    // return validation status
    return is_valid;
}
