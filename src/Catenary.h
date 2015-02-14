// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef TRANSMISSIONLINE_CATENARY_H_
#define TRANSMISSIONLINE_CATENARY_H_

#include <list>
#include <string>

#include "SupportLibrary.h"

/**
 * @par CATENARY OVERVIEW
 *
 * This class models a 2D catenary.
 *
 * The shape of the catenary curve is defined by the horizontal tension and unit
 * weight, while the endpoit spacing defines the portion of the curve that is
 * used.
 *
 * @par COORDINATE SYSTEM
 *
 * The coordinate system origin (0,0) is the catenary lowpoint, unless shifted.
 * A shifted system uses the left endpoint as the origin.
 *  x = horizontal
 *  y = vertical
 *
 * @par ASSUMPTIONS
 *
 * - constant horizontal tension
 * - uniform unit weight
 */
class Catenary2D {
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
   * @brief Gets the constant (H/w), which determines the shape of the curve.
   * @return The constant of the curve.
   */
  double Constant() const;

  /**
   * @brief Gets a coordinate point at a location on the curve.
   * @param position_fraction A decimal ranging from 0 and 1  that describes
   *        position on the curve by the fraction of curve length from the left
   *        end point.
   * @param is_shifted_origin A flag that shifts the coordinate system origin to
   *        the left end point.
   * @return A 2D coordinate point containing the x and y position of the
   *         catenary curve.
   */
  Point2D Coordinate(const double& position_fraction,
                     const bool& is_shifted_origin = false) const;
  /**
   * @brief Gets a coordinate point at a location on the straight line
   *        connecting the end points.
   * @param position_fraction A decimal ranging from 0 and 1  that describes
   *        position on the curve by the fraction of curve length from the left
   *        end point.
   * @param is_shifted_origin A flag that shifts the coordinate system origin to
   *        the left end point.
   * @return A 2D coordinate point containing the x and y position of the
   *         straight line connecting the end points.
   */
  Point2D CoordinateChord(const double& position_fraction,
                          const bool& is_shifted_origin = false) const;

  /**
   * @brief Gets the curve length.
   * @return The curve length between endpoints.
   */
  double Length() const;

  /**
   * @brief Gets the length of catenary slack.
   * @return The difference in length between the curve length and straight-line
   *         length.
   */
  double LengthSlack() const;

  /**
   * @brief Gets the position fraction of the origin.
   * @return The position fraction of the origin, or the lowpoint.
   */
  double PositionFractionOrigin() const;

  /**
   * @brief Gets the position fraction of the sag point.
   * @return The position fraction of the sag point, or where the tangent line
   *         of the catenary is parallel with the straight line between
   *         endpoints.
   */
  double PositionFractionSagPoint() const;

  /**
   * @brief Gets the sag of the catenary.
   * @return The vertical distance bewteen the straight line and the catenary
   *         the sag point.
   */
  double Sag() const;

  /**
   * @brief Gets a tangent angle from the horizontal axis to the curve tangent
   *        line.
   * @param position_fraction A decimal ranging from 0 and 1  that describes
   *        position on the curve by the fraction of curve length from the left
   *        end point.
   * @param direction The axis direction of the tangent line.
   * @return A tangent angle.
   */
  double TangentAngle(const double& position_fraction,
                      const AxisDirectionType& direction) const;

  /**
   * @brief Gets a tangent unit vector.
   * @param position_fraction A decimal ranging from 0 and 1  that describes
   *        position on the curve by the fraction of curve length from the left
   *        end point.
   * @param direction The axis direction of the tangent line.
   * @return A tangent unit vector.
   */
  Vector2D TangentVector(const double& position_fraction,
                         const AxisDirectionType& direction) const;

  /**
   * @brief Gets the tension.
   * @param position_fraction A decimal ranging from 0 and 1  that describes
   *        position on the curve by the fraction of curve length from the left
   *        end point.
   * @return The tension at the location on the catenary.
   */
  double Tension(const double& position_fraction) const;

  /**
   * @brief Gets the tension.
   * @param position_fraction A decimal ranging from 0 and 1  that describes
   *        position on the curve by the fraction of curve length from the left
   *        end point.
   * @param direction The axis direction of the tangent line.
   * @return A tension vector.
   */
  Vector2D Tension(const double& position_fraction,
                   const AxisDirectionType& direction) const;

  /**
   * @brief Gets the average catenary tension.
   * @param num_points The number of points used to calculate average tension.
   *        If zero is specified, the calculation technique will use the
   *        Ehrenburg approximation method.
   * @return The average tension of the catenary.
   */
  double TensionAverage(const int& num_points = 100) const;

  /**
   * @brief Gets the maximum tension of the catenary.
   * @return The maximum tension, which is at the highest end point.
   */
  double TensionMax() const;

  /**
   * @brief Validates class data.
   * @param is_included_warnings A flag that tightens the acceptable value
   *        range.
   * @param messages_error A list of detailed error messages. If this is
   *        provided, any validation errors will be appended to the list.
   * @return A boolean value indicating status of class data.
   */
  bool Validate(bool is_included_warnings = true,
                std::list<std::string>* messages_error = nullptr) const;

  /**
   * @brief Sets the endpoint spacing.
   * @param x The vector spacing between the left and right end point. The
   *        vector starts at the left point and ends at the right point.
   */
  void set_spacing_endpoints(const Vector2D& spacing_endpoints);

  /**
   * @brief Sets the horizontal tension.
   * @param tension_horizontal The horizontal tension, which is the tension at
   *        the lowpoint/origin. The horizontal tension is perpendicular to the
   *        unit weight.
   */
  void set_tension_horizontal(const double& tension_horizontal);

  /**
   * @brief Sets the unit weight.
   * @param weight_unit The unit weight, which is the uniform load on the
   *        catenary. The unit weight is perpendicular to the horizontal
   *        tension.
   */
  void set_weight_unit(const double& weight_unit);

  /**
   * @brief Gets the endpoint spacing vector, starting from the left point and
   *        ending at the right point.
   * @return A copy of the endpoint spacing vector.
   */
  Vector2D spacing_endpoints() const;

  /**
   * @brief Gets the horizontal tension.
   * @return A copy of the curve horizontal tension.
   */
  double tension_horizontal() const;

  /**
   * @brief Gets the unit weight.
   * @return A unit weight.
   */
  double weight_unit() const;

private:

  /**
   * @brief Gets the x coordinate of the catenary. This function removes the
   *        H/w constant ordinate shift.
   * @param length_origin_to_position The length of the curve from the origin to
   *        the position.
   * @param direction_origin_to_position The direction from the origin to the
   *        position.
   * @return The x coordinate for the specified position.
   */
  double CoordinateX(const double& length_origin_to_position,
                     const AxisDirectionType& direction_origin_to_position)
  const;

  /**
   * @brief Gets the y coordinate of the catenary. This function removes the
   *        H/w constant ordinate shift.
   * @param length_origin_to_position The length of the curve from the origin to
   *        the position.
   * @param direction_origin_to_position The direction from the origin to the
   *        position.
   * @return The y coordinate for the specified position.
   */
  double CoordinateY(const double& length_origin_to_position,
                     const AxisDirectionType& direction_origin_to_position)
  const;

  /**
   * @brief Gets the length from the origin for the specified position.
   * @param coordinate The coordinate for the specified position.
   * @return The curve length from the catenary origin to the coordinate.
   */
  double LengthFromOrigin(const Point2D& coordinate) const;

  /**
   * @brief Determines if class is updated.
   * @return A boolean indicating if class is updated.
   */
  bool IsUpdated() const;

  /**
   * @brief Gets the fraction fo the length that relates to a specific position
   *        on the catenary. This is referenced fromt he left end point.
   * @param tangent_angle The tangent angle of a speicifc position on the
   *        catenary.
   * @return The fraction of the length for the catenary position, from the left
   *         end point.
   */
  double PositionFraction(const double& tangent_angle) const;

  /**
   * @brief Updates cached member variables and modifies control variables if
   *        update is required.
   * @return A boolean indicating if class updates completed successfully.
   */
  bool Update() const;

  /**
   * @brief Updates the endpoint coordinates.
   * @return A boolean indicating the success status of the update.
   */
  void UpdateEndPoints() const;

  // member variables
  mutable bool    is_updated_points_end_;
  mutable Point2D point_end_left_;
  mutable Point2D point_end_right_;
  Vector2D        spacing_endpoints_;
  double          tension_horizontal_;
  double          weight_unit_;
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
//    Vector3D  m_AttachmentSpacing;
//    double    m_CatenaryHorizontalTension;
//
//    mutable Catenary  m_Catenary;
//    mutable bool        bUpdateRequired_CatenaryFit;
//};
//

#endif // TRANSMISSIONLINE_CATENARY_H_
