// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef OTLS_MODELS_BASE_HELPER_H_
#define OTLS_MODELS_BASE_HELPER_H_

#include <string>

namespace helper {

  /// \brief Converts a double precision number to a formatted string.
  /// \param[in] value
  ///   The value to be converted.
  /// \param[in] precision_decimal
  ///   The precision to round the number.
  /// \return A string representation of the rounded number.
  std::string DoubleToFormattedString(const double& value,
                                      const int& precision_decimal);

  /// \brief Rounds the number to the specified precision.
  /// \param[in] number
  ///   The number to be rounded.
  /// \param[in] precision
  ///   The number of decimal places to round to.
  /// \return The rounded number.
  double Round(const double& number, const int& precision);

} // namespace support

#endif // OTLS_MODELS_BASE_HELPER_H_
