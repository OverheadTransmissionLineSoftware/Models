// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/thermalrating/thermal_rating_weather.h"

ThermalRatingWeather::ThermalRatingWeather() {
  angle_wind = -999999;
  elevation = -999999;
  radiation_solar = -999999;
  speed_wind = -999999;
  temperature_air = -999999;
}

ThermalRatingWeather::~ThermalRatingWeather() {
}

bool ThermalRatingWeather::Validate(
    const bool& /*is_included_warnings*/,
    std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "THERMAL RATING WEATHER";

  // validates angle-wind
  if ((angle_wind < 0) || (90 < angle_wind)) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid angle between wind and cable";
      messages->push_back(message);
    }
  }

  // validates elevation
  if (elevation < 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid elevation";
      messages->push_back(message);
    }
  }

  // validates radiation-solar
  if (radiation_solar < 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid solar radiation";
      messages->push_back(message);
    }
  }

  // validates speed-wind
  if (speed_wind < 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid wind speed";
      messages->push_back(message);
    }
  }

  // validates temperature-air
  if ((temperature_air < -100) || (100 < temperature_air)) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid air temperature";
      messages->push_back(message);
    }
  }

  return is_valid;
}
