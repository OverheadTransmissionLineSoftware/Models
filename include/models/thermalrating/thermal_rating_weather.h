// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef OTLS_MODELS_THERMALRATING_THERMALRATINGWEATHER_H_
#define OTLS_MODELS_THERMALRATING_THERMALRATINGWEATHER_H_

#include <list>

#include "models/base/error_message.h"

/// \par OVERVIEW
///
/// This struct contains weather information for a thermal rating analysis.
struct ThermalRatingWeather {
 public:
  /// \brief Constructor.
  ThermalRatingWeather();

  /// \brief Destructor.
  ~ThermalRatingWeather();

  /// \brief Validates member variables.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings = true,
                std::list<ErrorMessage>* messages = nullptr) const;

  /// \var angle_wind
  ///   The angle between the wind and cable axes (degrees). This can be between
  ///   0-90.
  double angle_wind;

  /// \var elevation
  ///   The elevation from sea level.
  double elevation;

  /// \var radiation_solar
  ///   The amount of solar radiation.
  double radiation_solar;

  /// \var speed_wind
  ///   The wind speed.
  double speed_wind;

  /// \var temperature_air
  ///   The temperature of the ambient air.
  double temperature_air;
};

#endif  // OTLS_MODELS_THERMALRATING_THERMALRATINGWEATHER_H_
