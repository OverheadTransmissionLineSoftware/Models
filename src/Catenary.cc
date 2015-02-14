// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "Catenary.h"

#include <cmath>

Catenary2D::Catenary2D()
{
  tension_horizontal_ = -999999;
  weight_unit_ = -999999;

  is_updated_points_end_ = true;
}

Catenary2D::~Catenary2D()
{}

/**
 * \f[ \frac{H}{w} = \frac{HorizontalTension}{UnitWeight} \f]
 */
double Catenary2D::Constant() const
{
  double constant = -999999;

  if (IsUpdated() == false) {
    if (Update() == false) {
      return constant;
    }
  }

  const double H = tension_horizontal_;
  const double w = weight_unit_;

  return H / w;
}

/**
 * Solves for length and direction from origin, and then converts to
 * coordinates.
 */
Point2D Catenary2D::Coordinate(const double& position_fraction,
                               const bool& is_shifted_origin) const
{
  Point2D coordinate;

  if (IsUpdated() == false) {
    if (Update() == false) {
      return coordinate;
    }
  }

  // length from left end to position
  double length_left_to_position = position_fraction * Length();

  // length from origin to left endpoint
  const double length_origin_to_left = LengthFromOrigin(point_end_left_);

  // solve for length from origin and direction from origin
  double length_origin_to_position;
  AxisDirectionType direction_origin_to_position;

  // select how to calculate length and position from origin based on
  // endpoint coordinates

  // both endpoints are left from origin
  if (point_end_left_.x < 0 && point_end_right_.x < 0) {
    length_origin_to_position = length_origin_to_left -
                                length_left_to_position;
    direction_origin_to_position = AxisDirectionType::POSITIVE;
  }
  // one endpoint left from origin, one endpoint right from origin
  else if (point_end_left_.x < 0 && 0 < point_end_right_.x) {

    // left from origin
    if (length_left_to_position < length_origin_to_left) {
      length_origin_to_position = length_origin_to_left -
                                  length_left_to_position;
      direction_origin_to_position = AxisDirectionType::NEGATIVE;
    }
    // at origin
    else if (length_left_to_position == length_origin_to_left) {
      length_origin_to_position = 0;
      direction_origin_to_position = AxisDirectionType::POSITIVE;
    }
    // right from origin
    else if (length_origin_to_left < length_left_to_position) {
      length_origin_to_position = length_left_to_position -
                                  length_origin_to_left;
      direction_origin_to_position = AxisDirectionType::POSITIVE;
    }
  }
  // both endpoints are AOL of origin
  else if (0 < point_end_left_.x && 0 < point_end_right_.x) {
    length_origin_to_position = length_origin_to_left +
                                length_left_to_position;
    direction_origin_to_position = AxisDirectionType::POSITIVE;
  }

  // solve for catenary coordinate
  coordinate.x = CoordinateX(length_origin_to_position,
                             direction_origin_to_position);
  coordinate.y = CoordinateY(length_origin_to_position,
                             direction_origin_to_position);

  return coordinate;
}

Point2D Catenary2D::CoordinateChord(const double& position_fraction,
                                    const bool& is_shifted_origin) const
{
  Point2D coordinate_chord;

  if (IsUpdated() == false) {
    if (Update() == false) {
      return coordinate_chord;
    }
  }

  // get a catenary coordinate
  Point2D coordinate_catenary = Coordinate(position_fraction,
                                is_shifted_origin);

  // calculate a chord coordinate
  coordinate_chord.x = coordinate_catenary.x;
                       coordinate_chord.y = point_end_left_.y
                           + ((coordinate_catenary.x - point_end_left_.x)
                           * (spacing_endpoints_.y() / spacing_endpoints_.x()));

  // check if shifted coordinate is requested, modify if necessary
  if (is_shifted_origin == true) {
    coordinate_chord.x = coordinate_chord.x - point_end_left_.x;
                         coordinate_chord.y = coordinate_chord.y
                                                - point_end_left_.y;
  }

  return coordinate_chord;
}

///**
// * If the lowpoint is within the curve section being used, the length from the
// * origin to the left endpoint and the length from the origin to the right
// * endpoint are summed. If the origin is beyond an endpoint, the length to the
// * closest endpoint is treated as negative.
// * @see Catenary2D::LengthFromOrigin
// */
//double Catenary2D::Length() const
//{
//  if (bUpdateRequired == true) {
//    Update_CoordinateSystem();
//  }
//
//  // calculate curve length to endpoints
//  const double curveLength_OriginToEndPointBOL = Get_CurveLength_FromOrigin(
//        point_end_left_.x);
//  const double curveLength_OriginToEndPointAOL = Get_CurveLength_FromOrigin(
//        point_end_right_.x);
//
//  double CurveLength;
//
//  // selects calculation based on endpoint coordinate positions
//  if (point_end_left_.x < 0 && point_end_right_.x < 0) {
//    CurveLength = curveLength_OriginToEndPointBOL - curveLength_OriginToEndPointAOL;
//  } else if (point_end_left_.x < 0 && 0 < point_end_right_.x) {
//    CurveLength = curveLength_OriginToEndPointBOL + curveLength_OriginToEndPointAOL;
//  } else if (0 < point_end_left_.x && 0 < point_end_right_.x) {
//    CurveLength = curveLength_OriginToEndPointAOL - curveLength_OriginToEndPointBOL;
//  }
//
//  return CurveLength;
//}
//
//Vector2D Catenary::Get_EndPointSpacing() const
//{
//  return m_EndPointSpacing;
//}
//
//double Catenary::Get_HorizontalTension() const
//{
//  return tension_horizontal_;
//}
//
///**
// * The slope at the position is calculated, and then adjusted to a unit vector based on the
// * direction the vector is pointing.
// * Slope equation is:
// * \f[ Slope = sinh \left( \frac{x}{\frac{H}{w}} \right) \f]
// */
//Vector2D Catenary::Get_TangentVector(const double& positionAlongCurve_Fraction,
//                                     const std::string& direction) const
//{
//  if (bUpdateRequired == true) {
//    Update_CoordinateSystem();
//  }
//
//  // get coordinate at position
//  const Point2D coordinate = Get_Coordinate(positionAlongCurve_Fraction);
//
//  // calculate slope at position
//  const double H = tension_horizontal_;
//  const double w = weight_unit_;
//  const double x = coordinate.x;
//
//  const double Slope = sinh(x / (H / w));
//
//  // calculate magnitude of x (= 1) and y (= slope) vector components
//  const double VectorMagnitude = sqrt(pow(Slope, 2) + pow(1, 2));
//
//  // solve for a 2D tangent unit vector
//  // BOL reverses the sign on both the x and y components, AOL remains the same sign
//  Vector2D TangentVector;
//  if (direction == "BOL") {
//    TangentVector.x = -(1 / VectorMagnitude);
//    TangentVector.y = -(Slope / VectorMagnitude);
//  } else if (direction == "AOL") {
//    TangentVector.x = 1 / VectorMagnitude;
//    TangentVector.y = Slope / VectorMagnitude;
//  }
//
//  return TangentVector;
//}
//
///**
// * A tangent unit vector is calculated and then multiplied by the tension magnitude.
// * The tension magnitude equation is:
// * \f[ Tension = H cosh \left( \frac{x}{\frac{H}{w}} \right) \f]
// * @see Catenary::Get_TangentVector
// */
//Vector2D Catenary::Get_Tension(const double& positionAlongCurve_Fraction,
//                               const std::string& direction) const
//{
//  if (bUpdateRequired == true) {
//    Update_CoordinateSystem();
//  }
//
//  // get coordinate at position
//  const Point2D Coordinate = Get_Coordinate(positionAlongCurve_Fraction);
//
//  // calculate tension magnitude
//  const double H = tension_horizontal_;
//  const double w = weight_unit_;
//  const double x = Coordinate.x;
//
//  const double TensionMagnitude = H * cosh(x / (H / w));
//
//  // get tangent vector
//  const Vector2D TangentVector = Get_TangentVector(positionAlongCurve_Fraction,
//                                 direction);
//
//  // resolve tension magnitude to tangent vector
//  Vector2D Tension;
//  Tension.x = TensionMagnitude * TangentVector.x;
//  Tension.y = TensionMagnitude * TangentVector.y;
//
//  return Tension;
//}
//
//double Catenary::Get_UnitWeight() const
//{
//  return weight_unit_;
//}
//
///**
// * Assigns parameter values to member variables and marks that an update is required.
// */
//void Catenary::Set_EndpointSpacing(const double& x, const double& y)
//{
//  m_EndPointSpacing.x = x;
//  m_EndPointSpacing.y = y;
//
//  bUpdateRequired_CoordinateSystem = true;
//}
//
///**
// * Assigns parameter value to a member variable and marks that an update is required.
// */
//void Catenary::Set_Tension(const double& tension,
//                           const double& positionAlongCurve_Fraction)
//{
//  // if an invalid tension position is provided, mark as a horizontal tension
//  if (positionAlongCurve_Fraction < 0) {
//    tension_horizontal_ = tension;
//    m_PositionAlongCurve_Fraction = -999999;
//    bUpdateRequired_HorizontalTension = false;
//  }
//
//  // if a valid tension position is provided, mark that horizontal tension needs to be solved for
//  else if (0 <= positionAlongCurve_Fraction) {
//    tension_horizontal_ = tension;
//    m_PositionAlongCurve_Fraction = positionAlongCurve_Fraction;
//    bUpdateRequired_HorizontalTension = true;
//  }
//
//  bUpdateRequired_CoordinateSystem = true;
//}
//
///**
// * Assigns parameter value to a member variable and marks that an update is required.
// */
//void Catenary::Set_UnitWeight(const double& unitWeight)
//{
//  weight_unit_ = unitWeight;
//
//  bUpdateRequired_CoordinateSystem = true;
//}
//
//std::list<std::string> Catenary2D::Validate(bool includeWarnings) const
//{
//  std::list<std::string> checkList;
//
//  // check horizontal tension
//  if (tension_horizontal_ <= 0) {
//    checkList.push_back("CATENARY - Invalid horizontal tension");
//  } else if (100000 < tension_horizontal_ && includeWarnings == true) {
//    checkList.push_back("CATENARY - Horizontal tension exceeds 100,000 lb");
//  }
//
//  // check unit weight
//  if (weight_unit_ <= 0) {
//    checkList.push_back("CATENARY - Invalid unit weight");
//  } else if (15 < weight_unit_ && includeWarnings == true) {
//    checkList.push_back("CATENARY - Unit weight exceeds 15 lb/ft");
//  }
//
//  // check endpoint spacing - horizontal
//  if (m_EndPointSpacing.x < 0) {
//    checkList.push_back("CATENARY - Invalid horizontal endpoint spacing");
//  } else if (5000 < m_EndPointSpacing.x && includeWarnings == true) {
//    checkList.push_back("CATENARY - Horizontal endpoint spacing exceeds 5000 ft");
//  }
//
//  // endpoint spacing - vertical
//  if (m_EndPointSpacing.y  <= -2000 || 2000 <= m_EndPointSpacing.y) {
//    checkList.push_back("CATENARY - Invalid vertical endpoint spacing");
//  } else if ((m_EndPointSpacing.y <=-1000 || 1000 <= m_EndPointSpacing.y)
//             && includeWarnings == true) {
//    checkList.push_back("CATENARY - Vertical endpoint spacing exceeds 1000 ft");
//  }
//
//  return checkList;
//}
//
///**
// * The function is a derivation of the equation for curve length.
// * @see Catenary::Get_CurveLength_FromOrigin
// * \f[ x = \frac{H}{w} sinh^{-1} \left(\frac{L}{\frac{H}{w}}\right) \f]
// */
//double Catenary::Coordinate_x(const double& curveLength_FromOrigin,
//                              const std::string& directionFromOrigin) const
//{
//  const double L = curveLength_FromOrigin;
//  const double H = tension_horizontal_;
//  const double w = weight_unit_;
//
//  double coordinate_x;
//
//  // BOL from origin - negative x coordinate
//  if (directionFromOrigin == "BOL") {
//    coordinate_x = - (H / w) * (asinh(L / (H / w)));
//  }
//  // AOL from origin - positive x coordinate
//  else if (direction == "AOL") {
//    coordinate_x = (H / w) * (asinh(L / (H / w)));
//  }
//
//  return coordinate_x;
//}
//
///**
// * \f[ y = \frac{H}{w} \cosh \left(\frac{x}{\frac{H}{w}-1}\right) \f]
// */
//double Catenary::Coordinate_y(const double& curveLengthFromOrigin,
//                              const double& directionFromOrigin) const
//{
//  const double x = Coordinate_x(curveLengthFromOrigin, directionFromOrigin);
//  const double H = tension_horizontal_;
//  const double w = weight_unit_;
//
//  return (H / w) * (cosh(x / (H / w)) - 1);
//}
//
///**
// * \f[ CurveLength = \left| \frac{H}{w} sinh^{-1} \left(\frac{x}{\frac{H}{w}}\right) \right| \f]
// */
//double Catenary::CurveLength_FromOrigin(const Point2D coordinate) const
//{
//  const double x = coordinate.x;
//  const double H = tension_horizontal_;
//  const double w = weight_unit_;
//
//  return std::abs((H / w) * sinh(x / (H / w)));
//}
//
///**
// *
// */
//bool Catenary2D::Update()
//{
//
//}
//
///**
// * Hyperbolic identity equations are used to determine the x distance that each endpoint is from the
// * origin. They are as follows:
// * \f[ xBOL = \frac{A}{2} - \frac{H}{w} sinh^{-1} \left( \frac{\frac{B}{2}}{ \frac{H}{w} sinh \left( \frac{\frac{A}{2}}{\frac{H}{w}} \right)} \right) \f]
// * \f[ xAOL = \frac{A}{2} + \frac{H}{w} sinh^{-1} \left( \frac{\frac{B}{2}}{ \frac{H}{w} sinh \left( \frac{\frac{A}{2}}{\frac{H}{w}} \right)} \right) \f]
// * If a negative value is the solution of either equation, it indicates that the origin is not
// * bound by the endpoints. The endpoint coordinates are then solved for and stored for later
// * calculations.
// */
//void Catenary::Update_CoordinateSystem() const
//{
//  // check class inputs and exit if errors are present
//  std::list<std::string> errorList = CheckData(false);
//  if (0 < errorList.size()) {
//    return;
//  }
//
//  // define equation variables
//  const double H = tension_horizontal_;
//  const double w = weight_unit_;
//  const double A = m_EndPointSpacing.x;
//  const double B = m_EndPointSpacing.y;
//  const double z = (A / 2) / (H / w);
//
//  // solve for back-on-line endpoint coordinate
//  point_end_left_.x = (H / w) * (asinh(B * z) / (A * sinh(z)) - z);
//  point_end_right_.y =
//
//    // solve for ahead-on-line endpoint coordinate
//    point_end_right_.x = (H / w) * (asinh(B * z) / (A * sinh(z)) + z);
//  point_end_right_.y =
//
//    // reset update flag
//    bUpdateRequired = false;
//}
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// CONSTRUCTOR / DESTRUCTOR ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////
////SpanCable::SpanCable()
////{
////    m_CatenaryHorizontalTension = -9999;
////
////    bUpdateRequired_CatenaryFit = true;
////}
////
////SpanCable::~SpanCable()
////{}
////
////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// PUBLIC MEMBER FUNCTIONS /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////
/////**
//// * This function should be used before extracting any information from the class.
//// *
//// * Data errors indicate that a class data member contains an invalid value, and that the class will
//// * provide unpredictable results and may generate run-time calculation errors.
//// * Data warnings indicate that a class data member contains an extreme value that is typically
//// * outside of the normal bounds, but will still provide a calculable result.
//// */
////std::list<std::string> SpanCable::CheckData(bool includeWarnings) const
////{
////    std::list<std::string> checkList;
////
////    // check base class
////    checkList.splice(checkList.end(), UnitCable::CheckData(includeWarnings));
////
////    // attachment spacing
////    if (m_AttachmentSpacing.x <= 0)
////    {
////        checkList.push_back("SPANCABLE - Invalid horizontal attachment spacing");
////    }
////    else if (5000 < m_AttachmentSpacing.x && includeWarnings == true)
////    {
////        checkList.push_back("SPANCABLE - Horizontal attachment spacing exceeds 5000 ft");
////    }
////
////    if (m_AttachmentSpacing.z < -2000 && 2000 < m_AttachmentSpacing.z)
////    {
////        checkList.push_back("SPANCABLE - Invalid vertical attachment spacing");
////    }
////    else if (m_AttachmentSpacing.z < -1000 && 1000 < m_AttachmentSpacing.z)
////    {
////        checkList.push_back("SPANCABLE - Invalid vertical attachment spacing");
////    }
////
////    // horizontal tension
////    if (m_CatenaryHorizontalTension <= 0)
////    {
////        checkList.push_back("Invalid horizontal tension");
////    }
////    else if (100000 < m_CatenaryHorizontalTension && includeWarnings == true)
////    {
////        checkList.push_back("Horizontal tension exceeds 100,000 lb");
////    }
////
////    return checkList;
////}
////
////Vector3D SpanCable::Get_AttachmentSpacing() const
////{
////    return m_AttachmentSpacing;
////}
////
////
////double SpanCable::Get_CableLength() const
////{
////    if (bUpdateRequired_CatenaryFit == true)
////    {
////        Update_CatenaryFit();
////    }
////
////    return m_Catenary.Get_CurveLength();
////}
////
/////**
//// * The cable slack is the difference between the cable length and straight line distance between
//// * attachment points
//// */
////double SpanCable::Get_CableSlack() const
////{
////    if (bUpdateRequired_CatenaryFit == true)
////    {
////        Update_CatenaryFit();
////    }
////
////    return Get_CableLength() - m_AttachmentSpacing.Magnitude();
////}
////
////double SpanCable::Get_CatenaryCurveConstant() const
////{
////    if (bUpdateRequired_CatenaryFit == true)
////    {
////        Update_CatenaryFit();
////    }
////
////    return m_Catenary.Get_CurveConstant();
////}
////
////double SpanCable::Get_CatenaryHorizontalTension() const
////{
////    return m_CatenaryHorizontalTension;
////}
////
/////**
//// *
//// */
////// GET COORDINATE
////// position of spancable, using spancable coordinate system
////Point3D SpanCable::Get_Coordinate(const double& PositionAlongCable_Fraction) const
////{
////    if (bUpdateRequired_CatenaryFit == true)
////    {
////        Update_CatenaryFit();
////    }
////
////    // solve for length along the cable
////    const double PositionAlongCable = PositionAlongCable_Fraction * Get_CableLength();
////
////    // get coordinates and endpoint spacing from catenary
////    Vector2D CatenaryEndPointSpacing = m_Catenary.Get_EndPointSpacing();
////    const Point2D CatenaryEndPointBOL = m_Catenary.Get_Coordinate(0);
////    const Point2D CatenaryCoordinate = m_Catenary.Get_Coordinate(PositionAlongCable_Fraction);
////
////    // solve for spancable coordinate
////    Point3D Coordinate;
////    Coordinate.x = PositionAlongCable * (m_AttachmentSpacing.Magnitude() / CatenaryEndPointSpacing.Magnitude());
////    Coordinate.y = (CatenaryCoordinate.y - CatenaryEndPointBOL.y) * sin(Get_SwingAngle() * degrees_to_radians);
////    Coordinate.z = (CatenaryCoordinate.y - CatenaryEndPointBOL.y) * cos(Get_SwingAngle() * degrees_to_radians);
////    return Coordinate;
////}
////
////// GET SAG
////// vector magnitude spacing between straight line connecting attachment points and parallel tangency line on catenary
////double SpanCable::Get_Sag() const
////{
////    if (bUpdateRequired_CatenaryFit == true)
////    {
////        Update_CatenaryFit();
////    }
////
////    // insert code here
////
////    return 0;
////}
////
////Vector3D SpanCable::Get_TangentVector(const double& PositionAlongCable_Fraction,
////                                      const std::string& Direction_BOLorAOL) const
////{
////    if (bUpdateRequired_CatenaryFit == true)
////    {
////        Update_CatenaryFit();
////    }
////
////    Vector3D tangentVector;
////
////    // do stuff
////
////    return tangentVector;
////}
////
////Vector3D SpanCable::Get_Tension(const double& PositionAlongCable_Fraction,
////                                const std::string& Direction_BOLorAOL) const
////{
////    if (bUpdateRequired_CatenaryFit == true)
////    {
////        Update_CatenaryFit();
////    }
////
////    Vector3D Tension;
////    // solve for tension here
////
////    return Tension;
////}
////
////std::list<std::string> SpanCable::SaveData() const
////{
////    std::list<std::string> saveList;
////
////    // formatted so "=" aligns for all columns with an 8 space tab character (most text files)
////    saveList.push_back("[SPANCABLE]");
////    saveList.push_back("attachment spacing - horizontal = " + std::to_string(m_AttachmentSpacing.x));
////    saveList.push_back("attachment spacing - vertical = " + std::to_string(m_AttachmentSpacing.z));
////    saveList.push_back("catenary horizontal tension   = " + std::to_string(m_CatenaryHorizontalTension));
////    saveList.splice(saveList.end(), m_Cable.SaveData());
////    saveList.splice(saveList.end(), m_CableLoadCase.SaveData());
////
////    return saveList;
////}
////
/////**
//// * This method defines the cable attachment spacing and marks the catenary to be updated.
//// */
////void SpanCable::Set_AttachmentSpacing(const double& horizontalSpacing, const double& verticalSpacing)
////{
////    m_AttachmentSpacing.x = horizontalSpacing;
////    m_AttachmentSpacing.y= 0;
////    m_AttachmentSpacing.z = verticalSpacing;
////
////    bUpdateRequired_CatenaryFit = true;
////}
////
/////**
//// * This method defines the catenary horizontal tension and marks the catenary to be updated.
//// */
////void SpanCable::Set_CatenaryHorizontalTension(const double& horizontalTension)
////{
////    m_CatenaryHorizontalTension = horizontalTension;
////
////    bUpdateRequired_CatenaryFit = true;
////}
////
////
////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// PROTECTED MEMBER FUNCTIONS ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////
////// CATENARY FIT
////// use unit loads and attachment spacing to fit a catenary curve, which models spancable position
////void SpanCable::Update_CatenaryFit() const
////{
////    // check all spancable data before fitting catenary
////    std::list<std::string> errorList = CheckData(false);
////    if (0 < errorList.size())
////    {
////        return;
////    }
////
////    // fit a catenary to spancable attachments while under the unit loads solved for above
////
////    const double B = m_AttachmentSpacing.z;
////    const double C = m_AttachmentSpacing.Magnitude();
////    const double V = std::abs(m_UnitLoads.z);
////    const double w = m_UnitLoads.Magnitude();
////    const double H = m_CatenaryHorizontalTension;
////
////    // solve for adjusted catenary endpoint spacing due to shifted plane relative to vertical
////    const double b = B * (V / w));
////    const double a = sqrt( pow(C,2) - pow(B * (V / w), 2) );
////
////    // set catenary parameters
////    m_Catenary.Set_EndpointSpacing(a, b);
////    m_Catenary.Set_HorizontalTension(H);
////    m_Catenary.Set_UnitWeight(w);
////
////
////    // reset update flag
////    bUpdateRequired_CatenaryFit = false;
////}
