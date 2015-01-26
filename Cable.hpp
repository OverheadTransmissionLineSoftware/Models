// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>


#pragma once

#include <list>
#include <vector>
#include <string>


/**
 * @par CABLE COMPONENT OVERVIEW
 *
 * This class models a transmission cable component.
 *
 * @par INPUT VERIFICATION
 *
 * This class contains built-in input verification. To avoid runtime errors when using this class,
 * use the error checking functionality before extracting information.
 */
class CableComponent
{
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
     * @brief Check for errors and warnings in class data.
     * @param includeWarnings An option that includes data warnings in the return list.
     * @return A list of strings containing class data errors/warnings.
     */
    std::list<std::string> CheckData(bool includeWarnings = true) const;

    // member variables
    double					m_ElasticAreaModulus;
    std::vector<double> 	m_LoadStrainCoefficients;
    double					m_ThermalExpansionCoefficient;
};


/**
 * @par CABLE OVERVIEW
 *
 * This class models a transmission cable.
 *
 * @par INPUT VERIFICATION
 *
 * This class contains built-in input verification. To avoid runtime errors when using this class,
 * use the error checking functionality before extracting information.
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
     * @brief Check for errors and warnings in class data.
     * @param includeWarnings An option that includes data warnings in the return list.
     * @return A list of strings containing class data errors/warnings.
     */
    std::list<std::string> CheckData(bool includeWarnings = true) const;

    // member variables
    double          m_Area_CrossSection;
    double			m_Area_Electrical;
    CableComponent	m_Core;
    double			m_Diameter;
    std::string 	m_Name;
    double 			m_RatedBreakingStrength;
    CableComponent	m_Shell;
    double 			m_Temperature_LoadStrainCoefficients;
    std::string		m_Type;
    double			m_UnitWeight;
};
