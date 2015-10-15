// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/transmissionline/weather_load_case.h"

#include <cmath>

WeatherLoadCase::WeatherLoadCase() {
  density_ice = -999999;
  description = "";
  pressure_wind = -999999;
  temperature_cable = -999999;
  thickness_ice = -999999;
}

WeatherLoadCase::~WeatherLoadCase() {
}

bool WeatherLoadCase::Validate(const bool& is_included_warnings,
                               std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "WEATHER LOAD CASE";

  // validates density-ice
  if (density_ice < 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid ice density";
      messages->push_back(message);
    }
  }

  // validates pressure-wind
  if (pressure_wind < 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid wind pressure";
      messages->push_back(message);
    }
  }

  // validates temperature-cable
  if (temperature_cable < -50) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid cable temperature";
      messages->push_back(message);
    }
  }

  // validates thickness-ice
  if (thickness_ice < 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid ice thickness";
      messages->push_back(message);
    }
  }

  return is_valid;
}
