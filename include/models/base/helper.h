// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef MODELS_BASE_HELPER_H_
#define MODELS_BASE_HELPER_H_

#include <list>
#include <string>

namespace helper {

/// \brief Compares the characters of two strings.
/// \param[in] str1
///   The first string.
/// \param[in] str2
///   The second string.
/// \return This function returns -1 if the first string is less than the
///   second, 0 if they are equal, and 1 if the first is greater than the
///   second.
/// This function is NOT case sensitive. 'A' and 'a' are treated as identical
/// characters for comparison.
int CompareStrings(const std::string& str1, const std::string& str2);

/// \brief Converts a double precision number to a formatted string.
/// \param[in] value
///   The value to be converted.
/// \param[in] precision_decimal
///   The precision to round the number.
/// \return A string representation of the rounded number.
std::string DoubleToFormattedString(const double& value,
                                    const int& precision_decimal);

/// \brief Determines if the string is numeric.
/// \param[in] str
///   The string.
/// \return If the string is numeric.
bool IsNumeric(const std::string& str);

/// \brief Linearly interpolates/extrapolates a y value.
/// \param[in] x1
///   The x value for the first line point.
/// \param[in] y1
///   The y value for the first line point.
/// \param[in] x2
///   The x value for the second line point.
/// \param[in] y2
///   The y value for the second line point.
/// \param[in] x
///   The x value.
/// \return The y value.
double LinearY(const double& x1, const double& y1,
               const double& x2, const double& y2, const double& x);

/// \brief Parses a string.
/// \param[in] str
///   The string.
/// \param[in] delimiter
///   The delimiter character.
/// \return The parsed strings.
std::list<std::string> Parse(const std::string& str,
                             const char& delimiter);

/// \brief Rounds the number to the specified precision.
/// \param[in] number
///   The number to be rounded.
/// \param[in] precision
///   The number of decimal places to round to.
/// \return The rounded number.
double Round(const double& number, const int& precision);

}  // namespace helper

#endif  // MODELS_BASE_HELPER_H_
