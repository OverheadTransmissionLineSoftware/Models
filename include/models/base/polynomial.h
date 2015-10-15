// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef OTLS_MODELS_BASE_POLYNOMIAL_H_
#define OTLS_MODELS_BASE_POLYNOMIAL_H_

#include <list>
#include <vector>

#include "models/base/error_message.h"

/// \par OVERVIEW
///
/// This class is a mathematical polynomial function.

/// \par DERIVATIVE
///
/// This class calculates a derivate polynomial for slope and for iteratively
/// solving for x values (Newton's method).
class Polynomial {
 public:
  /// \brief Default constructor.
  Polynomial();

  /// \brief Alternate constructor.
  /// \param[in] coefficients
  ///   A list of coefficients.
  Polynomial(const std::vector<double>* coefficients);

  /// \brief Destructor.
  ~Polynomial();

  /// \brief Gets the derivative of the polynomial.
  /// \return A polynomial with derivative coefficients.
  Polynomial Derivative() const;

  /// \brief Gets the maximum polynomial order.
  /// \return The maximum polynomial order.
  int OrderMax() const;

  /// \brief Gets the slope.
  /// \param[in] x
  ///   The x value for the point of slope.
  /// \return The slope.
  double Slope(const double& x) const;

  /// \brief Validates member variables.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings = true,
                std::list<ErrorMessage>* messages = nullptr) const;

  /// \brief Gets the x value.
  /// \param[in] y
  ///   The y value.
  /// \param[in] decimal_precision_y
  ///   An integer that controls the precision, or decimal place, the solution
  ///   must equal the target y value.
  /// \param[in] x_guess
  ///   An x value that is the starting guess for the iterative solution.
  /// \return The x value.
  double X(const double& y, const int& decimal_precision_y,
           const double& x_guess) const;

  /// \brief Gets the y value.
  /// \param[in] x
  ///   The x value.
  /// \return The y value.
  double Y(const double& x) const;

  /// \brief Gets the polynomial coefficients.
  /// \return The polynomial coefficients.
  const std::vector<double>* coefficients() const;

  /// \brief Sets the polynomial coefficients.
  /// \param[in] coefficients
  ///   A vector of polynomial coefficients. The vector index correlates to the
  ///   polynomial order.
  void set_coefficients(const std::vector<double>* coefficients);

 private:
  /// \brief Determines if class is updated.
  /// \return A boolean indicating if class is updated.
  bool IsUpdated() const;

  /// \brief Updates cached member variables and modifies control variables if
  ///   update is required.
  /// \return A boolean indicating if class updates completed successfully.
  bool Update() const;

  /// \brief Updates the derivative polynomial.
  /// \return A boolean indicating the success status of the update.
  bool UpdateDerivative() const;

  /// \var coefficients_
  ///   The coefficients that determine the shape and order of the polynomial.
  const std::vector<double>* coefficients_;

  /// \var derivative_
  ///   The derivative of the polynomial. The derivate polynomial contains y
  ///   values that correspond to the slop of the polynomial.
  mutable Polynomial* derivative_;

  /// \var is_updated_derivative_
  ///   An indicator that tells if the derivative is updated.
  mutable bool is_updated_derivative_;
};

#endif // OTLS_MODELS_BASE_POLYNOMIAL_H_
