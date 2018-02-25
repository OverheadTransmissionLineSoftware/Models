// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/sagtension/cable_elongation_model.h"

#include <algorithm>
#include <cmath>

#include "models/base/helper.h"

/// This is used for sorting a vector of Point2d objects. For this class, it is
/// used for sorting the region points from the component elongation models.
struct Point2dSortXInreasing {
  /// \brief This comparison operator determines if the first point x value is
  ///   less than the x value of the second point.
  /// \param[in] p1
  ///   The first point.
  /// \param[in] p2
  ///   The second point.
  /// \return A boolean indicating if the first point x value is less than
  ///    the second point x value.
  bool operator() (const Point2d<double>& p1, const Point2d<double>& p2) const {
    return p1.x < p2.x;
  }
};

CableElongationModel::CableElongationModel() {
  cable_ = nullptr;

  is_updated_state_ = false;
  is_updated_stretch_ = false;
}

CableElongationModel::~CableElongationModel() {
}

double CableElongationModel::Load(
    const CableElongationModel::ComponentType& type_component,
    const double& strain) const {
  // updates class if necessary
  if ((IsUpdated() == false) && (Update() == false)) {
    return -999999;
  }

  double load = -999999;
  if (type_component == CableElongationModel::ComponentType::kCombined) {
    load = LoadCombined(strain);
  } else if (type_component == CableElongationModel::ComponentType::kCore) {
    load = LoadCore(strain);
  } else if (type_component == CableElongationModel::ComponentType::kShell) {
    load = LoadShell(strain);
  }

  return load;
}

double CableElongationModel::Slope(
    const CableElongationModel::ComponentType& type_component,
    const double& strain) const {
  // updates class if necessary
  if ((IsUpdated() == false) && (Update() == false)) {
    return -999999;
  }

  double slope = -999999;
  if (type_component == CableElongationModel::ComponentType::kCombined) {
    slope = SlopeCombined(strain);
  } else if (type_component == CableElongationModel::ComponentType::kCore) {
    slope = SlopeCore(strain);
  } else if (type_component == CableElongationModel::ComponentType::kShell) {
    slope = SlopeShell(strain);
  }

  return slope;
}

double CableElongationModel::Strain(
    const CableElongationModel::ComponentType& type_component,
    const double& load) const {
  // updates class if necessary
  if ((IsUpdated() == false) && (Update() == false)) {
    return -999999;
  }

  double strain = -999999;
  if (type_component == CableElongationModel::ComponentType::kCombined) {
    strain = StrainCombined(load);
  } else if (type_component == CableElongationModel::ComponentType::kCore) {
    strain = StrainCore(load);
  } else if (type_component == CableElongationModel::ComponentType::kShell) {
    strain = StrainShell(load);
  }

  return strain;
}

bool CableElongationModel::Validate(
    const bool& is_included_warnings,
    std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "CABLE ELONGATION MODEL";

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

  // validates state
  if (state_.Validate(is_included_warnings, messages) == false) {
    is_valid = false;
  }

  // validates state-stretch
  if (state_stretch_.Validate(is_included_warnings, messages) == false) {
    is_valid = false;
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

    return is_valid;
  }

  // validates that at least one component exists
  if (cable_->IsEnabled(SagTensionCable::ComponentType::kCombined) == false) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "No valid components";
      messages->push_back(message);
    }

    return is_valid;
  }

  // validates component-core
  if ((cable_->IsEnabled(SagTensionCable::ComponentType::kCore) == true)
      && (model_core_.Validate(is_included_warnings, messages) == false)) {
    is_valid = false;
  }

  // validates component-shell
  if ((cable_->IsEnabled(SagTensionCable::ComponentType::kShell))
      && (model_shell_.Validate(is_included_warnings, messages) == false)) {
    is_valid = false;
  }

  // validates components-strain-limit
  if (ValidateComponentsStrainLimit(is_included_warnings,
                                    messages) == false) {
    is_valid = false;
  }

  // validates components-strain-unloaded
  if (ValidateComponentsStrainUnloaded(is_included_warnings,
                                       messages) == false) {
    is_valid = false;
  }

  return is_valid;
}

const SagTensionCable* CableElongationModel::cable() const {
  return cable_;
}

void CableElongationModel::set_cable(const SagTensionCable* cable) {
  cable_ = cable;

  if (cable_ != nullptr) {
    // updates core model
    if (cable_->IsEnabled(SagTensionCable::ComponentType::kCore) == true) {
      model_core_.set_component_cable(cable_->component_core());
      model_core_.set_temperature_reference(
          cable_->temperature_properties_components());
    } else {
      model_core_ = CableComponentElongationModel();
    }

    // updates shell model
    if (cable_->IsEnabled(SagTensionCable::ComponentType::kShell) == true) {
      model_shell_.set_component_cable(cable_->component_shell());
      model_shell_.set_temperature_reference(
          cable_->temperature_properties_components());
    } else {
      model_shell_ = CableComponentElongationModel();
    }
  }

  is_updated_stretch_ = false;
  is_updated_state_ = false;
}

void CableElongationModel::set_state(const CableState& state) {
  state_ = state;

  is_updated_state_ = false;
}

void CableElongationModel::set_state_stretch(
    const CableStretchState& state_stretch) {
  state_stretch_ = state_stretch;

  is_updated_stretch_ = false;
  is_updated_state_ = false;
}

CableState CableElongationModel::state() const {
  return state_;
}

CableStretchState CableElongationModel::state_stretch() const {
  return state_stretch_;
}

bool CableElongationModel::IsUpdated() const {
  return (is_updated_stretch_ == true) && (is_updated_state_);
}

double CableElongationModel::LoadCombined(const double& strain) const {
  return LoadCore(strain) + LoadShell(strain);
}

double CableElongationModel::LoadCore(const double& strain) const {
  if (cable_->IsEnabled(SagTensionCable::ComponentType::kCore) == true) {
    return model_core_.Load(strain);
  } else {
    return 0;
  }
}

double CableElongationModel::LoadShell(const double& strain) const {
  if (cable_->IsEnabled(SagTensionCable::ComponentType::kShell) == true) {
    return model_shell_.Load(strain);
  } else {
    return 0;
  }
}

double CableElongationModel::SlopeCombined(const double& strain) const {
  return SlopeCore(strain) + SlopeShell(strain);
}

double CableElongationModel::SlopeCore(const double& strain) const {
  if (cable_->IsEnabled(SagTensionCable::ComponentType::kCore) == true) {
    return model_core_.Slope(strain);
  } else {
    return 0;
  }
}

double CableElongationModel::SlopeShell(const double& strain) const {
  if (cable_->IsEnabled(SagTensionCable::ComponentType::kShell) == true) {
    return model_shell_.Slope(strain);
  } else {
    return 0;
  }
}

double CableElongationModel::StrainCombined(
    const double& load,
    const int& precision_decimal_load) const {
  // x = strain
  // y = load

  // determines which region the target load is in, and sets left and right
  // points
  const Point2d<double>& point_regions_min = *points_regions_.cbegin();
  const Point2d<double>& point_regions_max = *(points_regions_.cend() - 1);
  Point2d<double> point_left;
  Point2d<double> point_right;

  // load is less than all points in the sorted collection
  if (load <= point_regions_min.y) {

    // determines if core and/or shell can be compressed
    bool is_compressible_core =
        cable_->component_core()->modulus_compression_elastic_area() == 0;
    bool is_compressible_shell =
        cable_->component_shell()->modulus_compression_elastic_area() == 0;

    if ((is_compressible_core == false) && (is_compressible_shell == false)) {
      // cable has no compressive ability, returns minimum value
      return point_regions_min.x;
    } else {
      // cable has compressive ability, can extrapolate into compression region
      point_right.x = point_regions_min.x;
      point_right.y = point_regions_min.y;

      point_left.x = point_right.x - 0.0005;
      point_left.y = LoadCombined(point_left.x);
    }

  // load is greater than all points in the sorted collection
  } else if (point_regions_max.y <= load) {
    point_left.x = point_regions_max.x;
    point_left.y = point_regions_max.y;

    point_right.x = point_left.x + 0.0005;
    point_right.y = LoadCombined(point_right.x);

  // load is between the points in the sorted collection
  } else {
    // searches for boundary region points
    for (auto iter = points_regions_.cbegin(); iter != points_regions_.cend();
         iter++) {
      point_left = *iter;
      point_right = *(iter + 1);

      if ((point_left.y <= load) && (load <= point_right.y)) {
        break;
      }
    }
  }

  // declares and initialize current point
  Point2d<double> point_current;

  // declares iteration variables
  // iterates until current point load matches target load
  int iter = 1;
  double precision = 1 / std::pow(10, precision_decimal_load);
  double slope_line =-999999;

  while ((precision < std::abs(point_current.y - load)) && (iter < 100)) {

    // calculates new strain value by using tangent line between points,
    // extrapolates using line
    slope_line = (point_right.y - point_left.y)
                  / (point_right.x - point_left.x);
    point_current.x = point_left.x + ((load - point_left.y) / slope_line);

    // gets current point load
    point_current.y = LoadCombined(point_current.x);

    // current point is left of left/right points
    if (point_current.x < point_left.x) {

      point_right.x = point_left.x;
      point_right.y = point_left.y;

      point_left.x = point_current.x;
      point_left.y = point_current.y;

    // current point is between left/right points
    } else if ((point_left.x <= point_current.x)
         && (point_current.x <= point_right.x)) {

      if (point_current.y < load) {
          point_left.x = point_current.x;
          point_left.y = point_current.y;
      } else if (load < point_current.y) {
          point_right.x = point_current.x;
          point_right.y = point_current.y;
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

  if (iter < 100) {
    return point_current.x;
  } else {
    return -999999;
  }
}

double CableElongationModel::StrainCore(const double& load) const {
  if (cable_->IsEnabled(SagTensionCable::ComponentType::kCore) == true) {
    return model_core_.Strain(load);
  } else {
    return 0;
  }
}

double CableElongationModel::StrainShell(const double& load) const {
  if (cable_->IsEnabled(SagTensionCable::ComponentType::kShell) == true) {
    return model_shell_.Strain(load);
  } else {
    return 0;
  }
}

bool CableElongationModel::Update() const {
  // updates stretch
  if (is_updated_stretch_ == false) {
    is_updated_stretch_ = UpdateComponentsStretch();
    if (is_updated_stretch_ == false) {
      return false;
    }
  }

  // updates state
  if (is_updated_state_ == false) {
    is_updated_state_ = UpdateComponentsState(state_);
    if (is_updated_state_ == false) {
      return false;
    }
  }

  // if it reaches this point, update was successful
  return true;
}

bool CableElongationModel::UpdateComponentsState(
    const CableState& state) const {
  // if core component is enabled, updates state
  if (cable_->IsEnabled(SagTensionCable::ComponentType::kCore) == true) {
    model_core_.set_state(&state);
  }

  // if shell component is enabled, updates state
  if (cable_->IsEnabled(SagTensionCable::ComponentType::kShell) == true) {
    model_shell_.set_state(&state);
  }

  // updates the region points
  UpdatePointsRegions();

  return true;
}

/// The component models are set to zero stretch load, and are shifted to the
/// stretch temperature. The strain for the entire cable is calculated at the
/// stretch load for the entire cable. The load for each component is
/// calculated and updated.
bool CableElongationModel::UpdateComponentsStretch() const {
  // initializes the stretch state for the components
  if (cable_->IsEnabled(SagTensionCable::ComponentType::kCore) == true) {
    state_stretch_core_ = state_stretch_;
    state_stretch_core_.load = 0;
    model_core_.set_state_stretch(&state_stretch_core_);
  }

  if (cable_->IsEnabled(SagTensionCable::ComponentType::kShell) == true) {
    state_stretch_shell_ = state_stretch_;
    state_stretch_shell_.load = 0;
    model_shell_.set_state_stretch(&state_stretch_shell_);
  }

  // checks if stretch is not defined or not required
  if (state_stretch_.load == 0) {
    return true;
  }

  // updates component model states to the stretch state
  CableState state;
  state.temperature = state_stretch_.temperature;
  state.type_polynomial = state_stretch_.type_polynomial;

  UpdateComponentsState(state);

  // gets the stretch point of the entire cable
  Point2d<double> point_stretch;
  point_stretch.y = state_stretch_.load;
  point_stretch.x = StrainCombined(point_stretch.y);

  // updates component stretch loads
  if (cable_->IsEnabled(SagTensionCable::ComponentType::kCore) == true) {
    state_stretch_core_.load = LoadCore(point_stretch.x);
    model_core_.set_state_stretch(&state_stretch_core_);
  }

  if (cable_->IsEnabled(SagTensionCable::ComponentType::kShell) == true) {
    state_stretch_shell_.load = LoadShell(point_stretch.x);
    model_shell_.set_state_stretch(&state_stretch_shell_);
  }

  return true;
}

bool CableElongationModel::UpdatePointsRegions() const {
  // initializes container
  points_regions_.clear();

  // gets region boundary points for core and adds to container
  if (cable_->IsEnabled(SagTensionCable::ComponentType::kCore) == true) {
    std::vector<Point2d<double>> points_core = model_core_.PointsRegions();
    for (auto iter = points_core.cbegin(); iter != points_core.cend(); iter++) {
      const Point2d<double>& point = *iter;
      points_regions_.push_back(point);
    }
  }

  // gets region boundary points for shell and adds to vector
  if (cable_->IsEnabled(SagTensionCable::ComponentType::kShell) == true) {
    std::vector<Point2d<double>> points_shell = model_shell_.PointsRegions();
    for (auto iter = points_shell.cbegin(); iter != points_shell.cend();
        iter++) {
      const Point2d<double>& point = *iter;
      points_regions_.push_back(point);
    }
  }

  // sorts the vector according to the x value of the point
  std::sort(points_regions_.begin(), points_regions_.end(),
            Point2dSortXInreasing());

  // re-calculates loads using total load function
  for (auto iter = points_regions_.begin(); iter != points_regions_.end();
       iter++) {
    Point2d<double>& point = *iter;
    point.y = LoadCombined(point.x);
  }

  return true;
}

bool CableElongationModel::ValidateComponentsStrainLimit(
    const bool& is_included_warnings,
    std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "CABLE ELONGATION MODEL";

  // exits if warnings are not included
  if (is_included_warnings == false) {
    return is_valid;
  }

  // modifies component states to reference temperature
  CableState state;
  state.temperature = *cable_->temperature_properties_components();
  state.type_polynomial = SagTensionCableComponent::PolynomialType::kLoadStrain;

  UpdateComponentsState(state);

  // gets strain at rated strength
  const double strain_max = StrainCombined(*cable_->strength_rated());

  // checks core, if enabled
  if (cable_->IsEnabled(SagTensionCable::ComponentType::kCore) == true) {
    // gets polynomial limit for core
    const double strain_limit_polynomial_core = StrainCore(
        *model_core_.component_cable()->load_limit_polynomial(
            state_.type_polynomial));

    // compares against max strain
    // if component limit is less that rated strength strain, generates error
    if (strain_limit_polynomial_core < strain_max) {
      is_valid = false;
      if (messages != nullptr) {
        message.description = "Core polynomial limit is less than rated "
                              "strength of cable";
        messages->push_back(message);
      }
    }

  }

  // checks shell, if enabled
  if (cable_->IsEnabled(SagTensionCable::ComponentType::kShell) == true) {
    // gets polynomial limit for shell
    const double strain_limit_polynomial_shell = StrainShell(
        *model_shell_.component_cable()->load_limit_polynomial(
            state_.type_polynomial));

    // compares against max strain
    // if component limit is less that rated strength strain, generates error
    if (strain_limit_polynomial_shell < strain_max) {
      is_valid = false;
      if (messages != nullptr) {
        message.description = "Shell polynomial limit is less than rated "
                              "strength of cable";
        messages->push_back(message);
      }
    }
  }

  // restores component states
  UpdateComponentsState(state_);

  return is_valid;
}

bool CableElongationModel::ValidateComponentsStrainUnloaded(
    const bool& is_included_warnings,
    std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "CABLE ELONGATION MODEL";

  // exits if warnings are not included
  if (is_included_warnings == false) {
    return is_valid;
  }

  // determines if this check is necessary, both core and shell must be enabled
  if ((cable_->IsEnabled(SagTensionCable::ComponentType::kCore) == false)
      || (cable_->IsEnabled(SagTensionCable::ComponentType::kCore) == false)) {
    return is_valid;
  }

  // creates states and stretch states for components at reference temp with
  // no stretch
  CableState state;
  state.temperature = *cable_->temperature_properties_components();
  state.type_polynomial = SagTensionCableComponent::PolynomialType::kLoadStrain;

  CableStretchState state_stretch;
  state_stretch.load = 0;
  state_stretch.temperature = *cable_->temperature_properties_components();
  state_stretch.type_polynomial =
      SagTensionCableComponent::PolynomialType::kLoadStrain;

  model_core_.set_state(&state);
  model_core_.set_state_stretch(&state_stretch);

  model_shell_.set_state(&state);
  model_shell_.set_state_stretch(&state_stretch);

  // gets unloaded unstretched strains for core and shell, calculates
  // difference
  const double strain_unloaded_unstretched_core = StrainCore(0);
  const double strain_unloaded_unstretched_shell = StrainShell(0);
  const double strain_difference =
      strain_unloaded_unstretched_core - strain_unloaded_unstretched_shell;

  // validates unloaded unstretched strain differences
  if (0.0005 < std::abs(strain_difference)
      || ((0.0001 < std::abs(strain_difference))
      && (is_included_warnings == true))) {
    is_valid = false;
    if (messages != nullptr) {
      double value = strain_unloaded_unstretched_core
                     - strain_unloaded_unstretched_shell;
      message.description = "Unloaded unstretched strain difference between "
                            "shell and core = "
                            + helper::DoubleToFormattedString(value, 5);
      messages->push_back(message);
    }
  }

  // restores component states
  model_core_.set_state(&state_);
  model_core_.set_state_stretch(&state_stretch_);

  model_shell_.set_state(&state_);
  model_shell_.set_state_stretch(&state_stretch_);

  return is_valid;
}
