// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef MODELS_TRANSMISSIONLINE_WEATHER_LOAD_CASE_H_
#define MODELS_TRANSMISSIONLINE_WEATHER_LOAD_CASE_H_

#include <list>
#include <string>

#include "models/base/error_message.h"

/// \par OVERVIEW
///
/// This struct is a container for a weather load case.
struct WeatherLoadCase {
 public:
  /// \brief Default constructor.
  WeatherLoadCase();

  /// \brief Destructor.
  ~WeatherLoadCase();

  /// \brief Validates member variables.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings = true,
                std::list<ErrorMessage>* messages = nullptr) const;

  /// \var density_ice
  ///   The density of the accumulated ice.
  double density_ice;

  /// \var description
  ///   A description of the weather load case, typically done as:
  ///   (thickness_ice-pressure_wind-temperature_cable)
  std::string description;

  /// \var pressure_wind
  ///   The pressure of the wind.
  double pressure_wind;

  /// \var temperature_cable
  ///   The temperature of the cable.
  double temperature_cable;

  /// \var thickness_ice
  ///   The thickness of the accumulated ice.
  double thickness_ice;
};

#endif  // MODELS_TRANSMISSIONLINE_WEATHER_LOAD_CASE_H_
