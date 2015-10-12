// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef OTLS_MODELS_BASE_UNITS_H_
#define OTLS_MODELS_BASE_UNITS_H_

namespace units {

/// \par OVERVIEW
///
/// This enum contains types of unit conversions.
enum class ConversionType {
  kDegreesToRadians,
  kFeetToInches,
  kInchesToFeet,
  kRadiansToDegrees,
};

/// \par OVERVIEW
///
/// Unit styles can either be consistent or different.  When consistent, all
/// values use one common unit for each measurement type. Ex: length/distance
/// is always represented in meters. When different, values can be represented
/// in the unit that is most measurable. Ex: length/distance can be measured
/// in meters or millimeters, based on the size of the object.
///
/// This is needed because calculation libraries require consistent units, but
/// user interface controls and generated XML files need to be in convenient,
/// understandable units.
///
/// The consistent style units are as follows:
///   - Length = [meters or feet]
///   - Load = [N or lbs]
///   - Temperature = [deg C or deg F]
///
/// The different style units vary for every struct, depending on how the user
/// typically prefers to see the information represented.
enum class UnitStyle {
  kConsistent,
  kDifferent
};

/// \par OVERVIEW
///
/// This enum contains unit systems.
enum class UnitSystem {
  kImperial,
  kMetric
};

const double kPi = 3.14159265358979;

/// \brief Converts the value.
/// \param[in] value
///   The starting value to convert.
/// \param[in] type_conversion
///   The type of conversion, which description starts and ends with the
///   units.
/// \param[in] exponent
///   The exponent of the unit conversion. This applies to the starting and
///   end units.
/// \param[in] is_numerator
///   A boolean indicating if the unit being converted from is in the
///   numerator.
/// \return The converted value.
double Convert(const double& value,
               const ConversionType& type,
               const int& exponent = 1,
               const bool& is_numerator = true);

} // namespace units

#endif // OTLS_MODELS_BASE_UNITS_H_
