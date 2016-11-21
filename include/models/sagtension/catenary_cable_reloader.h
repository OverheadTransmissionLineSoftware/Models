// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef OTLS_MODELS_SAGTENSION_CATENARYCABLERELOADER_H_
#define OTLS_MODELS_SAGTENSION_CATENARYCABLERELOADER_H_

#include <list>

#include "models/base/error_message.h"
#include "models/base/vector.h"
#include "models/sagtension/cable_strainer.h"
#include "models/transmissionline/catenary.h"

/// \par OVERVIEW
///
/// This class reloads a catenary to a specified state and loading by using
/// cable elongation models.
///
/// \par CATENARY
///
/// The catenary represents a mechanically loaded cable. The catenary tension
/// varies along the curve, and must be approximated by an effective or average
/// tension in order to interact with the cable elongation models.
///
/// \par CABLE ELONGATION MODELS
///
/// This class supports having two models for the cable: reference and reloaded.
/// These must be identical cables, but can be different states. This allows the
/// cable to be reloaded to different temperatures and stretch amounts.
///
/// \par UNLOADING
///
/// The catenary is unloaded using the reference cable model. The unloaded
/// length is cached and used for reloading the catenary.
///
/// \par LOADING
///
/// Using the unloaded length, the cable is strained from the reference cable
/// model to the reloaded cable model. The horizontal tension of the reloaded
/// catenary is numerically solved. The horizontal tension solution will have
/// a catenary length and cable length (as predicted by the cable strainer)
/// that match.
class CatenaryCableReloader {
 public:
  /// \brief Default constructor.
  CatenaryCableReloader();

  /// \brief Destructor.
  ~CatenaryCableReloader();

  /// \brief Gets the reloaded catenary.
  /// \return The reloaded catenary.
  Catenary3d CatenaryReloaded() const;

  /// \brief Gets the length of the cable when unloaded, using the reference
  ///   cable model.
  /// \return The length of cable when unloaded, using the reference cable
  ///   model.
  double LengthUnloaded() const;

  /// \brief Gets the horizontal tension of the reloaded catenary.
  /// \return The horizontal tension of the reloaded catenary.
  double TensionHorizontal() const;

  /// \brief Validates member variables.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings,
                std::list<ErrorMessage>* messages = nullptr) const;

  /// \brief Gets the reference (starting) catenary.
  /// \return The reference catenary.
  const Catenary3d* catenary() const;

  /// \brief Gets the reference (starting) cable model.
  /// \return The reference cable model.
  const CableElongationModel* model_reference() const;

  /// \brief Gets the reloaded cable model.
  /// \return The reloaded cable model.
  const CableElongationModel* model_reloaded() const;

  /// \brief Sets the reference catenary.
  /// \param[in] catenary
  ///   The reference catenary.
  void set_catenary(const Catenary3d* catenary);

  /// \brief Sets the reference (starting) cable model.
  /// \param[in] model_reference
  ///   The reference cable model.
  void set_model_reference(const CableElongationModel* model_reference);

  /// \brief Sets the reloaded cable model.
  /// \param[in] model_reloaded
  ///   The reloaded cable model.
  void set_model_reloaded(const CableElongationModel* model_reloaded);

  /// \brief Sets the unit weight of the reloaded catenary.
  /// \param[in] weight_unit_reloaded
  ///   The unit weight of the reloaded catenary.
  void set_weight_unit_reloaded(const Vector3d* weight_unit_reloaded);

  /// \brief Gets the unit weight of the reloaded catenary.
  /// \return The unit weight of the reloaded catenary.
  const Vector3d* weight_unit_reloaded() const;

 private:
  /// \brief Initializes the reloaded catenary.
  /// \return A boolean indicating the success status.
  bool InitializeReloadedCatenary() const;

  /// \brief Initializes the strainer.
  /// \return A boolean indicating the success status.
  bool InitializeStrainer() const;

  /// \brief Determines if class is updated.
  /// \return A boolean indicating if class is updated.
  bool IsUpdated() const;

  /// \brief Gets the difference in length between the catenary curve and the
  /// loaded cable.
  /// \param[in] tension_horizontal
  ///   The horizontal tension for the catenary.
  /// \return The difference in length between the catenary curve and the loaded
  ///   cable.
  /// Length difference = catenary length - cable length
  double LengthDifference(const double& tension_horizontal) const;

  /// \brief Solves the reloaded catenary horizontal tension.
  /// \return The success status of solution.
  bool SolveReloadedCatenaryTension() const;

  /// \brief Updates cached member variables and modifies control variables if
  ///   update is required.
  /// \return A boolean indicating if class updates completed successfully.
  bool Update() const;

  /// \brief Updates the unloaded length of cable using the reference cable
  ///   model.
  /// \return The success status of the update.
  bool UpdateLengthUnloaded() const;

  /// \brief Updates the catenary horizontal tension and load of the cable
  ///   strainer.
  /// \param[in] tension_horizontal
  ///   The horizontal tension for the catenary.
  /// \return The success status of the update.
  bool UpdatedReloadedCatenaryAndStrainer(
      const double& tension_horizontal) const;

  /// \var catenary_
  ///   The reference catenary.
  const Catenary3d* catenary_;

  /// \var catenary_reloaded_
  ///   The reloaded catenary.
  mutable Catenary3d catenary_reloaded_;

  /// \var is_updated_catenary_reloaded_
  ///   An indicator that tells if the catenary has been reloaded.
  mutable bool is_updated_catenary_reloaded_;

  /// \var is_updated_length_unloaded_
  ///   An indicator that tells if the unloaded length is updated.
  mutable bool is_updated_length_unloaded_;

  /// \var length_unloaded_
  ///   The length of cable when unloaded, using the reference cable model.
  mutable double length_unloaded_;

  /// \var model_reference_
  ///   The reference (starting) cable model.
  const CableElongationModel* model_reference_;

  /// \var model_reloaded_
  ///   The reloaded cable model.
  const CableElongationModel* model_reloaded_;

  /// \var strainer_
  ///   The strainer that uses cable elongation models to strain the cable from
  ///   an unloaded to a loaded state.
  mutable CableStrainer strainer_;

  /// \var weight_unit_reloaded_
  ///   The unit weight of the reloaded catenary.
  const Vector3d* weight_unit_reloaded_;
};

#endif // OTLS_MODELS_SAGTENSION_CATENARYCABLERELOADER_H_
