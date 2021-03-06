// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef MODELS_SAGGING_STOPWATCH_SAGGER_H_
#define MODELS_SAGGING_STOPWATCH_SAGGER_H_

#include <list>

#include "models/base/units.h"
#include "models/transmissionline/catenary.h"

/// \par OVERVIEW
///
/// This class solves for the return wave time to be used for sagging a
/// transmission cable.
class StopwatchSagger {
 public:
  /// \brief Default constructor.
  StopwatchSagger();

  /// \brief Destructor.
  ~StopwatchSagger();

  /// \brief Gets the return time for a specified wave.
  /// \param[in] wave
  ///   The wave number, which must be 1 or greater.
  /// \return The return time for a specified wave.
  double TimeReturn(const int& wave) const;

  /// \brief Validates member variables.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings = true,
                std::list<ErrorMessage>* messages = nullptr) const;

  /// \brief Gets the traveling wave velocity.
  /// \return The traveling wave velocity.
  double VelocityWave() const;

  /// \brief Gets the catenary.
  /// \return The catenary.
  Catenary3d catenary() const;

  /// \brief Sets the catenary.
  /// \param[in] catenary
  ///   The catenary.
  void set_catenary(const Catenary3d& catenary);

  /// \brief Sets the units.
  /// \param[in] units
  ///   The unit system.
  void set_units(const units::UnitSystem& units);

  /// \brief Gets the units.
  /// \return The unit system.
  units::UnitSystem units() const;

 private:
  /// \brief Determines if class is updated.
  /// \return A boolean indicating if class is updated.
  bool IsUpdated() const;

  /// \brief Updates cached member variables and modifies control variables if
  ///    update is required.
  /// \return A boolean indicating if class updates completed successfully.
  bool Update() const;

  /// \brief Updates the catenary length and wave velocity.
  /// \return If the values have been successfully updated.
  bool UpdateLengthAndVelocity() const;

  /// \var catenary_
  ///   The hyperbolic curve that models the cable position and tension.
  Catenary3d catenary_;

  /// \var is_updated_
  ///   An indicator that tells if the class has been updated.
  mutable bool is_updated_;

  /// \var length_
  ///   The catenary length.
  mutable double length_;

  /// \var units_
  ///   The unit system.
  units::UnitSystem units_;

  /// \var velocity_wave_
  ///   The velocity of the wave along the catenary.
  mutable double velocity_wave_;
};

#endif  // MODELS_SAGGING_STOPWATCH_SAGGER_H_
