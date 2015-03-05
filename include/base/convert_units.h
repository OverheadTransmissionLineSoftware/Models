// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef TLSLIBRARIES_BASE_CONVERTUNITS_H_
#define TLSLIBRARIES_BASE_CONVERTUNITS_H_

namespace convertunits {

const double kPi = 3.14159265358979;
const double kRadiansToDegrees = (180 / kPi);
const double kDegreesToRadians = (kPi / 180);

} // namespace convertunits

namespace supportfunctions {

/// \brief Rounds the number to the specified precision.
/// \param[in] number
///   The number to be rounded.
/// \param[in] precision
///   The number of decimal places to round to.
/// \return The rounded number.
double Round(const double& number, const int& precision);

} // namespace supportfunctions

#endif // TLSLIBRARIES_BASE_CONVERTUNITS_H_
