// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include <cmath>

#include "SupportLibrary.hpp"
#include "LoadCase_Weather.hpp"


////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// CONSTRUCTOR / DESTRUCTOR ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////


LoadCase_Weather::LoadCase_Weather()
{
    m_CableCondition = "";
    m_CableTemperature = -999999;
    m_IceDensity = -999999;
    m_IceThickness = -999999;
    m_WindPressure = -999999;
    m_WindSpeed = -999999;
}

LoadCase_Weather::~LoadCase_Weather()
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
std::list<std::string> LoadCase_Weather::CheckData(bool includeWarnings) const
{
    std::list<std::string> checkList;

    // check cable condition
    if (m_CableCondition != "INITIAL" || m_CableCondition != "FINAL")
    {
        checkList.push_back("LOADCASE | WEATHER - Invalid cable condition");
    }

    // check cable temperature
    if (m_CableTemperature < -50)
    {
        checkList.push_back("LOADCASE | WEATHER - Invalid cable temperature");
    }
    else if (500 < m_CableTemperature && includeWarnings == true)
    {
        checkList.push_back("LOADCASE | WEATHER - Cable temperature exceeds 212 deg F");
    }

    // check ice density
    if (m_IceDensity < 0)
    {
        checkList.push_back("LOADCASE | WEATHER - Invalid ice density");
    }
    else if (100 < m_IceDensity && includeWarnings == true)
    {
        checkList.push_back("LOADCASE | WEATHER - Ice density exceeds 100 lbs/ft");
    }

    // check ice thickness
    if (m_IceThickness < 0)
    {
        checkList.push_back("LOADCASE | WEATHER - Invalid ice thickness");
    }
    else if (4 < m_IceThickness && includeWarnings == true)
    {
        checkList.push_back("LOADCASE | WEATHER - Ice thickness exceeds 4 inches");
    }

    // check wind pressure
    if (m_WindPressure < 0)
    {
        checkList.push_back("LOADCASE | WEATHER - Invalid wind pressure");
    }
    else if (25 < m_WindPressure && includeWarnings == true)
    {
        checkList.push_back("LOADCASE | WEATHER - Wind pressure exceeds 25 psf");
    }

    return checkList;
}
