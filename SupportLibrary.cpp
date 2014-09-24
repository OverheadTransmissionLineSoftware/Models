#include <cmath>
#include <string>

#include "SupportLibrary.hpp"

double Round(double number, int precision)
{
    return round(number * pow(10, precision)) / pow(10, precision);
}

//﻿std::string Trim(const std::string& str, const std::string& whitespace = " ")
//{
//    std::string::const_iterator strBegin = str.find_first_not_of(whitespace);
//    if (strBegin == std::string::npos)
//        return ""; // no content
//
//    const std::string strEnd = str.find_last_not_of(whitespace);
//    const std::string strRange = strEnd - strBegin + 1;
//
//    return str.substr(strBegin, strRange);
//}

//std::string RemoveExtraZeroes(std::string number)
//{
//	std::string newString;
//
//	for (std::string::const_iterator iter = number.begin(); iter!= number.end(); iter++)
//	{
//		const char character = *iter;
//
////		if (character == "0") {
////			delete character;
////		}
////		else if (character = ".") {
////			delete character;
////			return newString;
////		}
////		else if (character != "0") {
////			return newString;
////		}
//	}
//}


////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// 2D VECTOR CLASS //////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// CONSTRUCTOR / DESTRUCTOR ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////


Vector2D::Vector2D()
{
    x = -999999;
    y = -999999;
}

Vector2D::Vector2D(double X, double Y)
{
    x = X;
    y = Y;
}

Vector2D::~Vector2D()
{}


////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// PUBLIC MEMBER FUNCTIONS /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Determine which quadrant (I, II, III, IV) the vector is in, and use the arc tangent function
 * to calculate the angle.
 */
double Vector2D::Angle() const
{
    double angle;

    // use standard trigonometry to solve for angle of vector
    if (x == 0)
    {
        if (y == 0)
        {
            angle = -999999;
        }
        else if (0 < y)
        {
            angle = (pi / 2) * radians_to_degrees;
        }
        else if(y < 0)
        {
            angle = 3 * (pi / 2) * radians_to_degrees;
        }
    }
    else
    {
        // select quadrant
        if (0 < y)
        {
            if (0 < x)          // quadrant I
            {
                angle = atan(y / x) * radians_to_degrees;
            }
            else if (x < 0)     // quadrant II
            {
                angle = 180 + atan(y / x) * radians_to_degrees;
            }
        }
        else if (y <= 0)
        {
            if (x < 0)          // quadrant III
            {
                angle = 180 + atan(y / x) * radians_to_degrees;
            }
            else if (0 < x)     // quadrant IV
            {
                angle = 360 + atan(y / x) * radians_to_degrees;
            }
        }
    }

    return angle;
}

/**
 * The square root of the sum of the squares.
 */
double Vector2D::Magnitude() const
{
    return sqrt( pow(x, 2) + pow(y, 2) );
}

/**
 * The vector is converted into the radial coordinate system (magnitude and angle), rotated, and
 * then vector components are converted back into Cartesian components.
 */
void Vector2D::Rotate(const double& rotationAngle)
{
    // check if rotation angle is zero and exit if so
    if (std::abs(rotationAngle) < 0.00005)
        return;

    // define in terms of radial coordinate system (magnitude and angle)
    const double magnitude = Magnitude();
    const double angle = Angle();

    // calculate new angle (rotation included)
    const double newAngle = angle + rotationAngle;

    // resolve back to new x and y cartesian components
    x = magnitude * cos(newAngle * degrees_to_radians);
    y = magnitude * sin(newAngle * degrees_to_radians);
}

/**
 * The vector components are multiplied by the scale factor.
 */
void Vector2D::Scale(const double& scaleFactor)
{
    // check if scale factor is greater than or equal to zero
    if (scaleFactor < 0)
    {
        return;
    }

    x = x * scaleFactor;
    y = y * scaleFactor;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// 3D VECTOR CLASS //////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// CONSTRUCTOR / DESTRUCTOR ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////


Vector3D::Vector3D()
{
    x = -999999;
    y = -999999;
    z = -999999;
}

Vector3D::Vector3D(double X, double Y, double Z)
{
    x = X;
    y = Y;
    z = Z;
}

Vector3D::~Vector3D()
{}


////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// PUBLIC MEMBER FUNCTIONS /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////


/**
 * The planar vector components are assigned to a 2D vector. The angle is calculated using the
 * Vector2D::Angle function.
 * @see Vector2D
 */
double Vector3D::Angle(const std::string& plane) const
{
    // create a 2D vector of planar components
    Vector2D vector2D;

    // set vector components based on selected plane
    if (plane == "XY")
    {
        vector2D.x = x;
        vector2D.y = y;
    }
    else if (plane == "XZ")
    {
        vector2D.x = x;
        vector2D.y = z;
    }
    else if (plane == "YX")
    {
        vector2D.x = y;
        vector2D.y = x;
    }
    else if (plane == "YZ")
    {
        vector2D.x = y;
        vector2D.y = z;
    }
    else if (plane == "ZX")
    {
        vector2D.x = z;
        vector2D.y = x;
    }
    else if (plane == "ZY")
    {
        vector2D.x = z;
        vector2D.y = y;
    }

    return vector2D.Angle();
}

/**
 * The square root of the sum of the squares.
 */
double Vector3D::Magnitude() const
{
    return sqrt( pow(x, 2) + pow(y, 2) + pow(z, 2) );
}

/**
 * The planar vector components are assigned to a 2D vector. The rotation is done using the
 * Vector2D::Rotate function.
 * @see Vector2D
 */
void Vector3D::Rotate(const std::string& plane, const double& rotationAngle)
{
    // check if rotation angle is zero
    if (std::abs(rotationAngle) < 0.00005)
    {
        return;
    }

    // create a 2D vector of the planar components
    Vector2D vector2D;

    // set vector components based on selected plane
    if (plane == "XY")
    {
        vector2D.x = x;
        vector2D.y = y;
    }
    else if (plane == "XZ")
    {
        vector2D.x = x;
        vector2D.y = z;
    }
    else if (plane == "YX")
    {
        vector2D.x = y;
        vector2D.y = x;
    }
    else if (plane == "YZ")
    {
        vector2D.x = y;
        vector2D.y = z;
    }
    else if (plane == "ZX")
    {
        vector2D.x = z;
        vector2D.y = x;
    }
    else if (plane == "ZY")
    {
        vector2D.x = z;
        vector2D.y = y;
    }

    // rotate the plane
    vector2D.Rotate(rotationAngle);

    // assign rotated 2D vector components to new planar components
    if (plane == "XY")
    {
        x = vector2D.x;
        y = vector2D.y;
    }
    else if (plane == "XZ")
    {
        x = vector2D.x;
        z = vector2D.y;
    }
    else if (plane == "YX")
    {
        y = vector2D.x;
        x = vector2D.y;
    }
    else if (plane == "YZ")
    {
        y = vector2D.x;
        z = vector2D.y;
    }
    else if (plane == "ZX")
    {
        z = vector2D.x;
        x = vector2D.y;
    }
    else if (plane == "ZY")
    {
        z = vector2D.x;
        y = vector2D.y;
    }
}

/**
 * The vector components are multiplied by the scale factor.
 */
void Vector3D::Scale(const double& scaleFactor)
{
    // check if scale factor is greater than or equal to zero
    if (scaleFactor < 0)
    {
        return;
    }

    x = x * scaleFactor;
    y = y * scaleFactor;
    z = z * scaleFactor;
}
