// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef OTLS_MODELS_SAGTENSION_CABLESTRAINER_H_
#define OTLS_MODELS_SAGTENSION_CABLESTRAINER_H_

#include <list>

#include "models/base/error_message.h"
#include "models/sagtension/cable_elongation_model.h"
#include "models/sagtension/cable_state.h"
#include "models/sagtension/sag_tension_cable.h"

/// \par OVERVIEW
///
/// This class strains the cable from a starting state to a finish state.
///
/// \par START AND FINISH STATE
///
/// The following parameters can be specified for both starting and finish
/// states:
///   - cable state (temperature, stretch load, stretch temperature)
///   - load
///   - polynomial type
///
/// \par CABLE ELONGATION MODELS
///
/// This class uses cable elongation models to determine the cable strain at
/// the start and finish states. To determine the strain from the start to
/// finish state, the strain values from the two states are compared.
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

  /// \brief Validates member variables.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings,
                std::list<ErrorMessage>* messages = nullptr) const;

  /// \brief Gets the cable.
  /// \return A copy of the cable.
  const SagTensionCable* cable() const;

  /// \brief Gets the length of the cable at the start state.
  /// \return The length of cable at the start state.
  double length_start() const;

  /// \brief Gets the load of the cable at the finish state.
  /// \return The load of the cable at the finish state.
  double load_finish() const;

  /// \brief Gets the load of the cable at the start state.
  /// \return The load of the cable at the start state.
  double load_start() const;

  /// \brief Sets the cable.
  /// \param[in] cable
  ///   The cable.
  void set_cable(const SagTensionCable* cable);

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

  /// \brief Sets the finish state.
  /// \param[in] state_finish
  ///   The finish state.
  void set_state_finish(const CableState* state_finish);

  /// \brief Sets the start state.
  /// \param[in] state_start
  ///   The start state.
  void set_state_start(const CableState* state_start);

  /// \brief Gets the finish state.
  /// \return The finish state.
  const CableState* state_finish() const;

  /// \brief Gets the start state.
  /// \return The start state.
  const CableState* state_start() const;

 private:
  /// \var length_start_
  ///   The length of the cable in the start state.
  double length_start_;

  /// \var load_finish_
  ///  The load of the cable in the finish state.
  double load_finish_;

  /// \var load_start_
  ///   The load of the cable in the start state.
  double load_start_;

  /// \var model_elongation_finish_
  ///   The elongation model for the cable in the finish state.
  CableElongationModel model_elongation_finish_;

  /// \var model_elongation_start_
  ///   The elongation model for the cable in the start state.
  CableElongationModel model_elongation_start_;
};

#endif // OTLS_MODELS_SAGTENSION_CABLESTRAINER_H_
