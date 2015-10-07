// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/sagtension/cable_elongation_model.h"

#include <algorithm>

/// This is used for sorting a vector of Point2d objects. For this class, it is
/// used for sorting the region points from the component elongation models.
struct Point2dSortXInreasing
{
  /// \brief This comparison operator determines if the first point x value is
  ///   less than the x value of the second point.
  /// \param[in] p1
  ///   The first point.
  /// \param[in] p2
  ///   The second point.
  /// \return A boolean indicating if the first point x value is less than
  ///    the second point x value.
  bool operator() (const Point2d& p1, const Point2d& p2) const {
    return p1.x < p2.x;
  }
};

CableElongationModel::CableElongationModel() {

  cable_ = nullptr;
  state_ = nullptr;

  is_enabled_core_ = false;
  is_enabled_shell_ = false;

  is_updated_components_properties_ = false;
  is_updated_components_state_ = false;
}

CableElongationModel::~CableElongationModel() {
}

double CableElongationModel::Load(
    const CableElongationModel::ComponentType& type_component,
    const double& strain) const {

  // updates class if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
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
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
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
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
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
    std::list<std::string>* messages_error) const {

  bool is_valid = true;

  // validates cable
  if (cable_ == nullptr) {
    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CABLE ELONGATION MODEL - Invalid cable");
    }
  } else {
    if (cable_->Validate(is_included_warnings, messages_error) == false) {
      is_valid = false;
    }
  }

  // validates state
  if (state_ == nullptr) {
    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CABLE ELONGATION MODEL - Invalid state");
    }
  } else {
    if (state_->Validate(is_included_warnings, messages_error) == false) {
      is_valid = false;
    }
  }

  // continues validation if no errors are present
  if (is_valid == true) {
    // validates if class updates
    if (Update() == false) {

      is_valid = false;
      if (messages_error != nullptr) {
        messages_error->push_back(
            "CABLE ELONGATION MODEL - Error updating class");
      }

      return is_valid;
    }

    // validates that at least one component exists
    if ((is_enabled_core_ == false) && (is_enabled_shell_ == false)) {

      is_valid = false;
      if (messages_error != nullptr) {
        messages_error->push_back(
            "CABLE ELONGATION MODEL - No valid components");
      }

      return is_valid;
    }

    // validates component-core
    if (is_enabled_core_ == true) {
      if (component_core_.Validate(is_included_warnings,
                                   messages_error) == false) {
        is_valid = false;
      }
    }

    // validates component-shell
    if (is_enabled_shell_ == true) {
      if (component_shell_.Validate(is_included_warnings,
                                    messages_error) == false) {
        is_valid = false;
      }
    }

    // validates components-strain-limit
    if (ValidateComponentsStrainLimit(is_included_warnings,
                                      messages_error) == false) {
      is_valid = false;
    }

    // validates components-strain-unloaded
    if (ValidateComponentsStrainUnloaded(is_included_warnings,
                                         messages_error) == false) {
      is_valid = false;
    }

  }

  return is_valid;
}

const Cable* CableElongationModel::cable() const {
  return cable_;
}

void CableElongationModel::set_cable(const Cable* cable) {

  cable_ = cable;

  is_updated_components_properties_ = false;
  is_updated_components_state_ = false;
}

void CableElongationModel::set_state(const CableState* state) {

  state_ = state;

  is_updated_components_state_ = false;
}

const CableState* CableElongationModel::state() const {
  return state_;
}

bool CableElongationModel::IsUpdated() const {

  if ((is_updated_components_properties_ == true)
      && (is_updated_components_state_ == true)) {
    return true;
  } else {
    return false;
  }
}

double CableElongationModel::LoadCombined(const double& strain) const {

  return LoadCore(strain) + LoadShell(strain);
}

double CableElongationModel::LoadCore(const double& strain) const {

  if (is_enabled_core_ == true) {
    return component_core_.Load(strain);
  } else {
    return 0;
  }
}

double CableElongationModel::LoadShell(const double& strain) const {

  if (is_enabled_shell_ == true) {
    return component_shell_.Load(strain);
  } else {
    return 0;
  }
}

double CableElongationModel::SlopeCombined(const double& strain) const {

  return SlopeCore(strain) + SlopeShell(strain);
}

double CableElongationModel::SlopeCore(const double& strain) const {

  if (is_enabled_core_ == true) {
    return component_core_.Slope(strain);
  } else {
    return 0;
  }
}

double CableElongationModel::SlopeShell(const double& strain) const {

  if (is_enabled_shell_ == true) {
    return component_shell_.Slope(strain);
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
  const Point2d& point_regions_min = *points_regions_.cbegin();
  const Point2d& point_regions_max = *(points_regions_.cend() - 1);
  Point2d point_left;
  Point2d point_right;

  // load is less than all points in the sorted collection
  if (load <= point_regions_min.y) {
    point_right.x = point_regions_min.x;
    point_right.y = point_regions_min.y;

    point_left.x = point_right.x - 0.0005;
    point_left.y = LoadCombined(point_left.x);

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
  Point2d point_current;

  // declares iteration variables
  // iterates until current point load matches target load
  int iter = 1;
  double precision = 1 / pow(10, precision_decimal_load);
  double slope_line =-999999;

  while ((precision < abs(point_current.y - load)) && (iter < 100)) {

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

  if (is_enabled_core_ == true) {
    return component_core_.Strain(load);
  } else {
    return 0;
  }
}

double CableElongationModel::StrainShell(const double& load) const {

  if (is_enabled_shell_ == true) {
    return component_shell_.Strain(load);
  } else {
    return 0;
  }
}

bool CableElongationModel::Update() const {

  // updates components-properties
  if (is_updated_components_properties_ == false) {

    // updates if components are enabled
    is_updated_components_properties_ = UpdateComponentsEnabled();
    if (is_updated_components_properties_ == false) {
      return false;
    }

    // updates component properties
    is_updated_components_properties_ = UpdateComponentsProperties();
    if (is_updated_components_properties_ == false) {
      return false;
    }
  }

  // updates components-state
  if (is_updated_components_state_ == false) {

    // updates components-stretch
    is_updated_components_state_ = UpdateComponentsLoadStretch();
    if (is_updated_components_state_ == false) {
      return false;
    }

    // updates components-temperature
    is_updated_components_state_ =
        UpdateComponentsTemperature(state_->temperature);
    if (is_updated_components_state_ == false) {
      return false;
    }
  }

  // if it reaches this point, update was successful
  return true;
}

/// Based on the active polynomial, the routine searches the component
/// polynomial coefficients. If non-zero values are present, the component
/// is flagged as enabled.
bool CableElongationModel::UpdateComponentsEnabled() const {

  // checks core for valid polynomial coefficients
  is_enabled_core_ = false;

  // gets active polynomial
  std::vector<double> coefficients_polynomial_core;
  if (cable_->type_polynomial_active == CableComponent::PolynomialType::kCreep) {
    coefficients_polynomial_core =
        cable_->component_core.coefficients_polynomial_creep;
  } else if (cable_->type_polynomial_active == CableComponent::PolynomialType::kLoadStrain) {
    coefficients_polynomial_core =
        cable_->component_core.coefficients_polynomial_loadstrain;
  }

  // scans polynomial coefficients for a non-zero value
  for (unsigned int iter = 1; iter <= coefficients_polynomial_core.size();
       iter++) {

    if (coefficients_polynomial_core.at(iter) != 0) {
      is_enabled_core_ = true;
      break;
    }
  }

  // checks shell for valid polynomial coefficients
  is_enabled_shell_ = false;

  // gets active polynomial
  std::vector<double> coefficients_polynomial_shell;
  if (cable_->type_polynomial_active == CableComponent::PolynomialType::kCreep) {
    coefficients_polynomial_shell =
        cable_->component_shell.coefficients_polynomial_creep;
  } else if (cable_->type_polynomial_active == CableComponent::PolynomialType::kLoadStrain) {
    coefficients_polynomial_shell =
        cable_->component_shell.coefficients_polynomial_loadstrain;
  }

  // scans polynomial coefficients for a non-zero value
  for (unsigned int iter = 1; iter <= coefficients_polynomial_core.size();
       iter++) {

    if (coefficients_polynomial_shell.at(iter) != 0) {
      is_enabled_shell_ = true;
      break;
    }
  }

  return true;
}

/// The component models are set to zero stretch load, and are shifted to the
/// stretch temperature. The strain for the entire cable is calculated at the
/// stretch load for the entire cable. The load for each component is
/// calculated and updated.
bool CableElongationModel::UpdateComponentsLoadStretch() const {

  // initializes stretch
  if (is_enabled_core_ == true) {
    component_core_.set_load_stretch(0);
  }

  if (is_enabled_shell_ == true) {
    component_shell_.set_load_stretch(0);
  }

  // checks if stretch is not defined or not required
  if ((state_->load_stretch == 0)
      || (cable_->type_polynomial_active
          == CableComponent::PolynomialType::kCreep)) {
    return true;
  }

  // modifies temperature to stretch temperature
  if (UpdateComponentsTemperature(state_->temperature_stretch) == false) {
    return false;
  }

  // gets the stretch point of the cable
  Point2d point_stretch;
  point_stretch.y = state_->load_stretch;
  point_stretch.x = StrainCombined(point_stretch.y);

  // gets stretch load of core using strain from entire cable
  double load_stretch_core = LoadCore(point_stretch.x);

  // gets stretch load of shell using strain from entire cable
  double load_stretch_shell = LoadShell(point_stretch.x);

  // updates component stretch loads
  if (is_enabled_core_ == true) {
    component_core_.set_load_stretch(load_stretch_core);
  }

  if (is_enabled_shell_ == true) {
    component_shell_.set_load_stretch(load_stretch_shell);
  }

  // resets temperature to initial value
  UpdateComponentsTemperature(state_->temperature);

  return true;
}

bool CableElongationModel::UpdateComponentsProperties() const {

  // if core component is enabled, updates properties
  if (is_enabled_core_ == true) {
    component_core_.set_component_cable(&cable_->component_core);
    component_core_.set_temperature_reference(
        &cable_->temperature_properties_components);
    component_core_.set_type_polynomial_active(&cable_->type_polynomial_active);
  }

  // if shell component is enabled, updates properties
  if (is_enabled_shell_ == true) {
    component_shell_.set_component_cable(&cable_->component_shell);
    component_shell_.set_temperature_reference(
        &cable_->temperature_properties_components);
    component_shell_.set_type_polynomial_active(
        &cable_->type_polynomial_active);
  }

  return true;
}

bool CableElongationModel::UpdateComponentsTemperature(
    const double& temperature) const {

  // updates component-core
  if (is_enabled_core_ == true) {
    component_core_.set_temperature(&temperature);
  }

  // updates component-shell
  if (is_enabled_shell_ == true) {
    component_shell_.set_temperature(&temperature);
  }

  // updates region points
  if (UpdatePointsRegions() == false) {
    return false;
  }

  return true;
}

bool CableElongationModel::UpdatePointsRegions() const {

  // initializes container
  points_regions_.clear();

  // gets region boundary points for core and adds to container
  if (is_enabled_core_ == true) {

    std::vector<Point2d> points_core = component_core_.PointsRegions();
    for (auto iter = points_core.cbegin(); iter != points_core.cend(); iter++) {
      const Point2d& point = *iter;
      points_regions_.push_back(point);
    }
  }

  // gets region boundary points for shell and adds to vector
  if (is_enabled_shell_ == true) {

    std::vector<Point2d> points_shell = component_shell_.PointsRegions();
    for (auto iter = points_shell.cbegin(); iter != points_shell.cend();
        iter++) {
      const Point2d& point = *iter;
      points_regions_.push_back(point);
    }
  }

  // sorts the vector according to the x value of the point
  std::sort(points_regions_.begin(), points_regions_.end(),
            Point2dSortXInreasing());

  // re-calculates loads using total load function
  for (auto iter = points_regions_.begin(); iter != points_regions_.end();
       iter++) {
    Point2d& point = *iter;
    point.y = LoadCombined(point.x);
  }

  return true;
}

bool CableElongationModel::ValidateComponentsStrainLimit(
    const bool& is_included_warnings,
    std::list<std::string>* messages_error) const {

  bool is_valid = true;

  // exits if warnings are not included
  if (is_included_warnings == false) {
    return is_valid;
  }

  // modifies temperature to reference temperature
  if (UpdateComponentsTemperature(
      cable_->temperature_properties_components) == false) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back(
          "CABLE ELONGATION MODEL - Could not verify component limits");
    }

    return is_valid;
  }

  // gets strain at rated strength
  double strain_max = StrainCombined(cable_->strength_rated);

  // checks core, if enabled
  if (is_enabled_core_ == true) {

    // gets polynomial limit for core
    double strain_limit_polynomial_core = StrainCore(
        component_core_.component_cable()->load_limit_polynomial_creep);

    // compares against max strain
    // if component limit is less that rated strength strain, generates error
    if (strain_limit_polynomial_core < strain_max) {

      is_valid = false;
      if (messages_error != nullptr) {
        messages_error->push_back(
            "CABLE ELONGATION MODEL - Core polynomial limit is less than "
            "rated strength of cable");
      }
    }

  }

  // checks shell, if enabled
  if (is_enabled_shell_ == true) {

    // gets polynomial limit for shell
    double strain_limit_polynomial_shell = StrainShell(
        component_shell_.component_cable()->load_limit_polynomial_loadstrain);

    // compares against max strain
    // if component limit is less that rated strength strain, generates error
    if (strain_limit_polynomial_shell < strain_max) {

      is_valid = false;
      if (messages_error != nullptr) {
        messages_error->push_back(
            "CABLE ELONGATION MODEL - Shell polynomial limit is less than "
            "rated strength of cable");
      }
    }
  }

  // restores temperature
  UpdateComponentsTemperature(state_->temperature);

  return is_valid;
}

bool CableElongationModel::ValidateComponentsStrainUnloaded(
    const bool& is_included_warnings,
    std::list<std::string>* messages_error) const {

  bool is_valid = true;

  // exits if warnings are not included
  if (is_included_warnings == false) {
    return is_valid;
  }

  // determines if this check is necessary, both core and shell must be enabled
  if ((is_enabled_core_ == false) || (is_enabled_shell_ == false)) {
    return is_valid;
  }

  // caches temperature and stretch for components, modifies temperature and
  // stretch to reference values
  const double load_stretch_core = component_core_.load_stretch();
  const double load_stretch_shell = component_shell_.load_stretch();
  component_core_.set_load_stretch(0);
  component_shell_.set_load_stretch(0);
  UpdateComponentsTemperature(cable_->temperature_properties_components);

  // gets unloaded unstretched strains for core and shell, calculates
  // difference
  const double strain_unloaded_unstretched_core = StrainCore(0);
  const double strain_unloaded_unstretched_shell = StrainShell(0);
  const double strain_difference =
      strain_unloaded_unstretched_core - strain_unloaded_unstretched_shell;

  // validates unloaded unstretched strain differences
  if (0.0005 < abs(strain_difference)
      || ((0.0001 < abs(strain_difference))
      && (is_included_warnings == true))) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back(
          "CABLE ELONGATION MODEL - Unloaded unstretched strain difference"
          " between shell and core");
          //& Round(Abs(strain_unloaded_unstretched_core _
          //            - strain_unloaded_unstretched_shell), 5))
    }
  }

  // restores temperature
  component_core_.set_load_stretch(load_stretch_core);
  component_shell_.set_load_stretch(load_stretch_shell);
  UpdateComponentsTemperature(state_->temperature);

  return is_valid;
}
