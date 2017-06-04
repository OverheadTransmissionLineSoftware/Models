// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef OTLS_MODELS_BASE_GEOMETRICSHAPES_H_
#define OTLS_MODELS_BASE_GEOMETRICSHAPES_H_

#include <list>

#include "models/base/error_message.h"

/// \par OVERVIEW
///
/// This class models a generic shape. It is a purely virtual class, and must
/// be inherited by a unique shape.
class Shape {
 public:
  Shape() {};
  ~Shape() {};

  /// \brief The area of the shape cross section.
  virtual double AreaCrossSection() const = 0;

  /// \brief The volume of the shape.
  virtual double Volume() const = 0;
};

/// \par OVERVIEW
///
/// This class models a cylinder, and provides some geometric calculations.
/// \see Shape
class Cylinder : public Shape {
 public:
  /// \brief Default constructor.
  Cylinder();

  /// \brief Destructor.
  ~Cylinder();

  /// \brief Gets the cross-sectional area.
  /// \return The cross-sectional area.
  double AreaCrossSection() const;

  /// \brief Validates member variables.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings = true,
                std::list<ErrorMessage>* messages = nullptr) const;

  /// \brief Gets the volume.
  /// \return The volume.
  double Volume() const;

  /// \brief Gets the diameter.
  /// \return The diameter.
  double diameter() const;

  /// \brief Gets the length.
  /// \return The length.
  double length() const;

  ///
  /// \brief Sets the diameter.
  /// \param[in] diameter
  ///   The diameter.
  void set_diameter(const double& diameter);

  /// \brief Sets the length.
  /// \param[in] length
  ///   The length.
  void set_length(const double& length);

 private:
  /// \var diameter_
  ///   The diameter.
  double diameter_;

  /// \var length_
  ///   The length.
  double length_;
};

#endif  // OTLS_MODELS_BASE_GEOMETRICSHAPES_H_
