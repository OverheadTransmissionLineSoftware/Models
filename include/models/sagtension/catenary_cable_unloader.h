// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef MODELS_SAGTENSION_CATENARY_CABLE_UNLOADER_H_
#define MODELS_SAGTENSION_CATENARY_CABLE_UNLOADER_H_

#include <list>

#include "models/base/error_message.h"
#include "models/sagtension/cable_strainer.h"
#include "models/transmissionline/catenary.h"

/// \par OVERVIEW
///
/// This class unloads a catenary to a specified state by using cable
/// elongation models.
///
/// \par CATENARY
///
/// The catenary represents a mechanically loaded cable. The catenary tension
/// varies along the curve, and must be approximated by an effective or average
/// tension in order to interact with the cable elongation models.
///
/// \par CABLE ELONGATION MODELS
///
/// This class supports having two models for the cable: reference and unloaded.
/// These must be identical cables, but can be different states. This allows the
/// cable to be unloaded to different temperatures and stretch amounts.
class CatenaryCableUnloader {
 public:
  /// \brief Default constructor.
  CatenaryCableUnloader();

  /// \brief Destructor.
  ~CatenaryCableUnloader();

  /// \brief Gets the length of the cable when unloaded.
  /// \return The length when unloaded.
  double LengthUnloaded() const;

  /// \brief Validates member variables.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings,
                std::list<ErrorMessage>* messages) const;

  /// \brief Gets the catenary.
  /// \return The catenary.
  const Catenary3d* catenary() const;

  /// \brief Gets the reference (starting) elongation model.
  /// \return The reference (starting) elongation model.
  const CableElongationModel* model_reference() const;

  /// \brief Gets the unloaded elongation model.
  /// \return The unloaded elongation model.
  const CableElongationModel* model_unloaded() const;

  /// \brief Sets the catenary.
  /// \param[in] catenary
  ///   The catenary.
  void set_catenary(const Catenary3d* catenary);

  /// \brief Sets the reference (starting) elongation model.
  /// \param[in] model_reference
  ///   The reference elongation model.
  void set_model_reference(const CableElongationModel* model_reference);

  /// \brief Sets the unloaded elongation model.
  /// \param[in] model_unloaded
  ///   The unloaded elongation model.
  void set_model_unloaded(const CableElongationModel* model_unloaded);

 private:
  /// \var catenary_
  ///   The catenary.
  const Catenary3d* catenary_;

  /// \var strainer_
  ///   The strainer that uses cable elongation models to strain the cable from
  ///   a loaded to an unloaded state.
  mutable CableStrainer strainer_;
};

#endif  // MODELS_SAGTENSION_CATENARY_CABLE_UNLOADER_H_
