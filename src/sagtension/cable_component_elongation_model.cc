// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "sagtension/cable_component_elongation_model.h"

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
  if (is_stretched == false) {
    return LoadUnstretched(strain);
  } else if (is_stretched == true) {
    return LoadStretched(strain);
  }
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

  if (is_stretched == false) {
    return point_unloaded_unstretched_;
  } else if (is_stretched == true) {
    return point_unloaded_stretched_;
  }
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

double CableComponentElongationModel::SlopeTangent(
    const double& strain,
    const bool& is_stretched) const {

}

double CableComponentElongationModel::Strain(const double& load,
                                             const bool& is_stretched) const {

  // update class, if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  // select function based on cable stretched condition
  if (is_stretched == false) {
    return StrainUnstretched(load);
  } else if (is_stretched == true) {
    return StrainStretched(load);
  }
}

bool CableComponentElongationModel::Validate(
    const bool& is_included_warnings,
    std::list<std::string>* messages_error) const {

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
    }
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
    }
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

bool CableComponentElongationModel::ValidatePolynomialOrigin(
    const bool& is_included_warnings,
    std::list<std::string>* messages_error) const {

}

bool CableComponentElongationModel::ValidatePolynomialShape(
    const bool& is_included_warnings,
    std::list<std::string>* messages_error) const {

}
