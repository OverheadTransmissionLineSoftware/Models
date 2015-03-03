// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef BASE_CONVERT_UNITS_H_
#define BASE_CONVERT_UNITS_H_

namespace convertunits {

const double kPi = 3.14159265358979;
const double kRadiansToDegrees = (180 / kPi);
const double kDegreesToRadians = (kPi / 180);

} // namespace convertunits

namespace supportfunctions {

double Round(const double& number, const int& precision);

} // namespace supportfunctions

#endif // BASE_CONVERT_UNITS_H_
