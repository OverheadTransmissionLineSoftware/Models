// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef MODELS_THERMALRATING_CABLE_HEAT_TRANSFER_STATE_H_
#define MODELS_THERMALRATING_CABLE_HEAT_TRANSFER_STATE_H_

#include <list>

#include "models/base/error_message.h"

/// \par OVERVIEW
///
/// This struct contains the various methods of heat transfer to/from a cable.
///
/// \par HEAT TRANSFER DIRECTION
///
/// Heat that is removed from the cable is negative, while heat added to the
/// cable is positive.
struct CableHeatTransferState {
 public:
  /// \brief Constructor.
  CableHeatTransferState();

  /// \brief Destructor.
  ~CableHeatTransferState();

  /// \brief Validates member variables.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings = true,
                std::list<ErrorMessage>* messages = nullptr) const;

  /// \var heat_convection
  ///   The heat transfer due to convection. This should always be negative.
  double heat_convection;

  /// \var heat_radiation
  ///   The heat transfer due to radiation. This should always be negative.
  double heat_radiation;

  /// \var heat_resistance
  ///   The heat transfer due to electrical resistance. This should always be
  ///   positive.
  double heat_resistance;

  /// \var heat_solar
  ///   The heat transfer due to solar. This should always be positive.
  double heat_solar;

  /// \var heat_storage
  ///   The heat transfer rate to/from the cable material (thermal storage).
  ///   This can either be positive or negative.
  double heat_storage;
};

#endif  // MODELS_THERMALRATING_CABLE_HEAT_TRANSFER_STATE_H_
