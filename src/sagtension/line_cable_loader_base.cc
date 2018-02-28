// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/sagtension/line_cable_loader_base.h"

#include <cmath>

#include "models/sagtension/catenary_cable_reloader.h"
#include "models/transmissionline/cable_unit_load_solver.h"

LineCableLoaderBase::LineCableLoaderBase() {
  line_cable_ = nullptr;

  is_updated_catenary_constraint_ = false;
  is_updated_stretch_ = false;
}

LineCableLoaderBase::~LineCableLoaderBase() {
}

CableStretchState LineCableLoaderBase::StretchStateCreep() const {
  // updates class if necessary
  if ((IsUpdated() == false) && (Update() == false)) {
    return CableStretchState();
  }

  return state_stretch_creep_;
}

CableStretchState LineCableLoaderBase::StretchStateLoad() const {
  // updates class if necessary
  if ((IsUpdated() == false) && (Update() == false)) {
    return CableStretchState();
  }

  return state_stretch_load_;
}

bool LineCableLoaderBase::Validate(
    const bool& is_included_warnings,
    std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "LINE CABLE LOADER BASE";

  // validates cable-sagtension
  if (cable_sagtension_.Validate(is_included_warnings, messages) == false) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid sag-tension cable";
      messages->push_back(message);
    }
  }

  // validates line cable
  if (line_cable_ == nullptr) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid line cable";
      messages->push_back(message);
    }
  } else {
    if (line_cable_->Validate(is_included_warnings, messages) == false) {
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
      message.description = "";
      if (is_updated_catenary_constraint_ == false) {
        message.description = "Error updating class. Could not solve for "
                              "constraint catenary.";
      } else if (is_updated_stretch_ == false) {
        message.description = "Error updating class. Could not solve for "
                              "creep or load based stretch.";
      }
      messages->push_back(message);
    }
  }

  return is_valid;
}

const LineCable* LineCableLoaderBase::line_cable() const {
  return line_cable_;
}

void LineCableLoaderBase::set_line_cable(const LineCable* line_cable) {
  line_cable_ = line_cable;

  // updates sag-tension cable
  if (line_cable == nullptr) {
    cable_sagtension_.set_cable_base(nullptr);
  } else {
    cable_sagtension_.set_cable_base(line_cable_->cable());
  }

  is_updated_catenary_constraint_ = false;
  is_updated_stretch_ = false;
}

bool LineCableLoaderBase::InitializeLineCableModels() const {
  // declares working variable
  // all models are initialized with zero stretch
  CableState state;

  CableStretchState state_stretch;
  state_stretch.load = 0;

  // initializes the constraint cable model
  CableConstraint constraint = line_cable_->constraint();
  state.temperature = constraint.case_weather->temperature_cable;
  state.type_polynomial = SagTensionCableComponent::PolynomialType::kLoadStrain;

  if (constraint.condition == CableConditionType::kInitial) {
    state_stretch.temperature = 0;  // doesn't matter, model is never stretched
    state_stretch.type_polynomial =
      SagTensionCableComponent::PolynomialType::kLoadStrain;
  } else if (constraint.condition == CableConditionType::kCreep) {
    state_stretch.temperature =
      line_cable_->weathercase_stretch_creep()->temperature_cable;
    state_stretch.type_polynomial =
      SagTensionCableComponent::PolynomialType::kCreep;
  } else if (constraint.condition == CableConditionType::kLoad) {
    state_stretch.temperature =
      line_cable_->weathercase_stretch_load()->temperature_cable;
    state_stretch.type_polynomial =
      SagTensionCableComponent::PolynomialType::kLoadStrain;
  } else {
    return false;
  }

  model_constraint_.set_cable(&cable_sagtension_);
  model_constraint_.set_state(state);
  model_constraint_.set_state_stretch(state_stretch);

  // updates the creep stretch model
  state.temperature =
      line_cable_->weathercase_stretch_creep()->temperature_cable;
  state.type_polynomial = SagTensionCableComponent::PolynomialType::kCreep;

  state_stretch.temperature = state.temperature;
  state_stretch.type_polynomial =
    SagTensionCableComponent::PolynomialType::kCreep;

  model_creep_.set_cable(&cable_sagtension_);
  model_creep_.set_state(state);
  model_creep_.set_state_stretch(state_stretch);

  // updates the load stretch model
  state.temperature =
      line_cable_->weathercase_stretch_load()->temperature_cable;
  state.type_polynomial = SagTensionCableComponent::PolynomialType::kLoadStrain;

  state_stretch.temperature = state.temperature;
  state_stretch.type_polynomial =
    SagTensionCableComponent::PolynomialType::kLoadStrain;

  model_load_.set_cable(&cable_sagtension_);
  model_load_.set_state(state);
  model_load_.set_state_stretch(state_stretch);

  return true;
}

bool LineCableLoaderBase::IsUpdated() const {
  return (is_updated_catenary_constraint_ == true)
      && (is_updated_stretch_ == true);
}

Vector3d LineCableLoaderBase::UnitLoad(
    const WeatherLoadCase& weathercase) const {
  CableUnitLoadSolver solver;
  solver.set_diameter_cable(&line_cable_->cable()->diameter);
  solver.set_weight_unit_cable(&line_cable_->cable()->weight_unit);

  return solver.UnitCableLoad(weathercase);
}

bool LineCableLoaderBase::Update() const {
  // updates constraint catenary
  is_updated_catenary_constraint_ = UpdateConstraintCatenary();
  if (is_updated_catenary_constraint_ == false) {
    return false;
  }

  InitializeLineCableModels();

  is_updated_catenary_constraint_ = UpdateConstraintCableModel();
  if (is_updated_catenary_constraint_ == false) {
    return false;
  }

  // updates load-stretch
  is_updated_stretch_ = UpdateLoadStretch();
  if (is_updated_stretch_ == false) {
    return false;
  }

  // if it reaches this point, update was successful
  return true;
}

/// This function checks the condition of the line cable constraint, and
/// determines if stretch is necessary to solve for. Solving for the stretch
/// requires an iterative approach. The line cable is reloaded from the
/// constraint cable model to the stretch cable model and loading. The solution
/// is found when the stretch load in the constraint cable model matches the
/// reloaded catenary average tension (which is reloaded at the stretch cable
/// model).
bool LineCableLoaderBase::UpdateConstraintCableModel() const {
  // checks if no stretch is defined (initial modulus)
  // constraint cable model was already initialized with zero stretch
  if (line_cable_->constraint().condition == CableConditionType::kInitial) {
    return true;
  }

  // x = stretch load
  // y = stretch load difference
  //     i.e.(reloaded catenary avg tension - constraint model stretch load)

  // iterative routine to determine solution
  // solution reached when y = 0
  const double target_solution = 0;

  // initializes the stretch state
  CableStretchState state_stretch = model_constraint_.state_stretch();

  // initializes the reloader
  Vector3d weight_unit_reloaded;
  CatenaryCableReloader reloader;
  reloader.set_catenary(&catenary_constraint_);
  reloader.set_model_reference(&model_constraint_);

  if (line_cable_->constraint().condition == CableConditionType::kCreep) {
    weight_unit_reloaded = UnitLoad(*line_cable_->weathercase_stretch_creep());

    reloader.set_model_reloaded(&model_creep_);
    reloader.set_weight_unit_reloaded(&weight_unit_reloaded);
  } else if (line_cable_->constraint().condition == CableConditionType::kLoad) {
    weight_unit_reloaded = UnitLoad(*line_cable_->weathercase_stretch_load());

    reloader.set_model_reloaded(&model_load_);
    reloader.set_weight_unit_reloaded(&weight_unit_reloaded);
  } else {
    // condition isn't recognized
    return false;
  }

  // initializes left point
  Point2d<double> point_left;
  point_left.x = 0;
  point_left.y = reloader.CatenaryReloaded().TensionAverage()
                 - state_stretch.load;

  // initializes right point
  Point2d<double> point_right;
  point_right.x = *cable_sagtension_.strength_rated();

  state_stretch.load = point_right.x;
  model_constraint_.set_state_stretch(state_stretch);
  reloader.set_model_reference(&model_constraint_);

  point_right.y = reloader.CatenaryReloaded().TensionAverage()
                  - state_stretch.load;

  // initializes current point
  Point2d<double> point_current;

  // iterates until target solution is reached
  unsigned int iter = 1;
  const int iter_max = 100;
  const double precision = 0.01;
  double slope_line = -999999;

  while ((precision < std::abs(point_current.y - target_solution))
      && (iter < iter_max)) {
    // solves for new stretch value is calculated for current point
    slope_line = (point_right.y - point_left.y)
                  / (point_right.x - point_left.x);
    point_current.x = point_left.x
        + ((target_solution - point_left.y) / slope_line);

    // updates constraint model and reloader
    state_stretch.load = point_current.x;
    model_constraint_.set_state_stretch(state_stretch);
    reloader.set_model_reference(&model_constraint_);

    // solves for new horizontal tension value for current point
    point_current.y = reloader.CatenaryReloaded().TensionAverage()
                      - state_stretch.load;

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

  // returns success status
  if (iter < iter_max) {
    return true;
  } else {
    // re-initializes constraint model stretch
    state_stretch.load = 0;
    model_constraint_.set_state_stretch(state_stretch);
    return false;
  }
}

bool LineCableLoaderBase::UpdateConstraintCatenary() const {
  Catenary3d catenary = line_cable_->CatenaryRulingSpan();
  if (catenary.Validate() == true) {
    catenary_constraint_ = catenary;
    return true;
  } else {
    return false;
  }
}

bool LineCableLoaderBase::UpdateLoadStretch() const {
  // initializes working variables
  CatenaryCableReloader reloader;
  reloader.set_catenary(&catenary_constraint_);
  reloader.set_model_reference(&model_constraint_);

  Vector3d weight_unit_stretch;
  const WeatherLoadCase* weathercase_stretch = nullptr;

  // solves the stretch due to creep
  if (line_cable_->constraint().condition == CableConditionType::kCreep) {
    // stretch has already been solved for during constraint model update
    state_stretch_creep_ = model_constraint_.state_stretch();
  } else {
    // solves for the unit load at the creep stretch case
    weathercase_stretch = line_cable_->weathercase_stretch_creep();
    weight_unit_stretch = UnitLoad(*weathercase_stretch);

    // reloads the constraint catenary to the creep stretch case
    reloader.set_model_reloaded(&model_creep_);
    reloader.set_weight_unit_reloaded(&weight_unit_stretch);

    if (reloader.Validate(false, nullptr) == false) {
      return false;
    } else {
      state_stretch_creep_.load = reloader.CatenaryReloaded().TensionAverage();
      state_stretch_creep_.temperature = model_creep_.state().temperature;
      state_stretch_creep_.type_polynomial =
          model_creep_.state().type_polynomial;
    }
  }

  // solves the stretch due to load
  if (line_cable_->constraint().condition == CableConditionType::kLoad) {
    // stretch has already been solved for during constraint model update
    state_stretch_load_ = model_constraint_.state_stretch();
  } else {
    // solves for the unit load at the load stretch case
    weathercase_stretch = line_cable_->weathercase_stretch_load();
    weight_unit_stretch = UnitLoad(*weathercase_stretch);

    // reloads the constraint catenary to the load stretch case
    reloader.set_model_reloaded(&model_load_);
    reloader.set_weight_unit_reloaded(&weight_unit_stretch);

    if (reloader.Validate(false, nullptr) == false) {
      return false;
    } else {
      state_stretch_load_.load = reloader.CatenaryReloaded().TensionAverage();
      state_stretch_load_.temperature = model_load_.state().temperature;
      state_stretch_load_.type_polynomial = model_load_.state().type_polynomial;
    }
  }

  return true;
}
