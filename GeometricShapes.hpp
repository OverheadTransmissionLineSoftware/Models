#pragma once

#include <list>
#include <string>

/**
 * @par SHAPE OVERVIEW
 *
 * This class models a generic shape. It must not be used directly, but instead, derived from by the
 * a unique shape.
 */
class Shape
{
public:
    Shape() {};
    ~Shape() {};

    virtual double Get_CrossSectionArea() = 0;
    virtual double Get_SurfaceArea() = 0;
    virtual double Get_Volume() = 0;
};

/**
 * @par CYLINDER OVERVIEW
 *
 * This class models a cylinder.
 * @see Shape
 *
 * @par INPUT VERIFICATION
 * This class contains built-in input verification. To avoid runtime errors when using this class,
 * use the error checking functionality before extracting information.
 */
class Cylinder : public Shape
{
public:
    /**
     * @brief Default constructor.
     */
    Cylinder();

    /**
     * @brief Destructor.
     */
    ~Cylinder();

    /**
     * @brief Check for errors and warnings in class data.
     * @param includeWarnings An option that includes data warnings in the return list.
     * @return A list of strings containing class data errors/warnings.
     */
    std::list<std::string> CheckData(bool includeWarnings = true) const;

    /**
     * @brief Get the cross-sectional area.
     * @return The cross-sectional area.
     */
    double  Get_CrossSectionArea();

    /**
     * @brief Get the diameter.
     * @return The diameter.
     */
    double  Get_Diameter();

    /**
     * @brief Get the length.
     * @return The length.
     */
    double  Get_Length();

    /**
     * @brief Get the surface area.
     * @return The surface area.
     */
    double Get_SurfaceArea();

    /**
     * @brief Get the volume.
     * @return The volume.
     */
    double  Get_Volume();

    /**
     * @brief Save the class data for file output.
     * @return A list of strings containing class data.
     */
    std::list<std::string> SaveData() const;

    /**
     * @brief Set the diameter.
     * @param diameter The diameter.
     */
    void    Set_Diameter(const double& diameter);

    /**
     * @brief Set the length.
     * @param length The length.
     */
    void    Set_Length(const double& length);

protected:
    double  m_Diameter;
    double  m_Length;
};

//
//SPHERE
//November 2013
//
//'OVERVIEW
//'This class models a sphere, and provides some geometric calculations.
//
//'ERROR CHECKING
//'This class contains built-in error checking. To avoid runtime errors when using this class, use the
//'error checking functionality before extracting information.
//*/
//
//class Sphere : public Shape
//{
//public:
//	// constructor/destructor
//	Sphere();
//	~Sphere();
//
//	// access protected variables
//	void    Set_Diameter(const double& Diameter);
//
//	double  Get_Diameter();
//
//	// calculation functions
//	double  Get_Volume();
//
//protected:
//	// members
//	double  m_Diameter;
//};
