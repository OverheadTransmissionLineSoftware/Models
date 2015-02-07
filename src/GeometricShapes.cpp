// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include <cmath>

#include "GeometricShapes.hpp"
#include "SupportLibrary.hpp"


////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// CONSTRUCTOR / DESTRUCTOR ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////


Cylinder::Cylinder()
{
    m_Diameter = -999999;
    m_Length = -999999;
}

Cylinder::~Cylinder()
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
std::list<std::string> Cylinder::CheckData(bool includeWarnings) const
{
    std::list<std::string> checkList;

    // diameter
    if (m_Diameter <= 0)
    {
        checkList.push_back("CYLINDER - Invalid diameter");
    }
    else if (100 < m_Diameter && includeWarnings == true)
    {
        checkList.push_back("CYLINDER - Diameter exceeds 100 inches");
    }

    // length
    if (m_Length <= 0)
    {
        checkList.push_back("CYLINDER - Invalid length");
    }
    else if (120 < m_Length && includeWarnings == true)
    {
        checkList.push_back("CYLINDER - Length exceeds 120 inches");
    }

    return checkList;
}

double Cylinder::Get_Diameter()
{
    return m_Diameter;
}

double Cylinder::Get_CrossSectionArea()
{
    return (m_Diameter * pi);
}

double Cylinder::Get_Length()
{
    return m_Length;
}

double Cylinder::Get_SurfaceArea()
{
    return 0;
}

double Cylinder::Get_Volume()
{
    return (pi / 4) * pow(m_Diameter, 2) * m_Length;
}

std::list<std::string> Cylinder::SaveData() const
{
    std::list<std::string> saveList;

    saveList.push_back("[CYLINDER]");
    saveList.push_back("diameter			= " + std::to_string(m_Diameter));
    saveList.push_back("length				= " + std::to_string(m_Length));

    return saveList;
}

void Cylinder::Set_Diameter(const double& Diameter)
{
    m_Diameter = Diameter;
}

void Cylinder::Set_Length(const double& Length)
{
    m_Length = Length;
}
