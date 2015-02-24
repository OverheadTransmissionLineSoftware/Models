// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef TRANSMISSIONLINE_WEATHERLOADCASE_H_
#define TRANSMISSIONLINE_WEATHERLOADCASE_H_

#include <list>
#include <string>

/// \par OVERVIEW
///
/// This class is a container for weather load cases.
struct WeatherLoadCase {
 public:
  /// \brief Default constructor.
  WeatherLoadCase();

  /// \brief Destructor.
  ~WeatherLoadCase();

  /// \brief Validates class data.
  /// \param is_included_warnings A flag that tightens the acceptable value
  ///        range.
  /// \param messages_error A list of detailed error messages. If this is
  ///        provided, any validation errors will be appended to the list.
  /// \return A boolean value indicating status of class data.
  bool Validate(bool is_included_warnings = true,
                std::list<std::string>* messages_error = nullptr) const;

  // member variables
  double      density_ice;
  std::string description;
  double      pressure_wind;
  double      temperature_cable;
  double      thickness_ice;
};

#endif // TRANSMISSIONLINE_WEATHERLOADCASE_H_
