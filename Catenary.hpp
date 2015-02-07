// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#pragma once

#include <string>
#include <list>
#include <vector>

#include "SupportLibrary.hpp"

/**
 * @par CATENARY OVERVIEW
 *
 * This class models a catenary curve between two end points. The shape of the catenary curve is
 * defined by the horizontal tension and unit weight, while the endpoint spacing defines the portion
 * of the curve that is used.
 *
 * @par COORDINATE SYSTEM
 *
 * The coordinate system origin (0,0) is the catenary lowpoint.
 *
 * @par ASSUMPTIONS
 *
 * - constant horizontal tension
 * - uniform unit weight
 *
 * @par INPUT VERIFICATION
 *
 * This class contains built-in input verification. To avoid runtime errors when using this class,
 * use the error checking functionality before extracting information.
 */
class Catenary2D
{
public:
    /**
     * @brief Default constructor.
     */
    Catenary2D();

    /**
     * @brief Destructor.
     */
    ~Catenary2D();

    /**
     * @brief Check for errors and warnings in class data.
     * @param includeWarnings An option that includes data warnings in the return list.
     * @return A list of strings containing class data errors/warnings.
     */
    std::list<std::string> CheckData(bool includeWarnings = true) const;

    /**
     * @brief Get the constant (H/w), which determines the shape of the curve.
     * @return A copy of the curve constant.
     */
    double Get_Constant() const;

    /**
     * @brief Get a coordinate point at a location along the curve.
     * @param positionAlongCurve_Fraction A number between 0 and 1 describing position on the curve.
     * @return A coordinate point containing the x and y position of the catenary curve.
     */
    Point2D Get_Coordinate(const double& positionAlongCurve_Fraction) const;
    /**
     * @brief
     * @return
     */
    Point2D Get_Coordinate_Chord(const double& positionAlongCurve_Fraction) const;

    /**
     * @brief Get the endpoint spacing vector, starting from back-on-line and looking ahead-on-line.
     * @return A copy of the endpoint spacing vector. x is horizontal and y is vertical.
     */
    Vector2D Get_EndPointSpacing() const;

    /**
     * @brief Get the horizontal tension.
     * @return A copy of the curve horizontal tension.
     */
    double Get_HorizontalTension() const;

    /**
     * @brief Get the curve length.
     * @return The curve length between endpoints.
     */
    double Get_Length() const;

    /**
     * @brief Get a tangent unit vector.
     * @param positionAlongCurve_Fraction A number between 0 and 1 describing position on the curve.
     * @param direction The direction of the vector. Can be "BOL" for back-on-line, or "AOL" for ahead-on-line.
     * @return A tangent unit vector.
     */
    double Get_Tangent_Angle(const double& positionAlongCurve_Fraction,
                             const std::string& direction) const;

    /**
     * @brief Get a tangent unit vector.
     * @param positionAlongCurve_Fraction A number between 0 and 1 describing position on the curve.
     * @param direction The direction of the vector. Can be "BOL" for back-on-line, or "AOL" for ahead-on-line.
     * @return A tangent unit vector.
     */
    Vector2D Get_Tangent_Vector(const double& positionAlongCurve_Fraction, const std::string& direction) const;

    /**
     * @brief Get the tension.
     * @param positionAlongCurve_Fraction    A number between 0 and 1 describing position on the curve.
     * @return A tension magnitude.
     */
    double Get_Tension(const double& positionAlongCurve_Fraction) const;

    /**
     * @brief Get the tension.
     * @param positionAlongCurve_Fraction    A number between 0 and 1 describing position on the curve.
     * @param direction    The direction of the vector. Can be 'BOL' for back-on-line, or 'AOL' for ahead-on-line.
     * @return A tension vector.
     */
    Vector2D Get_Tension(const double& positionAlongCurve_Fraction, const std::string& direction) const;

    /**
     * @brief Get the tension.
     * @param positionAlongCurve_Fraction    A number between 0 and 1 describing position on the curve.
     * @return A tension magnitude.
     */
    double Get_Tension_Effective(const double& positionAlongCurve_Fraction) const;

    /**
     * @brief Get the unit weight.
     * @return A copy of the unit weight.
     */
    double Get_UnitWeight() const;

    /**
     * @brief Set the endpoint spacing.
     * @param x The horizontal separation of the endpoints, starting from back-on-line and looking ahead-on-line.
     * @param y The vertical spacing of the endpoints, starting from back-on-line and looking ahead-on-line.
     */
    void Set_EndpointSpacing(const double& x, const double& y);

    /**
     * @brief Set the horizontal tension.
     * @param horizontalTension The horizontal tension, which is the tension at the lowpoint. The
     *        horizontal tension is perpendicular to the unit weight.
     */
    void Set_Tension(const double& tension, const double& positionAlongCurve_Fraction = -1);

    /**
     * @brief Set the unit weight.
     * @param unitWeight The unit weight. The unit weight is perpendicular to the horizontal tension.
     */
    void Set_UnitWeight(const double& unitWeight);

protected:

    /**
     * @brief Get the x coordinate.
     * @param curveLength_FromOrigin The curve length from the origin.
     * @param direction The side of the origin that the coordinate is on. Can be either "BOL" or "AOL".
     * @return The x coordinate for the specified position.
     */
    double Coordinate_x(const double& curveLength_FromOrigin, const std::string& direction) const;

    /**
     * @brief Get the y coordinate.
     * @param coordinate_x The x coordinate for the specified position.
     * @return The y coordinate for the specified position. Does not include H/W elevation shift (origin y coordinate = 0).
     */
    double Coordinate_y(const double& coordinate_x) const;

    /**
     * @brief Get the curve length from the origin for the specified position.
     * @param coordinate_x The x coordinate for the specified position.
     */
    double Get_CurveLength_FromOrigin(const double& coordinate_x) const;

    /**
     * @brief Update the coordinate system so that the lowpoint is the origin (0, 0).
     */
    void Update_EndPointCoordinates() const;

    // member variables
    double		    m_HorizontalTension;
    double		    m_UnitWeight;
    Vector2D	    m_EndPointSpacing;
    mutable Point2D 	    m_EndPointBOL;
    mutable Point2D 	    m_EndPointAOL;

    mutable double  m_PositionAlongCurve_Fraction;
    mutable bool 	bUpdateRequired;
};


/**
 * @par CATENARY3D OVERVIEW
 *
 * This class models a transmission cable in a single span, where the cable is strung from one
 * attachment point to another. Various loading conditions can be modeled. For each loadcase,
 * the horizontal tension from the sag-tension solution must be provided.
 *
 * The catenary is the basis for all spancable calculations. The coordinate systems for the
 * spancable and catenary classes are significantly different, however this class handles all
 * necessary conversions. The catenary model maintains accuracy for attachment elevation changes,
 * and any wind loadings.
 */

/**
 * @par COORDINATE SYSTEM
 *
 * The coordinate system origin (0,0,0) is the back-on-line attachment point.
 */

/**
 * @par ASSUMPTIONS
 *
 * - all assumptions defined in UnitCable apply
 * - all assumptions defined in Catenary apply
 */

///**
// * @par ERROR CHECKING
// *
// * This class contains built-in input verification. To avoid runtime errors when using this class, use the
// * error checking functionality before extracting information.
// */
//class Catenary3D : public UnitCable
//{
//public:
//    //! @brief Constructor.
//    SpanCable();
//
//    //! @brief Destructor.
//    ~SpanCable();
//
//    /**
//     * @brief Check for errors and warnings in class data.
//     * @param includeWarnings An option that includes data warnings in the return list.
//     * @return A list of strings containing class data errors/warnings.
//     */
//    std::list<std::string> CheckData(bool includeWarnings = true) const;
//
//    /**
//     * @brief Get the cable attachment spacing.
//     * @return A copy of the 3D attachment spacing vector.
//    */
//    Vector3D Get_AttachmentSpacing() const;
//
//    /**
//     * @brief Get the amount of cable length.
//     * @return The cable length.
//    */
//    double Get_CableLength() const;
//
//    /**
//     * @brief Get the amount of cable slack.
//     * @return The amount of cable slack.
//    */
//    double Get_CableSlack() const;
//
//    /**
//     * @brief Get the catenary curve constant.
//     * @return The catenary curve constant.
//     */
//    double Get_CatenaryCurveConstant() const;
//
//    /**
//     * @brief Get the catenary horizontal tension.
//     * @return A copy of the catenary horizontal tension.
//     */
//    double Get_CatenaryHorizontalTension() const;
//
//    /**
//     * @brief Get a coordinate point at a cable location.
//     * @param PositionAlongCable_Fraction A number between 0 and 1 describing position on the cable.
//     * @return A coordinate point.
//     */
//    Point3D Get_Coordinate(const double& PositionAlongCable_Fraction) const;
//
//    /**
//     * @brief Get the sag of the cable.
//     * @return double
//     */
//    double Get_Sag() const;
//
//    /**
//     * @brief Get a tangent vector.
//     * @param positionAlongCable_Fraction A number between 0 and 1 describing position on the cable.
//     * @param direction Either BOL or AOL.
//     * @return A tangent vector at the specified position on the cable.
//     */
//    Vector3D Get_TangentVector(const double& positionAlongCable_Fraction,
//                               const std::string& direction) const;
//
//    /**
//     * @brief Get a tension vector.
//     * @param positionAlongCable_Fraction A number between 0 and 1 describing position on the cable.
//     * @param direction Either BOL or AOL.
//     * @return A tension vector at the specified position on the cable.
//     */
//    Vector3D Get_Tension(const double& positionAlongCable_Fraction,
//                         const std::string& direction) const;
//
//    /**
//     * @brief Save the class data for file output.
//     * @return A list of strings containing class data.
//     */
//    std::list<std::string> SaveData() const;
//
//    /**
//     * @brief Set the cable attachment spacing.
//     * @param horizontalSpacing The horizontal spacing between the cable attachment points, starting from back-on-line and looking ahead-on-line.
//     * @param verticalSpacing The vertical spacing between the cable attachment points, starting from back-on-line and looking ahead-on-line.
//     */
//    void Set_AttachmentSpacing(const double& horizontalSpacing, const double& verticalSpacing);
//
//    /**
//     * @brief Set the catenary horizontal tension, which will be used to model cable position and tension.
//     * @param horizontalTension The catenary tension is perpendicular to the unit weight. Is typically solved for in the sag-tension process.
//     * @see Catenary
//     */
//    void Set_CatenaryHorizontalTension(const double& horizontalTension);
//
//protected:
//    void        Update_CatenaryFit() const;
//
//    // member variables
//    Vector3D 	m_AttachmentSpacing;
//    double 		m_CatenaryHorizontalTension;
//
//    mutable Catenary 	m_Catenary;
//    mutable bool        bUpdateRequired_CatenaryFit;
//};
//
