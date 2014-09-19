#include "Cable.hpp"


////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// CONSTRUCTOR / DESTRUCTOR ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////


CableComponent::CableComponent()
{
    m_ElasticAreaModulus = -999999;
    m_ThermalExpansionCoefficient = -999999;
}

CableComponent::~CableComponent()
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
std::list<std::string> CableComponent::CheckData(bool includeWarnings) const
{
    std::list<std::string> checkList;

    // check elastic area modulus
    if (m_ElasticAreaModulus < 0)
    {
        checkList.push_back("CABLE COMPONENT - Invalid area elastic modulus");
    }

//    // check load-strain coefficients
//    if (m_LoadStrainCoefficients.count() != 5) {
//        checkList.push_back("CABLE COMPONENT - Invalid load-strain coefficients")
//    }

    // check thermal expansion coefficient
    if (m_ThermalExpansionCoefficient <= -0.005 || 0.005 < m_ThermalExpansionCoefficient)
    {
        checkList.push_back("CABLE COMPONENT - Invalid thermal coefficient");
    }

    return checkList;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// CONSTRUCTOR / DESTRUCTOR ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////


Cable::Cable()
{
    m_Area_CrossSection = -999999;
    m_Area_Electrical = -999999;
    m_Diameter = -999999;
    m_Name = "";
    m_RatedBreakingStrength = -999999;
    m_Temperature_LoadStrainCoefficients = -999999;
    m_Type = "";
    m_UnitWeight = -999999;
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
std::list<std::string> Cable::CheckData(bool includeWarnings) const
{
    std::list<std::string> checkList;

    // check cross sectional area
    if (m_Area_CrossSection < 0)
    {
        checkList.push_back("CABLE - Invalid cross sectional area");
    }

    // check electrical area
    if (m_Area_Electrical < 0)
    {
        checkList.push_back("CABLE - Invalid electrical area");
    }

    // check core
    checkList.splice(checkList.end(), m_Core.CheckData(includeWarnings));

    // check diameter
    if (m_Diameter <= 0)
    {
        checkList.push_back("CABLE - Invalid diameter");
    }
    else if (3 < m_Diameter && includeWarnings == true)
    {
        checkList.push_back("CABLE - Diameter exceeds 3 inches");
    }

    // check rated breaking strength
    if (m_RatedBreakingStrength < 0)
    {
        checkList.push_back("CABLE - Invalid rated breaking strength");
    }

    // check shell
    checkList.splice(checkList.end(), m_Shell.CheckData(includeWarnings));

    // check temperature of load-strain coefficients
    if (m_Temperature_LoadStrainCoefficients < 0)
    {
        checkList.push_back("CABLE - Invalid temperature for load-strain coefficients");
    }

    // check unit weight
    if (m_UnitWeight <= 0)
    {
        checkList.push_back("CABLE - Invalid unit weight");
    }
    else if (10 < m_UnitWeight && includeWarnings == true)
    {
        checkList.push_back("CABLE - Unit weight exceeds 10 lbs/ft");
    }

    return checkList;
}
