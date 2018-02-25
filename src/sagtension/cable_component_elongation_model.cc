// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/sagtension/cable_component_elongation_model.h"

#include <cmath>

#include "models/base/helper.h"

CableComponentElongationModel::CableComponentElongationModel() {
  component_ = nullptr;
  state_ = nullptr;
  state_stretch_ = nullptr;
  temperature_reference_ = nullptr;

  load_stretch_ = -999999;
  modulus_extrapolate_creep_ = -999999;
  modulus_extrapolate_loadstrain_ = -999999;
  strain_thermal_state_ = -999999;
  strain_thermal_stretch_ = -999999;

  is_updated_state_ = false;
  is_updated_stretch_ = false;
}

CableComponentElongationModel::~CableComponentElongationModel() {
}

double CableComponentElongationModel::Load(const double& strain) const {
  // updates class if necessary
  if ((IsUpdated() == false) && (Update() == false)) {
    return -999999;
  }

  // returns load for state model
  return Load(state_->type_polynomial, points_state_, strain_thermal_state_,
              strain);
}

std::vector<Point2d<double>> CableComponentElongationModel::PointsRegions()
    const {
  // updates class if necessary
  if ((IsUpdated() == false) && (Update() == false)) {
    return std::vector<Point2d<double>>(3);
  }

  return points_state_;
}

double CableComponentElongationModel::Slope(const double& strain) const {
  // updates class if necessary
  if ((IsUpdated() == false) && (Update() == false)) {
    return -999999;
  }

  // returns slope for state model
  return Slope(state_->type_polynomial, points_state_, strain_thermal_state_,
               strain);
}

double CableComponentElongationModel::Strain(const double& load) const {
  // updates class if necessary
  if ((IsUpdated() == false) && (Update() == false)) {
    return -999999;
  }

  // returns strain for state model
  return Strain(state_->type_polynomial, points_state_, strain_thermal_state_,
                load);
}

double CableComponentElongationModel::StrainThermal() const {
  // updates class if necessary
  if ((IsUpdated() == false) && (Update() == false)) {
    return -999999;
  }

  return strain_thermal_state_;
}

bool CableComponentElongationModel::Validate(
    const bool& is_included_warnings,
    std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "CABLE COMPONENT ELONGATION MODEL";

  // validates component
  if (component_ == nullptr) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid cable component";
      messages->push_back(message);
    }
  } else {
    if (component_->Validate(is_included_warnings, messages) == false) {
      is_valid = false;
    }
  }

  // validates state
  if (state_ == nullptr) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid state";
      messages->push_back(message);
    }
  } else {
    if (state_->Validate(is_included_warnings, messages) == false) {
      is_valid = false;
    }
  }

  // validates state-stretch
  if (state_stretch_ == nullptr) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid stretch state";
      messages->push_back(message);
    }
  } else {
    if (state_stretch_->Validate(is_included_warnings, messages) == false) {
      is_valid = false;
    }
  }

  // validates temperature-reference
  if ((temperature_reference_ == nullptr) || (*temperature_reference_ < -100)) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid reference temperature";
      messages->push_back(message);
    }
  }

  // returns if errors are present
  if (is_valid == false) {
    return false;
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

  // validates creep polynomial coefficients
  if (polynomial_creep_.OrderMax() != 4) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid number of creep polynomial coefficients";
      messages->push_back(message);
    }
  }

  // validates load-strain polynomial coefficients
  if (polynomial_loadstrain_.OrderMax() != 4) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid number of load-strain polynomial "
                            "coefficients";
      messages->push_back(message);
    }
  }

  // validates polynomial origin
  if (ValidatePolynomialOrigin(is_included_warnings,
                               messages) == false) {
    is_valid = false;
  }

  // validates polynomial shape
  if (ValidatePolynomialShape(is_included_warnings,
                              messages) == false) {
    is_valid = false;
  }

  return is_valid;
}

const SagTensionCableComponent*
    CableComponentElongationModel::component_cable() const {
  return component_;
}

void CableComponentElongationModel::set_component_cable(
    const SagTensionCableComponent* component_cable) {
  component_ = component_cable;

  // initializes polynomials
  if (component_ != nullptr) {
    const std::vector<double>* coefficients = nullptr;

    // creep polynomial
    coefficients = component_->coefficients_polynomial(
        SagTensionCableComponent::PolynomialType::kCreep);
    polynomial_creep_.set_coefficients(coefficients);

    // loadstrain polynomial
    coefficients = component_->coefficients_polynomial(
        SagTensionCableComponent::PolynomialType::kLoadStrain);
    polynomial_loadstrain_.set_coefficients(coefficients);

    // updates extrapolation slopes
    UpdateModulusExtrapolate();
  }

  is_updated_stretch_ = false;
  is_updated_state_ = false;
}

void CableComponentElongationModel::set_state(const CableState* state) {
  state_ = state;

  is_updated_state_ = false;
}

void CableComponentElongationModel::set_state_stretch(
    const CableStretchState* state_stretch) {
  state_stretch_ = state_stretch;

  is_updated_stretch_ = false;
  is_updated_state_ = false;
}

const CableState* CableComponentElongationModel::state() const {
  return state_;
}

const CableStretchState* CableComponentElongationModel::state_stretch() const {
  return state_stretch_;
}

void CableComponentElongationModel::set_temperature_reference(
    const double* temperature_reference) {
  temperature_reference_ = temperature_reference;

  is_updated_stretch_ = false;
  is_updated_state_ = false;
}

const double* CableComponentElongationModel::temperature_reference() const {
  return temperature_reference_;
}

double CableComponentElongationModel::ConvertToPercentStrainPolynomial(
    const double& strain,
    const double& strain_thermal) const {
  // shifts strain at component temperature to reference temperature to align
  // with polynomial
  // multiplies to convert to % strain
  return (strain - strain_thermal) * 100;
}

double CableComponentElongationModel::ConvertToStrain(
    const double& percent_strain_polynomial,
    const double& strain_thermal) const {
  // shifts percent strain at polynomial reference temperature to component
  // temperature, then divides to convert to strain
  return (percent_strain_polynomial / 100) + strain_thermal;
}

bool CableComponentElongationModel::IsUpdated() const {
  return (is_updated_state_ == true) && (is_updated_stretch_ == true);
}

double CableComponentElongationModel::Load(
    const SagTensionCableComponent::PolynomialType& type_polynomial,
    const std::vector<Point2d<double>>& points,
    const double& strain_thermal,
    const double& strain) const {
  double load = -999999;

  // creates friendly names for points
  const Point2d<double>& point_unloaded = points.at(0);
  const Point2d<double>& point_polynomial_start = points.at(1);
  const Point2d<double>& point_polynomial_end = points.at(2);

  // selects region based on strain value
  if (strain < point_unloaded.x) {
    // compressed region
    load = LoadLinearExtension(point_unloaded,
                               *component_->modulus_compression_elastic_area(),
                               strain);
  } else if ((point_unloaded.x <= strain)
      && (strain <= point_polynomial_start.x)) {
    // stretched region
    load = LoadLinearExtension(point_polynomial_start,
                               *component_->modulus_tension_elastic_area(),
                               strain);
  } else if ((point_polynomial_start.x < strain)
      && (strain < point_polynomial_end.x)) {
    // polynomial region
    const Polynomial* polynomial = nullptr;
    if (type_polynomial == SagTensionCableComponent::PolynomialType::kCreep) {
      polynomial = &polynomial_creep_;
    } else if (type_polynomial ==
        SagTensionCableComponent::PolynomialType::kLoadStrain) {
      polynomial = &polynomial_loadstrain_;
    } else {
      return -999999;
    }

    load = LoadPolynomial(*polynomial, strain_thermal, strain);
  } else if (point_polynomial_end.x <= strain) {
    // extrapolated region
    const double* modulus = nullptr;
    if (type_polynomial == SagTensionCableComponent::PolynomialType::kCreep) {
      modulus = &modulus_extrapolate_creep_;
    } else if (type_polynomial ==
        SagTensionCableComponent::PolynomialType::kLoadStrain) {
      modulus = &modulus_extrapolate_loadstrain_;
    } else {
      return -999999;
    }

    load = LoadLinearExtension(point_polynomial_end, *modulus, strain);
  }

  return load;
}

double CableComponentElongationModel::LoadLinearExtension(
    const Point2d<double>& point,
    const double& slope,
    const double& strain) const {
  // gets load by extending line from point
  return point.y + ((strain - point.x) * slope);
}

double CableComponentElongationModel::LoadPolynomial(
    const Polynomial& polynomial,
    const double& strain_thermal,
    const double& strain) const {
  // converts units to percent strain
  double percent_strain_polynomial =
      ConvertToPercentStrainPolynomial(strain,
                                       strain_thermal);

  // gets load from the polynomial
  return polynomial.Y(percent_strain_polynomial);
}

void CableComponentElongationModel::PointsRegions(
    const Polynomial& polynomial,
    const double& temperature,
    const double& load_limit_polynomial,
    const double& load_stretch,
    std::vector<Point2d<double>>& points) const {
  // initializes vector
  points.clear();

  // calculates the thermal strain
  const double strain_thermal = StrainThermal(temperature);

  // solves for the polynomial end point
  Point2d<double> point_polynomial_end;
  point_polynomial_end.y = load_limit_polynomial;
  point_polynomial_end.x = StrainPolynomial(polynomial, strain_thermal,
                                            point_polynomial_end.y);

  // solves for the polynomial start point
  Point2d<double> point_polynomial_start;
  point_polynomial_start.y = load_stretch;
  point_polynomial_start.x = StrainPolynomial(polynomial, strain_thermal,
                                              point_polynomial_start.y);

  // solves for unloaded point
  Point2d<double> point_unloaded;
  point_unloaded.y = 0;
  point_unloaded.x = StrainLinearExtension(
      point_polynomial_start,
      *component_->modulus_tension_elastic_area(),
      point_unloaded.y);

  // adds points to vector
  points.push_back(point_unloaded);
  points.push_back(point_polynomial_start);
  points.push_back(point_polynomial_end);
}

double CableComponentElongationModel::Slope(
    const SagTensionCableComponent::PolynomialType& type_polynomial,
    const std::vector<Point2d<double>>& points,
    const double& strain_thermal,
    const double& strain) const {
  double slope = -999999;

  // creates friendly names for points
  const Point2d<double>& point_unloaded = points.at(0);
  const Point2d<double>& point_polynomial_start = points.at(1);
  const Point2d<double>& point_polynomial_end = points.at(2);

  // selects region based on strain value
  if (strain < point_unloaded.x) {
    // compressed region
    slope = *component_->modulus_compression_elastic_area();
  } else if ((point_unloaded.x <= strain)
      && (strain <= point_polynomial_start.x)) {
    // stretched region
    slope = *component_->modulus_tension_elastic_area();
  } else if ((point_polynomial_start.x < strain)
      && (strain < point_polynomial_end.x)) {
    // polynomial region
    const Polynomial* polynomial = nullptr;
    if (type_polynomial == SagTensionCableComponent::PolynomialType::kCreep) {
      polynomial = &polynomial_creep_;
    } else if (type_polynomial ==
        SagTensionCableComponent::PolynomialType::kLoadStrain) {
      polynomial = &polynomial_loadstrain_;
    } else {
      return -999999;
    }

    slope = SlopePolynomial(*polynomial, strain_thermal, strain);
  } else if (point_polynomial_end.x <= strain) {
    // extrapolated region
    if (type_polynomial == SagTensionCableComponent::PolynomialType::kCreep) {
      slope = modulus_extrapolate_creep_;
    } else if (type_polynomial ==
        SagTensionCableComponent::PolynomialType::kLoadStrain) {
      slope = modulus_extrapolate_loadstrain_;
    } else {
      return -999999;
    }
  }

  return slope;
}

double CableComponentElongationModel::SlopePolynomial(
    const Polynomial& polynomial,
    const double& strain_thermal,
    const double& strain) const {
  // adjusts the strain (x-value) to align with the unshifted polynomial
  double percent_strain_polynomial =
      ConvertToPercentStrainPolynomial(strain,
                                       strain_thermal);

  // gets the slope tangent from the polynomial
  // multiplies to convert to load/strain slope
  return polynomial.Slope(percent_strain_polynomial) * 100;
}

double CableComponentElongationModel::Strain(
    const SagTensionCableComponent::PolynomialType& type_polynomial,
    const std::vector<Point2d<double>>& points,
    const double& strain_thermal,
    const double& load) const {
  double strain = -999999;

  // creates friendly names for points
  const Point2d<double>& point_unloaded = points.at(0);
  const Point2d<double>& point_polynomial_start = points.at(1);
  const Point2d<double>& point_polynomial_end = points.at(2);

  // selects region based on load value
  if (load < point_unloaded.y) {
    // compressed region
    strain = StrainLinearExtension(
        point_unloaded,
        *component_->modulus_compression_elastic_area(),
        load);
  } else if ((point_unloaded.y <= load)
      && (load <= point_polynomial_start.y)) {
    // stretched region
    strain = StrainLinearExtension(point_polynomial_start,
                                   *component_->modulus_tension_elastic_area(),
                                   load);
  } else if ((point_polynomial_start.y < load)
      && (load < point_polynomial_end.y)) {
    // polynomial region
    const Polynomial* polynomial = nullptr;
    if (type_polynomial == SagTensionCableComponent::PolynomialType::kCreep) {
      polynomial = &polynomial_creep_;
    } else if (type_polynomial ==
        SagTensionCableComponent::PolynomialType::kLoadStrain) {
      polynomial = &polynomial_loadstrain_;
    } else {
      return -999999;
    }

    strain = StrainPolynomial(*polynomial, strain_thermal, load);
  } else if (point_polynomial_end.y <= load) {
    // extrapolated region
    const double* modulus = nullptr;
    if (type_polynomial == SagTensionCableComponent::PolynomialType::kCreep) {
      modulus = &modulus_extrapolate_creep_;
    } else if (type_polynomial ==
        SagTensionCableComponent::PolynomialType::kLoadStrain) {
      modulus = &modulus_extrapolate_loadstrain_;
    } else {
      return -999999;
    }

    strain = StrainLinearExtension(point_polynomial_end, *modulus, load);
  }

  return strain;
}

double CableComponentElongationModel::StrainLinearExtension(
    const Point2d<double>& point,
    const double& slope,
    const double& load) const {
  // gets strain by extending line from point
  return point.x + ((load - point.y) / slope);
}

double CableComponentElongationModel::StrainPolynomial(
    const Polynomial& polynomial,
    const double& strain_thermal,
    const double& load) const {
  // gets raw strain from polynomial
  const double percent_strain_polynomial = polynomial.X(
      load,  // target value
      2,     // precision
      0.1);  // initial guess

  // converts polynomial percent strain at reference temperature to strain at
  // component temperature
  return ConvertToStrain(percent_strain_polynomial, strain_thermal);
}

double CableComponentElongationModel::StrainThermal(
    const double& temperature) const {
  // uses the thermal coefficient and temperature gradient to calculate thermal
  // strain
  return *component_->coefficient_expansion_linear_thermal()
         * (temperature - *temperature_reference_);
}

bool CableComponentElongationModel::Update() const {
  // updates stretch
  if (is_updated_stretch_ == false) {
    is_updated_stretch_ = UpdateStretch();
    if (is_updated_stretch_ == false) {
      return false;
    }
  }

  // updates state
  if (is_updated_state_ == false) {
    is_updated_state_ = UpdateState(*state_);
    if (is_updated_state_ == false) {
      return false;
    }
  }

  // if it reaches this point, update was successful
  return true;
}

void CableComponentElongationModel::UpdateModulusExtrapolate() const {
  const double kSlopeMin = 100000;

  // updates the creep polynomial extrapolation slope
  if (polynomial_creep_.coefficients()->empty() == true) {
    modulus_extrapolate_creep_ = -999999;
  } else {
    // gets max strain value for polynomial
    const double strain_max = StrainPolynomial(
        polynomial_creep_,
        0,
        *component_->load_limit_polynomial(
            SagTensionCableComponent::PolynomialType::kCreep));

    // solves for tangency slope
    modulus_extrapolate_creep_ = SlopePolynomial(polynomial_creep_,
                                                 0,
                                                 strain_max);

    // corrects if the slope is too low
    if (modulus_extrapolate_creep_ < kSlopeMin) {
      modulus_extrapolate_creep_ = kSlopeMin;
    }
  }

  // updates the load-strain polynomial extrapolation slope
  if (polynomial_loadstrain_.coefficients()->empty() == true) {
    modulus_extrapolate_loadstrain_ = -999999;
  } else {
    // gets max strain value for polynomial
    const double strain_max = StrainPolynomial(
        polynomial_loadstrain_,
        0,
        *component_->load_limit_polynomial(
            SagTensionCableComponent::PolynomialType::kLoadStrain));

    // solves for tangency slope
    modulus_extrapolate_loadstrain_ = SlopePolynomial(polynomial_loadstrain_,
                                                      0,
                                                      strain_max);

    // corrects if the slope is too low
    if (modulus_extrapolate_loadstrain_ < kSlopeMin) {
      modulus_extrapolate_loadstrain_ = kSlopeMin;
    }
  }
}

bool CableComponentElongationModel::UpdateState(const CableState& state) const {
  // updates strain-thermal
  strain_thermal_state_ = StrainThermal(state.temperature);

  // updates state polynomial pointer
  const Polynomial* polynomial = nullptr;
  if (state_->type_polynomial ==
      SagTensionCableComponent::PolynomialType::kCreep) {
    polynomial = &polynomial_creep_;
  } else if (state_->type_polynomial ==
      SagTensionCableComponent::PolynomialType::kLoadStrain) {
    polynomial = &polynomial_loadstrain_;
  }

  // updates points
  PointsRegions(*polynomial,
                state.temperature,
                *component_->load_limit_polynomial(state.type_polynomial),
                load_stretch_,
                points_state_);

  return true;
}

/// This function solves for the stretch load in the state model. If the state
/// and stretch polynomial types do not match, the stretch can be transferred
/// to a matching polynomial type. This is most common when the state model uses
/// a loadstrain polynomial, but the stretch is defined by the creep polynomial.
/// Transferring the stretch load to different polynomial types requires an
/// iterative approach. A load/strain point is calculated on the defining
/// stretch polynomial and the tension modulus is extended from it to intersect
/// the other stretch model (which has a polynomial type that matches the state
/// model). The intersection point defines the stretch load that will be modeled
/// in the component state model.
bool CableComponentElongationModel::UpdateStretch() const {
  // calculates the thermal strain for the stretch polynomials
  strain_thermal_stretch_ = StrainThermal(state_stretch_->temperature);

  // checks if no stretch is defined
  if (state_stretch_->load == 0) {
    load_stretch_ = 0;
    return true;
  }

  // checks if stretch and state polynomial types match
  if (state_->type_polynomial == state_stretch_->type_polynomial) {
    load_stretch_ = state_stretch_->load;
    return true;
  }

  // updates region points for the creep and loadstrain stretch models
  SagTensionCableComponent::PolynomialType type;

  type = SagTensionCableComponent::PolynomialType::kCreep;
  PointsRegions(polynomial_creep_,
                state_stretch_->temperature,
                *component_->load_limit_polynomial(type),
                0,
                points_stretch_creep_);

  type = SagTensionCableComponent::PolynomialType::kLoadStrain;
  PointsRegions(polynomial_loadstrain_,
                state_stretch_->temperature,
                *component_->load_limit_polynomial(type),
                0,
                points_stretch_load_);

  // creates an interface for the model the stretch is being transferred from
  const SagTensionCableComponent::PolynomialType* type_polynomial_from =
      &state_stretch_->type_polynomial;

  const std::vector<Point2d<double>>* points_from = nullptr;
  if (state_stretch_->type_polynomial ==
      SagTensionCableComponent::PolynomialType::kCreep) {
    points_from = &points_stretch_creep_;
  } else if (state_stretch_->type_polynomial ==
      SagTensionCableComponent::PolynomialType::kLoadStrain) {
    points_from = &points_stretch_load_;
  } else {
    return false;
  }

  // creates an interface for the model the stretch is being transferred to
  const SagTensionCableComponent::PolynomialType* type_polynomial_to =
      &state_->type_polynomial;

  const std::vector<Point2d<double>>* points_to = nullptr;
  if (state_->type_polynomial ==
      SagTensionCableComponent::PolynomialType::kCreep) {
    points_to = &points_stretch_creep_;
  } else if (state_->type_polynomial ==
      SagTensionCableComponent::PolynomialType::kLoadStrain) {
    points_to = &points_stretch_load_;
  } else {
    return false;
  }

  // iterative routine to determine solution
  // x = strain
  // y = load difference (model to - model from)
  // solution reached when y = 0

  // calculates the load-stretch point on the defining stretch polynomial
  // this is where the tension modulus will be extended from to intersect the
  // other stretch polynomial
  Point2d<double> point_stretch;
  point_stretch.y = state_stretch_->load;
  point_stretch.x = Strain(*type_polynomial_from, *points_from,
                           strain_thermal_stretch_, point_stretch.y);

  // initializes target
  double target_solution = 0;

  // initializes helper variables for calculating solution y values
  double load_from = -999999;
  double load_to = -999999;

  // initializes left point
  Point2d<double> point_left;
  point_left.x = point_stretch.x;
  load_from = LoadLinearExtension(point_stretch,
                                  *component_->modulus_tension_elastic_area(),
                                  point_left.x);
  load_to = Load(*type_polynomial_to, *points_to,
                 strain_thermal_stretch_, point_left.x);
  point_left.y = load_to - load_from;

  // initializes right point
  Point2d<double> point_right;
  point_right.x = point_left.x + 0.001;
  load_from = LoadLinearExtension(point_stretch,
                                  *component_->modulus_tension_elastic_area(),
                                  point_right.x);
  load_to = Load(*type_polynomial_to, *points_to,
                 strain_thermal_stretch_, point_right.x);
  point_right.y = load_to - load_from;

  // initializes current point
  Point2d<double> point_current;

  // iterates until intersection between elastic modulus and polynomial is found
  unsigned int iter = 0;
  const int iter_max = 100;

  while ((0.01 < std::abs(point_current.y - target_solution))
         && (iter <= iter_max)) {
    // gets current point x value using left and right points
    // calculates tangent line between points, extrapolates using line
    double slope_line = (point_right.y - point_left.y)
                        / (point_right.x - point_left.x);

    point_current.x = point_left.x
                      + ((target_solution - point_left.y) / slope_line);

    // gets current point y value
    load_from = LoadLinearExtension(point_stretch,
                                    *component_->modulus_tension_elastic_area(),
                                    point_current.x);
    load_to = Load(*type_polynomial_to, *points_to,
                   strain_thermal_stretch_,
                   point_current.x);
    point_current.y = load_to - load_from;

    // updates either left or right point based on current point
    if (point_current.x < point_left.x) {

      point_right.x = point_left.x;
      point_right.y = point_left.y;
      point_left.x = point_current.x;
      point_left.y = point_current.y;

    } else if ((point_left.x < point_current.x)
        && (point_current.x < point_right.x)) {

      if (point_current.y < target_solution) {
        point_right.x = point_current.x;
        point_right.y = point_current.y;
      } else if (target_solution < point_current.y) {
        point_left.x = point_current.x;
        point_left.y = point_current.y;
      }

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
    load_stretch_ = load_to;
    return true;
  } else {
    load_stretch_ = -999999;
    return false;
  }
}

bool CableComponentElongationModel::ValidatePolynomialOrigin(
    const bool& is_included_warnings,
    std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "CABLE COMPONENT ELONGATION MODEL";

  // exits if warnings are not included
  if (is_included_warnings == false) {
    return is_valid;
  }

  // validates creep polynomial origin
  double strain = -999999;
  strain = StrainPolynomial(polynomial_creep_, 0, 0);
  if ((strain < -0.001)
      || ((strain < -0.0001) && (is_included_warnings == true))
      || (0.001 < strain)
      || ((0.0001 < strain) && (is_included_warnings == true))) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Creep polynomial origin has a significant "
                            "non-zero strain";
      messages->push_back(message);
    }
  }

  // validates load-strain polynomial origin
  strain = StrainPolynomial(polynomial_loadstrain_, 0, 0);
  if ((strain < -0.001)
      || ((strain < -0.0001) && (is_included_warnings == true))
      || (0.001 < strain)
      || ((0.0001 < strain) && (is_included_warnings == true))) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Load-strain polynomial origin has a significant "
                            "non-zero strain";
      messages->push_back(message);
    }
  }

  return is_valid;
}

bool CableComponentElongationModel::ValidatePolynomialShape(
    const bool& is_included_warnings,
    std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "CABLE COMPONENT ELONGATION MODEL";

  // exits if warnings are not included
  if (is_included_warnings == false) {
    return is_valid;
  }

  // initializes iteration and calculation variables
  double strain_polynomial_min = 999999;
  double strain_polynomial_max = -999999;

  // searches creep polynomial for validation errors
  strain_polynomial_min = StrainPolynomial(polynomial_creep_, 0, 0);
  strain_polynomial_max = *component_->load_limit_polynomial(
      SagTensionCableComponent::PolynomialType::kCreep);

  for (int iter = 0; iter < 50; iter++) {
    // calculates evenly spaced strain values from minimum to maximum values
    const double strain = strain_polynomial_min + ((iter / 50)
                          * (strain_polynomial_max - strain_polynomial_min));

    // calculates slope at strain point
    const double slope = SlopePolynomial(polynomial_creep_, 0, strain);

    // checks if polynomial slope is negative
    if (slope < 0) {
      is_valid = false;
      if (messages != nullptr) {
        message.description = "Slope of creep polynomial tangent is negative "
                              "at strain = "
                              + helper::DoubleToFormattedString(strain, 4);
        messages->push_back(message);
      }
    }

    // checks if polynomial slope exceeds elastic area modulus
    if ((*component_->modulus_tension_elastic_area() < slope)
         && (is_included_warnings == true)) {
      is_valid = false;
      if (messages != nullptr) {
        message.description = "Slope of creep polynomial tangent is greater "
                              "than the tension elastic area modulus at "
                              "strain = "
                              + helper::DoubleToFormattedString(strain, 4);
        messages->push_back(message);
      }
    }
  }

  // searches load-strain polynomial for validation errors
  strain_polynomial_min = StrainPolynomial(polynomial_loadstrain_, 0, 0);
  strain_polynomial_max = *component_->load_limit_polynomial(
      SagTensionCableComponent::PolynomialType::kLoadStrain);

  for (int iter = 0; iter < 50; iter++) {
    // calculates evenly spaced strain values from minimum to maximum values
    const double strain = strain_polynomial_min + ((iter / 50)
                          * (strain_polynomial_max - strain_polynomial_min));

    // calculates slope at strain point
    const double slope = SlopePolynomial(polynomial_loadstrain_, 0, strain);

    // checks if polynomial slope is negative
    if (slope < 0) {
      is_valid = false;
      if (messages != nullptr) {
        message.description = "Slope of load-strain polynomial tangent is "
                              "negative at strain = "
                              + helper::DoubleToFormattedString(strain, 4);
        messages->push_back(message);
      }
    }

    // checks if polynomial slope exceeds elastic area modulus
    if ((*component_->modulus_tension_elastic_area() < slope)
         && (is_included_warnings == true)) {
      is_valid = false;
      if (messages != nullptr) {
        message.description = "Slope of load-strain polynomial tangent is "
                              "greater than the tension elastic area modulus "
                              "at strain = "
                              + helper::DoubleToFormattedString(strain, 4);
        messages->push_back(message);
      }
    }
  }

  return is_valid;
}
