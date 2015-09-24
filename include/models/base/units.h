// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef OTLS_MODELS_BASE_UNITS_H_
#define OTLS_MODELS_BASE_UNITS_H_

namespace units {

  enum class UnitSystem {
    Imperial,
    Metric
  };

  enum class ConversionType {
    kDegreesToRadians,
    kFeetToInches,
    kInchesToFeet,
    kRadiansToDegrees,
  };

  const double kPi = 3.14159265358979;

  /// \brief Converts the value.
  /// \param[in] value
  ///   The starting value to convert.
  /// \param[in] type_conversion
  ///   The type of conversion, which description starts and ends with the
  ///   units.
  /// \param[in] exponent
  ///   The exponent of the unit conversion. This applies to the starting and
  ///   end units.
  /// \param[in] is_numerator
  ///   A boolean indicating if the unit being converted from is in the
  ///   numerator.
  /// \return The converted value.
  double Convert(const double& value,
                 const ConversionType& type,
                 const int& exponent = 1,
                 const bool& is_numerator = true);

} // namespace units

#endif // OTLS_MODELS_BASE_UNITS_H_
