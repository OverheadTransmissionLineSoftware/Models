// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef TRANSMISSIONLINE_GEOMETRICSHAPES_H_
#define TRANSMISSIONLINE_GEOMETRICSHAPES_H_

#include <list>
#include <string>

/// \par OVERVIEW
///
/// This class models a generic shape. It is a purely virtual class, and must be
/// inherited by a unique shape.
///
class Shape {
 public:
  Shape() {};
  ~Shape() {};

  virtual double AreaCrossSection() const = 0;
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

  /// \brief Validates class data.
  /// \param is_included_warnings A flag that tightens the acceptable value
  ///       range.
  /// \param messages_error A list of detailed error messages. If this is
  ///        provided, any validation errors will be appended to the list.
  /// \return A boolean value indicating status of class data.
  bool Validate(bool is_included_warnings = true,
                std::list<std::string>* messages_error = nullptr) const;

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
  /// \param diameter The diameter.
  void set_diameter(const double& diameter);

  /// \brief Sets the length.
  /// \param length The length.
  void set_length(const double& length);

 private:
  /// \var diameter_ The diameter of the cylinder.
  double  diameter_;

  /// \var length_ The length of the cylinder.
  double  length_;
};

#endif // TRANSMISSIONLINE_GEOMETRICSHAPES_H_
