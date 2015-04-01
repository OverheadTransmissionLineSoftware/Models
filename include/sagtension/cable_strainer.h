// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef TLSLIBRARIES_SAGTENSION_CABLESTRAINER_H_
#define TLSLIBRARIES_SAGTENSION_CABLESTRAINER_H_

#include <list>
#include <string>

#include "sagtension/cable_elongation_model.h"
#include "transmissionline/cable.h"

/// \par OVERVIEW
///
struct CableStrainerState {
  bool is_stretched;
  double temperature;
};

/// \par OVERVIEW
///
/// This class strains the cable from a starting state to a finish state.
///
/// \par START AND FINISH STATE
///
/// The following parameters can be specified for both starting and finish
/// states:
///   - temperature
///   - load
///   - stretch condition
///
/// \par CABLE ELONGATION MODEL
///
/// This class uses a load-strain model to determine the cable strain at each
/// specified temperature, load, and stretch condition. To determine the strain
/// from the start to finish state, the strain values from the two states are
/// compared.
///
/// \par LENGTH
///
/// The starting length can be specified, and using the strain transition from
/// start to finish, the finish length can be calculated.
class CableStrainer {
 public:
  /// \brief Default constructor.
  CableStrainer();

  /// \brief Destructor.
  ~CableStrainer();

  /// \brief Gets the length of the cable in the finish state.
  /// \return The length in the finish state.
  double LengthFinish() const;

  /// \brief Gets the load of the core component at the finish state.
  /// \return The load of the core component at the finish state.
  double LoadFinishCore() const;

  /// \brief Gets the load of the shell component at the finish state.
  /// \return The load of the shell component at the finish state.
  double LoadFinishShell() const;

  /// \brief Gets the load of the core component at the start state.
  /// \return The load of the core component at the start state.
  double LoadStartCore() const;

  /// \brief Gets the load of the shell component at the start state.
  /// \return The load of the shell component at the start state.
  double LoadStartShell() const;

  /// \brief Gets the strain transition, or strain difference, between start
  ///   and finish states.
  /// \return The strain transition from start to finish state.
  /// This value may be positive or negative, indicating whether the cable is
  /// shrinking (negative) or elongating (positive).
  double StrainTransitionLoad() const;

  /// \brief Gets the thermal based strain transition, or strain difference,
  ///   between start and finish states.
  /// \return The thermal based strain transition.
  /// This value may be positive or negative, indicating whether the cable is
  /// shrinking (negative) or elongating (positive).
  double StrainTransitionThermal() const;

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
  /// \return A copy of the cable.
  Cable cable() const;

//  /// \brief Gets whether the cable is stretched at the finish state.
//  /// \return An indicator that tells if the cable is stretched at the finish
//  ///   state.
//  bool is_stretched_finish() const;
//
//  /// \brief Gets whether the cable is stretched at the start state.
//  /// \return An indicator that tells if the cable is stretched at the start
//  ///   state.
//  bool is_stretched_start() const;

  /// \brief Gets the length of the cable at the start state.
  /// \return The length of cable at the start state.
  double length_start() const;

  /// \brief Gets the load of the cable at the finish state.
  /// \return The load of the cable at the finish state.
  double load_finish() const;

  /// \brief Gets the load of the cable at the start state.
  /// \return The load of the cable at the start state.
  double load_start() const;

  /// \brief Gets the load that the cable is stretched to.
  /// \return The load that the cable was stretched to.
  double load_stretch() const;

  /// \brief Sets the cable.
  /// \param[in] cable
  ///   The cable.
  void set_cable(const Cable& cable);

  /// \brief Sets the length of the cable at the start state.
  /// \param[in] length_start
  ///   The length of the cable at the start state.
  void set_length_start(const double& length_start);

  /// \brief Sets the load of the cable at the finish state.
  /// \param[in] load_finish
  ///   The load of the cable at the finish state.
  void set_load_finish(const double& load_finish);

  /// \brief Sets the load of the cable at the start state.
  /// \param[in] load_start
  ///   The load of the cable at the start state.
  void set_load_start(const double& load_start);

  /// \brief Sets the load that the cable is stretched to.
  /// \param[in] load_stretch
  ///   The load that the cable was stretched to.
  void set_load_stretch(const double& load_stretch);

  /// \brief Sets the state parameters at the finish.
  /// \param[in] state_finish
  ///   The state parameters at the start.
  void set_state_finish(const CableStrainerState& state_finish);

  /// \brief Sets the state parameters at the start.
  /// \param[in] state_start
  ///   The state parameters at the start.
  void set_state_start(const CableStrainerState& state_finish);

  /// \brief Sets the temperature that the cable was stretched at.
  /// \param[in] temperature_stretch
  ///   The temperature that the cable was stretched at.
  void set_temperature_stretch(const double& temperature_stretch);

  /// \brief Gets the finish state parameters.
  /// \return The finish state parameters.
  CableStrainerState state_finish() const;

  /// \brief Gets the state parameters at the start.
  /// \return The state parameters at the start.
  CableStrainerState state_start() const;

  /// \brief Getst the temperature that the cable was stretched at.
  /// \return The temperature that the cable was stretched at.
  double temperature_stretch() const;

 private:
  /// \brief Gets the length after the cable has been strained.
  /// \param[in] length_before_strain
  ///   The length of the cable before straining.
  /// \param[in] strain_transition
  ///   The amount to strain the cable.
  /// \return The length after the cable has been strained.
  double LengthAfterStrain(const double& length_before_strain,
                           const double& strain_transition) const;

  /// \var is_stretched_finish_
  ///   And indicator that tells if the cable is stretched in the finish state.
  bool is_stretched_finish_;

  /// \var is_stretched_finish_
  ///   And indicator that tells if the cable is stretched in the start state.
  bool is_stretched_start_;

  /// \var length_start_
  ///   The length of the cable in the start state.
  mutable double length_start_;

  /// \var load_finish_
  ///  The load of the cable in the finish state.
  mutable double load_finish_;

  /// \var load_start_
  ///   The load of the cable in the start state.
  mutable double load_start_;

  /// \var model_elongation_finish_
  ///   The elongation model for the cable in the finish state.
  CableElongationModel model_elongation_finish_;

  /// \var model_elongation_start_
  ///   The elongation model for the cable in the start state.
  CableElongationModel model_elongation_start_;
};

#endif // TLSLIBRARIES_SAGTENSION_CABLESTRAINER_H_
