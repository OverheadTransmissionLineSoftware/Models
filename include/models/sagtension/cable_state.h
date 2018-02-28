// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef MODELS_SAGTENSION_CABLE_STATE_H_
#define MODELS_SAGTENSION_CABLE_STATE_H_

#include <list>

#include "models/base/error_message.h"
#include "models/sagtension/sag_tension_cable.h"

/// \par OVERVIEW
///
/// This struct contains cable elongation state parameters.
struct CableState {
 public:
  /// \brief Default constructor.
  CableState();

  /// \brief Destructor.
  ~CableState();

  /// \brief Validates member variables.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings = true,
                std::list<ErrorMessage>* messages = nullptr) const;

  /// \var temperature
  ///   The temperature of the cable.
  double temperature;

  /// \var type_polynomial
  ///   The polynomial that is used to describe the cable elongation
  ///   characteristics.
  SagTensionCableComponent::PolynomialType type_polynomial;
};

/// \par OVERVIEW
///
/// This struct contains additional cable elongation state parameters that are
/// needed when determining the stretch of a cable.
struct CableStretchState : public CableState {
  /// \brief Constructor.
  CableStretchState();

  /// \brief Destructor.
  ~CableStretchState();

  /// \brief Validates member variables.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings = true,
                std::list<ErrorMessage>* messages = nullptr) const;

  /// \var load
  ///   The load that the cable is stretched to.
  double load;
};

#endif  // MODELS_SAGTENSION_CABLE_STATE_H_
