// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef TLSLIBRARIES_SAGTENSION_CATENARYCABLE_H_
#define TLSLIBRARIES_SAGTENSION_CATENARYCABLE_H_

#include <list>
#include <string>

#include "sagtension/cable_state.h"
#include "transmissionline/cable.h"
#include "transmissionline/catenary.h"

/// \par OVERVIEW
///
/// This class combines the catenary functionality with the properties of a
/// cable. It represents a sag-tension solution, and is often referenced and/or
/// modified with the supporting sag-tension classes.
class CatenaryCable : public Catenary3d {
 public:
  /// \brief Default constructor.
  CatenaryCable();

  /// \brief Destructor.
  ~CatenaryCable();

  /// \brief Validates member variables.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages_error
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings,
                std::list<std::string>* messages_error) const;

  /// \brief Gets the cable.
  /// \return The cable.
  const Cable* cable() const;

  /// \brief Sets the cable.
  /// \param[in] cable
  ///   The cable.
  void set_cable(const Cable* cable);

  /// \brief Sets the cable state.
  /// \param[in] state
  ///   The cable state.
  void set_state(const CableState& state);

  /// \brief Gets the cable state.
  /// \return The cable state.
  const CableState* state() const;

 private:
  /// \brief Validates catenary and cable weight.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages_error
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating whether the catenary weight is equal to
  ///   or greater than the cable weight.
  bool ValidateCatenaryCableWeight(
      const bool& is_included_warnings,
      std::list<std::string>* messages_error) const;

  /// \var cable_
  ///   The cable.
  const Cable* cable_;

  /// \var state_
  ///   The cable state.
  CableState state_;
};

#endif // TLSLIBRARIES_SAGTENSION_CATENARYCABLE_H_
