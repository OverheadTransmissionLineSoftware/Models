// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef OTLS_MODELS_SAGGING_STOPWATCHSAGGER_H_
#define OTLS_MODELS_SAGGING_STOPWATCHSAGGER_H_

#include <list>

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

  /// \brief Gets the catenary.
  /// \return The catenary.
  Catenary3d catenary() const;

  /// \brief Sets the catenary.
  /// \param[in] catenary
  ///   The catenary.
  void set_catenary(const Catenary3d& catenary);

 private:
  /// \var catenary_
  ///   The hyperbolic curve that models the cable position and tension.
  Catenary3d catenary_;

  /// \var sag_
  ///   The catenary sag.
  double sag_;
};

#endif  // OTLS_MODELS_SAGGING_STOPWATCHSAGGER_H_
