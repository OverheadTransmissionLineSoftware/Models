// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef OTLS_MODELS_TRANSMISSIONLINE_ALIGNMENT_H_
#define OTLS_MODELS_TRANSMISSIONLINE_ALIGNMENT_H_

#include <list>

#include "models/base/error_message.h"

/// \par OVERVIEW
///
/// This struct is a container for an alignment point.
struct AlignmentPoint {
 public:
  /// \brief Default constructor.
  AlignmentPoint();

  /// \brief Destructor.
  ~AlignmentPoint();

  /// \brief Validates member variables.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings = true,
                std::list<ErrorMessage>* messages = nullptr) const;

  /// \var elevation
  ///   The alignment position (z plane only).
  double elevation;

  /// \var rotation
  ///   The alignment rotation, in degrees.
  double rotation;

  /// \var station
  ///   The alignment position (x-y plane only).
  double station;
};


/// \par OVERVIEW
///
/// This class is an alignment path for a transmission line.
///
/// \todo Add station equations.
class Alignment {
 public:
  /// \brief Default constructor.
  Alignment();

  /// \brief Destructor.
  ~Alignment();

  /// \brief Adds an alignment point.
  /// \param[in] point
  ///   The alignment point.
  /// The point will be sorted before adding to the list.
  /// \return The index of the alignment point.
  int AddPoint(const AlignmentPoint& point);

  /// \brief Deletes an alignment point.
  /// \param[in] index
  ///   The list index.
  void DeletePoint(const int& index);

  /// \brief Modifies an alignment point.
  /// \param[in] index
  ///   The list index.
  /// \param[in] point
  ///   The alignment point.
  /// \return The index of the alignment point.
  int ModifyPoint(const int& index, const AlignmentPoint& point);

  /// \brief Validates member variables.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings = true,
                std::list<ErrorMessage>* messages = nullptr) const;

  /// \brief Gets the alignment points.
  /// \return The alignment points.
  const std::list<AlignmentPoint>* points() const;

 private:
  /// \var points_
  ///   The sorted list of alignment points.
  std::list<AlignmentPoint> points_;
};

#endif // OTLS_MODELS_TRANSMISSIONLINE_ALIGNMENT_H_
