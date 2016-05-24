// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef OTLS_MODELS_BASE_UNITS_H_
#define OTLS_MODELS_BASE_UNITS_H_

// This namespace separates unit types (angles, force, length, etc) to make the
// interface easier to use and more extensible. Where unit conversions require
// addition/subtraction, the order of operations is important and easier to
// enforce when separated into separate unit types.
namespace units {

/// \par OVERVIEW
///
/// This enum contains types of angle unit conversions.
enum class AngleConversionType {
  kDegreesToRadians,
  kRadiansToDegrees
};

/// \par OVERVIEW
///
/// This enum contains types of force unit conversions.
enum class ForceConversionType {
  kNewtonsToPounds,
  kPoundsToNewtons
};

/// \par OVERVIEW
///
/// This enum contains types of length unit conversions.
enum class LengthConversionType {
  kFeetToInches,
  kFeetToMeters,
  kInchesToFeet,
  kMetersToFeet,
};

/// \par OVERVIEW
///
/// This enum contains types of temperature unit conversions.
enum class StressConversionType {
  kPascalToPsf,
  kPsfToPascal,
  kPsfToPsi,
  kPsiToPsf
};

/// \par OVERVIEW
///
/// This enum contains types of temperature unit conversions.
enum class TemperatureConversionType {
  kCelsiusToFahrenheit,
  kFahrenheitToCelsius
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
///   - Force = [N or lbs]
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

/// \brief Converts the angle value.
/// \param[in] value
///   The starting value to convert.
/// \param[in] type
///   The type of conversion.
/// \param[in] exponent
///   The exponent of the unit being converted.
/// \param[in] is_numerator
///   A boolean indicating if the unit being converted from is in the
///   numerator.
/// \return The converted value.
double ConvertAngle(const double& value,
                    const AngleConversionType& type,
                    const int& exponent = 1,
                    const bool& is_numerator = true);

/// \brief Converts the force value.
/// \param[in] value
///   The starting value to convert.
/// \param[in] type
///   The type of conversion.
/// \param[in] exponent
///   The exponent of the unit being converted.
/// \param[in] is_numerator
///   A boolean indicating if the unit being converted from is in the
///   numerator.
/// \return The converted value.
double ConvertForce(const double& value,
                    const ForceConversionType& type,
                    const int& exponent = 1,
                    const bool& is_numerator = true);

/// \brief Converts the length value.
/// \param[in] value
///   The starting value to convert.
/// \param[in] type
///   The type of conversion.
/// \param[in] exponent
///   The exponent of the unit being converted.
/// \param[in] is_numerator
///   A boolean indicating if the unit being converted from is in the
///   numerator.
/// \return The converted value.
double ConvertLength(const double& value,
                     const LengthConversionType& type,
                     const int& exponent = 1,
                     const bool& is_numerator = true);

/// \brief Converts the stress value.
/// \param[in] value
///   The starting value to convert.
/// \param[in] type
///   The type of conversion.
/// \param[in] exponent
///   The exponent of the unit being converted.
/// \param[in] is_numerator
///   A boolean indicating if the unit being converted from is in the
///   numerator.
/// \return The converted value.
double ConvertStress(const double& value,
                     const StressConversionType& type,
                     const int& exponent = 1,
                     const bool& is_numerator = true);

/// \brief Converts the temperature value.
/// \param[in] value
///   The starting value to convert.
/// \param[in] type
///   The type of conversion.
/// \param[in] exponent
///   The exponent of the unit being converted.
/// \param[in] is_numerator
///   A boolean indicating if the unit being converted from is in the
///   numerator.
/// \return The converted value.
double ConvertTemperature(const double& value,
                          const TemperatureConversionType& type,
                          const int& exponent = 1,
                          const bool& is_numerator = true);
} // namespace units

#endif // OTLS_MODELS_BASE_UNITS_H_
