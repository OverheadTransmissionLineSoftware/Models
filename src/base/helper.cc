// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "base/helper.h"

#include <cmath>

namespace helper {

double Round(const double& number, const int& precision) {
  return std::round(number * pow(10, precision)) / pow(10, precision);
}

}  // namespace support
