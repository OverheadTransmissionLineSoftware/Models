// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef OTLS_MODELS_SAGTENSION_CABLESTRAINER_H_
#define OTLS_MODELS_SAGTENSION_CABLESTRAINER_H_

#include <list>

#include "models/base/error_message.h"
#include "models/sagtension/cable_elongation_model.h"

/// \par OVERVIEW
///
/// This class strains the cable from a starting state to a finish state.
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

  /// \brief Gets the length of the cable at the start state.
  /// \return The length of cable at the start state.
  double length_start() const;

  /// \brief Gets the load of the cable at the finish state.
  /// \return The load of the cable at the finish state.
  double load_finish() const;

  /// \brief Gets the load of the cable at the start state.
  /// \return The load of the cable at the start state.
  double load_start() const;

  /// \brief Gets the model for the finish state.
  /// \return The model for the finish state.
  const CableElongationModel* model_finish() const;

  /// \brief Gets the model for the start state.
  /// \return The model for the start state.
  const CableElongationModel* model_start() const;

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

  /// \brief Sets the model for the finish state.
  /// \param[in] model_finish
  ///   The model for the finish state.
  void set_model_finish(const CableElongationModel* model_finish);

  /// \brief Sets the model for the start state.
  /// \param[in] model_start
  ///   The model for the start state.
  void set_model_start(const CableElongationModel* model_start);

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
  const CableElongationModel* model_elongation_finish_;

  /// \var model_elongation_start_
  ///   The elongation model for the cable in the start state.
  const CableElongationModel* model_elongation_start_;
};

#endif  // OTLS_MODELS_SAGTENSION_CABLESTRAINER_H_
