// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef MODELS_THERMALRATING_STEADY_CABLE_TEMPERATURE_SOLVER_H_
#define MODELS_THERMALRATING_STEADY_CABLE_TEMPERATURE_SOLVER_H_

#include <list>

#include "models/base/error_message.h"
#include "models/base/units.h"
#include "models/thermalrating/cable_heat_transfer_state.h"
#include "models/thermalrating/thermal_rating_cable.h"
#include "models/thermalrating/thermal_rating_weather.h"

/// \par OVERVIEW
///
/// This class solves for the steady-state temperature in a transmission
/// cable based on weather parameters and current.
///
/// \par HEAT TRANSFER
///
/// This class accounts for the following modes of heat transfer:
/// - convection
/// - radiation
/// - resistance
/// - solar
///
/// No energy is stored by the cable due to the steady-state condition. Applying
/// conservation of energy, the amount transferred into the cable equals the
/// amount that is dissipated.
///
/// The equations to solve for the convection, radiation, and resistance heat
/// transfer are temperature dependent, and can not be determined by using
/// current.
///
/// \par TEMPERATURE
///
/// Cable temperature is solved for iteratively using SteadyCableCurrentSolver.
/// An initial temperature is guessed, and the resulting current is compared.
/// Once the current matches to a set precision, the steady-state temperature
/// is solved.
class SteadyCableTemperatureSolver {
 public:
  /// \brief Constructor.
  SteadyCableTemperatureSolver();

  /// \brief Destructor.
  ~SteadyCableTemperatureSolver();

  /// \brief Gets the heat transfer state.
  /// \return The heat transfer state.
  CableHeatTransferState StateHeatTransfer() const;

  /// \brief Gets the cable temperature.
  /// \return The cable temperature.
  double TemperatureCable() const;

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

  /// \brief Gets the electrical current.
  /// \return The electrical current.
  double current() const;

  /// \brief Sets the cable.
  /// \param[in] cable
  ///   The cable.
  void set_cable(const ThermalRatingCable* cable);

  /// \brief Sets the electrical current.
  /// \param[in] current
  ///   The electrical current.
  void set_current(const double& current);

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
  /// \brief Determines if class is updated.
  /// \return A boolean indicating if class is updated.
  bool IsUpdated() const;

  /// \brief Solves for the cable temperature and heat transfer state.
  /// \return If the current has been solved succesfully.
  bool SolveTemperatureAndState() const;

  /// \brief Updates cached member variables and modifies control variables if
  ///   update is required.
  /// \return A boolean indicating if class updates completed successfully.
  bool Update() const;

  /// \var cable_
  ///   The cable.
  const ThermalRatingCable* cable_;

  /// \var current_
  ///   The amount of electric charge flowing through the cable.
  double current_;

  /// \var is_updated_
  ///   An indicator that tells if the class has been updated.
  mutable bool is_updated_;

  /// \var state_
  ///   The heat transfer state.
  mutable CableHeatTransferState state_;

  /// \var temperature_cable_
  ///   The temperature of the cable. This is equivalent to the cable surface
  ///   temperature.
  mutable double temperature_cable_;

  /// \var units_
  ///   The unit system. This is used to keep track of which constants to use.
  units::UnitSystem units_;

  /// \var weather_
  ///   The weather.
  const ThermalRatingWeather* weather_;
};

#endif  // MODELS_THERMALRATING_STEADY_CABLE_TEMPERATURE_SOLVER_H_
