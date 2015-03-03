// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "include/base/convert_units.h"

#include <cmath>

namespace supportfunctions {

double Round(const double& number, const int& precision) {
  return round(number * pow(10, precision)) / pow(10, precision);
}

} // namespace supportfunctions
