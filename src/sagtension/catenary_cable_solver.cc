// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/sagtension/catenary_cable_solver.h"

#include "models/transmissionline/cable_unit_load_calculator.h"
#include "models/transmissionline/catenary_solver.h"

CatenaryCableSolver::CatenaryCableSolver() {
  cable_ = nullptr;
  constraint_ = nullptr;
  spacing_attachments_ = nullptr;
  weathercase_stretch_creep_ = nullptr;
  weathercase_stretch_load_ = nullptr;

  is_updated_catenarycable_ = false;
}

CatenaryCableSolver::~CatenaryCableSolver() {
}

CatenaryCable CatenaryCableSolver::GetCatenaryCable() const {
  // updates class if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return CatenaryCable();
    }
  }

  return catenary_cable_;
}

bool CatenaryCableSolver::Validate(
    const bool& is_included_warnings,
    std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "CATENARY CABLE SOLVER";

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

  // validates constraint
  if (constraint_ == nullptr) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid constraint";
      messages->push_back(message);
    }
  } else {
    if (cable_->Validate(is_included_warnings, messages) == false) {
      is_valid = false;
    }
  }

  // validates spacing-attachments
  if (spacing_attachments_->x() <= 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid horizontal attachment spacing";
      messages->push_back(message);
    }
  }

  if (spacing_attachments_->y() != 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid attachment spacing";
      messages->push_back(message);
    }
  }

  if (2000 < abs(spacing_attachments_->z())) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid vertical attachment spacing";
      messages->push_back(message);
    }
  }

  // validates weathercase-stretch-creep
  if (weathercase_stretch_creep_ == nullptr) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid creep stretch weathercase";
      messages->push_back(message);
    }
  } else {
    if (weathercase_stretch_creep_->Validate(is_included_warnings,
                                             messages) == false) {
      is_valid = false;
    }
  }

  // validates weathercase-stretch-load
  if (weathercase_stretch_load_ == nullptr) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid load stretch weathercase";
      messages->push_back(message);
    }
  } else {
    if (weathercase_stretch_load_->Validate(is_included_warnings,
                                            messages) == false) {
      is_valid = false;
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

const SagTensionCable* CatenaryCableSolver::cable() const {
  return cable_;
}

const CableConstraint* CatenaryCableSolver::constraint() const {
  return constraint_;
}

void CatenaryCableSolver::set_cable(const SagTensionCable* cable) {
  cable_ = cable;
  is_updated_catenarycable_ = false;
}

void CatenaryCableSolver::set_constraint(const CableConstraint* constraint) {
  constraint_ = constraint;
  is_updated_catenarycable_ = false;
}

void CatenaryCableSolver::set_spacing_attachments(
    const Vector3d* spacing_attachments) {
  spacing_attachments_ = spacing_attachments;
  is_updated_catenarycable_ = false;
}

void CatenaryCableSolver::set_weathercase_stretch_creep(
    const WeatherLoadCase* weathercase_stretch_creep) {
  weathercase_stretch_creep_ = weathercase_stretch_creep;
  is_updated_catenarycable_ = false;
}

void CatenaryCableSolver::set_weathercase_stretch_load(
    const WeatherLoadCase* weathercase_stretch_load) {
  weathercase_stretch_load_ = weathercase_stretch_load;
  is_updated_catenarycable_ = false;
}

const WeatherLoadCase* CatenaryCableSolver::weathercase_stretch_creep() const {
  return weathercase_stretch_creep_;
}

const WeatherLoadCase* CatenaryCableSolver::weathercase_stretch_load() const {
  return weathercase_stretch_load_;
}

double CatenaryCableSolver::LoadStretchDifference(
    const double& load_stretch) const {
  // updates catenary cable and reloader with new stretch load
  UpdateCatenaryCableState(load_stretch);
  reloader_.set_catenary_cable(&catenary_cable_);

  // reloads the catenary cable at the stretch load case
  const CatenaryCable catenary_cable_reloaded =
      reloader_.CatenaryCableReloaded();

  const double tension_average_reloaded =
      catenary_cable_reloaded.TensionAverage();

  // calculates difference between reloaded and defined stretch load
  return tension_average_reloaded - load_stretch;
}

bool CatenaryCableSolver::InitializeReloader() const {
  const WeatherLoadCase* weathercase_stretch = nullptr;
  if (constraint_->condition == CableConditionType::kCreep) {
    weathercase_stretch = weathercase_stretch_creep_;
  } else if (constraint_->condition == CableConditionType::kLoad) {
    weathercase_stretch = weathercase_stretch_load_;
  }

  // builds reloaded state
  CableState state_reloaded;
  state_reloaded.load_stretch = 0;
  state_reloaded.temperature = weathercase_stretch->temperature_cable;
  state_reloaded.temperature_stretch = 0;
  if (constraint_->condition == CableConditionType::kCreep) {
    state_reloaded.type_polynomial =
        SagTensionCableComponent::PolynomialType::kCreep;
  } else if (constraint_->condition == CableConditionType::kLoad) {
    state_reloaded.type_polynomial =
        SagTensionCableComponent::PolynomialType::kLoadStrain;
  } else {
    return false;
  }

  // solves for reloaded unit weight
  CableUnitLoadCalculator calculator_unitload;
  calculator_unitload.set_diameter_cable(cable_->diameter());
  calculator_unitload.set_weight_unit_cable(cable_->weight_unit());
  const Vector3d weight_unit_reloaded =
      calculator_unitload.UnitCableLoad(*weathercase_stretch);

  // initializes reloader
  reloader_.set_state_reloaded(state_reloaded);
  reloader_.set_weight_unit_reloaded(weight_unit_reloaded);

  return true;
}

bool CatenaryCableSolver::IsUpdated() const {

  if (is_updated_catenarycable_ == true) {
    return true;
  } else {
    return false;
  }
}

/// This function is an iterative solution that reloads the section cable at
/// the stretch load case until the load of the reloaded catenary matches the
/// stretch load that defines it.
bool CatenaryCableSolver::SolveStateLoadStretch() const {

  // x = stretch load
  // y = stretch load difference
  //     i.e.(reloaded stretch load - defined stretch load)

  // iterative routine to determine solution
  // solution reached when y = 0
  const double target_solution = 0;

  // initializes the reloader
  InitializeReloader();

  // initializes left point
  Point2d point_left;
  point_left.x = 0;
  point_left.y = LoadStretchDifference(point_left.x);

  // initializes right point
  Point2d point_right;
  point_right.x = *catenary_cable_.cable()->strength_rated();
  point_right.y = LoadStretchDifference(point_right.x);

  // initializes current point
  Point2d point_current;

  // iterates until target solution is reached
  unsigned int iter = 1;
  const int iter_max = 100;
  const double precision = 0.1;
  double slope_line = -999999;

  while ((precision < abs(point_current.y - target_solution))
      && (iter < iter_max)) {

    // solves for new stretch value is calculated for current point
    slope_line = (point_right.y - point_left.y)
                  / (point_right.x - point_left.x);
    point_current.x = point_left.x
        + ((target_solution - point_left.y) / slope_line);

    // solves for new horizontal tension value for current point
    point_current.y = LoadStretchDifference(point_current.x);

    // current point is left of left/right points
    if (point_current.x < point_left.x) {

      point_right.x = point_left.x;
      point_right.y = point_left.y;
      point_left.x = point_current.x;
      point_left.y = point_current.y;

    // current point is between left/right points
    } else if ((point_left.x <= point_current.x)
        && (point_current.x <= point_right.x)) {

      if (point_current.y < target_solution) {
        point_right.x = point_current.x;
        point_right.y = point_current.y;
      } else if (target_solution < point_current.y) {
        point_left.x = point_current.x;
        point_left.y = point_current.y;
      }

    // current point is right of left/right points
    } else if (point_right.x < point_current.x) {

      point_left.x = point_right.x;
      point_left.y = point_right.y;
      point_right.x = point_current.x;
      point_right.y = point_current.y;
    }

    iter++;
  }

  // returns success status and caches result
  CableState state = *catenary_cable_.state();
  if (iter < iter_max) {
    state.load_stretch = point_current.x;
    catenary_cable_.set_state(state);
    return true;
  } else {
    state.load_stretch = -999999;
    catenary_cable_.set_state(state);
    return false;
  }
}

bool CatenaryCableSolver::Update() const {

  // updates catenary cable
  if (is_updated_catenarycable_ == false) {
    is_updated_catenarycable_ = UpdateCatenaryCable();
    if (is_updated_catenarycable_ == false) {
      return false;
    }
  }

  // if it reaches this point, update was successful
  return true;
}

bool CatenaryCableSolver::UpdateCatenaryCable() const {
  // updates catenary
  CatenarySolver solver;
  solver.set_cable(cable_->cable_base());
  solver.set_constraint(constraint_);
  solver.set_spacing_attachments(spacing_attachments_);

  if (solver.Validate(false, nullptr) == false) {
    return false;
  }

  Catenary3d catenary = solver.Catenary();

  catenary_cable_.set_spacing_endpoints(catenary.spacing_endpoints());
  catenary_cable_.set_tension_horizontal(catenary.tension_horizontal());
  catenary_cable_.set_weight_unit(catenary.weight_unit());

  // updates cable
  catenary_cable_.set_cable(cable_);

  // updates state
  CableState state;
  state.temperature = constraint_->case_weather->temperature_cable;
  state.type_polynomial = SagTensionCableComponent::PolynomialType::kLoadStrain;
  if (constraint_->condition == CableConditionType::kInitial) {
    state.temperature_stretch = 0;
  } else if (constraint_->condition == CableConditionType::kCreep) {
    state.temperature_stretch = weathercase_stretch_creep_->temperature_cable;
  } else if (constraint_->condition == CableConditionType::kLoad) {
    state.temperature_stretch = weathercase_stretch_load_->temperature_cable;
  } else {
    return false;
  }

  catenary_cable_.set_state(state);

  if (constraint_->condition == CableConditionType::kInitial) {
    state.load_stretch = 0;
    catenary_cable_.set_state(state);
  } else {
    if (SolveStateLoadStretch() == false) {
      return false;
    }
  }

  return true;
}

bool CatenaryCableSolver::UpdateCatenaryCableState(
    const double& load_stretch) const {

  // extracts catenary cable state, modifies, and updates
  CableState state = *catenary_cable_.state();
  state.load_stretch = load_stretch;
  catenary_cable_.set_state(state);

  return true;
}
