// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>


#pragma once

#include <iostream>
#include <string>

// COMMON CONSTANTS
const double pi = 3.14159265358979;
const double radians_to_degrees = (180 / pi);
const double degrees_to_radians = (pi / 180);

double Round(double number, int precision);
std::string RemoveExtraZeroes(std::string number);

/**
 * @par POINT 2D OVERVIEW
 *
 * This data structure represents a 2D point, with members being of the type 'double'.
 */
struct Point2D
{
    double x;
    double y;
};

/**
 * @par POINT 3D OVERVIEW
 *
 * This data structure represents a 3D point, with members being of the type 'double'.
 */
struct Point3D
{
    double x;
    double y;
    double z;
};

/**
 * @par VECTOR 2D OVERVIEW
 *
 * This class represents a mathematical vector in a 2D Cartesian coordinate system. Although the
 * variables of this class are defined using the Cartesian coordinate system, components of the radial
 * coordinate system (magnitude and angle) are member functions.
 *
 * The vector can be manipulated by rotating and scaling.
 */
class Vector2D
{
public:
    /**
     * @brief Default constructor.
     */
    Vector2D();

    /**
     * @brief Alternate constructor.
     * @param X vector component
     * @param Y vector component
     */
    Vector2D(double X, double Y);

    /**
     * @brief Destructor.
     */
    ~Vector2D();

    double x;
    double y;

    /**
     * @brief Get the angle, in degrees.
     * @return The angle between the positive horizontal axis and the vector in degrees, defined
     * using a counter-clockwise orientation. A positive value will always be returned.
     */
    double Angle() const;

    /**
     * @brief Get the magnitude.
     * @return Vector length.
     */
    double Magnitude() const;

    /**
     * @brief Rotate the vector, in degrees.
     * @param rotationAngle The angle to rotate, in degrees. Positive values rotate counter-clockwise,
     *        while negative values rotate clockwise.
     */
    void Rotate(const double& rotationAngle);

    /**
     * @brief Scale the vector.
     * @param scaleFactor The scaling factor for the magnitude. Must be greater than or equal to 0.
     */
    void Scale(const double& scaleFactor);
};

/**
 * @par VECTOR 3D OVERVIEW
 *
 * This class represents a mathematical vector in a 3D Cartesian coordinate system. Although the
 * variables of this class are defined using the Cartesian coordinate system, components of the radial
 * coordinate system (magnitude and angle) are member functions.
 *
 * The vector can be manipulated by rotating and scaling.
 */
class Vector3D
{
public:
    /**
     * @brief Default constructor.
     */
    Vector3D();

    /**
     * @brief Alternate constructor.
     * @param X vector component
     * @param Y vector component
     * @param Z vector component
     */
    Vector3D(double X, double Y, double Z);

    /**
     * @brief Destructor.
     */
    ~Vector3D();

    double x;
    double y;
    double z;

    /**
     * @brief Get the angle in a specific plane, in degrees.
     * @param plane The specified plane. Can be the following options: (XY, YX, YZ, ZY, ZX, XZ).
     * @return The angle between the positive horizontal axis and the vector in degrees, defined
     * using a counter-clockwise orientation. A positive value will always be returned.
     */
    double Angle(const std::string& plane) const;

    /**
     * @brief Get the magnitude.
     * @return Vector length.
     */
    double Magnitude() const;

    /**
     * @brief Rotate the vector in a specific plane, in degrees.
     * @param plane The plane to rotate. Can be the following options: (XY, YX, YZ, ZY, ZX, XZ).
     * @param rotationAngle The angle to rotate, in degrees. Positive values rotate counter-clockwise,
     *        while negative values rotate clockwise.
     */
    void Rotate(const std::string& plane, const double& rotationAngle);

    /**
     * @brief Scale the vector.
     * @param scaleFactor The scaling factor for the magnitude. Must be greater than or equal to 0.
     */
    void Scale(const double& scaleFactor);
};
