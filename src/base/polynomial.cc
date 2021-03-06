// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/base/polynomial.h"

#include <cmath>

#include "models/base/point.h"

Polynomial::Polynomial() {
  derivative_ = nullptr;
  is_updated_derivative_ = false;
}

Polynomial::Polynomial(const std::vector<double>* coefficients) {
  coefficients_ = coefficients;
  derivative_ = nullptr;
  is_updated_derivative_ = false;
}

Polynomial::~Polynomial() {
  delete derivative_;
}

Polynomial Polynomial::Derivative() const {
  Polynomial derivative;

  // updates class if necessary
  if ((IsUpdated() == false) && (Update() == false)) {
    return derivative;
  }

  derivative = *derivative_;
  return derivative;
}

int Polynomial::OrderMax() const {
  return coefficients_->size() - 1;
}

double Polynomial::Slope(const double& x) const {
  // updates class if necessary
  if ((IsUpdated() == false) && (Update() == false)) {
    return -999999;
  }

  return derivative_->Y(x);
}

bool Polynomial::Validate(const bool& /*is_included_warnings*/,
                          std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "POLYNOMIAL";

  // validates coefficients
  if (coefficients_ == nullptr) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid coefficients";
      messages->push_back(message);
    }
  }

  // returns if errors are present
  if (is_valid == false) {
    return is_valid;
  }

  // validates update process
  is_valid = Update();
  if (is_valid == false) {
    message.description = "Error solving derivative coefficients";
    messages->push_back(message);
  }

  return is_valid;
}

/// This method is iterative, and uses the Newton numerical method for solving
/// for x.
double Polynomial::X(const double& y, const int& decimal_precision_y,
                     const double& x_guess) const {
  // updates class if necessary
  if ((IsUpdated() == false) && (Update() == false)) {
    return -999999;
  }

  // converts to decimal precision
  const double precision_y = 1 / std::pow(10, decimal_precision_y);

  // gets a shifted polynomial so that y value is on x-axis
  std::vector<double> coefficients = *coefficients_;
  std::vector<double> coefficients_shifted = coefficients;  // copies coeffs
  coefficients_shifted.at(0) = coefficients_shifted.at(0) - y;
  Polynomial polynomial_shifted(&coefficients_shifted);

  // function point
  Point2d<double> point_function(x_guess, -999999);

  // derivative point
  Point2d<double> point_derivative(x_guess, -999999);

  // iterates until the value is within tolerance
  int iter = 0;
  while (precision_y < std::abs(point_function.y) && (iter < 100)) {
    // calculates y value for shifted polynomial and derivative
    point_function.y = polynomial_shifted.Y(point_function.x);
    point_derivative.y = derivative_->Y(point_derivative.x);

    // calculates a new x value for shifted polynomial and derivative
    if (point_derivative.y != 0) {
      point_function.x = point_function.x
                           - (point_function.y / point_derivative.y);
      point_derivative.x = point_function.x;
    }
    iter++;
  }

  return point_function.x;
}

double Polynomial::Y(const double& x) const {
  double y = 0;

  const int order_max = OrderMax();
  for (int order = 0; order <= order_max; order++) {
    const double& coefficient =  coefficients_->at(order);
    y = y + (coefficient * std::pow(x, order));
  }

  return y;
}

const std::vector<double>* Polynomial::coefficients() const {
  return coefficients_;
}

void Polynomial::set_coefficients(const std::vector<double>* coefficients) {
  coefficients_ = coefficients;

  is_updated_derivative_ = false;
}

bool Polynomial::IsUpdated() const {
  return is_updated_derivative_ == true;
}

bool Polynomial::Update() const {
  // updates derivative
  if (is_updated_derivative_ == false) {
    is_updated_derivative_ = UpdateDerivative();
    if (is_updated_derivative_ == false) {
      return false;
    }
  }

  return true;
}

bool Polynomial::UpdateDerivative() const {
  const int order_max = OrderMax();

  // calculates derivative coefficients
  coefficients_derivative_.clear();
  for (int order = 0; order <= order_max; order++) {
    if (order != 0) {
      const double& coefficient_function = coefficients_->at(order);
      double coefficient_derivative = coefficient_function * order;
      coefficients_derivative_.push_back(coefficient_derivative);
    }
  }

  // deletes old derivative polynomial and creates new one
  delete derivative_;
  derivative_ = new Polynomial();
  derivative_->set_coefficients(&coefficients_derivative_);

  return true;
}
