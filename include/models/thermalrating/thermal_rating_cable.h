// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef OTLS_MODELS_THERMALRATING_THERMALRATINGCABLE_H_
#define OTLS_MODELS_THERMALRATING_THERMALRATINGCABLE_H_

#include "models/base/error_message.h"
#include "models/transmissionline/cable.h"

/// \par OVERVIEW
///
/// This class provides an interface for the cable component for use in the
/// thermal rating library.
class ThermalRatingCableComponent {
 public:
  /// \brief Constructor.
  ThermalRatingCableComponent();

  /// \brief Destructor.
  ~ThermalRatingCableComponent();

  /// \brief Validates member variables.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings = true,
                std::list<ErrorMessage>* messages = nullptr) const;

  /// \brief Gets the heat capacity.
  /// \return Heat capacity.
  const double* capacity_heat() const;

  /// \brief Gets the base component.
  /// \return The base component.
  const CableComponent* component_base() const;

  /// \brief Sets the base cable component.
  /// \param[in] component_base
  ///   The base cable component.
  void set_component_base(const CableComponent* component_base);

 private:
  /// \var component_base_
  ///   The cable component that is interfaced.
  const CableComponent* component_base_;
};

/// \par OVERVIEW
///
/// This class provides an interface for the cable for use in the thermal rating
/// library.
class ThermalRatingCable {
 public:
  /// \brief Constructor.
  ThermalRatingCable();

  /// \brief Destructor.
  ~ThermalRatingCable();

  /// \brief Gets the AC resistance.
  /// \param[in] temperature
  ///   The temperature.
  /// \return The AC resistance.
  double Resistance(const double& temperature) const;

  /// \brief Validates member variables.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings = true,
                std::list<ErrorMessage>* messages = nullptr) const;

  /// \brief Gets the absorptivity.
  /// \return The absorptivity.
  const double* absorptivity() const;

  /// \brief Gets the base cable.
  /// \return The base cable.
  const Cable* cable_base() const;

  /// \brief Gets the core component.
  /// \return The core component.
  const ThermalRatingCableComponent* component_core() const;

  /// \brief Gets the shell component.
  /// \return The shell component.
  const ThermalRatingCableComponent* component_shell() const;

  /// \brief Gets the diameter.
  /// \return The diameter.
  const double* diameter() const;

  /// \brief Gets the emissivity.
  /// \return The emissivity.
  const double* emissivity() const;

  /// \brief Gets the ac resistances.
  /// \return The ac resistances.
  const std::list<Cable::ResistancePoint>* resistances_ac() const;

  /// \brief Sets the base cable.
  /// \param[in] cable_base
  ///   The base cable.
  void set_cable_base(const Cable* cable_base);

 private:
  /// \var cable_base_
  ///   The cable that is interfaced.
  const Cable* cable_base_;

  /// \var component_thermalrating_core_
  ///   The thermal rating core cable component.
  ThermalRatingCableComponent component_thermalrating_core_;

  /// \var component_thermalrating_shell_
  ///   The thermal rating shell cable component.
  ThermalRatingCableComponent component_thermalrating_shell_;
};

#endif  // OTLS_MODELS_THERMALRATING_THERMALRATINGCABLE_H_
