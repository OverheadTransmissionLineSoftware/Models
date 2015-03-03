// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "include/Base/polynomial.h"

#include <cmath>

#include "include/Base/point.h"

Polynomial::Polynomial() {
  derivative_ = nullptr;
  is_updated_derivate_ = false;
}

Polynomial::Polynomial(const std::vector<double> coefficients) {
  coefficients_ = coefficients;
}

Polynomial::~Polynomial() {}

int Polynomial::OrderMax() const {
  return coefficients_.size();
}

/// This method is iterative, and uses the Newton numerical method for solving
/// for x.
double Polynomial::x(const double& y, const int& decimal_precision_y,
                     const double& x_guess) const {

  // update class, if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  // convert to decimal precision
  const double precision_y = 1 / pow(10, decimal_precision_y);

  // get a shifted polynomial so that y value is on x-axis
  Polynomial polynomial_shifted;
  std::vector<double> coefficients_shifted = coefficients_;
  coefficients_shifted.at(0) = coefficients_shifted.at(0) - y;
  polynomial_shifted.set_coefficients(coefficients_shifted);

  // function point
  Point2d point_function = Point2d(x_guess, -999999);

  // derivative point
  Point2d point_derivative = Point2d(x_guess, -999999);

  // iterate until the value is within tolerance
  int iter = 0;
  while (precision_y < abs(point_function.y) && (iter < 100)) {

    // calculate y value for shifted polynomial and derivative
    point_function.y = polynomial_shifted.y(point_function.x);
    point_derivative.y = derivative_->y(point_derivative.x);

    // calculate a new x value for shifted polynomial and derivative
    if (point_derivative.y != 0) {
      point_function.x = point_function.x
                           - (point_function.y / point_derivative.y);
      point_derivative.x = point_function.x;
    }
    iter++;
  }

  return point_function.x;
}

double Polynomial::y(const double& x) const {

  double y = 0;

  const int order_max = OrderMax();
  for (int order = 0; order <= order_max; order++) {

    const double coefficient =  coefficients_.at(order);
    y = y + (coefficient * pow(x, order));
  }

  return y;
}

std::vector<double> Polynomial::coefficients() const {
  return coefficients_;
}

// TODO this function needs to return a copy of the derivative
Polynomial Polynomial::derivative() const {
  return *derivative_;
}

void Polynomial::set_coefficients(std::vector<double> coefficients) {
  coefficients_ = coefficients;
}

bool Polynomial::IsUpdated() const {

  if (is_updated_derivate_ == true) {
    return true;
  } else {
    return false;
  }
}

bool Polynomial::Update() const {

  // update derivative
  if (is_updated_derivate_ == false) {

    is_updated_derivate_ = UpdateDerivative();
    if (is_updated_derivate_ == false) {
      return false;
    }
  }

  return true;
}

bool Polynomial::UpdateDerivative() const {

  const int order_max = OrderMax();

  // calculate derivative coefficients
  std::vector<double> coefficients_derivative;
  for (int order = 0; order <= order_max; order++) {

    if (order != 0) {
      const double coefficient_function = coefficients_.at(order);
      double coefficient_derivative = coefficient_function * order;
      coefficients_derivative.push_back(coefficient_derivative);
    }
  }

  // assign to derivative
  derivative_->set_coefficients(coefficients_derivative);

  return true;
}