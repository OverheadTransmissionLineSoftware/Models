// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/thermalrating/transient_cable_temperature_solver.h"

#include "models/thermalrating/steady_cable_temperature_solver.h"

TransientCableTemperatureSolver::TransientCableTemperatureSolver() {
  cable_ = nullptr;
  current_steady_ = -999999;
  current_step_ = -999999;
  duration_ = -9999;
  units_ = units::UnitSystem::kNull;
  weather_ = nullptr;

  points_temperature_.clear();

  is_updated_ = false;
}

TransientCableTemperatureSolver::~TransientCableTemperatureSolver() {
}

const std::list<TemperaturePoint>*
    TransientCableTemperatureSolver::PointsTemperature() const {
  // updates class if necessary
  if ((IsUpdated() == false) && (Update() == false)) {
    return nullptr;
  }

  return &points_temperature_;
}

bool TransientCableTemperatureSolver::Validate(
    const bool& is_included_warnings,
    std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "TRANSIENT CABLE TEMPERATURE SOLVER";

  // validates current-steady
  if (current_steady_ < 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid steady-state electrical current";
      messages->push_back(message);
    }
  }

  // validates current-step
  if (current_step_ < 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid step electrical current";
      messages->push_back(message);
    }
  }

  // validates duration
  if (duration_ < 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid time duration";
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
  }

  return is_valid;
}

const ThermalRatingCable* TransientCableTemperatureSolver::cable() const {
  return cable_;
}

double TransientCableTemperatureSolver::current_steady() const {
  return current_steady_;
}

double TransientCableTemperatureSolver::current_step() const {
  return current_step_;
}

int TransientCableTemperatureSolver::duration() const {
  return duration_;
}

void TransientCableTemperatureSolver::set_cable(
    const ThermalRatingCable* cable) {
  cable_ = cable;
  is_updated_ = false;
}

void TransientCableTemperatureSolver::set_current_steady(
    const double& current_steady) {
  current_steady_ = current_steady;
  is_updated_ = false;
}

void TransientCableTemperatureSolver::set_current_step(
    const double& current_step) {
  current_step_ = current_step;
  is_updated_ = false;
}

void TransientCableTemperatureSolver::set_duration(const int& duration) {
  duration_ = duration;
  is_updated_ = false;
}

void TransientCableTemperatureSolver::set_units(
    const units::UnitSystem& units) {
  units_ = units;
  is_updated_ = false;
}

void TransientCableTemperatureSolver::set_weather(
    const ThermalRatingWeather* weather) {
  weather_ = weather;
  is_updated_ = false;
}

units::UnitSystem TransientCableTemperatureSolver::units() const {
  return units_;
}

const ThermalRatingWeather* TransientCableTemperatureSolver::weather() const {
  return weather_;
}

CableHeatTransferState TransientCableTemperatureSolver::HeatTransferState(
    const double& current,
    const double& temperature) const {
  CableHeatTransferState state;
  state.heat_convection = solver_.HeatConvection(temperature);
  state.heat_radiation = solver_.HeatRadiation(temperature);
  state.heat_resistance = solver_.HeatResistance(temperature, current);
  state.heat_solar = solver_.HeatSolar();
  state.heat_storage = solver_.HeatStored(
      state.heat_resistance + state.heat_solar,
      state.heat_convection + state.heat_radiation);

  return state;
}

bool TransientCableTemperatureSolver::IsUpdated() const {
  return is_updated_ == true;
}

double TransientCableTemperatureSolver::TemperatureNew(
    const double& temperature,
    const CableHeatTransferState& state,
    const int& time_delta) const {
  // gets the entire cable heat capacity
  const double capacity_heat = *cable_->component_core()->capacity_heat()
                               + *cable_->component_shell()->capacity_heat();

  // calculates temperature delta
  const double temperature_delta =
      time_delta * (state.heat_storage / capacity_heat);

  // combines and returns
  return temperature + temperature_delta;
}

double TransientCableTemperatureSolver::TemperatureSteady(
    const double& current) const {
  SteadyCableTemperatureSolver solver;
  solver.set_cable(cable_);
  solver.set_current(current);
  solver.set_units(units_);
  solver.set_weather(weather_);

  return solver.TemperatureCable();
}

bool TransientCableTemperatureSolver::Update() const {
  // solves for the cable temperature
  is_updated_ = UpdateTemperaturePoints();
  if (is_updated_ == false) {
    return false;
  }

  // if it reaches this point, update was successful
  return true;
}

bool TransientCableTemperatureSolver::UpdateTemperaturePoints() const {
  // initializes points
  points_temperature_.clear();

  // initializes heat transfer solver
  solver_.set_cable(cable_);
  solver_.set_units(units_);
  solver_.set_weather(weather_);
  if (solver_.Validate() == false) {
    return false;
  }

  // creates temperature points
  const TemperaturePoint* point_prev = nullptr;
  int time = 0;

  while (time <= duration_) {
    // creates a new temperature point
    TemperaturePoint point;
    point.time = time;

    // solves for temperature
    if (time == 0) {
      // calculates steady-state temperature
      point.temperature = TemperatureSteady(current_steady_);
    } else {
      // solves for the heat transfer state
      CableHeatTransferState state = HeatTransferState(current_step_,
                                                       point_prev->temperature);

      // calculates transient temperature
      point.temperature = TemperatureNew(point_prev->temperature, state, 1);
    }

    // adds to point list
    points_temperature_.push_back(point);

    // increments
    point_prev = &points_temperature_.back();
    time++;
  }

  return true;
}
