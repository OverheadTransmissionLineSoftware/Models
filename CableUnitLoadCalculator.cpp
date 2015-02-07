// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include <cmath>

#include "GeometricShapes.hpp"
#include "CableUnitLoadCalculator.hpp"


////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// CONSTRUCTOR / DESTRUCTOR ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

CableUnitLoadCalculator::CableUnitLoadCalculator()
{}

CableUnitLoadCalculator::~CableUnitLoadCalculator()
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
std::list<std::string> CableUnitLoadCalculator::CheckData(bool includeWarnings) const
{
    std::list<std::string> checkList;

    // check cable diameter
    if (m_Diameter_Cable < 0)
    {
        checkList.push_back("CABLE UNIT LOAD CALCULATOR - Invalid cable diameter");
    }

    // check cable weight
    if (m_UnitWeight_Cable < 0)
    {
        checkList.push_back("CABLE UNIT LOAD CALCULATOR - Invalid cable unit weight");
    }

    return checkList;
}

/**
 * This routine solves for the vertical and transverse components of the unit loading on the cable.
 *
 * The vertical load on the cable is solved using the following formula:
 * \f[ V = w + \left(dv) \f]
 *
 * The transverse load on the cable is solved using the following formula:
 * \f[ T = AP \f]
*/
Vector2D CableUnitLoadCalculator::Get_UnitLoad(const LoadCase_Weather& loadCase) const
{
    Vector2D unitLoad;

    // check diameter and weight
    std::list<std::string> errorList = CheckData(false);
    if ( 0 < errorList.size() )
    {
        return unitLoad;
    }

    Cylinder bareCable;
    bareCable.Set_Diameter(m_Diameter_Cable / 12);
    bareCable.Set_Length(1);

    Cylinder icedCable;
    icedCable.Set_Diameter( (m_Diameter_Cable + (2 * loadCase.m_IceThickness)) / 12 );
    icedCable.Set_Length(1);

    const double iceVolume = icedCable.Get_Volume() - bareCable.Get_Volume();
    const double iceDensity = loadCase.m_IceDensity;
    const double iceWeight = iceVolume * iceDensity;

    // transverse unit weight
    unitLoad.x = ( icedCable.Get_Diameter() * icedCable.Get_Length() ) * loadCase.m_WindPressure;
    // vertical unit weight
    unitLoad.y = m_UnitWeight_Cable + iceWeight;

    return unitLoad;
}
