// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "sagtension/cable_elongation_model.h"

CableElongationModel::CableElongationModel() {

  load_stretch_ = -999999;
  temperature_ = -999999;
  temperature_stretch_ = -999999;

  is_enabled_core_ = false;
  is_enabled_shell_ = false;

  is_updated_components_properties_ = false;
  is_updated_components_load_stretch_ = false;
  is_updated_components_temperature_ = false;
}

CableElongationModel::~CableElongationModel() {
}

double CableElongationModel::GetLoadCore(const double& strain,
                                         const bool& is_stretched) const {

  // update class, if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return LoadCore(strain, is_stretched);
}

double CableElongationModel::GetLoadShell(const double& strain,
                                          const bool& is_stretched) const {

  // update class, if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return LoadShell(strain, is_stretched);
}

double CableElongationModel::GetLoadTotal(const double& strain,
                                          const bool& is_stretched) const {

  // update class, if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return LoadTotal(strain, is_stretched);
}

double CableElongationModel::GetStrainCore(const double& load,
                                           const bool& is_stretched) const {

  // update class, if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return StrainCore(load, is_stretched);
}

double CableElongationModel::GetStrainShell(const double& load,
                                            const bool& is_stretched) const {

  // update class, if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return StrainShell(load, is_stretched);
}

double CableElongationModel::GetStrainTotal(const double& load,
                                            const bool& is_stretched) const {

  // update class, if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return StrainTotal(load, is_stretched);
}

bool CableElongationModel::Validate(
    const bool& is_included_warnings,
    std::list<std::string>* messages_error) const {

  // initialize
  bool is_valid = true;

  // validate cable
  if (cable_.Validate(is_included_warnings, messages_error) == false) {
    is_valid = false;
  }

  // validate load - stretch
  if (load_stretch_ < 0) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back(
          "CABLE ELONGATION MODEL - Invalid stretch load");
    }
  }

  // validate temperature
  if (temperature_ < -50) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back(
          "CABLE ELONGATION MODEL - Invalid temperature");
    }
  }

  // validate temperature - stretch
  if (temperature_stretch_ < -50) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back(
          "CABLE ELONGATION MODEL - Invalid stretch temperature");
    }
  }

  // continue validation if no errors are present
  if (is_valid == true) {

    // validate if class updates
    if (Update() == false) {

      is_valid = false;
      if (messages_error != nullptr) {
        messages_error->push_back(
            "CABLE ELONGATION MODEL - Error updating class");
      }

      return is_valid;
    }

    // validate that at least one component exists
    if ((is_enabled_core_ == false) && (is_enabled_shell_ == false)) {

      is_valid = false;
      if (messages_error != nullptr) {
        messages_error->push_back(
            "CABLE ELONGATION MODEL - No valid components");
      }

      return is_valid;
    }

    // validate component - core
    if (is_enabled_core_ == true) {
      if (component_core_.Validate(is_included_warnings,
                                   messages_error) == false) {
        is_valid = false;
      }
    }

    // validate component - shell
    if (is_enabled_shell_ == true) {
      if (component_shell_.Validate(is_included_warnings,
                                    messages_error) == false) {
        is_valid = false;
      }
    }

    // validate components - strain - limit
    if (ValidateComponentsStrainLimit(is_included_warnings,
                                      messages_error) == false) {
      is_valid = false;
    }

    // validate components - strain - unloaded
    if (ValidateComponentsStrainUnloaded(is_included_warnings,
                                         messages_error) == false) {
      is_valid = false;
    }

  }

  return is_valid;
}

Cable CableElongationModel::cable() const {
  return cable_;
}

double CableElongationModel::load_stretch() const {
  return load_stretch_;
}

void CableElongationModel::set_cable(const Cable& cable) {

  cable_ = cable;

  is_updated_components_properties_ = false;
  is_updated_components_load_stretch_ = false;
  is_updated_components_temperature_ = false;
}

void CableElongationModel::set_load_stretch(const double& load_stretch) {

  load_stretch_ = load_stretch;

  is_updated_components_load_stretch_ = false;
  is_updated_components_temperature_ = false;
}

void CableElongationModel::set_temperature(const double& temperature) {

  temperature_ = temperature;

  is_updated_components_temperature_ = false;
}

void CableElongationModel::set_temperature_stretch(
    const double& temperature_stretch) {

  temperature_stretch_ = temperature_stretch;

  is_updated_components_load_stretch_ = false;
  is_updated_components_temperature_ = false;
}

double CableElongationModel::temperature() const {
  return temperature_;
}

double CableElongationModel::temperature_stretch() const {
  return temperature_stretch_;
}

bool CableElongationModel::IsUpdated() const {

  if ((is_updated_components_properties_ == true)
      && (is_updated_components_load_stretch_ == true)
      && (is_updated_components_temperature_ == true)) {

    return true;
  } else {
    return false;
  }
}

double CableElongationModel::LoadCore(const double& strain,
                                      const bool& is_stretched) const {

  if (is_enabled_core_ == true) {
    return component_core_.Load(strain, is_stretched);
  } else {
    return 0;
  }
}

double CableElongationModel::LoadShell(const double& strain,
                                       const bool& is_stretched) const {

  if (is_enabled_shell_ == true) {
    return component_shell_.Load(strain, is_stretched);
  } else {
    return 0;
  }
}

double CableElongationModel::LoadTotal(const double& strain,
                                       const bool& is_stretched) const {

  return LoadCore(strain, is_stretched) + LoadShell(strain, is_stretched);
}

std::vector<Point2d> CableElongationModel::PointsDiscreteRegions(
    const bool& is_stretched) const {

  if (is_stretched == false) {
    return points_discrete_regions_unstretched_;
  } else {
    return points_discrete_regions_stretched_;
  }
}

double CableElongationModel::StrainCore(const double& load,
                                        const bool& is_stretched) const {

  if (is_enabled_core_ == true) {
    return component_core_.Strain(load, is_stretched);
  } else {
    return 0;
  }
}

double CableElongationModel::StrainShell(const double& load,
                                         const bool& is_stretched) const {

  if (is_enabled_shell_ == true) {
    return component_shell_.Strain(load, is_stretched);
  } else {
    return 0;
  }
}

double CableElongationModel::StrainTotal(
    const double& load,
    const bool& is_stretched,
    const int& precision_decimal_load) const {

  // x = strain
  // y = load

  // get region boundary points
  std::vector<Point2d> points_regions = PointsDiscreteRegions(is_stretched);

  // determine which region the target load is in, and set left and right points
  Point2d point_left;
  Point2d point_right;
  for (unsigned int index = 0; index <= points_regions.size(); index++) {

    // load is less than all points in the sorted collection
    if (load < points_regions.at(0).y) {
      point_right.x = points_regions.at(0).x;
      point_right.y = points_regions.at(0).y;

      point_left.x = point_right.x - 0.0005;
      point_left.y = LoadTotal(point_left.x, is_stretched);

      break;
    }

    // load is between the points in the sorted collection
    if (load < points_regions.at(index).y) {

      point_left.x = points_regions.at(index - 1).x;
      point_left.y = points_regions.at(index - 1).y;

      point_right.x = points_regions.at(index).x;
      point_right.y = points_regions.at(index).y;

      break;
    }

    // load is greater than all points in the sorted collection
    if (index == points_regions.size()) {

      point_left.x = points_regions.at(index).x;
      point_left.y = points_regions.at(index).y;

      point_right.x = point_left.x + 0.0005;
      point_right.y = LoadTotal(point_right.x, is_stretched);

      break;
    }
  }

  // declare and initialize current point
  Point2d point_current;

  // declare iteration variables
  // iterate until current point load matches target load
  int iter = 1;
  double precision = 1 / (10 ^ precision_decimal_load);
  double slope_line =-999999;

  while ((precision < abs(point_current.y - load)) && (iter < 100)) {

    // new strain value is calculated using tangent line between points,
    // extrapolate using line
    slope_line = (point_right.y - point_left.y)
                  / (point_right.x - point_left.x);
    point_current.x = point_left.x + ((load - point_left.y) / slope_line);

    // current point load
    point_current.y = LoadTotal(point_current.x, is_stretched);

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

  //return value
  if (iter < 100) {
    return point_current.x;
  } else {
    return -999999;
  }
}

bool CableElongationModel::Update() const {

  // update components - properties
  if (is_updated_components_properties_ == false) {

    // update whether components are enabled
    is_updated_components_properties_ = UpdateComponentsEnabled();
    if (is_updated_components_properties_ == false) {
      return false;
    }

    // update component properties
    is_updated_components_properties_ = UpdateComponentsProperties();
    if (is_updated_components_properties_ == false) {
      return false;
    }
  }

  // update components-load-stretch
  if (is_updated_components_load_stretch_ == false) {

    is_updated_components_load_stretch_ = UpdateComponentsLoadStretch();
    if (is_updated_components_load_stretch_ == false) {
      return false;
    }
  }

  // update components-temperature
  if (is_updated_components_temperature_ == false) {

    is_updated_components_temperature_ =
        UpdateComponentsTemperature(temperature_);
    if (is_updated_components_temperature_ == false) {
      return false;
    }
  }

  // if it reaches this point, update was successful
  return true;
}

bool CableElongationModel::UpdateComponentsEnabled() const {

  // check core for valid polynomial coefficients
  is_enabled_core_ = false;

  // get active polynomial
  std::vector<double> coefficients_polynomial_core;
  if (cable_.type_polynomial_active == CablePolynomialType::kCreep) {
    coefficients_polynomial_core =
        cable_.component_core.coefficients_polynomial_creep;
  } else if (cable_.type_polynomial_active == CablePolynomialType::kLoadStrain) {
    coefficients_polynomial_core =
        cable_.component_core.coefficients_polynomial_loadstrain;
  }

  // scan polynomial coefficients for a non-zero value
  for (unsigned int iter = 1; iter <= coefficients_polynomial_core.size();
       iter++) {

    if (coefficients_polynomial_core.at(iter) != 0) {
      is_enabled_core_ = true;
      break;
    }
  }

  // check shell for valid polynomial coefficients
  is_enabled_shell_ = false;

  std::vector<double> coefficients_polynomial_shell;
  if (cable_.type_polynomial_active == CablePolynomialType::kCreep) {
    coefficients_polynomial_shell =
        cable_.component_shell.coefficients_polynomial_creep;
  } else if (cable_.type_polynomial_active == CablePolynomialType::kLoadStrain) {
    coefficients_polynomial_shell =
        cable_.component_shell.coefficients_polynomial_loadstrain;
  }

  // scan polynomial coefficients for a non-zero value
  for (unsigned int iter = 1; iter <= coefficients_polynomial_core.size();
       iter++) {

    if (coefficients_polynomial_shell.at(iter) != 0) {
      is_enabled_shell_ = true;
      break;
    }
  }

  return true;
}

bool CableElongationModel::UpdateComponentsLoadStretch() const {

  // initialize stretch
  if (is_enabled_core_ == true) {
    component_core_.set_load_stretch(0);
  }

  if (is_enabled_shell_ == true) {
    component_shell_.set_load_stretch(0);
  }

  // check if stretch is not defined or not required
  if ((load_stretch_ == 0)
      || (cable_.type_polynomial_active == CablePolynomialType::kCreep)) {

    return true;
  }

  // modify temperature to stretch temperature
  if (UpdateComponentsTemperature(temperature_stretch_) == false) {
    return false;
  }

  // get the stretch point of the cable
  Point2d point_stretch;
  point_stretch.y = load_stretch_;
  point_stretch.x = StrainTotal(point_stretch.y, false);

  // get stretch load of core using strain from entire cable
  double load_stretch_core = LoadCore(point_stretch.x, false);

  // get stretch load of shell using strain from entire cable
  double load_stretch_shell = LoadShell(point_stretch.x, false);

  // update component stretch loads
  if (is_enabled_core_ == true) {
    component_core_.set_load_stretch(load_stretch_core);
  }

  if (is_enabled_shell_ == true) {
    component_shell_.set_load_stretch(load_stretch_shell);
  }

  // reset temperature to initial value
  UpdateComponentsTemperature(temperature_);

  return true;
}

bool CableElongationModel::UpdateComponentsProperties() const {

  // if core component is enabled, update properties
  if (is_enabled_core_ == true) {
    component_core_.set_component_cable(cable_.component_core);
    component_core_.set_temperature_reference(
        cable_.temperature_properties_components);
    component_core_.set_type_polynomial_active(cable_.type_polynomial_active);
  }

  // if shell component is enabled, update properties
  if (is_enabled_shell_ == true) {
    component_shell_.set_component_cable(cable_.component_shell);
    component_shell_.set_temperature_reference(
        cable_.temperature_properties_components);
    component_shell_.set_type_polynomial_active(cable_.type_polynomial_active);
  }

  return true;
}

bool CableElongationModel::UpdateComponentsTemperature(
    const double& temperature) const {

  // update component - core
  if (is_enabled_core_ == true) {
    component_core_.set_temperature(temperature);
  }

  // update component - shell
  if (is_enabled_shell_ == true) {
    component_shell_.set_temperature (temperature);
  }

  // update region boundary points
  if (UpdatePointsDiscreteRegions() == false) {
    return false;
  }

  return true;
}

bool CableElongationModel::UpdatePointsDiscreteRegions() const {

  // initialize point containers
  points_discrete_regions_unstretched_.clear();
  points_discrete_regions_stretched_.clear();

  // get region boundary points for core
  if (is_enabled_core_ == true) {

    // get points from component
    std::vector<Point2d> points_unstretched_core =
        component_core_.PointsDiscreteRegions(false);

    std::vector<Point2d> points_stretched_core =
        component_core_.PointsDiscreteRegions(true);

    // add to class collection
    for (unsigned int index = 1; index <= 3; index++) {
      points_discrete_regions_unstretched_.push_back(
          points_unstretched_core.at(index));
      points_discrete_regions_stretched_.push_back(
          points_stretched_core.at(index));
    }
  }

  // get region boundary points for shell
  if (is_enabled_shell_ == true) {

    // get points from component
    std::vector<Point2d> points_unstretched_shell =
        component_shell_.PointsDiscreteRegions(false);

    std::vector<Point2d> points_stretched_shell =
        component_shell_.PointsDiscreteRegions(true);

    // add to class collection
    for (unsigned int index = 1; index <= 3; index++) {
      points_discrete_regions_unstretched_.push_back(
          points_unstretched_shell.at(index));
      points_discrete_regions_stretched_.push_back(
          points_stretched_shell.at(index));
    }
  }

//  // sort points according to strain value
//  Dim sort_collection As SortCollection
//  Set sort_collection = libBase.NewSortCollection()
//  sort_collection.SortPoints2dAscending points_discrete_regions_unstretched_, _
//                                        "X"
//  sort_collection.SortPoints2dAscending points_discrete_regions_stretched_, _
//                                        "X"

  // re-calculate loads using total load function
  for (unsigned int index = 1; index <= 6; index++) {

    /// \todo this should use iterators and local objects to modify vector
    ///    contents
    points_discrete_regions_unstretched_.at(index).y =
        LoadTotal(points_discrete_regions_unstretched_.at(index).x, false);

    points_discrete_regions_stretched_.at(index).y =
        LoadTotal(points_discrete_regions_stretched_.at(index).x, true);
  }

  return true;
}

bool CableElongationModel::ValidateComponentsStrainLimit(
    const bool& is_included_warnings,
    std::list<std::string>* messages_error) const {

  // initialize
  bool is_valid = true;

  // exit if warnings are not included
  if (is_included_warnings == false) {
    return is_valid;
  }

  // modify temperature to reference temperature
  if (UpdateComponentsTemperature(
      cable_.temperature_properties_components) == false) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back(
          "CABLE ELONGATION MODEL - Could not verify component limits");
    }

    return is_valid;
  }

  // get strain at rated strength
  double strain_max = GetStrainTotal(cable_.strength_rated, false);

  // check core, if enabled
  if (is_enabled_core_ == true) {

    // get polynomial limit for core
    double strain_limit_polynomial_core = StrainCore(
        component_core_.component_cable().load_limit_polynomial_creep,
        false);

    // compare against max strain
    // if component limit is less that rated strength strain, generate error
    if (strain_limit_polynomial_core < strain_max) {

      is_valid = false;
      if (messages_error != nullptr) {
        messages_error->push_back(
            "CABLE ELONGATION MODEL - Core polynomial limit is less than "
            "rated strength of cable");
      }
    }

  }

  // check shell, if enabled
  if (is_enabled_shell_ == true) {

    // get polynomial limit for shell
    double strain_limit_polynomial_shell = StrainShell(
        component_shell_.component_cable().load_limit_polynomial_loadstrain,
        false);

    // compare against max strain
    // if component limit is less that rated strength strain, generate error
    if (strain_limit_polynomial_shell < strain_max) {

      is_valid = false;
      if (messages_error != nullptr) {
        messages_error->push_back(
            "CABLE ELONGATION MODEL - Shell polynomial limit is less than "
            "rated strength of cable");
      }
    }
  }

  // restore temperature
  UpdateComponentsTemperature(temperature_);

  return is_valid;
}

bool CableElongationModel::ValidateComponentsStrainUnloaded(
    const bool& is_included_warnings,
    std::list<std::string>* messages_error) const {

  // initialize
  bool is_valid = true;

  // exit if warnings are not included
  if (is_included_warnings == false) {
    return is_valid;
  }

  // determine if this check is necessary, both core and shell must be enabled
  if ((is_enabled_core_ == false) || (is_enabled_shell_ == false)) {
    return is_valid;
  }

  // modify temperature to reference temperature
  if (UpdateComponentsTemperature(
      cable_.temperature_properties_components) == false) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back(
          "CABLE ELONGATION MODEL - Could not verify component unloaded"
          " strains");
    }
    return is_valid;
  }

  // get unloaded unstretched strains for core and shell, calculte difference
  const double strain_unloaded_unstretched_core = StrainCore(0, false);
  const double strain_unloaded_unstretched_shell = StrainShell(0, false);
  const double strain_difference =
      strain_unloaded_unstretched_core - strain_unloaded_unstretched_shell;

  // compare
  if (0.0005 < abs(strain_difference)
      || ((0.0001 < abs(strain_difference))
      && (is_included_warnings == true))) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back(
          "CABLE ELONGATION MODEL - Unloaded unstretched strain difference"
          " between shell and core equals ");
          //& Round(Abs(strain_unloaded_unstretched_core _
          //            - strain_unloaded_unstretched_shell), 5))
    }
  }

  // restore temperature
  UpdateComponentsTemperature(temperature_);

  return is_valid;
}
