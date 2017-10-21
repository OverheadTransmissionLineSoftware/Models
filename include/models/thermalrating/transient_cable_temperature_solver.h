// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef OTLS_MODELS_THERMALRATING_TRANSIENTCABLETEMPERATURESOLVER_H_
#define OTLS_MODELS_THERMALRATING_TRANSIENTCABLETEMPERATURESOLVER_H_

#include "models/base/error_message.h"
#include "models/base/units.h"
#include "models/thermalrating/cable_heat_transfer_solver.h"
#include "models/thermalrating/cable_heat_transfer_state.h"
#include "models/thermalrating/thermal_rating_cable.h"
#include "models/thermalrating/thermal_rating_weather.h"

/// \par OVERVIEW
///
/// This struct contains a time-temperature point.
struct TemperaturePoint {
  /// \var time
  ///   The time, in seconds.
  int time;

  /// \var temperature
  ///   The cable temperature.
  double temperature;
};

/// \par OVERVIEW
///
/// This class solves for the transient temperature of a transmission cable
/// after the electrical loading has been changed.
///
/// \par CURRENT
///
/// The current change is modeled at time = 0. The steady (or starting) current
/// is the electrical loading before time = 0, while the step current is applied
/// and is constant for remaining time duration after time = 0.
///
/// \par TEMPERATURE
///
/// The cable is assumed to be at a steady-state temperature before the current
/// is changed. After the step current is applied, the cable temperature will
/// change over time to reach the new steady-state condition.
///
/// To solve for the temperature over time, the cable temperature is calculated
/// in discrete intervals. At each point, following methods of heat transfer
/// are calculated:
/// - convection
/// - radiation
/// - resistance
/// - solar
///
/// Applying conversation of energy principle, the heat stored in the cable
/// material is used to balance the total heat transfer to zero. The duration
/// of the discrete interval is combined with the storage heat transfer to solve
/// for a new future cable temperature.
///
/// Keeping the time interval small reduces the amount of error when solving for
/// the future temperature of the cable. The various heat transfer methods are
/// all temperature-dependent.
class TransientCableTemperatureSolver {
 public:
  /// \brief Constructor.
  TransientCableTemperatureSolver();

  /// \brief Destructor.
  ~TransientCableTemperatureSolver();

  /// \brief Gets the transient temperature points.
  /// \return The transient temperature points. If the class does not update, a
  ///   nullptr is returned.
  const std::list<TemperaturePoint>* PointsTemperature() const;

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

  /// \brief Gets the steady-state current.
  /// \return The steady-state current.
  double current_steady() const;

  /// \brief Gets the step current.
  /// \return The step current.
  double current_step() const;

  /// \brief Gets the duration.
  /// \return The duration.
  int duration() const;

  /// \brief Sets the cable.
  /// \param[in] cable
  ///   The cable.
  void set_cable(const ThermalRatingCable* cable);

  /// \brief Sets the steady-state current.
  /// \param[in] current_steady
  ///   The steady-state electrical current.
  void set_current_steady(const double& current_steady);

  /// \brief Sets the step current.
  /// \param[in] current_step
  ///   The electrical current to step to.
  void set_current_step(const double& current_step);

  /// \brief Sets the time duration.
  /// \param[in] duration
  ///   The time duration.
  void set_duration(const int& duration);

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
  /// \brief Solves for a heat transfer state.
  /// \param[in] current
  ///   The electrical current.
  /// \param[in] temperature
  ///   The cable temperature.
  /// \return The heat transer state.
  CableHeatTransferState HeatTransferState(const double& current,
                                           const double& temperature) const;

  /// \brief Determines if class is updated.
  /// \return A boolean indicating if class is updated.
  bool IsUpdated() const;

  /// \brief Gets a new temperature.
  /// \param[in] temperature
  ///   The existing temperature.
  /// \param[in] state
  ///   The heat transfer state.
  /// \param[in] time_delta
  ///   The time increment.
  /// \return A new temperature.
  double TemperatureNew(const double& temperature,
                        const CableHeatTransferState& state,
                        const int& time_delta) const;

  /// \brief Gets the steady-state temperature.
  /// \param[in] current
  ///   The electrical current.
  /// \return The steady-state temperature.
  double TemperatureSteady(const double& current) const;

  /// \brief Updates cached member variables and modifies control variables if
  ///   update is required.
  /// \return A boolean indicating if class updates completed successfully.
  bool Update() const;

  /// \brief Updates the temperature points.
  /// \return If the temperature points have been updated successfully.
  bool UpdateTemperaturePoints() const;

  /// \var cable_
  ///   The cable.
  const ThermalRatingCable* cable_;

  /// \var current_steady_
  ///   The starting steady-state current.
  double current_steady_;

  /// \var current_step_
  ///   The current to step to.
  double current_step_;

  /// \var duration_
  ///   The time duration.
  int duration_;

  /// \var is_updated_
  ///   An indicator that tells if the class has been updated.
  mutable bool is_updated_;

  /// \var points_temperature_
  ///   The calculated temperature points.
  mutable std::list<TemperaturePoint> points_temperature_;

  /// \var solver_
  ///   The heat transfer solver.
  mutable CableHeatTransferSolver solver_;

  /// \var units_
  ///   The unit system. This is used to keep track of which constants to use.
  units::UnitSystem units_;

  /// \var weather_
  ///   The weather.
  const ThermalRatingWeather* weather_;
};

#endif  // OTLS_MODELS_THERMALRATING_TRANSIENTCABLETEMPERATURESOLVER_H_
