// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/sagtension/line_cable_to_catenary_cable_converter.h"

#include "models/transmissionline/cable_unit_load_calculator.h"
#include "models/transmissionline/line_cable_to_catenary_converter.h"

LineCableToCatenaryCableConverter::LineCableToCatenaryCableConverter() {
  case_stretch_ = nullptr;
  line_cable_ = nullptr;

  is_updated_catenarycable_ = false;
}

LineCableToCatenaryCableConverter::~LineCableToCatenaryCableConverter() {
}

CatenaryCable LineCableToCatenaryCableConverter::GetCatenaryCable() const {

  // updates class if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return CatenaryCable();
    }
  }

  return catenary_cable_;
}

bool LineCableToCatenaryCableConverter::Validate(
    const bool& is_included_warnings,
    std::list<std::string>* messages_error) const {

  // initializes
  bool is_valid = true;

  // validates case-stretch
  if (case_stretch_ == nullptr) {
    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("LINE CABLE TO CATENARY CABLE CONVERTER - "
                                "Invalid stretch case");
    }
  } else {
    if (case_stretch_->Validate(is_included_warnings, messages_error) == false) {
      is_valid = false;
    }
  }

  // validates line cable
  if (line_cable_ == nullptr) {
    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("LINE CABLE TO CATENARY CABLE CONVERTER - "
                                "Invalid line cable");
    }
  } else {
    if (line_cable_->Validate(is_included_warnings, messages_error) == false) {
      is_valid = false;
    }
  }

  // validates type-stretch
  if ((type_stretch_ != CableConditionType::kCreep)
      && (type_stretch_ != CableConditionType::kLoad)) {
    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("LINE CABLE TO CATENARY CABLE CONVERTER - "
                                "Invalid stretch type");
    }
  }

  // further validates
  if (is_valid == true) {

    // validates update process
    if (Update() == false) {
      is_valid = false;
      if (messages_error != nullptr) {
        messages_error->push_back("LINE CABLE TO CATENARY CABLE CONVERTER - "
                                  "Error updating class");
      }
    }
  }

  return is_valid;
}

const WeatherLoadCase* LineCableToCatenaryCableConverter::case_stretch() const {
  return case_stretch_;
}

const LineCable* LineCableToCatenaryCableConverter::line_cable() const {
  return line_cable_;
}

void LineCableToCatenaryCableConverter::set_line_cable(
    const LineCable* line_cable) {

  line_cable_ = line_cable;

  is_updated_catenarycable_ = false;
}

void LineCableToCatenaryCableConverter::set_case_stretch(
    const WeatherLoadCase* case_stretch) {

  case_stretch_ = case_stretch;

  CableState state;
  state.temperature_stretch = case_stretch->temperature_cable;
  catenary_cable_.set_state(state);

  is_updated_catenarycable_ = false;
}

void LineCableToCatenaryCableConverter::set_type_stretch(
    const CableConditionType& type_stretch) {

  type_stretch_ = type_stretch;

  is_updated_catenarycable_ = false;
}

CableConditionType LineCableToCatenaryCableConverter::type_stretch() const {
  return type_stretch_;
}

double LineCableToCatenaryCableConverter::LoadStretchDifference(
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

bool LineCableToCatenaryCableConverter::InitializeReloader() const {

  // builds state
  CableState state_reloader;
  state_reloader.load_stretch = 0;
  state_reloader.temperature = case_stretch_->temperature_cable;
  state_reloader.temperature_stretch = 0;

  // solves for reloaded unit weight
  CableUnitLoadCalculator calculator_unitload;
  calculator_unitload.set_diameter_cable(&line_cable_->cable->diameter);
  calculator_unitload.set_weight_unit_cable(&line_cable_->cable->weight_unit);
  const Vector3d weight_unit_reloaded =
      calculator_unitload.UnitCableLoad(*case_stretch_);

  // initializes reloader
  reloader_.set_state_reloaded(state_reloader);
  reloader_.set_weight_unit_reloaded(weight_unit_reloaded);

  return true;
}

bool LineCableToCatenaryCableConverter::IsUpdated() const {

  if (is_updated_catenarycable_ == true) {
    return true;
  } else {
    return false;
  }
}

/// This function is an iterative solution that reloads the section cable at
/// the stretch load case until the load of the reloaded catenary matches the
/// stretch load that defines it.
bool LineCableToCatenaryCableConverter::SolveStateLoadStretch() const {

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
  point_right.x = catenary_cable_.cable()->strength_rated;
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

bool LineCableToCatenaryCableConverter::Update() const {

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

bool LineCableToCatenaryCableConverter::UpdateCatenaryCable() const {

  // updates catenary
  LineCableToCatenaryConverter converter;
  converter.set_line_cable(line_cable_);
  Catenary3d catenary = converter.Catenary();

  catenary_cable_.set_spacing_endpoints(catenary.spacing_endpoints());
  catenary_cable_.set_tension_horizontal(catenary.tension_horizontal());
  catenary_cable_.set_weight_unit(catenary.weight_unit());

  // updates cable
  catenary_cable_.set_cable(line_cable_->cable);

  // updates state
  CableState state;
  state.temperature = line_cable_->constraint.case_weather->temperature_cable;
  state.temperature_stretch = case_stretch_->temperature_cable;
  catenary_cable_.set_state(state);

  if (line_cable_->constraint.condition == CableConditionType::kInitial) {
    state.load_stretch = 0;
    catenary_cable_.set_state(state);
  } else {
    if (SolveStateLoadStretch() == false) {
      return false;
    }
  }

  return true;
}

bool LineCableToCatenaryCableConverter::UpdateCatenaryCableState(
    const double& load_stretch) const {

  // extracts catenary cable state, modifies, and updates
  CableState state = *catenary_cable_.state();
  state.load_stretch = load_stretch;
  catenary_cable_.set_state(state);

  return true;
}
