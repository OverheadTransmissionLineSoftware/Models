// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef MODELS_THERMALRATING_CABLE_HEAT_TRANSFER_SOLVER_H_
#define MODELS_THERMALRATING_CABLE_HEAT_TRANSFER_SOLVER_H_

#include <list>

#include "models/base/error_message.h"
#include "models/base/units.h"
#include "models/thermalrating/thermal_rating_cable.h"
#include "models/thermalrating/thermal_rating_weather.h"

/// \par OVERVIEW
///
/// This class models heat transfer to/from a cable.
///
/// \par HEAT TRANSFER
///
/// This class models the following methods of heat transfer:
///  - resistive (positive heat transfer)
///  - solar absorption (positive heat transfer)
///  - material storage (positive/negative heat transfer)
///  - surface radiation (negative heat transfer)
///  - wind convection (negative heat transfer)
///
/// \par STANDARD
///
/// This class follows the heat transfer equations defined in IEEE 738.
class CableHeatTransferSolver {
 public:
  /// \brief Constructor.
  CableHeatTransferSolver();

  /// \brief Destructor.
  ~CableHeatTransferSolver();

  /// \brief Gets the heat transfer due to convection.
  /// \param[in] temperature_cable
  ///   The cable temperature.
  /// \return The heat transfer due to convection.
  /// This heat transfer will always be negative, as it removes heat from the
  /// cable.
  double HeatConvection(const double& temperature_cable) const;

  /// \brief Gets the heat transfer due to radiation.
  /// \param[in] temperature_cable
  ///   The cable temperature.
  /// \return The heat transfer due to radiation.
  /// This heat transfer will always be negative, as it removes heat from the
  /// cable.
  double HeatRadiation(const double& temperature_cable) const;

  /// \brief Gets the heat transfer due to electrical resistance.
  /// \param[in] temperature_cable
  ///   The cable temperature.
  /// \param[in] current
  ///   The electrical current.
  /// \return The heat transfer due to electrical resistance.
  /// This heat transfer will always be positive, as it adds heat to the cable.
  double HeatResistance(const double& temperature_cable,
                        const double& current) const;

  /// \brief Gets the heat transfer due to solar radiation.
  /// \return The heat transfer due to solar radiation.
  /// This heat transfer will always be positive, as it adds heat to the cable.
  double HeatSolar() const;

  /// \brief Gets the heat that is stored.
  /// \param[in] heat_in
  ///   The heat being absorbed by the cable. This number should always be
  ///   positive, as it adds heat to the cable.
  /// \param[in] heat_out
  ///   The heat being emitted by the cable. This number should always be
  ///   negative, as it removes heat from the cable.
  /// \return The rate that heat that is being stored in the cable.
  double HeatStored(const double& heat_in, const double& heat_out) const;

  /// \brief Validates member variables.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings = true,
                std::list<ErrorMessage>* messages = nullptr) const;

  /// \brief Gets the cable.
  /// \return The cable.
  const ThermalRatingCable* cable() const;

  /// \brief Sets the cable.
  /// \param[in] cable
  ///   The cable.
  void set_cable(const ThermalRatingCable* cable);

  /// \brief Sets the unit system.
  /// \param[in] units
  ///   The unit system.
  void set_units(const units::UnitSystem& units);

  /// \brief Sets the weather.
  /// \param[in] weather
  ///   The weather.
  void set_weather(const ThermalRatingWeather* weather);

  /// \brief Gets the unit system.
  /// \return The unit system.
  units::UnitSystem units() const;

  /// \brief Gets the weather.
  /// \return The weather.
  const ThermalRatingWeather* weather() const;

 private:
  /// \brief Gets the heat transfer due to forced convection.
  /// \param[in] conductivity_air
  ///   The thermal conductivity of the air.
  /// \param[in] density_air
  ///   The density of the air.
  /// \param[in] viscosity_air
  ///   The viscosity of the air.
  /// \param[in] temperature_cable
  ///   The cable temperature.
  /// \return The heat transfer due to forced convection.
  double HeatConvectionForced(const double& conductivity_air,
                              const double& density_air,
                              const double& viscosity_air,
                              const double& temperature_cable) const;

  /// \brief Gets the heat transfer due to natural convection.
  /// \param[in] density_air
  ///   The density of the air.
  /// \param[in] temperature_cable
  ///   The cable temperature.
  /// \return The heat transfer due to natural convection.
  double HeatConvectionNatural(const double& density_air,
                               const double& temperature_cable) const;

  /// \var cable_
  ///   The cable.
  const ThermalRatingCable* cable_;

  /// \var units_
  ///   The unit system. This is used to keep track of which constants to use.
  units::UnitSystem units_;

  /// \var weather_
  ///   The weather.
  const ThermalRatingWeather* weather_;
};

#endif  // MODELS_THERMALRATING_CABLE_HEAT_TRANSFER_SOLVER_H_
