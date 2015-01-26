// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>


#pragma once

#include <list>
#include <string>

/**
 * @par LOADCASE_WEATHER OVERVIEW
 *
 * This class models weather conditions, and acts as a container to store properties for air, ice,
 * and wind.
 *
 * @par AIR
 *
 * Air is modeled by specifying density and temperature. These properties can vary based on changes
 * in elevation, humidity, etc, which this class does NOT model.
 *
 * @par ICE
 *
 * Ice is modeled by specifying density and thickness. Typical ice types for transmission design are
 * rime and glaze ice. Rime ice is formed by atmospheric moisture condensation, and is the less
 * dense of the ice types. Glaze ice is formed when the ice melts and re-freezes, and is more dense.
 *
 * @par WIND
 *
 * Wind is modeled by velocity/speed.
 *
 * @par INPUT VERIFICATION
 *
 * This class contains built-in input verification. To avoid runtime errors when using this class,
 * use the error checking functionality before extracting information.
*/
class LoadCase_Weather
{
public:
    /**
     * @brief Default constructor.
     */
    LoadCase_Weather();

    /**
     * @brief Destructor.
     */
    ~LoadCase_Weather();

    /**
     * @brief Check for errors and warnings in class data.
     * @param includeWarnings An option that includes data warnings in the return list.
     * @return A list of strings containing class data errors/warnings.
     */
    std::list<std::string> CheckData(bool includeWarnings = true) const;

    /**
     *
     */
    std::string	m_CableCondition;
    double		m_CableTemperature;
    double 		m_IceDensity;
    double 		m_IceThickness;
    double 		m_WindPressure;
    double      m_WindSpeed;
};

///**
// * @par SAG CONSTRAINT OVERVIEW
// *
// * This class models loading conditions for transmission cables. It is derived from LoadCase, while
// * adding inputs for cable temperature and load-strain modulus.
// * @see LoadCase
// *
// * @par ASSUMPTIONS
// *
// * - all assumptions defined in LoadCase apply
// * - ice accumulation is uniform and constant
// *
// * @par INPUT VERIFICATION
// * This class contains built-in input verification. To avoid runtime errors when using this class,
// * use the error checking functionality before extracting information.
// */
//class SagConstraint : public SagCase
//{
//public:
//    /**
//     * @brief Default constructor.
//     */
//    SagConstraint();
//
//    /**
//     * @brief Alternate constructor.
//     * @param sagCase A sagcase object.
//     */
//    SagConstraint(const SagCase& sagCase);
//
//    /**
//     * @brief Destructor.
//     */
//    ~SagConstraint();
//
//    /**
//     * @brief Check for errors and warnings in class data.
//     * @param includeWarnings An option that includes data warnings in the return list.
//     * @return A list of strings containing class data errors/warnings.
//     */
//    std::list<std::string> CheckData(bool includeWarnings = true) const;
//
//	/**
//	 *
//	 */
//	double m_TensionLimit;
//	double m_TensionType;
//};
