// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/base/helper.h"

#include <cmath>
#include <sstream>

namespace helper {

std::string DoubleToFormattedString(const double& value,
                                    const int& precision_decimal) {
  double value_rounded = helper::Round(value, precision_decimal);

  // creates a string stream to convert double to a string and returns
  // the string stream removes any trailing characters from the rounded number
  std::ostringstream stream;
  stream << value_rounded;
  return stream.str();
}

double Round(const double& number, const int& precision) {
  return std::round(number * pow(10, precision)) / pow(10, precision);
}

}  // namespace support
