//// This is free and unencumbered software released into the public domain.
//// For more information, please refer to <http://unlicense.org/>
//
//#pragma once
//
//#include <list>
//#include <string>
//
//#include "include/SupportLibrary.h"
//#include "include/WeatherLoadCase.h"
//
//
///**
// * @par CABLE UNIT LOAD CALCULATOR OVERVIEW
// *
// * This class uses the weather parameters in the LoadCase_Weather class and the cable properties
// * to generate unit loads on the cable. The unit loads are used for modeling a cable as a
// * catenary.
// *
// * \f[ DragForce = \frac{1}{2} \rho \nu^2 C_D A \f]
// *
// * This class models a unit length of cable. Unit loads are calculated in both the vertical and
// * transverse planes, and are based on the cable loading conditions and the geometry of the cable.
// *
// * @par ASSUMPTIONS
// *
// * - air is uniform and constant
// * - wind distribution is uniform and constant
// * - wind direction is perpendicular to the cross sectional area of the geometry
// * - drag coefficient is 1.0 (no aerodynamic effects)
// *
// * @par INPUT VERIFICATION
// * This class contains built-in input verification. To avoid runtime errors when using this class,
// * use the error checking functionality before extracting information.
// *
// */
//class CableUnitLoadCalculator
//{
//public:
//    /**
//     * @brief Default constructor.
//     */
//    CableUnitLoadCalculator();
//
//    /**
//     * @brief Destructor.
//     */
//    ~CableUnitLoadCalculator();
//
//    /**
//     * @brief Check for errors and warnings in class data.
//     * @param includeWarnings An option that includes data warnings in the return list.
//     * @return A list of strings containing class data errors/warnings.
//     */
//    std::list<std::string> CheckData(bool includeWarnings = true) const;
//
//    /**
//     *
//     */
//    Vector2D Get_UnitLoad(const LoadCase_Weather& loadCase) const;
//
//    // member variables
//    double m_Diameter_Cable;
//    double m_UnitWeight_Cable;
//};
