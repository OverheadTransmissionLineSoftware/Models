// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef BASE_POLYNOMIAL_H_
#define BASE_POLYNOMIAL_H_

#include <list>
#include <string>
#include <vector>

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
  Polynomial(const std::vector<double> coefficients);

  /// \brief Destructor.
  ~Polynomial();

  /// \brief Gets the slope of the polynomial.
  /// \param x
  ///   The x value for the point of slope.
  double Slope(const double& x) const;

  /// \brief Gets the maximum polynomial order.
  /// \return The maximum polynomial order.
  int OrderMax() const;

  /// \brief Gets the x value.
  /// \param y
  ///   The y value for the polynomial.
  /// \param decimal_precision_y
  ///   An integer that controls the precision, or decimal place, the solution
  ///   must equal the target y value.
  /// \param x_guess
  ///   An x value that is the starting guess for the iterative solution.
  double x(const double& y, const int& decimal_precision_y,
           const double& x_guess) const;

  /// \brief Gets the y value.
  /// \param x_value The x value at which the y value will be calculated.
  double y(const double& x) const;

  /// \brief Gets the polynomial coefficients.
  /// \return The polynomial coefficients.
  std::vector<double> coefficients() const;

  /// \brief Gets the derivative of the polynomial.
  /// \return A polynomial with derivative coefficients.
  Polynomial derivative() const;

  /// \brief Sets the polynomial coefficients.
  /// \param coefficients
  ///   A vector of polynomial coefficients. The vector index correlates to the
  ///   polynomial order.
  void set_coefficients(std::vector<double> coefficients);

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
  std::vector<double> coefficients_;

  /// \var derivative_
  ///   The derivative of the polynomial. The derivate polynomial contains y
  ///   values that correspond to the slop of the polynomial.
  mutable Polynomial* derivative_;

  /// \var is_updated_derivate_
  ///   An indicator that tells if the derivative is updated.
  mutable bool is_updated_derivate_;
};

#endif // BASE_POLYNOMIAL_H_
