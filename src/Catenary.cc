// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "Catenary.h"

#include <cmath>

#include "SupportLibrary.h"

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

/**
 * If the lowpoint is within the curve section being used, the length from the
 * origin to the left endpoint and the length from the origin to the right
 * endpoint are summed. If the origin is beyond an endpoint, the length to the
 * closest endpoint is treated as negative.
 * @see Catenary2D::LengthFromOrigin
 */
double Catenary2D::Length() const
{
  double length = -999999;

  if (IsUpdated() == false) {
    if (Update() == false) {
      return length;
    }
  }

  // length from origin to left end
  double length_origin_to_left = -999999;
  if (point_end_left_.x < 0) {
    length_origin_to_left = LengthFromOrigin(point_end_left_);
  } else if (0 <= point_end_left_.x) {
    length_origin_to_left = -LengthFromOrigin(point_end_left_);
  }

  // length from origin to right end
  double length_origin_to_right = -999999;
  if (point_end_right_.x < 0) {
    length_origin_to_right = -LengthFromOrigin(point_end_right_);
  } else if (0 <= point_end_right_.x) {
    length_origin_to_right = LengthFromOrigin(point_end_right_);
  }

  length = length_origin_to_left + length_origin_to_right;
  return length;
}

double Catenary2D::LengthSlack() const
{
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return Length() - spacing_endpoints().Magnitude();
}

double Catenary2D::PositionFractionOrigin() const
{
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  // get position along curve of low point, where tangent angle is zero
  return PositionFraction(0);
}

double Catenary2D::PositionFractionSagPoint() const
{
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  double tangent_angle = spacing_endpoints_.Angle(true);

  // get position fraction of chord tangent line
  return PositionFraction(tangent_angle);
}

double Catenary2D::Sag() const
{
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  // get sag position
  const double position_fraction_sagpoint = PositionFractionSagPoint();

  // get catenary coordinate at sag position
  const Point2D coordinate_catenary = Coordinate(position_fraction_sagpoint);

  // get a chord coordinate at sag position
  const Point2D coordinate_chord = CoordinateChord(position_fraction_sagpoint);

  // get height difference between chord and catenary
  return coordinate_chord.y - coordinate_catenary.y;
}

double Catenary2D::TangentAngle(const double& position_fraction,
                                const AxisDirectionType& direction) const
{
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  // get coordinate at position
  const Point2D coordinate = Coordinate(position_fraction);

  // calculate slope at position
  const double slope = sinh(coordinate.x / (tension_horizontal_/weight_unit_));

  // convert to degrees
  double tangent_angle = atan(slope) * (kRadiansToDegrees);

  // adjust if direction is negative
  if (direction == AxisDirectionType::NEGATIVE) {
    tangent_angle = tangent_angle * -1;
  }

  return tangent_angle;
}

/**
 * The slope at the position is calculated, and then adjusted to a unit vector
 * based on the direction the vector is pointing.
 * Slope equation is:
 * \f[ Slope = sinh \left( \frac{x}{\frac{H}{w}} \right) \f]
 */
Vector2D Catenary2D::TangentVector(const double& position_fraction,
                                   const AxisDirectionType& direction) const
{
  Vector2D tangent_vector;

  if (IsUpdated() == false) {
    if (Update() == false) {
      return tangent_vector;
    }
  }

  // get tangent angle
  const double angle_tangent = TangentAngle(position_fraction, direction);

  // resolve to a unit vector
  if (direction == AxisDirectionType::NEGATIVE) {
    tangent_vector.set_x( -(1 * cos(angle_tangent * kRadiansToDegrees)) );
    tangent_vector.set_y( sin(angle_tangent * kRadiansToDegrees) );
  } else if (direction == AxisDirectionType::POSITIVE) {
    tangent_vector.set_x( cos(angle_tangent * kRadiansToDegrees) );
    tangent_vector.set_y( sin(angle_tangent * kRadiansToDegrees) );
  }

  return tangent_vector;
}

double Catenary2D::Tension(const double& position_fraction) const
{
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  // get coordinate
  Point2D coordinate = Coordinate(position_fraction);

  return tension_horizontal_
         * cosh( coordinate.x / (tension_horizontal_/weight_unit_));
}

/**
 * A tangent unit vector is calculated and then multiplied by the tension magnitude.
 * The tension magnitude equation is:
 * \f[ Tension = H cosh \left( \frac{x}{\frac{H}{w}} \right) \f]
 * @see Catenary::Get_TangentVector
 */
Vector2D Catenary2D::Tension(const double& position_fraction,
                             const AxisDirectionType& direction) const
{
  Vector2D vector_tangent;

  if (IsUpdated() == false) {
    if (Update() == false) {
      return vector_tangent;
    }
  }

  // get a unit vector and scale according to tension magnitude
  vector_tangent = TangentVector(position_fraction, direction);
  const double magnitude = Tension(position_fraction);
  vector_tangent.Scale(magnitude);

  return vector_tangent;
}

double Catenary2D::TensionAverage(const int& num_points) const
{
  double tension_average = -999999;

  if (IsUpdated() == false) {
    if (Update() == false) {
      return tension_average;
    }
  }

  if (num_points == 0) {

    // calculate using Ehrenburg approximation
    const double h = tension_horizontal_;
    const double w = weight_unit_;
    const double l = Length();

    const double term_1 = (pow(h, 2) / (2 * w * l));
    const double term_2 = sinh(point_end_right_.x / (h / w))
                          * cosh(point_end_right_.x / (h / w));
    const double term_3 = sinh(point_end_left_.x / (h / w))
                          * cosh(point_end_left_.x / (h / w));
    const double term_4 = (point_end_right_.x - point_end_left_.x)
                          / (h / w);

    tension_average = term_1 * (term_2 - term_3 + term_4);

  } else if (0 < num_points) {

    // claculate average tension using evenly spaced points
    double sum = 0;

    for (int iter = 0; iter <= num_points; iter++) {

      const double position_fraction = iter / num_points;
      const double tension_magnitude = Tension(position_fraction);
      sum = sum + tension_magnitude;
    }

    tension_average = sum / num_points;
  }

  return tension_average;
}

double Catenary2D::TensionMax() const
{
  double tension_max = -999999;

  if (IsUpdated() == false) {
    if (Update() == false) {
      return tension_max;
    }
  }

  // select highest end point and calculate tension
  if (spacing_endpoints_.y() <= 0) {
    return Tension(0);
  } else {
    return Tension(1);
  }
}

bool Catenary2D::Validate(bool is_included_warnings,
                          std::list<std::string>* messages_error) const
{
  bool is_valid = true;

  // validate tension-horizontal
  if (tension_horizontal_ <= 0
      || (100000 < tension_horizontal_ && is_included_warnings == true)) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CATENARY - Invalid horizontal tension");
    }
  }

  // validate weight-unit
  if (weight_unit_ <= 0
      || (15 < weight_unit_ && is_included_warnings == true)) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CATENARY - Invalid unit weight");
    }
  }

  // validate spacing-endpoints-horizontal
  if (spacing_endpoints_.x() <= 0
      || (5000 < spacing_endpoints_.x() && is_included_warnings == true)) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CATENARY - Invalid horizontal endpoint spacing");
    }
  }

  // validate spacing-endpoint-vertical
  if (10000 < abs(spacing_endpoints_.y())
      || 2000 <= abs(spacing_endpoints_.y()) ) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CATENARY - Invalid vertical endpoint spacing");
    }
  }

  return is_valid;
}

void Catenary2D::set_spacing_endpoints(const Vector2D& spacing_endpoints)
{
  spacing_endpoints_ = spacing_endpoints;

  is_updated_points_end_ = false;
}

void Catenary2D::set_tension_horizontal(const double& tension_horizontal)
{
  tension_horizontal_ = tension_horizontal;

  is_updated_points_end_ = false;
}

void Catenary2D::set_weight_unit(const double& weight_unit)
{
  weight_unit_ = weight_unit;

  is_updated_points_end_ = false;
}

Vector2D Catenary2D::spacing_endpoints() const
{
  return spacing_endpoints();
}

double Catenary2D::tension_horizontal() const
{
  return tension_horizontal_;
}

double Catenary2D::weight_unit() const
{
  return weight_unit_;
}

/**
 * The function is a derivation of the equation for curve length.
 * @see Catenary::Get_CurveLength_FromOrigin
 * \f[ x = \frac{H}{w} sinh^{-1} \left(\frac{L}{\frac{H}{w}}\right) \f]
 */
double Catenary2D::CoordinateX(const double& length_origin_to_position,
                               const AxisDirectionType& direction_origin_to_position)
const
{
  const double l = length_origin_to_position;
  const double h = tension_horizontal_;
  const double w = weight_unit_;

  double coordinate_x = -999999;

  // BOL from origin - negative x coordinate
  if (direction_origin_to_position == AxisDirectionType::NEGATIVE) {
    coordinate_x = -(h/w) * (asinh(l / (h/w)));
  }
  // AOL from origin - positive x coordinate
  else if (direction_origin_to_position == AxisDirectionType::POSITIVE) {
    coordinate_x = (h/w) * (asinh(l / (h/w)));
  }

  return coordinate_x;
}

/**
 * \f[ y = \frac{H}{w} \cosh \left(\frac{x}{\frac{H}{w}-1}\right) \f]
 */
double Catenary2D::CoordinateY(const double& length_origin_to_position,
                               const AxisDirectionType& direction_origin_to_position)
const
{
  const double x = CoordinateX(length_origin_to_position,
                               direction_origin_to_position);
  const double h = tension_horizontal_;
  const double w = weight_unit_;

  return (h/w) * (cosh(x / (h/w)) - 1);
}

/**
 * \f[ CurveLength = \left| \frac{H}{w} sinh^{-1} \left(\frac{x}{\frac{H}{w}}\right) \right| \f]
 */
double Catenary2D::LengthFromOrigin(const Point2D& coordinate) const
{
  const double x = coordinate.x;
  const double h = tension_horizontal_;
  const double w = weight_unit_;

  return std::abs((h/w) * sinh(x / (h/w)));
}

/**
 *
 */
bool Catenary2D::Update() const
{
  if (is_updated_points_end_ == false) {

    is_updated_points_end_ = UpdateEndPoints();
    if (is_updated_points_end_ == false) {
      return false;
    }
  }

  // if it reaches this point, update was successful
  return true;
}

/**
 * Hyperbolic identity equations are used to solve for the endpoint coordinates:
 * \f[ xBOL = \frac{A}{2} - \frac{H}{w} sinh^{-1} \left( \frac{\frac{B}{2}}{ \frac{H}{w} sinh \left( \frac{\frac{A}{2}}{\frac{H}{w}} \right)} \right) \f]
 * \f[ xAOL = \frac{A}{2} + \frac{H}{w} sinh^{-1} \left( \frac{\frac{B}{2}}{ \frac{H}{w} sinh \left( \frac{\frac{A}{2}}{\frac{H}{w}} \right)} \right) \f]
 */
bool Catenary2D::UpdateEndPoints() const
{
  const double h = tension_horizontal_;
  const double w = weight_unit_;
  const double a = spacing_endpoints_.x();
  const double b = spacing_endpoints_.y();
  const double z = (a/2) / (h/w);

  // solve for left endpoint coordinate
  point_end_left_.x = (h/w) * (asinh(b * z) / (a * sinh(z)) - z);
  point_end_left_.y = CoordinateY(LengthFromOrigin(point_end_left_),
                                  AxisDirectionType::NEGATIVE);

  // solve for right endpoint coordinate
  point_end_right_.x = (h/w) * (asinh(b * z) / (a * sinh(z)) + z);
  point_end_right_.y = CoordinateY(LengthFromOrigin(point_end_right_),
                                   AxisDirectionType::POSITIVE);

  return true;
}

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
