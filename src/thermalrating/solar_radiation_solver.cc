// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/thermalrating/solar_radiation_solver.h"

#include <cmath>

#include "models/base/polynomial.h"

SolarRadiationSolver::SolarRadiationSolver() {
  azimuth_cable_ = -999999;
  day_ = -9999;
  elevation_ = -999999;
  hour_ = -9999;
  latitude_ = -999999;
  quality_atmosphere_ = AtmosphereQualityType::kNull;
  units_ = units::UnitSystem::kNull;

  altitude_sun_ = -999999;
  azimuth_sun_ = -999999;
  coefficients_atmosphere_.resize(7, 0);
  coefficients_elevation_.resize(3, 0);
  radiation_ = -999999;

  is_updated_ = false;
}

SolarRadiationSolver::~SolarRadiationSolver() {
}

double SolarRadiationSolver::AltitudeSun() const {
  // updates class if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return altitude_sun_;
}

double SolarRadiationSolver::AzimuthSun() const {
  // updates class if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return azimuth_sun_;
}

double SolarRadiationSolver::Radiation() const {
  // updates class if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return radiation_;
}

bool SolarRadiationSolver::Validate(const bool& is_included_warnings,
                                    std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "SOLAR RADIATION SOLVER";

  // validates azimuth-cable
  if ((azimuth_cable_ < 0) || (360 < azimuth_cable_)) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid cable azimuth";
      messages->push_back(message);
    }
  }

  // validates day
  if ((day_ <= 0) || (365 < day_)) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid day";
      messages->push_back(message);
    }
  }

  // validates elevation
  if (elevation_ < 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid elevation";
      messages->push_back(message);
    }
  }

  // validates hour
  if ((hour_ < 0) || (24 <= hour_)) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid hour";
      messages->push_back(message);
    }
  }

  // validates latitude
  if ((latitude_ < -90) || (90 < latitude_)) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid latitude";
      messages->push_back(message);
    }
  }

  // validates quality-atmosphere
  if (quality_atmosphere_ == AtmosphereQualityType::kNull) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid atmosphere quality";
      messages->push_back(message);
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

  // returns if errors are present
  if (is_valid == false) {
    return is_valid;
  }

  // validates update process
  if (Update() == false) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Error updating class";
      messages->push_back(message);
    }
  }

  return is_valid;
}

double SolarRadiationSolver::azimuth_cable() const {
  return azimuth_cable_;
}

int SolarRadiationSolver::day() const {
  return day_;
}

double SolarRadiationSolver::elevation() const {
  return elevation_;
}

int SolarRadiationSolver::hour() const {
  return hour_;
}

double SolarRadiationSolver::latitude() const {
  return latitude_;
}

AtmosphereQualityType SolarRadiationSolver::quality() const {
  return quality_atmosphere_;
}

void SolarRadiationSolver::set_azimuth_cable(const double& azimuth_cable) {
  azimuth_cable_ = azimuth_cable;
  is_updated_ = false;
}

void SolarRadiationSolver::set_day(const int& day) {
  day_ = day;
  is_updated_ = false;
}

void SolarRadiationSolver::set_elevation(const double& elevation) {
  elevation_ = elevation;
  is_updated_ = false;
}

void SolarRadiationSolver::set_hour(const int& hour) {
  hour_ = hour;
  is_updated_ = false;
}

void SolarRadiationSolver::set_latitude(const double& latitude) {
  latitude_ = latitude;
  is_updated_ = false;
}

void SolarRadiationSolver::set_quality_atmosphere(
    const AtmosphereQualityType& quality) {
  quality_atmosphere_ = quality;
  is_updated_ = false;
}

void SolarRadiationSolver::set_units(const units::UnitSystem& units) {
  units_ = units;
  is_updated_ = false;
}

units::UnitSystem SolarRadiationSolver::units() const {
  return units_;
}

int SolarRadiationSolver::AngleHour(const int& hour) const {
  return (hour - 12) * 15;
}

double SolarRadiationSolver::AngleIncidence() const {
  // converts terms to radians
  const double altitude_sun_rad = units::ConvertAngle(
      altitude_sun_,
      units::AngleConversionType::kDegreesToRadians);
  const double azimuth_sun_rad = units::ConvertAngle(
      azimuth_sun_,
      units::AngleConversionType::kDegreesToRadians);
  const double azimuth_cable_rad = units::ConvertAngle(
      azimuth_cable_,
      units::AngleConversionType::kDegreesToRadians);

  // breaks down the into smaller chunks
  const double term1 = std::cos(altitude_sun_rad);
  const double term2 = std::cos(azimuth_sun_rad - azimuth_cable_rad);

  // calculates the incidence angle and converts to degrees
  double angle_incidence = std::acos(term1 * term2);
  angle_incidence = units::ConvertAngle(
      angle_incidence,
      units::AngleConversionType::kRadiansToDegrees);

  return angle_incidence;
}

double SolarRadiationSolver::Declination() const {
  // gets an angle based on day of year
  const double angle_degrees = static_cast<double>(284 + day_) / 365 * 360;
  const double angle_radians = units::ConvertAngle(
      angle_degrees,
      units::AngleConversionType::kDegreesToRadians);

  // scales to fit max declination
  return 23.46 * std::sin(angle_radians);
}

double SolarRadiationSolver::FactorElevationCorrection() const {
  Polynomial polynomial(&coefficients_elevation_);
  return polynomial.Y(elevation_);
}

bool SolarRadiationSolver::IsUpdated() const {
  if (is_updated_ == true) {
    return true;
  } else {
    return false;
  }
}

bool SolarRadiationSolver::SolveAltitudeSolar(
    const double& declination,
    const double& angle_hour) const {
  // converts terms to radians
  const double angle_hour_rad = units::ConvertAngle(
      angle_hour,
      units::AngleConversionType::kDegreesToRadians);
  const double declination_rad = units::ConvertAngle(
      declination,
      units::AngleConversionType::kDegreesToRadians);
  const double latitude_rad = units::ConvertAngle(
      latitude_,
      units::AngleConversionType::kDegreesToRadians);

  // calculates the solar altitude
  const double term1 = std::cos(latitude_rad) * std::cos(declination_rad)
                       * std::cos(angle_hour_rad);
  const double term2 = std::sin(latitude_rad) * std::sin(declination_rad);
  altitude_sun_ = std::asin(term1 + term2);

  altitude_sun_ =
      units::ConvertAngle(altitude_sun_,
      units::AngleConversionType::kRadiansToDegrees);

  return true;
}

bool SolarRadiationSolver::SolveAzimuthSolar(
    const double& declination,
    const double& angle_hour) const {
  // converts terms to radians
  const double angle_hour_rad = units::ConvertAngle(
      angle_hour,
      units::AngleConversionType::kDegreesToRadians);
  const double declination_rad = units::ConvertAngle(
      declination,
      units::AngleConversionType::kDegreesToRadians);
  const double latitude_rad = units::ConvertAngle(
      latitude_,
      units::AngleConversionType::kDegreesToRadians);

  // calculates the solar azimuth variable
  const double term1 = std::sin(angle_hour_rad);
  const double term2 = std::sin(latitude_rad) * std::cos(angle_hour_rad);
  const double term3 = std::cos(latitude_rad) * std::tan(declination_rad);
  const double chi = term1 / (term2 - term3);

  // selects a constant
  double kC = -999999;;
  if (angle_hour < 0) {
    if (chi < 0) {
      kC = 180;
    } else {
      kC = 0;
    }
  } else if (0 <= angle_hour) {
    if (chi < 0) {
      kC = 360;
    } else {
      kC = 180;
    }
  } else {
    return false;
  }

  // calculates solar azimuth
  const double term4 = units::ConvertAngle(
      std::atan(chi),
      units::AngleConversionType::kRadiansToDegrees);

  azimuth_sun_ = kC + term4;

  return true;
}

bool SolarRadiationSolver::SolveRadiation() const {
  // calculates the radiation using a polynomial
  // this is the radiation at sea level (elevation = 0)
  Polynomial polynomial;
  polynomial.set_coefficients(&coefficients_atmosphere_);
  radiation_ = polynomial.Y(altitude_sun_);

  // adjusts radiation based on incidence angle and elevation
  const double angle_incidence = AngleIncidence();
  const double angle_incidence_rad = units::ConvertAngle(
      angle_incidence,
      units::AngleConversionType::kDegreesToRadians);

  polynomial.set_coefficients(&coefficients_elevation_);
  const double factor_elevation_correction = polynomial.Y(elevation_);

  radiation_ = radiation_ * std::sin(angle_incidence_rad)
               * factor_elevation_correction;

  return true;
}

bool SolarRadiationSolver::Update() const {
  // triggers updates if needed
  if (is_updated_ == false) {
    // updates polynomial coefficients
    is_updated_ = UpdatePolynomialCoefficients();
    if (is_updated_ == false) {
      return false;
    }

    // updates solar position
    is_updated_ = UpdateSolarPosition();
    if (is_updated_ == false) {
      return false;
    }

    // solves for radiation
    is_updated_ = SolveRadiation();
    if (is_updated_ == false) {
      return false;
    }
  }

  // if it reaches this point, update was successful
  return true;
}

bool SolarRadiationSolver::UpdatePolynomialCoefficients() const {
  if (units_ == units::UnitSystem::kImperial) {
    // imperial unit constants
    if (quality_atmosphere_ == AtmosphereQualityType::kClear) {
      coefficients_atmosphere_[0] = -3.9241;
      coefficients_atmosphere_[1] = 5.9276;
      coefficients_atmosphere_[2] = -1.7856 * std::pow(10, -1);
      coefficients_atmosphere_[3] = 3.223 * std::pow(10, -3);
      coefficients_atmosphere_[4] = -3.3549 * std::pow(10, -5);
      coefficients_atmosphere_[5] = 1.8053 * std::pow(10, -7);
      coefficients_atmosphere_[6] = -3.7868 * std::pow(10, -10);
    } else if (quality_atmosphere_ == AtmosphereQualityType::kIndustrial) {
      coefficients_atmosphere_[0] = 4.9408;
      coefficients_atmosphere_[1] = 1.3202;
      coefficients_atmosphere_[2] = 6.1444 * std::pow(10, -2);
      coefficients_atmosphere_[3] = -2.9411 * std::pow(10, -3);
      coefficients_atmosphere_[4] = 5.07752 * std::pow(10, -5);
      coefficients_atmosphere_[5] = -4.03627 * std::pow(10, -7);
      coefficients_atmosphere_[6] = 1.22967 * std::pow(10, -9);
    } else {
      return false;
    }

    coefficients_elevation_[0] = 1;
    coefficients_elevation_[1] = 3.500 * std::pow(10, -5);
    coefficients_elevation_[2] = -1.000 * std::pow(10, -9);

  } else if (units_ == units::UnitSystem::kMetric) {
    // metric unit constants
    if (quality_atmosphere_ == AtmosphereQualityType::kClear) {
      coefficients_atmosphere_[0] = -42.2391;
      coefficients_atmosphere_[1] = 63.8044;
      coefficients_atmosphere_[2] = -1.9220;
      coefficients_atmosphere_[3] = 3.46921 * std::pow(10, -2);
      coefficients_atmosphere_[4] = -3.61118 * std::pow(10, -4);
      coefficients_atmosphere_[5] = 1.94318 * std::pow(10, -6);
      coefficients_atmosphere_[6] = -4.07608 * std::pow(10, -9);
    } else if (quality_atmosphere_ == AtmosphereQualityType::kIndustrial) {
      coefficients_atmosphere_[0] = 53.1821;
      coefficients_atmosphere_[1] = 14.2110;
      coefficients_atmosphere_[2] = 6.6138 * std::pow(10, -1);
      coefficients_atmosphere_[3] = -3.1658 * std::pow(10, -2);
      coefficients_atmosphere_[4] = 5.4654 * std::pow(10, -4);
      coefficients_atmosphere_[5] = -4.3446 * std::pow(10, -6);
      coefficients_atmosphere_[6] = 1.3236 * std::pow(10, -8);
    } else {
      return false;
    }

    coefficients_elevation_[0] = 1;
    coefficients_elevation_[1] = 1.148 * std::pow(10, -4);
    coefficients_elevation_[2] = -1.108 * std::pow(10, -8);

  } else {
    return false;
  }

  return true;
}

bool SolarRadiationSolver::UpdateSolarPosition() const {
  // calculates starting parameters
  const double declination = Declination();
  const double angle_hour = AngleHour(hour_);

  // solves azimuth
  if (SolveAzimuthSolar(declination, angle_hour) == false) {
    return false;
  }

  // solves altitude
  if (SolveAltitudeSolar(declination, angle_hour) == false) {
    return false;
  }

  return true;
}
