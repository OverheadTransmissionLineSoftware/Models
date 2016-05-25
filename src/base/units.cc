// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/base/units.h"

#include <cmath>

namespace units {

// angle conversion factors
const double kAngleDegreesToRadians = (kPi / 180);
const double kAngleRadiansToDegrees = 180 / kPi;

// force conversion factors
const double kForceNewtonsToPounds = 1 / 4.4482216152605;
const double kForcePoundsToNewtons = 4.4482216152605;

// length conversion factors
const double kLengthCentimetersToMeters = 0.01;
const double kLengthFeetToInches = 12;
const double kLengthFeetToMeters = 0.3048;
const double kLengthInchesToFeet = 0.083333333333333;
const double kLengthMetersToCentimeters = 100;
const double kLengthMetersToFeet = 1 / 0.3048;
const double kLengthMetersToMillimeters = 1000;
const double kLengthMillimetersToMeters = 0.001;

// pressure conversion factors
const double kStressMegaPascalToPascal = 0.000001;
const double kStressPascalToMegaPascal = 1000000;
const double kStressPascalToPsf = 1 / 47.88026;
const double kStressPsfToPascal = 47.88026;
const double kStressPsfToPsi = 0.00694444444;
const double kStressPsiToPsf = 144;

// temperature conversion factors
const double kTemperatureCelsiusToFahrenheit = 1.8;
const double kTemperatureFahrenheitToCelsius = 1 / 1.8;

/// \brief This is a generic function that converts a value by multiplication.
/// \param[in] value
///   The starting value to convert.
/// \param[in] factor
///   The multiplication factor to scale the input value.
/// \param[in] exponent
///   The exponent of the unit being converted.
/// \param[in] is_numerator
///   A boolean indicating if the unit being converted is in the
///   numerator.
/// \return The converted value.
/// This function adjusts the base multiplication factor to convert the units,
/// and then returns the converted value.
double Convert(const double& value,
               const double& factor,
               const double& exponent,
               const double& is_numerator) {
  // adjusts the factor based on the exponent
  double factor_adj = pow(factor, exponent);

  // inverts factor if unit value is not in the numerator
  if (is_numerator == false) {
    factor_adj = 1 / factor_adj;
  }

  // applies adjusted factor for conversion
  return value * factor_adj;
}

double ConvertAngle(const double& value,
                    const AngleConversionType& type,
                    const int& exponent,
                    const bool& is_numerator) {
  if (type == AngleConversionType::kDegreesToRadians) {
    return Convert(value, kAngleDegreesToRadians, exponent, is_numerator);
  } else if (type == AngleConversionType::kRadiansToDegrees) {
    return Convert(value, kAngleRadiansToDegrees, exponent, is_numerator);
  } else {
    return -999999;
  }
}

double ConvertForce(const double& value,
                    const ForceConversionType& type,
                    const int& exponent,
                    const bool& is_numerator) {
  if (type == ForceConversionType::kNewtonsToPounds) {
    return Convert(value, kForceNewtonsToPounds, exponent, is_numerator);
  } else if (type == ForceConversionType::kPoundsToNewtons) {
    return Convert(value, kForcePoundsToNewtons, exponent, is_numerator);
  } else {
    return -999999;
  }
}

double ConvertLength(const double& value,
                     const LengthConversionType& type,
                     const int& exponent,
                     const bool& is_numerator) {
  if (type == LengthConversionType::kCentimetersToMeters) {
    return Convert(value, kLengthCentimetersToMeters, exponent, is_numerator);
  } else if (type == LengthConversionType::kFeetToInches) {
    return Convert(value, kLengthFeetToInches, exponent, is_numerator);
  } else if (type == LengthConversionType::kFeetToMeters) {
    return Convert(value, kLengthFeetToMeters, exponent, is_numerator);
  } else if (type == LengthConversionType::kInchesToFeet) {
    return Convert(value, kLengthInchesToFeet, exponent, is_numerator);
  } else if (type == LengthConversionType::kMetersToCentimeters) {
    return Convert(value, kLengthMetersToCentimeters, exponent, is_numerator);
  } else if (type == LengthConversionType::kMetersToFeet) {
    return Convert(value, kLengthMetersToFeet, exponent, is_numerator);
  } else if (type == LengthConversionType::kMetersToMillimeters) {
    return Convert(value, kLengthMetersToMillimeters, exponent, is_numerator);
  } else if (type == LengthConversionType::kMillimetersToMeters) {
    return Convert(value, kLengthMillimetersToMeters, exponent, is_numerator);
  } else {
    return -999999;
  }
}

double ConvertStress(const double& value,
                     const StressConversionType& type,
                     const int& exponent,
                     const bool& is_numerator) {
  if (type == StressConversionType::kMegaPascalToPascal) {
    return Convert(value, kStressMegaPascalToPascal, exponent, is_numerator);
  } else if (type == StressConversionType::kPascalToMegaPascal) {
    return Convert(value, kStressPascalToMegaPascal, exponent, is_numerator);
  } else if (type == StressConversionType::kPascalToPsf) {
    return Convert(value, kStressPascalToPsf, exponent, is_numerator);
  } else if (type == StressConversionType::kPsfToPascal) {
    return Convert(value, kStressPsfToPascal, exponent, is_numerator);
  } else if (type == StressConversionType::kPsfToPsi) {
    return Convert(value, kStressPsfToPsi, exponent, is_numerator);
  } else if (type == StressConversionType::kPsiToPsf) {
    return Convert(value, kStressPsiToPsf, exponent, is_numerator);
  } else {
    return -999999;
  }
}

double ConvertTemperature(const double& value,
                          const TemperatureConversionType& type,
                          const int& exponent,
                          const bool& is_numerator) {
  if (type == TemperatureConversionType::kCelsiusToFahrenheit) {
    double value_adj = Convert(value, kTemperatureCelsiusToFahrenheit, exponent,
                               is_numerator);
    return value_adj + 32;
  } else if (type == TemperatureConversionType::kFahrenheitToCelsius) {
    double value_adj = value - 32;
    return Convert(value_adj, kTemperatureFahrenheitToCelsius, exponent,
                   is_numerator);
  } else {
    return -999999;
  }
}

} // namespace units
