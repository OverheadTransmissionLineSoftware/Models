// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/base/helper.h"

#include <cmath>
#include <sstream>

namespace helper {

int CompareStrings(const std::string& str1, const std::string& str2) {
  // compares the numerical value of each character in the string
  // if a difference is found, it will return immediately
  unsigned int i = 0;
  while (i < str1.length() && i < str2.length()) {
    if (tolower(str1[i]) < tolower(str2[i])) {
      return -1;
    } else if (tolower(str1[i]) > tolower(str2[i])) {
      return 1;
    }
    ++i;
  }

  // an end of a string is reached
  return str1.length() > str2.length();
}

std::string DoubleToFormattedString(const double& value,
                                    const int& precision_decimal) {
  double value_rounded = helper::Round(value, precision_decimal);

  // creates a string stream to convert double to a string and returns
  // the string stream removes any trailing characters from the rounded number
  std::ostringstream stream;
  stream.precision(precision_decimal);
  stream << std::fixed << value_rounded;
  return stream.str();
}

bool IsNumeric(const std::string& str) {
  std::istringstream stream(str);
  double d;
  char c;

  // attempts to extract a double from the string
  // expects fail when extracting any remaining characters after double
  return stream >> d && !(stream >> c);
}

double LinearY(const double& x1, const double& y1,
               const double& x2, const double& y2, const double& x) {
  return y1 + (x - x1) * ((y2 - y1) / (x2 - x1));
}

std::list<std::string> Parse(const std::string& str,
                             const char& delimiter) {
  std::list<std::string> strs;

  // creates a stringstream to convert each substring
  std::stringstream ss;
  ss.str(str);
  std::string item;
  while (std::getline(ss, item, delimiter)) {
    strs.push_back(item);
  }

  return strs;
}

double Round(const double& number, const int& precision) {
  return std::round(number * std::pow(10, precision)) / std::pow(10, precision);
}

}  // namespace helper
