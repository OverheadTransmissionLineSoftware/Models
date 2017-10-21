// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef OTLS_MODELS_THERMALRATING_SOLARRADIATIONSOLVER_H_
#define OTLS_MODELS_THERMALRATING_SOLARRADIATIONSOLVER_H_

#include <list>
#include <vector>

#include "models/base/error_message.h"
#include "models/base/units.h"

/// \par OVERVIEW
///
/// This enums contains types of atmosphere quality types.
enum class AtmosphereQualityType {
  kNull,
  kClear,
  kIndustrial
};

/// \par OVERVIEW
///
/// This class solves for the effective solar radiation on a transmission cable.
///
/// \par SUN POSITIONING
///
/// This class models the sun position. It will convert day/hour/latitude to
/// azimuth/altitude.
///
/// \par RADIATION AND CORRECTIONS
///
/// The radiation is based on a polynomial that uses solar altitude. This
/// radiation value is corrected for the following factors:
/// - angle of incidence between sun and cable
/// - atmosphere quality
/// - elevation
///
/// \par UNITS
///
/// This class requires a unit system to be specified due to the constants
/// that are used in the formulas.
class SolarRadiationSolver {
 public:
  /// \brief Constructor.
  SolarRadiationSolver();

  /// \brief Destructor.
  ~SolarRadiationSolver();

  /// \brief Gets the altitude of the sun.
  /// \return The altitude of the sun.
  double AltitudeSun() const;

  /// \brief Gets the azimuth of the sun.
  /// \return The azimuth of the sun.
  double AzimuthSun() const;

  /// \brief Gets the radiation.
  /// \return The radiation.
  double Radiation() const;

  /// \brief Validates member variables.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings = true,
                std::list<ErrorMessage>* messages = nullptr) const;

  /// \brief Gets the cable azimuth.
  /// \return The cable azimuth.
  double azimuth_cable() const;

  /// \brief Gets the day.
  /// \return The day.
  int day() const;

  /// \brief Gets the elevation.
  /// \return The elevation.
  double elevation() const;

  /// \brief Gets the hour.
  /// \return The hour.
  int hour() const;

  /// \brief Gets the latitude.
  /// \return The latitude.
  double latitude() const;

  /// \brief Gets the atmosphere quality.
  /// \return The atmosphere quality.
  AtmosphereQualityType quality() const;

  /// \brief Sets the cable azimuth.
  /// \param[in] azimuth_cable
  ///   The cable azimuth.
  void set_azimuth_cable(const double& azimuth_cable);

  /// \brief Sets the day.
  /// \param[in] day
  ///   The day.
  void set_day(const int& day);

  /// \brief Sets the elevation.
  /// \param[in] elevation
  ///   The elevation.
  void set_elevation(const double& elevation);

  /// \brief Sets the hour.
  /// \param[in] hour
  ///   The hour.
  void set_hour(const int& hour);

  /// \brief Sets the latitude.
  /// \param[in] latitude
  ///   The latitude.
  void set_latitude(const double& latitude);

  /// \brief Sets the atmosphere quality.
  /// \param[in] quality
  ///   The atmosphere quality.
  void set_quality_atmosphere(const AtmosphereQualityType& quality);

  /// \brief Sets the unit system.
  /// \param[in] units
  ///   The unit system.
  void set_units(const units::UnitSystem& units);

  /// \brief Gets the unit system.
  /// \return The unit system.
  units::UnitSystem units() const;

 private:
  /// \brief Gets the hour angle.
  /// \param[in] hour
  ///   The hour.
  /// \return The hour angle (degrees).
  /// The hour angle is the number of hours from noon x 15 degrees.
  /// Ex: 11:00 = -15 degrees, 13:00 = 15 degrees
  int AngleHour(const int& hour) const;

  /// \brief Gets the incidence angle between the cable and sun.
  /// \return The indidence angle between the cable and sun (degrees).
  double AngleIncidence() const;

  /// \brief Gets the solar declination.
  /// \return The solar declination (degrees).
  /// The declination is the position of the sun relative to the equator. On
  /// the spring/fall equinox this angle is zero, and for summer/winter
  /// solstices this angle can be either +/- 23.45 degrees.
  double Declination() const;

  /// \brief Gets the solar elevation correction factor.
  /// \return The elevation correction factor.
  double FactorElevationCorrection() const;

  /// \brief Determines if class is updated.
  /// \return A boolean indicating if class is updated.
  bool IsUpdated() const;

  /// \brief Solves the solar altitude.
  /// \param[in] declination
  ///   The declination angle (degrees).
  /// \param[in] angle_hour
  ///   The hour angle (degrees).
  /// \return If the solar altitude has been solved for successfully.
  bool SolveAltitudeSolar(const double& declination,
                          const double& angle_hour) const;

  /// \brief Solves the solar azimuth.
  /// \param[in] declination
  ///   The declination angle (degrees).
  /// \param[in] angle_hour
  ///   The hour angle (degrees).
  /// \return If the solar azimuth has been solved for successfully.
  bool SolveAzimuthSolar(const double& declination,
                         const double& angle_hour) const;

  /// \brief Solves for the radiation.
  /// \return If the radiation was successfully solved for.
  bool SolveRadiation() const;

  /// \brief Updates cached member variables and modifies control variables if
  ///   update is required.
  /// \return A boolean indicating if class updates completed successfully.
  bool Update() const;

  /// \brief Updates the polynomial coefficients.
  /// \return If the polynomial coefficients have been updated successfully.
  bool UpdatePolynomialCoefficients() const;

  /// \brief Updates the solar position (azimuth and altitude).
  /// \return If the solar position parameters have been updated succesfully.
  bool UpdateSolarPosition() const;

  /// \var altitude_sun_
  ///   The solar altitude angle. This is the angle from the horizon, and can
  ///   be between 0-90 degrees.
  mutable double altitude_sun_;

  /// \var azimuth_cable_
  ///   The cable azimuth angle (degrees). This is the angle from the north pole
  ///   rotating clockwise toward east, and can be between 0-360 degrees.
  double azimuth_cable_;

  /// \var azimuth_sun_
  ///   The solar azimuth angle (degrees). This is the angle from the horizon,
  ///   and can be between 0-90 degrees.
  mutable double azimuth_sun_;

  /// \var coefficients_atmosphere_
  ///   The polynomial coefficients used to calculate radiation. These are
  ///   selected based on atmosphere quality.
  ///   x = solar altitude, y = solar radiation
  mutable std::vector<double> coefficients_atmosphere_;

  /// \var coefficients_elevation_
  ///   The polynomial coefficients used to correct radiation based on
  ///   elevation.
  ///   x = elevation, y = solar radiation correction factor
  mutable std::vector<double> coefficients_elevation_;

  /// \var day_
  ///   The day of the year. Jan 1 = 1, Feb 1 = 32, ..
  int day_;

  /// \var elevation_
  ///   The elevation above sea level.
  double elevation_;

  /// \var hour_
  ///   The hour of the day. 00:00 = 0, 01:00 = 1, ..
  int hour_;

  /// \var is_updated_
  ///   An indicator that tells if the class has been updated.
  mutable bool is_updated_;

  /// \var latitude_
  ///   The latitude (N-S location). -90 to +90 degrees
  double latitude_;

  /// \var quality_atmosphere_
  ///   The atmosphere quality.
  AtmosphereQualityType quality_atmosphere_;

  /// \var radiation_
  ///   The solar radiation per unit area.
  mutable double radiation_;

  /// \var units_
  ///   The unit system. This is used to keep track of which constants to use.
  units::UnitSystem units_;
};

#endif  // OTLS_MODELS_THERMALRATING_SOLARRADIATIONSOLVER_H_
