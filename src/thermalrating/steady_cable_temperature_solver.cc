// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/thermalrating/steady_cable_temperature_solver.h"

#include <cmath>

#include "models/base/point.h"
#include "models/thermalrating/steady_cable_current_solver.h"

SteadyCableTemperatureSolver::SteadyCableTemperatureSolver() {
  cable_ = nullptr;
  current_ = -999999;
  units_ = units::UnitSystem::kNull;
  weather_ = nullptr;

  state_ = CableHeatTransferState();
  temperature_cable_ = -999999;

  is_updated_ = false;
}

SteadyCableTemperatureSolver::~SteadyCableTemperatureSolver() {
}

CableHeatTransferState SteadyCableTemperatureSolver::StateHeatTransfer() const {
  // updates class if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return CableHeatTransferState();
    }
  }

  return state_;
}

double SteadyCableTemperatureSolver::TemperatureCable() const {
  // updates class if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return temperature_cable_;
}

bool SteadyCableTemperatureSolver::Validate(
    const bool& is_included_warnings,
    std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "STEADY CABLE TEMPERATURE SOLVER";

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

  // validates current
  if (current_ < 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid electrical current";
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

const ThermalRatingCable* SteadyCableTemperatureSolver::cable() const {
  return cable_;
}

double SteadyCableTemperatureSolver::current() const {
  return current_;
}

void SteadyCableTemperatureSolver::set_cable(const ThermalRatingCable* cable) {
  cable_ = cable;
  is_updated_ = false;
}

void SteadyCableTemperatureSolver::set_current(const double& current) {
  current_ = current;
  is_updated_ = false;
}

void SteadyCableTemperatureSolver::set_units(const units::UnitSystem& units) {
  units_ = units;
}

void SteadyCableTemperatureSolver::set_weather(
    const ThermalRatingWeather* weather) {
  weather_ = weather;
}

units::UnitSystem SteadyCableTemperatureSolver::units() const {
  return units_;
}

const ThermalRatingWeather* SteadyCableTemperatureSolver::weather() const {
  return weather_;
}

bool SteadyCableTemperatureSolver::IsUpdated() const {
  if (is_updated_ == true) {
    return true;
  } else {
    return false;
  }
}

bool SteadyCableTemperatureSolver::SolveTemperatureAndState() const {
  // creates a steady-state cable current solver
  SteadyCableCurrentSolver solver;
  solver.set_cable(cable_);
  solver.set_units(units_);
  solver.set_weather(weather_);

  // x = cable temperature
  // y = current

  // initializes target
  double target_solution = current_;

  // declares and initializes left point
  solver.set_temperature_cable(weather_->temperature_air);

  Point2d<double> point_left;
  point_left.x = solver.temperature_cable();
  point_left.y = solver.Current();

  // declares and initializes right point
  solver.set_temperature_cable(weather_->temperature_air + 10);

  Point2d<double> point_right;
  point_right.x = solver.temperature_cable();
  point_right.y = solver.Current();

  // declares current point
  Point2d<double> point_current;

  // iterates
  unsigned int iter = 0;
  const int iter_max = 100;
  while (0.1 < std::abs(point_left.x - point_right.x) && (iter <= iter_max)) {

    // gets current point x value using left and right points
    // calculates tangent line between points, extrapolates using line
    double slope_line = (point_right.y - point_left.y)
                        / (point_right.x - point_left.x);

    point_current.x = point_left.x
                      + ((target_solution - point_left.y) / slope_line);

    // gets current point y value
    solver.set_temperature_cable(point_current.x);
    point_current.y = solver.Current();

    // updates either left or right point based on current point
    if (point_current.x < point_left.x) {

      point_right.x = point_left.x;
      point_right.y = point_left.y;
      point_left.x = point_current.x;
      point_left.y = point_current.y;

    } else if ((point_left.x < point_current.x)
        && (point_current.x < point_right.x)) {

      if (point_current.y < target_solution) {
        point_right.x = point_current.x;
        point_right.y = point_current.y;
      } else if (target_solution < point_current.y) {
        point_left.x = point_current.x;
        point_left.y = point_current.y;
      }

    } else if (point_right.x < point_current.x) {

      point_left.x = point_right.x;
      point_left.y = point_right.y;
      point_right.x = point_current.x;
      point_right.y = point_current.y;
    }
    iter++;
  }

  // caches results and returns success status
  if (iter < iter_max) {
    state_ = solver.StateHeatTransfer();
    temperature_cable_ = point_current.x;
    return true;
  } else {
    state_ = CableHeatTransferState();
    temperature_cable_ = -999999;
    return false;
  }
}

bool SteadyCableTemperatureSolver::Update() const {
  // solves for the cable temperature
  is_updated_ = SolveTemperatureAndState();
  if (is_updated_ == false) {
    return false;
  }

  // if it reaches this point, update was successful
  return true;
}
