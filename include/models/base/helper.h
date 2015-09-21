// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef TLSLIBRARIES_BASE_HELPER_H_
#define TLSLIBRARIES_BASE_HELPER_H_

namespace helper {

  /// \brief Rounds the number to the specified precision.
  /// \param[in] number
  ///   The number to be rounded.
  /// \param[in] precision
  ///   The number of decimal places to round to.
  /// \return The rounded number.
  double Round(const double& number, const int& precision);

} // namespace support

#endif // TLSLIBRARIES_BASE_HELPER_H_