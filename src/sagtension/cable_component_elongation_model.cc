// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "sagtension/cable_component_elongation_model.h"

#include "base/convert_units.h"

CableComponentElongationModel::CableComponentElongationModel() {

  load_stretch_ = -999999;
  temperature_ = -999999;
  temperature_reference_ = -999999;

  strain_thermal_ = -999999;

  is_updated_polynomial_ = false;
  is_updated_point_limit_polynomial_ = false;
  is_updated_point_stretch_ = false;
  is_updated_strain_thermal_ = false;
}

CableComponentElongationModel::~CableComponentElongationModel() {
}

double CableComponentElongationModel::Load(const double& strain,
                                           const bool& is_stretched) const {

  // update class, if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  // select function based on cable stretched condition
  double load = -999999;
  if (is_stretched == false) {
    load = LoadUnstretched(strain);
  } else if (is_stretched == true) {
    load = LoadStretched(strain);
  }

  return load;
}

Point2d CableComponentElongationModel::PointLimitPolynomial() const {

  // update class, if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return Point2d();
    }
  }

  return point_limit_polynomial_;
}

Point2d CableComponentElongationModel::PointStretch() const {

  // update class, if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return Point2d();
    }
  }

  return point_stretch_;
}

Point2d CableComponentElongationModel::PointUnloaded(
    const bool& is_stretched) const {

  // update class, if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return Point2d();
    }
  }

  Point2d point_unloaded = Point2d(-999999, -999999);
  if (is_stretched == false) {
    point_unloaded = point_unloaded_unstretched_;
  } else if (is_stretched == true) {
    point_unloaded = point_unloaded_stretched_;
  }

  return point_unloaded;
}

std::vector<Point2d> CableComponentElongationModel::PointsDiscreteRegions(
    const bool& is_stretched) const {

  // update class, if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return std::vector<Point2d>(3);
    }
  }

  std::vector<Point2d> points;
  points.push_back(PointUnloaded(is_stretched));
  points.push_back(PointLimitPolynomial());
  points.push_back(PointStretch());

  return points;
}

double CableComponentElongationModel::Slope(
    const double& strain,
    const bool& is_stretched) const {

  // update class, if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  double slope = -999999;
  if (is_stretched == false) {
    slope = SlopeUnstretched(strain, is_stretched);
  } else if (is_stretched == true) {
    slope = SlopeStretched(strain, is_stretched);
  }

  return slope;
}

double CableComponentElongationModel::Strain(const double& load,
                                             const bool& is_stretched) const {

  // update class, if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  double strain = -999999;
  if (is_stretched == false) {
    strain = StrainUnstretched(load);
  } else if (is_stretched == true) {
    strain = StrainStretched(load);
  }

  return strain;
}

bool CableComponentElongationModel::Validate(
    const bool& is_included_warnings,
    std::list<std::string>* messages_error) const {

  bool is_valid = true;

  // validate component-cable
  if (component_cable_.Validate(is_included_warnings,
                                messages_error) == false) {
    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CABLE COMPONENT ELONGATION MODEL - Invalid "
                                "cable component");
    }
  }

  // validate load-stretch
  if (load_stretch_ < 0) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CABLE COMPONENT ELONGATION MODEL - Invalid "
                                "stretch load");
    }
  }

  // validate polynomial
  if (polynomial_.coefficients().size() != 5) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CABLE COMPONENT ELONGATION MODEL - Invalid "
                                "number of polynomial coefficients");
    }
  }

  // validate temperature
  if ((temperature_ < 0)
    || ((500 < temperature_) && (is_included_warnings == true))) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CABLE COMPONENT ELONGATION MODEL - Invalid "
                                "temperature");
    }
  }

  // validate temperature - reference
  if ((temperature_reference_ < 0)
     || ((100 < temperature_reference_) && (is_included_warnings == true))) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CABLE COMPONENT ELONGATION MODEL - Invalid "
                                "reference temperature");
    }
  }

  // if all class inputs are validated, continue validation checks
  if (is_valid == true) {

    // validate if class updates
    if (Update() == false) {

      is_valid = false;
      if (messages_error != nullptr) {
          messages_error->push_back("CABLE COMPONENT ELONGATION MODEL - Error "
                                    "updating class");
      }

      return is_valid;
    }

    // validate polynomial origin
    if (ValidatePolynomialOrigin(is_included_warnings,
                                messages_error) == false) {
      is_valid = false;
    }

    // validate polynomial shape
    if (ValidatePolynomialShape(is_included_warnings,
                                messages_error) == false) {
      is_valid = false;
    }
  }

  return is_valid;
}

CableComponent CableComponentElongationModel::component_cable() const {
  return component_cable_;
}

double CableComponentElongationModel::load_stretch() const {
  return load_stretch_;
}

void CableComponentElongationModel::set_component_cable(
    const CableComponent& component_cable) {

  component_cable_ = component_cable;

  is_updated_strain_thermal_ = false;
  is_updated_point_limit_polynomial_ = false;
  is_updated_point_stretch_ = false;
}

void CableComponentElongationModel::set_load_stretch(
    const double& load_stretch) {

  load_stretch_ = load_stretch;

  is_updated_point_stretch_ = false;
}

void CableComponentElongationModel::set_temperature(const double& temperature) {

  temperature_ = temperature;

  is_updated_strain_thermal_ = false;
  is_updated_point_limit_polynomial_ = false;
  is_updated_point_stretch_ = false;
}

void CableComponentElongationModel::set_temperature_reference(
    const double& temperature_reference) {

  temperature_reference_ = temperature_reference;

  is_updated_strain_thermal_ = false;
  is_updated_point_limit_polynomial_ = false;
  is_updated_point_stretch_ = false;
}

void CableComponentElongationModel::set_type_polynomial_active(
    const CablePolynomialType& type_polynomial_active) {

  type_polynomial_active_ = type_polynomial_active;

  is_updated_polynomial_ = false;
  is_updated_strain_thermal_ = false;
  is_updated_point_limit_polynomial_ = false;
  is_updated_point_stretch_ = false;
}

double CableComponentElongationModel::temperature() const {
  return temperature_;
}

double CableComponentElongationModel::temperature_reference() const {
  return temperature_reference_;
}

CablePolynomialType CableComponentElongationModel::type_polynomial_active()
    const {
  return type_polynomial_active_;
}

double CableComponentElongationModel::ConvertToPercentStrainPolynomial(
    const double& strain) const {

  // shift strain at component temperature to reference temperature to align
  // with polynomial
  // multiply to convert to % strain
  return (strain - strain_thermal_) * 100;
}

double CableComponentElongationModel::ConvertToStrain(
    const double& percent_strain_polynomial) const {

  // shift percent strain at polynomial reference temperature to component
  // temperature, then divide to convert to strain
  return (percent_strain_polynomial / 100) + strain_thermal_;
}

bool CableComponentElongationModel::IsUpdated() const {

  if ((is_updated_point_limit_polynomial_ == true)
      && (is_updated_point_stretch_ == true)
      && (is_updated_strain_thermal_ == true)) {

    return true;
  } else {
    return false;
  }
}

double CableComponentElongationModel::LoadCompression(
    const double& strain,
    const bool& is_stretched) const {

  // get the unloaded strain based on stretch condition
  double strain_unloaded = -999999;
  if (is_stretched == false) {
    strain_unloaded = point_unloaded_unstretched_.x;
  } else if (is_stretched == true) {
    strain_unloaded = point_unloaded_stretched_.x;
  }

  // calculate load using compression modulus
  return ((strain - strain_unloaded)
           * component_cable_.modulus_compression_elastic_area) + 0;
}

double CableComponentElongationModel::LoadPolynomial(
    const double& strain) const {

  // convert units to percent strain
  double percent_strain_polynomial = ConvertToPercentStrainPolynomial(strain);

  // get load from the polynomial
  return polynomial_.y(percent_strain_polynomial);
}

/// This function determines which region the strain value is in, and then
/// solves or calls the appropriate function.
double CableComponentElongationModel::LoadStretched(
    const double& strain) const {

  // strain is less than unloaded stretched strain
  if (strain < point_unloaded_stretched_.x) {

    // calculate load using compression modulus
    return LoadCompression(strain, true);

  // strain is between unloaded stretched point strain and stretch point strain
  } else if ((point_unloaded_stretched_.x <= strain)
      && (strain <= point_stretch_.x)) {

    // use stretch point and unload using tension elastic area modulus
    return point_stretch_.y - ((point_stretch_.x - strain)
              * component_cable_.modulus_tension_elastic_area);

  // strain is above the stretch point
  } else if (point_stretch_.x < strain) {

    // cable is effectively unstretched at this strain
    return LoadUnstretched(strain);

  } else {
    return -999999;
  }
}

/// This function determines which region the strain value is in, and then
/// solves or calls the appropriate function.
double CableComponentElongationModel::LoadUnstretched(
    const double& strain) const {

  // strain is less than unloaded unstretched strain
  if (strain < point_unloaded_unstretched_.x) {

    // calculate load using compression modulus
    return LoadCompression(strain, false);

  // strain is equal to the unloaded unstretched strain
  } else if (strain == point_unloaded_unstretched_.x) {

    return 0;

  // strain is above the unload unstretched strain
  } else if (point_unloaded_unstretched_.x < strain) {

    // strain is less than strain limit
    if (strain <= point_limit_polynomial_.x) {

      // get load from the polynomial
      return LoadPolynomial(strain);

    // strain is above the limit point strain
    } else if (point_limit_polynomial_.x < strain) {

      // get load by extending tension modulus from maximum point
      // stretch doesn't matter because extrapolated line and tension modulus
      // line are equal
      return point_limit_polynomial_.y + ((strain - point_limit_polynomial_.x)
              * component_cable_.modulus_tension_elastic_area);

    } else {
      return -999999;
    }
  } else {
    return -999999;
  }
}

double CableComponentElongationModel::SlopeStretched(
    const double& strain,
    const bool& is_stretched) const {

  // strain is less than unloaded stretched strain
  if (strain < point_unloaded_stretched_.x) {

    return component_cable_.modulus_compression_elastic_area;

  } else if (strain < point_unloaded_stretched_.x) {

    if (strain < point_stretch_.x) {

      return component_cable_.modulus_tension_elastic_area;

    } else {

      // cable is effectively ustretched at this strain
      return SlopeUnstretched(strain, is_stretched);
    }
  } else {
    return -999999;
  }
}

double CableComponentElongationModel::SlopeUnstretched(
    const double& strain,
    const bool& is_stretched) const {

  // strain is less than unloaded unstretched strain
  if (strain < point_unloaded_unstretched_.x) {

    // slope is equal to component compression modulus
    return component_cable_.modulus_compression_elastic_area;

  // strain is greather than unloaded unstretched strain
  } else if (point_unloaded_unstretched_.x < strain) {

    // strain is less than polynomial limit
    if (strain <= point_limit_polynomial_.x) {

      // adjust the strain (x-value) to align with the unshifted polynomial
      // convert units to percent strain
      double percent_strain_polynomial =
          ConvertToPercentStrainPolynomial(strain);

      // get the slope tangent from the polynomial
      // multiply to convert to load/strain slope
      return polynomial_.Slope(percent_strain_polynomial) * 100;

    } else if (point_limit_polynomial_.x < strain) {

      // slope is equal to tension elastic modulus
      return component_cable_.modulus_tension_elastic_area;
    } else {
      return -999999;
    }
  } else {
    return -999999;
  }
}

double CableComponentElongationModel::StrainCompression(
    const double& load,
    const double& is_stretched) const {

  // get the unloaded strain based on stretch condition
  double strain_unloaded = -999999;
  if (is_stretched == false) {
    strain_unloaded = point_unloaded_unstretched_.x;
  } else if (is_stretched == true) {
    strain_unloaded = point_unloaded_stretched_.x;
  }

  // calculate strain using compression modulus
  return strain_unloaded - (0 - load)
          / component_cable_.modulus_compression_elastic_area;
}

double CableComponentElongationModel::StrainPolynomial(
    const double& load) const {

  // get raw strain from creep polynomial
  // calculate strain percent from polynomial
  const double percent_strain_polynomial = polynomial_.x(
      load,  // target value
      2,     // precision
      0.1);  // initial guess

  // convert polynomial percent strain at reference temperature to strain at
  // component temperature
  return ConvertToStrain(percent_strain_polynomial);
}

/// This function determines which region the load value is in, and then
/// solves or calls the appropriate function.
double CableComponentElongationModel::StrainStretched(
    const double& load) const {

  // load is less than zero
  if (load < 0) {

    // calculate strain using compression modulus
    return StrainCompression(load, true);

  // load is between zero and stretch point load
  } else if ((0 <= load) && (load <= point_stretch_.y)) {

    // use stretch point and unload using tension elastic area modulus
    return point_stretch_.x - ((point_stretch_.y - load)
             / component_cable_.modulus_tension_elastic_area);

  // load is above the stretch point
  } else if (point_stretch_.y < load) {

    // cable is effectively unstretched at this load
    return StrainUnstretched(load);

  } else {
    return -999999;
  }
}

/// This function determines which region the load value is in, and then
/// solves or calls the appropriate function.
double CableComponentElongationModel::StrainUnstretched(
    const double& load) const {

  // load is less than zero
  if (load < 0) {

    // calculate strain using compression modulus
    return StrainCompression(load, false);

  // load is equal to zero
  } else if (load == 0) {

    // get strain from cached point
    return point_unloaded_unstretched_.x;

  } else if (0 < load) {

    // load is less than or equal to polynomial limit
    if (load <= point_limit_polynomial_.y) {

      // get strain from polynomial
      return StrainPolynomial(load);

    // load exceeds polynomial limit
    } else if (point_limit_polynomial_.y < load) {

      // get strain by extending tension modulus from maximum point
      // stretch doesn't matter because extrapolated line and tension modulus
      // line are equal
      return point_limit_polynomial_.x + (load - point_limit_polynomial_.y)
              / component_cable_.modulus_tension_elastic_area;

    } else {
      return -999999;
    }
  } else {
    return -999999;
  }
}

bool CableComponentElongationModel::Update() const {

  // update polynomial
  if (is_updated_polynomial_ == false) {

    is_updated_polynomial_ = UpdatePolynomial();
    if (is_updated_polynomial_ == false) {
      return false;
    }
  }

  // update strain-thermal
  if (is_updated_strain_thermal_ == false) {

    // update strain-thermal
    is_updated_strain_thermal_ = UpdateStrainThermal();
    if (is_updated_strain_thermal_ == false) {
      return false;
    }

    // update point-unloaded-unstretched
    is_updated_strain_thermal_ = UpdatePointUnloadedUnstretched();
    if (is_updated_strain_thermal_ == false) {
      return false;
    }
  }

  // update polynomial limit point
  if (is_updated_point_limit_polynomial_ == false) {

    is_updated_point_limit_polynomial_ = UpdatePointLimitPolynomial();
    if (is_updated_point_limit_polynomial_ == false) {
      return false;
    }
  }

  // update stretch
  if (is_updated_point_stretch_ == false) {

    // update stretch point
    is_updated_point_stretch_ = UpdatePointStretch();
    if (is_updated_point_stretch_ == false) {
      return false;
    }

    // update unloaded stretched point
    is_updated_point_stretch_ = UpdatePointUnloadedStretched();
    if (is_updated_point_stretch_ == false) {
      return false;
    }
  }

  // if it reaches this point, update was successful
  return true;
}

bool CableComponentElongationModel::UpdatePointLimitPolynomial() const {

  // update polynomial limit point
  if (type_polynomial_active_ == CablePolynomialType::kCreep) {
    point_limit_polynomial_.y = component_cable_.load_limit_polynomial_creep;
  } else if (type_polynomial_active_ == CablePolynomialType::kLoadStrain) {
    point_limit_polynomial_.y =
        component_cable_.load_limit_polynomial_loadstrain;
  }

  point_limit_polynomial_.x = StrainPolynomial(point_limit_polynomial_.y);

  // return update status
  return true;
}

bool CableComponentElongationModel::UpdatePointStretch() const {

  // solve for the stretch point
  point_stretch_.y = load_stretch_;
  point_stretch_.x = StrainUnstretched(point_stretch_.y);

  // return update status
  return true;
}

bool CableComponentElongationModel::UpdatePointUnloadedStretched() const {

  // get strain at zero load - stretched
  point_unloaded_stretched_.y = 0;
  point_unloaded_stretched_.x = point_stretch_.x
      - (point_stretch_.y - 0) / component_cable_.modulus_tension_elastic_area;

  // return update status
  return true;
}

bool CableComponentElongationModel::UpdatePointUnloadedUnstretched() const {

  // solve for the unloaded unstretched point
  point_unloaded_unstretched_.y = 0;
  point_unloaded_unstretched_.x = StrainPolynomial(
                                     point_unloaded_unstretched_.y);

  // return update status
  return true;
}

bool CableComponentElongationModel::UpdatePolynomial() const {

  if (type_polynomial_active_ == CablePolynomialType::kCreep) {
    polynomial_.set_coefficients(
        component_cable_.coefficients_polynomial_creep);
  } else if (type_polynomial_active_ == CablePolynomialType::kLoadStrain) {
    polynomial_.set_coefficients(
        component_cable_.coefficients_polynomial_loadstrain);
  }

  // return update status
  return true;
}

bool CableComponentElongationModel::UpdateStrainThermal() const {

  // use the thermal coefficient and temperature gradient to calculate thermal
  //strain
  strain_thermal_ = component_cable_.coefficient_expansion_linear_thermal
                    * (temperature_ - temperature_reference_);

  // return update status
  return true;
}

/// \todo
/// This function needs to be updated so it does not reference any mutators.
/// The way it is written right now messes up the const-ness during validation.
bool CableComponentElongationModel::ValidatePolynomialOrigin(
    const bool& is_included_warnings,
    std::list<std::string>* messages_error) const {

  // initialize
  bool is_valid = true;
//
//  // exit if warnings are not included
//  if (is_included_warnings == false) {
//    return is_valid;
//  }
//
//  // cache current temperature
//  const double temperature_component = temperature_;
//
//  // set component temperature to reference temperature and update
//  set_temperature(temperature_reference_);
//  if (Update() == false) {
//    is_valid = false;
//    set_temperature(temperature_component);
//    return is_valid;
//  }
//
//  // validate polynomial origin
//  double strain_polynomial_unloaded = point_unloaded_unstretched_.x;
//
//  if ((strain_polynomial_unloaded < -0.001)
//      || ((strain_polynomial_unloaded < -0.0001)
//      && (is_included_warnings == true))
//      || (0.001 < strain_polynomial_unloaded)
//      || ((0.0001 < strain_polynomial_unloaded)
//      && (is_included_warnings == true))) {
//
//    is_valid = false;
//    if (messages_error != nullptr) {
//      messages_error->push_back("CABLE COMPONENT ELONGATION MODEL - Polynomial "
//                                "origin has a significant non-zero strain "
//                                "value which equals "
//                                supportfunctions::Round(
//                                    strain_polynomial_unloaded, 4));
//    }
//  }
//
//  // reset component temperature
//  set_temperature(temperature_);

  return is_valid;
}

bool CableComponentElongationModel::ValidatePolynomialShape(
    const bool& is_included_warnings,
    std::list<std::string>* messages_error) const {

  // initialize
  bool is_valid = true;

  // exit if warnings are not included
  if (is_included_warnings == false) {
    return is_valid;
  }

  // initialize iteration and calculation variables
  const double strain_polynomial_min = point_unloaded_unstretched_.x;
  const double strain_polynomial_max = point_limit_polynomial_.x;

  // search polynomial for validation errors
  for (int iter = 0; iter <-50; iter++) {

    // calculate an evenly spaced strain values from minimum to maximum values
    const double strain = strain_polynomial_min + ((iter / 50)
                          * (strain_polynomial_max - strain_polynomial_min));

    // calculate slope at strain point
    const double slope = Slope(strain, false);

    // check if polynomial slope is negative
    if (slope < 0) {

      is_valid = false;
      if (messages_error != nullptr) {
        messages_error->push_back(
            "CABLE COMPONENT ELONGATION MODEL - Slope of polynomial tangent is "
            "negative at strain = ");
            // std::to_string(supportfunctions::Round(strain, 4)));
      }
    }

    // check if polynomial slope exceeds elastic area modulus
    if ((component_cable_.modulus_tension_elastic_area < slope)
        && (is_included_warnings == true)) {

      is_valid = false;
      if (messages_error != nullptr) {
        messages_error->push_back(
            "CABLE COMPONENT ELONGATION MODEL - Slope of polynomial tangent is "
            " greater than the tension elastic area modulus at strain = ");
            // std::to_string(supportfunctions::Round(strain, 4)));
      }
    }
  }

  return is_valid;
}
