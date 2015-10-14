// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/sagtension/line_cable_reloader.h"

#include "models/sagtension/catenary_cable_reloader.h"
#include "models/sagtension/catenary_cable_solver.h"
#include "models/transmissionline/cable_unit_load_calculator.h"

LineCableReloader::LineCableReloader() {
  line_cable_ = nullptr;
  load_stretch_creep_ = -999999;
  load_stretch_load_ = -999999;
  weathercase_reloaded_ = nullptr;

  is_updated_catenarycable_constraint_ = false;
  is_updated_stretch_ = false;
  is_updated_catenarycable_reloaded_ = false;
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

  Catenary3d catenary;
  catenary.set_spacing_endpoints(catenarycable_reloaded_.spacing_endpoints());
  catenary.set_tension_horizontal(catenarycable_reloaded_.tension_horizontal());
  catenary.set_weight_unit(catenarycable_reloaded_.weight_unit());
  return catenary;
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

  return solver_component_tension_.TensionAverageComponent(type_component);
}

double LineCableReloader::TensionHorizontal() const {
  // updates class if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return catenarycable_reloaded_.tension_horizontal();
}

double LineCableReloader::TensionHorizontalComponent(
    const CableElongationModel::ComponentType& type_component) const {
  // updates class if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return solver_component_tension_.TensionHorizontalComponent(type_component);
}

bool LineCableReloader::Validate(
    const bool& is_included_warnings,
    std::list<std::string>* messages_error) const {
  // initializes
  bool is_valid = true;

  // validates cable-sagtension
  if (cable_sagtension_.Validate(is_included_warnings,
                                 messages_error) == false) {
    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("LINE CABLE RELOADER - Invalid sag-tension "
                                "cable");
    }
  }

  // validates condition-reloaded
  if ((condition_reloaded_ != CableConditionType::kCreep)
      && (condition_reloaded_ != CableConditionType::kInitial)
      && (condition_reloaded_ != CableConditionType::kLoad)) {
    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("LINE CABLE RELAODER - Invalid reloaded "
                                "condition");
    }
  }

  // validates line cable
  if (line_cable_ == nullptr) {
    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("LINE CABLE RELOADER - Invalid line cable");
    }
  } else {
    if (line_cable_->Validate(is_included_warnings, messages_error) == false) {
      is_valid = false;
    }
  }

  // validates weathercase-reloaded
  if (weathercase_reloaded_ == nullptr) {
    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("LINE CABLE RELOADER - Invalid reloaded case");
    }
  } else {
    if (weathercase_reloaded_->Validate(is_included_warnings,
                                 messages_error) == false) {
      is_valid = false;
    }
  }

  // further validates
  if (is_valid == true) {

    // validates update process
    if (Update() == false) {
      is_valid = false;
      if (messages_error != nullptr) {
        messages_error->push_back("LINE CABLE RELOADER - Error updating class");
      }
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

  is_updated_catenarycable_reloaded_ = false;
}

void LineCableReloader::set_condition_reloaded(
    const CableConditionType& condition_reloaded) {
  condition_reloaded_ = condition_reloaded;

  is_updated_catenarycable_reloaded_ = false;
}

void LineCableReloader::set_length_unloaded_unstretched_adjustment(
    const double& length_unloaded_unstretched_adjustment) {
  length_unloaded_unstretched_adjustment_ =
      length_unloaded_unstretched_adjustment;

  is_updated_catenarycable_reloaded_ = false;
}

void LineCableReloader::set_line_cable(const LineCable* line_cable) {
  line_cable_ = line_cable;
  cable_sagtension_.set_cable_base(line_cable_->cable);

  is_updated_catenarycable_constraint_ = false;
  is_updated_stretch_ = false;
  is_updated_catenarycable_reloaded_ = false;
}

const WeatherLoadCase* LineCableReloader::weathercase_reloaded() const {
  return weathercase_reloaded_;
}

bool LineCableReloader::IsUpdated() const {
  if ((is_updated_catenarycable_constraint_ == true)
      && (is_updated_stretch_ == true)
      && (is_updated_catenarycable_reloaded_ == true)) {
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
  // updates constraint catenary-cable
  if (is_updated_catenarycable_constraint_ == false) {
    is_updated_catenarycable_constraint_ = UpdateConstraintCatenaryCable();
    if (is_updated_catenarycable_constraint_ == false) {
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

  // updates reloaded catenary-cable
  if (is_updated_catenarycable_reloaded_ == false) {
    is_updated_catenarycable_reloaded_ = UpdateReloadedCatenaryCable();
    if (is_updated_catenarycable_reloaded_ == false) {
      return false;
    }

    is_updated_catenarycable_reloaded_ =
        UpdateCatenaryCableComponentTensionSolver();
    if (is_updated_catenarycable_reloaded_ == false) {
      return false;
    }
  }

  // if it reaches this point, update was successful
  return true;
}

bool LineCableReloader::UpdateCatenaryCableComponentTensionSolver() const {
  solver_component_tension_.set_catenary_cable(&catenarycable_reloaded_);
  return true;
}

bool LineCableReloader::UpdateConstraintCatenaryCable() const {
  // builds a line cable to catenary cable converter
  CatenaryCableSolver solver;
  solver.set_cable(&cable_sagtension_);
  solver.set_constraint(&line_cable_->constraint);
  solver.set_spacing_attachments(&line_cable_->spacing_attachments_ruling_span);
  solver.set_weathercase_stretch_creep(line_cable_->weathercase_stretch_creep);
  solver.set_weathercase_stretch_load(line_cable_->weathercase_stretch_load);

  if (solver.Validate() == false) {
    return false;
  } else {
    catenarycable_constraint_ = solver.GetCatenaryCable();
    return true;
  }
}

bool LineCableReloader::UpdateLoadStretch() const {
  // variables used for solving stretch
  CableState state_stretch;
  Vector3d weight_unit_stretch;
  const WeatherLoadCase* weathercase_stretch = nullptr;

  // solves the stretch due to creep
  if (line_cable_->constraint.condition == CableConditionType::kCreep) {
    // creep stretch has been solved for in constraint catenary
    load_stretch_creep_ = catenarycable_constraint_.state()->load_stretch;
  } else {
    // reloads the constraint catenary cable to the creep stretch case
    weathercase_stretch = line_cable_->weathercase_stretch_creep;

    state_stretch.load_stretch = 0;
    state_stretch.temperature = weathercase_stretch->temperature_cable;
    state_stretch.temperature_stretch = weathercase_stretch->temperature_cable;
    state_stretch.type_polynomial =
        SagTensionCableComponent::PolynomialType::kCreep;

    weight_unit_stretch = UnitLoad(*weathercase_stretch);

    /// \todo this does not use the proper loader
    ///   Returning the average tension of the sag-tension solution will not
    ///   determine the amount of creep-stretch the cable has experienced.
    CatenaryCableReloader reloader;
    reloader.set_catenary_cable(&catenarycable_constraint_);
    reloader.set_state_reloaded(state_stretch);
    reloader.set_weight_unit_reloaded(weight_unit_stretch);

    if (reloader.Validate(false, nullptr) == false) {
      return false;
    } else {
      load_stretch_creep_ = reloader.CatenaryCableReloaded().TensionAverage();
    }
  }

  // solves the stretch due to load
  if (line_cable_->constraint.condition == CableConditionType::kLoad) {
    // creep stretch has been solved for in constraint catenary
    load_stretch_creep_ = catenarycable_constraint_.state()->load_stretch;
  } else {
    // reloads the constraint catenary cable to the load stretch case
    weathercase_stretch = line_cable_->weathercase_stretch_load;

    state_stretch.load_stretch = 0;
    state_stretch.temperature = weathercase_stretch->temperature_cable;
    state_stretch.temperature_stretch = weathercase_stretch->temperature_cable;
    state_stretch.type_polynomial =
        SagTensionCableComponent::PolynomialType::kLoadStrain;

    weight_unit_stretch = UnitLoad(*weathercase_stretch);

    CatenaryCableReloader reloader;
    reloader.set_catenary_cable(&catenarycable_constraint_);
    reloader.set_state_reloaded(state_stretch);
    reloader.set_weight_unit_reloaded(weight_unit_stretch);

    if (reloader.Validate(false, nullptr) == false) {
      return false;
    } else {
      load_stretch_load_ = reloader.CatenaryCableReloaded().TensionAverage();
    }
  }

  return true;
}

bool LineCableReloader::UpdateReloadedCatenaryCable() const {
  // builds reloaded state
  CableState state_reloaded;
  state_reloaded.temperature = weathercase_reloaded_->temperature_cable;

  if (condition_reloaded_ == CableConditionType::kInitial) {
    state_reloaded.load_stretch = 0;
    state_reloaded.temperature_stretch = 0;
    state_reloaded.type_polynomial =
        SagTensionCableComponent::PolynomialType::kLoadStrain;
  } else if (condition_reloaded_ == CableConditionType::kCreep) {
    state_reloaded.load_stretch = load_stretch_creep_;
    state_reloaded.temperature_stretch =
        line_cable_->weathercase_stretch_creep->temperature_cable;
    state_reloaded.type_polynomial =
        SagTensionCableComponent::PolynomialType::kCreep;
  } else if (condition_reloaded_ == CableConditionType::kLoad) {
    state_reloaded.load_stretch = load_stretch_load_;
    state_reloaded.temperature_stretch =
        line_cable_->weathercase_stretch_load->temperature_cable;
    state_reloaded.type_polynomial =
        SagTensionCableComponent::PolynomialType::kLoadStrain;
  }

  // gets reloaded unit weight
  Vector3d weight_unit_reloaded = UnitLoad(*weathercase_reloaded_);

  // builds reloader and gets the reloaded catenary cable
  CatenaryCableReloader reloader;
  reloader.set_catenary_cable(&catenarycable_constraint_);
  reloader.set_state_reloaded(state_reloaded);
  reloader.set_weight_unit_reloaded(weight_unit_reloaded);

  if (reloader.Validate(false, nullptr) == false) {
    return false;
  } else {
    catenarycable_reloaded_ = reloader.CatenaryCableReloaded();
    return true;
  }
}