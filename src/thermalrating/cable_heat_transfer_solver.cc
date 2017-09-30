// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/thermalrating/cable_heat_transfer_solver.h"

#include <cmath>

CableHeatTransferSolver::CableHeatTransferSolver() {
  cable_ = nullptr;
  units_ = units::UnitSystem::kNull;
  weather_ = nullptr;
}

CableHeatTransferSolver::~CableHeatTransferSolver() {
}

double CableHeatTransferSolver::HeatConvection(
    const double& temperature_cable) const {
  // calculates the film temperature
  const double temperature_film =
      (temperature_cable + weather_->temperature_air) / 2;

  // calculates the thermal conductivity of air
  double conductivity_air = -999999;
  if (units_ == units::UnitSystem::kImperial) {
    conductivity_air = 7.388 * std::pow(10, -3)
        + 2.279 * std::pow(10, -5) * temperature_film
        - 1.343 * std::pow(10, -9) * std::pow(temperature_film, 2);
  } else if (units_ == units::UnitSystem::kMetric) {
    conductivity_air = 2.424 * std::pow(10, -2)
        + 7.477 * std::pow(10, -5) * temperature_film
        - 4.407 * std::pow(10, -9) * std::pow(temperature_film, 2);
  } else {
    return false;
  }

  // calculates the corrected air density
  double density_air = -999999;
  if (units_ == units::UnitSystem::kImperial) {
    density_air = (0.080695 - 2.901 * std::pow(10, -6) * weather_->elevation
        + 3.7 * std::pow(10, -11) * std::pow(weather_->elevation, 2))
        / (1 + 0.00367 * temperature_film);
  } else if (units_ == units::UnitSystem::kMetric) {
    density_air = (1.293 - 1.525 * std::pow(10, -4) * weather_->elevation
        + 6.379 * std::pow(10, -9) * std::pow(weather_->elevation, 2))
        / (1 + 0.00367 * temperature_film);
  } else {
    return false;
  }

  // calculates the dynamic viscosity of air
  double viscosity_air = -999999;
  if (units_ == units::UnitSystem::kImperial) {
    viscosity_air = (0.00353 * std::pow(temperature_film + 273, 1.5))
        / (temperature_film + 383.4);

    // adjusts units from ft-hr to ft-s
    viscosity_air = viscosity_air / 3600;
  } else if (units_ == units::UnitSystem::kMetric) {
    viscosity_air = (1.458 * std::pow(10, -6)
        * std::pow(temperature_film + 273, 1.5))
        / (temperature_film + 383.4);
  } else {
    return false;
  }

  // gets forced convection
  const double heat_convection_forced = HeatConvectionForced(
      conductivity_air,
      density_air,
      viscosity_air,
      temperature_cable);

  // gets natural convection
  const double heat_convection_natural = HeatConvectionNatural(
      density_air,
      temperature_cable);

  // determines which method of convection is highest and saves it
  // switches sign to indicate that heat is removed
  if (heat_convection_natural < heat_convection_forced) {
    return -1 * heat_convection_forced;
  } else {
    return -1 * heat_convection_natural;
  }
}

double CableHeatTransferSolver::HeatRadiation(
    const double& temperature_cable) const {
  // selects constant based on unit system
  double k = -999999;
  if (units_ == units::UnitSystem::kImperial) {
    k = 1.656;
  } else if (units_ == units::UnitSystem::kMetric) {
    k = 17.8;
  }

  // breaks down radiation equation into smaller chunks
  const double k1 = k * *cable_->diameter() * *cable_->emissivity();
  const double k2 = std::pow((temperature_cable + 273) / 100, 4);
  const double k3 = std::pow((weather_->temperature_air + 273) / 100, 4);

  // switches sign to indicate that heat is removed
  return -1 * k1 * (k2 - k3);
}

double CableHeatTransferSolver::HeatResistance(
    const double& temperature_cable,
    const double& current) const {
  // gets the resistance of the cable at a specific temperature
  const double resistance_cable = cable_->Resistance(temperature_cable);

  // determines the resistive heat
  return std::pow(current, 2) * resistance_cable;
}

double CableHeatTransferSolver::HeatSolar() const {
  // gets the projected area of cable (area per linear length)
  const double area_projected = *cable_->diameter() * 1;

  return *cable_->absorptivity() * weather_->radiation_solar * area_projected;
}

double CableHeatTransferSolver::HeatStored(const double& heat_in,
                                           const double& heat_out) const {
  return heat_in + heat_out;
}

bool CableHeatTransferSolver::Validate(
    const bool& is_included_warnings,
    std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "CABLE HEAT TRANSFER SOLVER";

  // validates cable
  if (cable_ == nullptr) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid cable";
      messages->push_back(message);
    }
  } else {
    if (cable_->Validate(is_included_warnings, messages) == false) {
      is_valid = false;
    }
  }

  // validates units
  if (units_ == units::UnitSystem::kNull) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid unit system";
      messages->push_back(message);
    }
  }

  // validates weather
  if (weather_ == nullptr) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid weather";
      messages->push_back(message);
    }
  } else {
    if (weather_->Validate(is_included_warnings, messages) == false) {
      is_valid = false;
    }
  }

  return is_valid;
}

const ThermalRatingCable* CableHeatTransferSolver::cable() const {
  return cable_;
}

void CableHeatTransferSolver::set_cable(const ThermalRatingCable* cable) {
  cable_ = cable;
}

void CableHeatTransferSolver::set_units(const units::UnitSystem& units) {
  units_ = units;
}

void CableHeatTransferSolver::set_weather(const ThermalRatingWeather* weather) {
  weather_ = weather;
}

units::UnitSystem CableHeatTransferSolver::units() const {
  return units_;
}

const ThermalRatingWeather* CableHeatTransferSolver::weather() const {
  return weather_;
}

double CableHeatTransferSolver::HeatConvectionForced(
    const double& conductivity_air,
    const double& density_air,
    const double& viscosity_air,
    const double& temperature_cable) const {
  // calculates the dimensionless reynolds number
  double num_reynolds = (*cable_->diameter() * density_air
                         * weather_->speed_wind) / viscosity_air;

  // calculates wind direction factor
  const double angle_wind_rad = units::ConvertAngle(
      weather_->angle_wind,
      units::AngleConversionType::kDegreesToRadians);

  const double factor_wind_direction =
      1.194 - std::cos(angle_wind_rad)
      + 0.194 * std::cos(2 * angle_wind_rad)
      + 0.368 * std::sin(2 * angle_wind_rad);

  // uses curve fitted equations to determine the forced heat convection
  // the equations are fitted for different wind speeds, and underestimate the
  // actual convection when the curve fit diverges
  const double heat_convection_forced_1 =
      factor_wind_direction
      * (1.01 + 1.35 * std::pow(num_reynolds, 0.52))
      * conductivity_air
      * (temperature_cable - weather_->temperature_air);

  const double heat_convection_forced_2 =
      factor_wind_direction
      * 0.754 * std::pow(num_reynolds, 0.6)
      * conductivity_air
      * (temperature_cable - weather_->temperature_air);

  // returns the highest forced convection value
  if (heat_convection_forced_1 <= heat_convection_forced_2) {
    return heat_convection_forced_2;
  } else {
    return heat_convection_forced_1;
  }
}

double CableHeatTransferSolver::HeatConvectionNatural(
    const double& density_air,
    const double& temperature_cable) const {
  // selects constant based on unit system
  double k = -999999;
  if (units_ == units::UnitSystem::kImperial) {
    k = 1.825;
  } else if (units_ == units::UnitSystem::kMetric) {
    k = 3.645;
  } else {
    return -999999;
  }

  // calculates
  return k * std::pow(density_air, 0.5)
      * std::pow(*cable_->diameter(), 0.75)
      * std::pow(temperature_cable - weather_->temperature_air, 1.25);
}
