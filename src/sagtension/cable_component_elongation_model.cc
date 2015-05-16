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
  is_updated_regions_ = false;
}

CableComponentElongationModel::~CableComponentElongationModel() {
}

double CableComponentElongationModel::Load(const double& strain) const {

  // update class, if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  // selects region based on strain value
  double load = -999999;
  if (strain < point_unloaded_.x) {

    load = LoadCompressed(strain);

  } else if ((point_unloaded_.x <= strain)
      && (strain <= point_polynomial_start_.x)) {

    load = LoadStretched(strain);

  } else if ((point_polynomial_start_.x < strain)
      && (strain < point_polynomial_end_.x)) {

    load = LoadPolynomial(strain);

  } else if (point_polynomial_end_.x <= strain) {

    load = LoadExtrapolated(strain);
  }

  return load;
}

Point2d CableComponentElongationModel::PointPolynomialEnd() const {

  // updates class if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return Point2d();
    }
  }

  return point_polynomial_end_;
}

Point2d CableComponentElongationModel::PointPolynomialStart() const {

  // updates class if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return Point2d();
    }
  }

  return point_polynomial_start_;
}

std::vector<Point2d> CableComponentElongationModel::PointsRegions() const {

  // updates class if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return std::vector<Point2d>(3);
    }
  }

  std::vector<Point2d> points;
  points.push_back(PointUnloaded());
  points.push_back(PointPolynomialStart());
  points.push_back(PointPolynomialEnd());

  return points;
}

Point2d CableComponentElongationModel::PointUnloaded() const {

  // updates class if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return Point2d();
    }
  }

  return point_unloaded_;
}

double CableComponentElongationModel::Slope(const double& strain) const {

  // updates class if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  // selects region based on strain value
  double slope = -999999;
  if (strain < point_unloaded_.x) {

    slope = SlopeCompressed(strain);

  } else if ((point_unloaded_.x <= strain)
      && (strain <= point_polynomial_start_.x)) {

    slope = SlopeStretched(strain);

  } else if ((point_polynomial_start_.x < strain)
      && (strain < point_polynomial_end_.x)) {

    slope = SlopePolynomial(strain);

  } else if (point_polynomial_end_.x <= strain) {

    slope = SlopeExtrapolated(strain);
  }

  return slope;
}

double CableComponentElongationModel::Strain(const double& load) const {

  // updates class if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  // selects region based on load value
  double strain = -999999;
  if (load < point_unloaded_.y) {

    strain = StrainCompressed(load);

  } else if ((point_unloaded_.y <= load)
      && (load <= point_polynomial_start_.y)) {

    strain = StrainStretched(load);

  } else if ((point_polynomial_start_.y < load)
      && (load < point_polynomial_end_.y)) {

    strain = StrainPolynomial(load);

  } else if (point_polynomial_end_.y <= load) {

    strain = StrainExtrapolated(load);
  }

  return strain;
}

double CableComponentElongationModel::StrainThermal() const {

  // updates class if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return strain_thermal_;
}

bool CableComponentElongationModel::Validate(
    const bool& is_included_warnings,
    std::list<std::string>* messages_error) const {

  bool is_valid = true;

  // validates component-cable
  if (component_cable_.Validate(is_included_warnings,
                                messages_error) == false) {
    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CABLE COMPONENT ELONGATION MODEL - Invalid "
                                "cable component");
    }
  }

  // validates load-stretch
  if (load_stretch_ < 0) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CABLE COMPONENT ELONGATION MODEL - Invalid "
                                "stretch load");
    }
  }

  // validates temperature
  if ((temperature_ < 0)
    || ((500 < temperature_) && (is_included_warnings == true))) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CABLE COMPONENT ELONGATION MODEL - Invalid "
                                "temperature");
    }
  }

  // validates temperature-reference
  if ((temperature_reference_ < 0)
     || ((100 < temperature_reference_) && (is_included_warnings == true))) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CABLE COMPONENT ELONGATION MODEL - Invalid "
                                "reference temperature");
    }
  }

  // if all class inputs are validated, continues validation checks
  if (is_valid == true) {

    // validates if class updates
    if (Update() == false) {

      is_valid = false;
      if (messages_error != nullptr) {
          messages_error->push_back("CABLE COMPONENT ELONGATION MODEL - Error "
                                    "updating class");
      }

      return is_valid;
    }

    // validates polynomial coefficients
    if (polynomial_.OrderMax() != 4) {

      is_valid = false;
      if (messages_error != nullptr) {
        messages_error->push_back("CABLE COMPONENT ELONGATION MODEL - Invalid "
                                  "number of polynomial coefficients");
      }
    }

    // validates polynomial origin
    if (ValidatePolynomialOrigin(is_included_warnings,
                                messages_error) == false) {
      is_valid = false;
    }

    // validates polynomial shape
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

  is_updated_polynomial_ = false;
  is_updated_regions_ = false;
}

void CableComponentElongationModel::set_load_stretch(
    const double& load_stretch) {

  load_stretch_ = load_stretch;

  is_updated_regions_ = false;
}

void CableComponentElongationModel::set_temperature(const double& temperature) {

  temperature_ = temperature;

  is_updated_regions_ = false;
}

void CableComponentElongationModel::set_temperature_reference(
    const double& temperature_reference) {

  temperature_reference_ = temperature_reference;

  is_updated_regions_ = false;
}

void CableComponentElongationModel::set_type_polynomial_active(
    const CablePolynomialType& type_polynomial_active) {

  type_polynomial_active_ = type_polynomial_active;

  is_updated_polynomial_ = false;
  is_updated_regions_ = false;
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

  // shifts strain at component temperature to reference temperature to align
  // with polynomial
  // multiplies to convert to % strain
  return (strain - strain_thermal_) * 100;
}

double CableComponentElongationModel::ConvertToStrain(
    const double& percent_strain_polynomial) const {

  // shifts percent strain at polynomial reference temperature to component
  // temperature, then divides to convert to strain
  return (percent_strain_polynomial / 100) + strain_thermal_;
}

bool CableComponentElongationModel::IsUpdated() const {

  if ((is_updated_polynomial_ == true)
      && (is_updated_regions_ == true)) {
    return true;
  } else {
    return false;
  }
}

double CableComponentElongationModel::LoadCompressed(
    const double& strain) const {

  // calculates load using compression modulus
  return ((strain - point_unloaded_.x)
           * component_cable_.modulus_compression_elastic_area) + 0;
}

double CableComponentElongationModel::LoadExtrapolated(
    const double& strain) const {

  // gets load by extending tension modulus from maximum point
  return point_polynomial_end_.y + ((strain - point_polynomial_end_.x)
          * component_cable_.modulus_tension_elastic_area);
}

double CableComponentElongationModel::LoadPolynomial(
    const double& strain) const {

  // converts units to percent strain
  double percent_strain_polynomial = ConvertToPercentStrainPolynomial(strain);

  // gets load from the polynomial
  return polynomial_.Y(percent_strain_polynomial);
}

double CableComponentElongationModel::LoadStretched(
    const double& strain) const {

  // uses polynomial start point and unload using tension elastic area modulus
  return point_polynomial_start_.y - ((point_polynomial_start_.x - strain)
          * component_cable_.modulus_tension_elastic_area);
}

double CableComponentElongationModel::SlopeCompressed(
    const double& strain) const {

  return component_cable_.modulus_compression_elastic_area;
}

double CableComponentElongationModel::SlopeExtrapolated(
    const double& strain) const {

  return component_cable_.modulus_tension_elastic_area;
}

double CableComponentElongationModel::SlopePolynomial(
    const double& strain) const {

  // adjusts the strain (x-value) to align with the unshifted polynomial
  double percent_strain_polynomial = ConvertToPercentStrainPolynomial(strain);

  // gets the slope tangent from the polynomial
  // multiplies to convert to load/strain slope
  return polynomial_.Slope(percent_strain_polynomial) * 100;
}

double CableComponentElongationModel::SlopeStretched(
    const double& strain) const {

  return component_cable_.modulus_tension_elastic_area;
}

double CableComponentElongationModel::StrainCompressed(
    const double& load) const {

  // calculate strain using compression modulus
  return point_unloaded_.x - (0 - load)
          / component_cable_.modulus_compression_elastic_area;
}

double CableComponentElongationModel::StrainExtrapolated(
    const double& load) const {

  // gets strain by extending tension modulus from polynomial end point
  return point_polynomial_end_.x + ((load - point_polynomial_end_.y)
          / component_cable_.modulus_tension_elastic_area);
}

double CableComponentElongationModel::StrainPolynomial(
    const double& load) const {

  // gets raw strain from polynomial
  const double percent_strain_polynomial = polynomial_.X(
      load,  // target value
      2,     // precision
      0.1);  // initial guess

  // converts polynomial percent strain at reference temperature to strain at
  // component temperature
  return ConvertToStrain(percent_strain_polynomial);
}

double CableComponentElongationModel::StrainStretched(
    const double& load) const {

  // uses polynomial start point and unload using tension elastic area modulus
  return point_polynomial_start_.x - ((point_polynomial_start_.y - load)
           / component_cable_.modulus_tension_elastic_area);
}

bool CableComponentElongationModel::Update() const {

  // updates polynomial
  if (is_updated_polynomial_ == false) {

    is_updated_polynomial_ = UpdatePolynomial();
    if (is_updated_polynomial_ == false) {
      return false;
    }
  }

  // updates thermal shift and regions
  if (is_updated_regions_ == false) {

    is_updated_regions_ = UpdateRegions(temperature_);
    if (is_updated_regions_ == false) {
      return false;
    }
  }

  // if it reaches this point, update was successful
  return true;
}

bool CableComponentElongationModel::UpdatePointPolynomialEnd() const {

  if (type_polynomial_active_ == CablePolynomialType::kCreep) {
    point_polynomial_end_.y = component_cable_.load_limit_polynomial_creep;
  } else if (type_polynomial_active_ == CablePolynomialType::kLoadStrain) {
    point_polynomial_end_.y =
        component_cable_.load_limit_polynomial_loadstrain;
  }

  point_polynomial_end_.x = StrainPolynomial(point_polynomial_end_.y);

  return true;
}

bool CableComponentElongationModel::UpdatePointPolynomialStart() const {

  point_polynomial_start_.y = load_stretch_;
  point_polynomial_start_.x = StrainPolynomial(point_polynomial_start_.y);

  return true;
}

bool CableComponentElongationModel::UpdatePointUnloaded() const {

  point_unloaded_.y = 0;
  point_unloaded_.x = StrainStretched(point_unloaded_.y);

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

  return true;
}

bool CableComponentElongationModel::UpdateRegions(
    const double& temperature) const {

  // updates strain-thermal
  is_updated_regions_ = UpdateStrainThermal(temperature);
  if (is_updated_regions_ == false) {
    return false;
  }

  // updates polynomial end point
  is_updated_regions_ = UpdatePointPolynomialEnd();
  if (is_updated_regions_ == false) {
    return false;
  }

  // updates polynomial start point
  is_updated_regions_ = UpdatePointPolynomialStart();
  if (is_updated_regions_ == false) {
    return false;
  }

  // updates unloaded point
  is_updated_regions_ = UpdatePointUnloaded();
  if (is_updated_regions_ == false) {
    return false;
  }

  return true;
}

bool CableComponentElongationModel::UpdateStrainThermal(
    const double& temperature) const {

  // uses the thermal coefficient and temperature gradient to calculate thermal
  // strain
  strain_thermal_ = component_cable_.coefficient_expansion_linear_thermal
                    * (temperature_ - temperature_reference_);

  return true;
}

bool CableComponentElongationModel::ValidatePolynomialOrigin(
    const bool& is_included_warnings,
    std::list<std::string>* messages_error) const {

  bool is_valid = true;

  // exits if warnings are not included
  if (is_included_warnings == false) {
    return is_valid;
  }

  // caches current temperature, sets component temperature to reference and
  // updates
  const double temperature_component = temperature_;

  if (UpdateRegions(temperature_reference_) == false) {
    is_valid = false;
    UpdateRegions(temperature_component);
    return is_valid;
  }

  // validate polynomial origin
  double strain_polynomial_unloaded = StrainPolynomial(0);

  if ((strain_polynomial_unloaded < -0.001)
      || ((strain_polynomial_unloaded < -0.0001)
      && (is_included_warnings == true))
      || (0.001 < strain_polynomial_unloaded)
      || ((0.0001 < strain_polynomial_unloaded)
      && (is_included_warnings == true))) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CABLE COMPONENT ELONGATION MODEL - Polynomial "
                                "origin has a significant non-zero strain ");
    }
  }

  // resets component temperature
  UpdateRegions(temperature_);

  return is_valid;
}

bool CableComponentElongationModel::ValidatePolynomialShape(
    const bool& is_included_warnings,
    std::list<std::string>* messages_error) const {

  bool is_valid = true;

  // exits if warnings are not included
  if (is_included_warnings == false) {
    return is_valid;
  }

  // caches current temperature, sets component temperature to reference and
  // updates
  const double temperature_component = temperature_;

  if (UpdateRegions(temperature_reference_) == false) {
    is_valid = false;
    UpdateRegions(temperature_component);
    return is_valid;
  }

  // initialize iteration and calculation variables
  const double strain_polynomial_min = StrainPolynomial(0);
  const double strain_polynomial_max = point_polynomial_end_.x;

  // search polynomial for validation errors
  for (int iter = 0; iter <-50; iter++) {

    // calculate an evenly spaced strain values from minimum to maximum values
    const double strain = strain_polynomial_min + ((iter / 50)
                          * (strain_polynomial_max - strain_polynomial_min));

    // calculate slope at strain point
    const double slope = SlopePolynomial(strain);

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
