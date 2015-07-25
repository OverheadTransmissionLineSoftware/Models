// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "base/units.h"

#include <cmath>

namespace units {

const double kFeetToInches = 12;
const double kInchesToFeet = 0.083333333333333;
const double kRadiansToDegrees = (180 / kPi);
const double kDegreesToRadians = (kPi / 180);

double Convert(const double& value,
               const ConversionType& type,
               const int& exponent,
               const bool& is_numerator) {

  // gets factor
  double factor = -999999;
  if (type == ConversionType::kDegreesToRadians) {
    factor = pow(kDegreesToRadians, exponent);
  } else if (type == ConversionType::kFeetToInches) {
    factor = pow(kFeetToInches, exponent);
  } else if (type == ConversionType::kInchesToFeet) {
    factor = pow(kInchesToFeet, exponent);
  } else if (type == ConversionType::kRadiansToDegrees) {
    factor = pow(kRadiansToDegrees, exponent);
  }

  // inverts if unit value is not in the numerator
  if (is_numerator == false) {
    factor = 1 / factor;
  }

  // multiplies factor by value for conversion
  return value * factor;
}

} // namespace units