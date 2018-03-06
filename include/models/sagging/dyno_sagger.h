// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef MODELS_SAGGING_DYNO_SAGGER_H_
#define MODELS_SAGGING_DYNO_SAGGER_H_

#include <list>

#include "models/transmissionline/catenary.h"

/// \par OVERVIEW
///
/// This class solves for the dynamometer tension to be used for sagging a
/// transmission cable.
class DynoSagger {
 public:
  /// \par OVERVIEW
  ///
  /// This enumeration contains dyno span end locations.
  enum class SpanEndLocation {
    kAhead,
    kBack,
    kNull
  };

  /// \brief Default constructor.
  DynoSagger();

  /// \brief Destructor.
  ~DynoSagger();

  /// \brief Gets the dyno tension.
  /// \return The dyno tension.
  double Tension() const;

  /// \brief Validates member variables.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings = true,
                std::list<ErrorMessage>* messages = nullptr) const;

  /// \brief Gets the catenary.
  /// \return The catenary.
  Catenary3d catenary() const;

  /// \brief Gets the span end where the dyno is location.
  /// \return The dyno
  SpanEndLocation location() const;

  /// \brief Sets the catenary.
  /// \param[in] catenary
  ///   The catenary.
  void set_catenary(const Catenary3d& catenary);

  /// \brief Sets the location.
  /// \param[in] location
  ///   The span end location.
  void set_location(const SpanEndLocation& location);

 private:
  /// \var catenary_
  ///   The hyperbolic curve that models the cable position and tension.
  Catenary3d catenary_;

  /// \var location_
  ///   The span end that the dyno is located at.
  SpanEndLocation location_;
};

#endif  // MODELS_SAGGING_DYNO_SAGGER_H_
