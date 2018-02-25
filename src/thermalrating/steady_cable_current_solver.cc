// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/thermalrating/steady_cable_current_solver.h"

#include <cmath>

#include "models/base/helper.h"
#include "models/thermalrating/cable_heat_transfer_solver.h"

SteadyCableCurrentSolver::SteadyCableCurrentSolver() {
  cable_ = nullptr;
  temperature_cable_ = -999999;
  units_ = units::UnitSystem::kNull;
  weather_ = nullptr;

  state_ = CableHeatTransferState();
  current_ = -999999;

  is_updated_ = false;
}

SteadyCableCurrentSolver::~SteadyCableCurrentSolver() {
}

double SteadyCableCurrentSolver::Current() const {
  // updates class if necessary
  if ((IsUpdated() == false) && (Update() == false)) {
    return -999999;
  }

  return current_;
}

CableHeatTransferState SteadyCableCurrentSolver::StateHeatTransfer() const {
  // updates class if necessary
  if ((IsUpdated() == false) && (Update() == false)) {
    return CableHeatTransferState();
  }

  return state_;
}

bool SteadyCableCurrentSolver::Validate(
    const bool& is_included_warnings,
    std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "STEADY CABLE CURRENT SOLVER";

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

  // validates temperature-cable
  if ((temperature_cable_ < -100) || (500 < temperature_cable_)) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid cable temperature";
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

  // validates update process
  if (Update() == false) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Error updating class";
      messages->push_back(message);
    }

    return is_valid;
  }

  return is_valid;
}

const ThermalRatingCable* SteadyCableCurrentSolver::cable() const {
  return cable_;
}

void SteadyCableCurrentSolver::set_cable(const ThermalRatingCable* cable) {
  cable_ = cable;
  is_updated_ = false;
}

void SteadyCableCurrentSolver::set_temperature_cable(
    const double& temperature_cable) {
  temperature_cable_ = temperature_cable;
  is_updated_ = false;
}

void SteadyCableCurrentSolver::set_units(const units::UnitSystem& units) {
  units_ = units;
}

void SteadyCableCurrentSolver::set_weather(
    const ThermalRatingWeather* weather) {
  weather_ = weather;
}

double SteadyCableCurrentSolver::temperature_cable() const {
  return temperature_cable_;
}

units::UnitSystem SteadyCableCurrentSolver::units() const {
  return units_;
}

const ThermalRatingWeather* SteadyCableCurrentSolver::weather() const {
  return weather_;
}

bool SteadyCableCurrentSolver::IsUpdated() const {
  return is_updated_ == true;
}

bool SteadyCableCurrentSolver::SolveCurrent() const {
  // gets the resistance of the cable at a specific temperature
  const double resistance_cable = cable_->Resistance(temperature_cable_);

  // determines the current
  current_ = std::sqrt(std::abs(state_.heat_resistance) / resistance_cable);

  // reverses sign of current if resistance heat is negative
  if (state_.heat_resistance < 0) {
    current_ = current_ * -1;
  }

  return true;
}

bool SteadyCableCurrentSolver::SolveHeatTransferState() const {
  // sets up heat transfer solver
  CableHeatTransferSolver solver;
  solver.set_cable(cable_);
  solver.set_units(units_);
  solver.set_weather(weather_);
  if (solver.Validate() == false) {
    return false;
  }

  // solves for the various methods of heat transfer
  state_ = CableHeatTransferState();
  state_.heat_convection = solver.HeatConvection(temperature_cable_);
  state_.heat_radiation = solver.HeatRadiation(temperature_cable_);
  state_.heat_solar = solver.HeatSolar();
  state_.heat_storage = 0;

  // resistance heat transfer is solved using by using conservation of energy
  // energy in = energy out
  state_.heat_resistance = -1 * (state_.heat_convection
                           + state_.heat_radiation
                           + state_.heat_solar);

  return true;
}

bool SteadyCableCurrentSolver::Update() const {
  // updates the heat transfer state
  is_updated_ = SolveHeatTransferState();
  if (is_updated_ == false) {
    return false;
  }

  // updates the current
  is_updated_ = SolveCurrent();
  if (is_updated_ == false) {
    return false;
  }

  // if it reaches this point, update was successful
  return true;
}
