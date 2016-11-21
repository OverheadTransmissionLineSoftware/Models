// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/sagtension/line_cable_reloader.h"

#include "models/sagtension/catenary_cable_reloader.h"
#include "models/transmissionline/catenary_solver.h"
#include "models/transmissionline/cable_unit_load_calculator.h"

LineCableReloader::LineCableReloader() {
  line_cable_ = nullptr;
  weathercase_reloaded_ = nullptr;

  is_updated_catenary_constraint_ = false;
  is_updated_stretch_ = false;
  is_updated_catenary_reloaded_ = false;
}

LineCableReloader::~LineCableReloader() {
}

Catenary3d LineCableReloader::CatenaryReloaded() const {
  // updates class if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return Catenary3d();
    }
  }

  return catenary_reloaded_;
}

double LineCableReloader::LoadStretch(
    const CableConditionType& condition) const {
  // updates class if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  if (condition == CableConditionType::kInitial) {
    return 0;
  } else if (condition == CableConditionType::kCreep) {
    return load_stretch_creep_;
  } else if (condition == CableConditionType::kLoad) {
    return load_stretch_load_;
  } else {
    return -999999;
  }
}

double LineCableReloader::TensionAverageComponent(
    const CableElongationModel::ComponentType& type_component) const {
  // updates class if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  // gets total strain
  const double strain_total =
      model_reloaded_.Strain(CableElongationModel::ComponentType::kCombined,
                             catenary_reloaded_.TensionAverage());

  return model_reloaded_.Load(type_component, strain_total);
}

double LineCableReloader::TensionHorizontal() const {
  // updates class if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return catenary_reloaded_.tension_horizontal();
}

/// The component horizontal tension cannot be directly solved for. It is
/// calculated by scaling the average component load by the ratio of the entire
/// cable horizontal tension and entire cable average load.
double LineCableReloader::TensionHorizontalComponent(
    const CableElongationModel::ComponentType& type_component) const {
  // updates class if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  // gets average tension of component
  const double tension_average_component =
      TensionAverageComponent(type_component);

  // uses ratio of average tension and horizontal tension to scale value
  return tension_average_component
      * (catenary_reloaded_.tension_horizontal()
      / catenary_reloaded_.TensionAverage());
}

bool LineCableReloader::Validate(
    const bool& is_included_warnings,
    std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "LINE CABLE RELOADER";

  // validates cable-sagtension
  if (cable_sagtension_.Validate(is_included_warnings,
                                 messages) == false) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid sag-tension cable";
      messages->push_back(message);
    }
  }

  // validates condition-reloaded
  if ((condition_reloaded_ != CableConditionType::kCreep)
      && (condition_reloaded_ != CableConditionType::kInitial)
      && (condition_reloaded_ != CableConditionType::kLoad)) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid reloaded condition";
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

  // validates weathercase-reloaded
  if (weathercase_reloaded_ == nullptr) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid reloaded weathercase";
      messages->push_back(message);
    }
  } else {
    if (weathercase_reloaded_->Validate(is_included_warnings,
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
      message.description = "";
      if (is_updated_catenary_constraint_ == false) {
        message.description = "Error updating class. Could not solve for "
                              "constraint catenary.";
      } else if (is_updated_stretch_ == false) {
        message.description = "Error updating class. Could not solve for "
                              "creep or load based stretch.";
      } else if (is_updated_catenary_reloaded_ == false) {
        message.description = "Error updating class. Could not solve for "
                              "reloaded catenary.";
      }
      messages->push_back(message);
    }
  }

  return is_valid;
}

CableConditionType LineCableReloader::condition_reloaded() const {
  return condition_reloaded_;
}

double LineCableReloader::length_unloaded_unstretched_adjustment() const {
  return length_unloaded_unstretched_adjustment_;
}

const LineCable* LineCableReloader::line_cable() const {
  return line_cable_;
}

void LineCableReloader::set_weathercase_reloaded(
    const WeatherLoadCase* case_reloaded) {
  weathercase_reloaded_ = case_reloaded;

  is_updated_catenary_reloaded_ = false;
}

void LineCableReloader::set_condition_reloaded(
    const CableConditionType& condition_reloaded) {
  condition_reloaded_ = condition_reloaded;

  is_updated_catenary_reloaded_ = false;
}

void LineCableReloader::set_length_unloaded_unstretched_adjustment(
    const double& length_unloaded_unstretched_adjustment) {
  length_unloaded_unstretched_adjustment_ =
      length_unloaded_unstretched_adjustment;

  is_updated_catenary_reloaded_ = false;
}

void LineCableReloader::set_line_cable(const LineCable* line_cable) {
  line_cable_ = line_cable;

  // updates sag-tension cable
  if (line_cable == nullptr) {
    cable_sagtension_.set_cable_base(nullptr);
  } else {
    cable_sagtension_.set_cable_base(line_cable_->cable);
  }

  is_updated_catenary_constraint_ = false;
  is_updated_stretch_ = false;
  is_updated_catenary_reloaded_ = false;
}

const WeatherLoadCase* LineCableReloader::weathercase_reloaded() const {
  return weathercase_reloaded_;
}

bool LineCableReloader::InitializeLineCableModels() const {
  // declare working variable
  // all models are initialized with zero stretch
  CableState state;
  state.load_stretch = 0;

  // initializes the constraint cable model
  state.temperature = line_cable_->constraint.case_weather->temperature_cable;

  if (line_cable_->constraint.condition == CableConditionType::kInitial) {
    state.temperature_stretch = 0;  // doesn't matter, model is never stretched
  } else if (line_cable_->constraint.condition == CableConditionType::kCreep) {
    state.temperature_stretch =
        line_cable_->weathercase_stretch_creep->temperature_cable;
  } else if (line_cable_->constraint.condition == CableConditionType::kLoad) {
    state.temperature_stretch =
        line_cable_->weathercase_stretch_load->temperature_cable;
  } else {
    state.temperature_stretch = -999999;
  }

  state.type_polynomial = SagTensionCableComponent::PolynomialType::kLoadStrain;

  model_constraint_.set_cable(&cable_sagtension_);
  model_constraint_.set_state(state);

  // updates the creep stretch model
  state.temperature = line_cable_->weathercase_stretch_creep->temperature_cable;
  state.temperature_stretch = 0;  // doesn't matter, model is never stretched
  state.type_polynomial = SagTensionCableComponent::PolynomialType::kCreep;

  model_creep_.set_cable(&cable_sagtension_);
  model_creep_.set_state(state);

  // updates the load stretch model
  state.temperature = line_cable_->weathercase_stretch_load->temperature_cable;
  state.temperature_stretch = 0;  // doesn't matter, model is never stretched
  state.type_polynomial = SagTensionCableComponent::PolynomialType::kLoadStrain;

  model_load_.set_cable(&cable_sagtension_);
  model_load_.set_state(state);

  return true;
}

bool LineCableReloader::IsUpdated() const {
  if ((is_updated_catenary_constraint_ == true)
      && (is_updated_stretch_ == true)
      && (is_updated_catenary_reloaded_ == true)) {
    return true;
  } else {
    return false;
  }
}

Vector3d LineCableReloader::UnitLoad(
    const WeatherLoadCase& weathercase) const {
  CableUnitLoadCalculator calculator;
  calculator.set_diameter_cable(&line_cable_->cable->diameter);
  calculator.set_weight_unit_cable(&line_cable_->cable->weight_unit);

  return calculator.UnitCableLoad(weathercase);
}

bool LineCableReloader::Update() const {
  // updates constraint catenary
  if (is_updated_catenary_constraint_ == false) {
    is_updated_catenary_constraint_ = UpdateConstraintCatenary();
    if (is_updated_catenary_constraint_ == false) {
      return false;
    }

    InitializeLineCableModels();

    is_updated_catenary_constraint_ = UpdateConstraintCableModel();
    if (is_updated_catenary_constraint_ == false) {
      return false;
    }
  }

  // updates load-stretch
  if (is_updated_stretch_ == false) {
    is_updated_stretch_ = UpdateLoadStretch();
    if (is_updated_stretch_ == false) {
      return false;
    }
  }

  // updates reloaded catenary
  if (is_updated_catenary_reloaded_ == false) {
    is_updated_catenary_reloaded_ = UpdateReloadedCableModel();
    if (is_updated_catenary_reloaded_ == false) {
      return false;
    }

    is_updated_catenary_reloaded_ = UpdateReloadedCatenary();
    if (is_updated_catenary_reloaded_ == false) {
      return false;
    }
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
bool LineCableReloader::UpdateConstraintCableModel() const {
  // checks if no stretch is defined (initial modulus)
  // constraint cable model was already initialized with zero stretch
  if (line_cable_->constraint.condition == CableConditionType::kInitial) {
    return true;
  }

  // x = stretch load
  // y = stretch load difference
  //     i.e.(reloaded stretch load - defined stretch load)

  // iterative routine to determine solution
  // solution reached when y = 0
  const double target_solution = 0;

  // initializes the state
  CableState state = model_constraint_.state();

  // initializes the reloader
  Vector3d weight_unit_reloaded;
  CatenaryCableReloader reloader;
  reloader.set_catenary(&catenary_constraint_);
  reloader.set_model_reference(&model_constraint_);

  if (line_cable_->constraint.condition == CableConditionType::kCreep) {
    return false;  // not yet supported
  } else if (line_cable_->constraint.condition == CableConditionType::kLoad) {
    weight_unit_reloaded = UnitLoad(*line_cable_->weathercase_stretch_load);

    reloader.set_model_reloaded(&model_load_);
    reloader.set_weight_unit_reloaded(&weight_unit_reloaded);
  } else {
    // condition isn't recognized
    return false;
  }

  // initializes left point
  Point2d point_left;
  point_left.x = 0;
  point_left.y = reloader.CatenaryReloaded().TensionAverage()
                 - state.load_stretch;

  // initializes right point
  Point2d point_right;
  point_right.x = *cable_sagtension_.strength_rated();

  state.load_stretch = point_right.x;
  model_constraint_.set_state(state);
  reloader.set_model_reference(&model_constraint_);

  point_right.y = reloader.CatenaryReloaded().TensionAverage()
                  - state.load_stretch;

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

    // updates constraint model and reloader
    state.load_stretch = point_current.x;
    model_constraint_.set_state(state);
    reloader.set_model_reference(&model_constraint_);

    // solves for new horizontal tension value for current point
    point_current.y = reloader.CatenaryReloaded().TensionAverage()
                      - state.load_stretch;

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
    state.load_stretch = 0;
    model_constraint_.set_state(state);
    return false;
  }
}

bool LineCableReloader::UpdateConstraintCatenary() const {
  // builds a catenary solver
  CatenarySolver solver;
  solver.set_cable(cable_sagtension_.cable_base());
  solver.set_constraint(&line_cable_->constraint);
  solver.set_spacing_attachments(&line_cable_->spacing_attachments_ruling_span);

  if (solver.Validate() == false) {
    return false;
  } else {
    catenary_constraint_ = solver.Catenary();
    return true;
  }
}

bool LineCableReloader::UpdateLoadStretch() const {
  // initializes working variables
  CatenaryCableReloader reloader;
  reloader.set_catenary(&catenary_constraint_);
  reloader.set_model_reference(&model_constraint_);

  Vector3d weight_unit_stretch;
  const WeatherLoadCase* weathercase_stretch = nullptr;

  // solves the stretch due to creep
  if (line_cable_->constraint.condition == CableConditionType::kCreep) {
    // stretch has already been solved for during constraint model update
    load_stretch_creep_ = model_constraint_.state().load_stretch;
  } else {
    // solves for the unit load at the creep stretch case
    weathercase_stretch = line_cable_->weathercase_stretch_creep;
    weight_unit_stretch = UnitLoad(*weathercase_stretch);

    // reloads the constraint catenary to the creep stretch case
    reloader.set_model_reloaded(&model_creep_);
    reloader.set_weight_unit_reloaded(&weight_unit_stretch);

    if (reloader.Validate(false, nullptr) == false) {
      return false;
    } else {
      load_stretch_creep_ = reloader.CatenaryReloaded().TensionAverage();
    }
  }

  // solves the stretch due to load
  if (line_cable_->constraint.condition == CableConditionType::kLoad) {
    // stretch has already been solved for during constraint model update
    load_stretch_load_ = model_constraint_.state().load_stretch;
  } else {
    // solves for the unit load at the load stretch case
    weathercase_stretch = line_cable_->weathercase_stretch_load;
    weight_unit_stretch = UnitLoad(*weathercase_stretch);

    // reloads the constraint catenary to the load stretch case
    reloader.set_model_reloaded(&model_load_);
    reloader.set_weight_unit_reloaded(&weight_unit_stretch);

    if (reloader.Validate(false, nullptr) == false) {
      return false;
    } else {
      load_stretch_load_ = reloader.CatenaryReloaded().TensionAverage();
    }
  }

  return true;
}

bool LineCableReloader::UpdateReloadedCableModel() const {
  // builds cable state based on reloaded weathercase and stretch
  CableState state;

  if (condition_reloaded_ == CableConditionType::kInitial) {
    // stretch parameters don't matter, model isn't stretched
    state.load_stretch = 0;
    state.temperature_stretch = 0;
  } else if (condition_reloaded_ == CableConditionType::kCreep) {
    state.load_stretch = load_stretch_creep_;
    state.temperature_stretch = model_creep_.state().temperature;
  } else if (condition_reloaded_ == CableConditionType::kLoad) {
    state.load_stretch = load_stretch_load_;
    state.temperature_stretch = model_load_.state().temperature;
  }

  state.temperature = weathercase_reloaded_->temperature_cable;
  state.type_polynomial = SagTensionCableComponent::PolynomialType::kLoadStrain;

  // updates model
  model_reloaded_.set_cable(&cable_sagtension_);
  model_reloaded_.set_state(state);

  return true;
}

bool LineCableReloader::UpdateReloadedCatenary() const {
  // calculates reloaded unit weight
  Vector3d weight_unit_reloaded = UnitLoad(*weathercase_reloaded_);

  // builds reloader and gets the calculates the reloaded catenary
  CatenaryCableReloader reloader;
  reloader.set_catenary(&catenary_constraint_);
  reloader.set_model_reference(&model_constraint_);
  reloader.set_model_reloaded(&model_reloaded_);
  reloader.set_weight_unit_reloaded(&weight_unit_reloaded);

  if (reloader.Validate(false, nullptr) == false) {
    return false;
  } else {
    catenary_reloaded_ = reloader.CatenaryReloaded();
    return true;
  }
}
