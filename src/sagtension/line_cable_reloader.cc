// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/sagtension/line_cable_reloader.h"

#include "models/sagtension/catenary_cable_reloader.h"
#include "models/sagtension/line_cable_to_catenary_cable_converter.h"
#include "models/transmissionline/cable_unit_load_calculator.h"

LineCableReloader::LineCableReloader() {

  case_reloaded_ = nullptr;
  case_stretch_ = nullptr;
  line_cable_ = nullptr;

  is_updated_catenarycable_constraint_ = false;
  is_updated_load_stretch_ = false;
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

double LineCableReloader::LoadStretch() const {

  // updates class if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return load_stretch_;
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

  // validates case-reloaded
  if (case_reloaded_ == nullptr) {
    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("LINE CABLE RELOADER - Invalid reloaded case");
    }
  } else {
    if (case_reloaded_->Validate(is_included_warnings,
                                 messages_error) == false) {
      is_valid = false;
    }
  }

  // validates loadcase-stretch
  if (case_stretch_ == nullptr) {
    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("LINE CABLE RELOADER - Invalid stretch case");
    }
  } else {
    if (case_stretch_->Validate(is_included_warnings,
                                messages_error) == false) {
      is_valid = false;
    }
  }

  // validates line cable
  if (line_cable_ == nullptr) {
    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("LINE CABLE RELOADER - Invalid lien cable");
    }
  } else {
    if (line_cable_->Validate(is_included_warnings, messages_error) == false) {
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

const WeatherLoadCase* LineCableReloader::case_reloaded() const {
  return case_reloaded_;
}

const WeatherLoadCase* LineCableReloader::case_stretch() const {
  return case_stretch_;
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

void LineCableReloader::set_case_reloaded(
    const WeatherLoadCase* case_reloaded) {

  case_reloaded_ = case_reloaded;

  is_updated_catenarycable_reloaded_ = false;
}

void LineCableReloader::set_case_stretch(const WeatherLoadCase* case_stretch) {

  case_stretch_ = case_stretch;

  is_updated_catenarycable_constraint_ = false;
  is_updated_load_stretch_ = false;
  is_updated_catenarycable_reloaded_ = false;
}

void LineCableReloader::set_condition_reloaded(
    const CableConditionType& condition_reloaded) {

  condition_reloaded_ = condition_reloaded;

  is_updated_load_stretch_ = false;
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

  is_updated_catenarycable_constraint_ = false;
  is_updated_load_stretch_ = false;
  is_updated_catenarycable_reloaded_ = false;
}

void LineCableReloader::set_type_stretch(
    const CableConditionType& type_stretch) {

  type_stretch_ = type_stretch;

  is_updated_catenarycable_constraint_ = false;
  is_updated_load_stretch_ = false;
  is_updated_catenarycable_reloaded_ = false;
}

CableConditionType LineCableReloader::type_stretch() const {
  return type_stretch_;
}

bool LineCableReloader::IsUpdated() const {

  if ((is_updated_catenarycable_constraint_ == true)
      && (is_updated_load_stretch_ == true)
      && (is_updated_catenarycable_reloaded_ == true)) {
    return true;
  } else {
    return false;
  }
}

Vector3d LineCableReloader::UnitLoad(
    const WeatherLoadCase& case_weather) const {

  CableUnitLoadCalculator calculator;
  calculator.set_diameter_cable(&line_cable_->cable->diameter);
  calculator.set_weight_unit_cable(&line_cable_->cable->weight_unit);

  return calculator.UnitCableLoad(case_weather);
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
  if (is_updated_load_stretch_ == false) {
    is_updated_load_stretch_ = UpdateLoadStretch();
    if (is_updated_load_stretch_ == false) {
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
  LineCableToCatenaryCableConverter converter;
  converter.set_case_stretch(case_stretch_);
  converter.set_line_cable(line_cable_);
  converter.set_type_stretch(type_stretch_);

  if (converter.Validate() == false) {
    return false;
  }

  // gets catenary cable for line cable constraint
  catenarycable_constraint_ = converter.GetCatenaryCable();
  return true;
}

bool LineCableReloader::UpdateLoadStretch() const {

  // checks if stretch does not need to be solved for
  if (condition_reloaded_ == CableConditionType::kInitial) {
    load_stretch_ = 0;
    return true;
  } else if (condition_reloaded_ == line_cable_->constraint.condition) {
    load_stretch_ = catenarycable_constraint_.state()->load_stretch;
    return true;
  }

  // builds stretch state
  CableState state_stretch;
  state_stretch.load_stretch = 0;
  state_stretch.temperature = case_stretch_->temperature_cable;
  state_stretch.temperature_stretch = 0;

  // gets stretch unit weight
  Vector3d weight_unit_stretch = UnitLoad(*case_stretch_);

  // reloads the constraint catenary cable to the stretch case
  CatenaryCableReloader reloader;
  reloader.set_catenary_cable(&catenarycable_constraint_);
  //reloader.set_length_unloaded_unstretched_adjustment(0);
  reloader.set_state_reloaded(state_stretch);
  reloader.set_weight_unit_reloaded(weight_unit_stretch);

  // gets reloaded catenary cable and updates reloaded state based on the
  // amount the cable is stretched
  load_stretch_ = reloader.CatenaryCableReloaded().TensionAverage();

  return true;
}

bool LineCableReloader::UpdateReloadedCatenaryCable() const {

  // builds reloaded state
  CableState state_reloaded;
  state_reloaded.load_stretch = load_stretch_;
  state_reloaded.temperature = case_reloaded_->temperature_cable;
  state_reloaded.temperature_stretch = case_stretch_->temperature_cable;

  // gets reloaded unit weight
  Vector3d weight_unit_reloaded = UnitLoad(*case_reloaded_);

  // builds reloader and gets the reloaded catenary cable
  CatenaryCableReloader reloader;
  reloader.set_catenary_cable(&catenarycable_constraint_);
  //reloader.set_length_unloaded_unstretched_adjustment(0);
  reloader.set_state_reloaded(state_reloaded);
  reloader.set_weight_unit_reloaded(weight_unit_reloaded);

  catenarycable_reloaded_ = reloader.CatenaryCableReloaded();

  return true;
}
