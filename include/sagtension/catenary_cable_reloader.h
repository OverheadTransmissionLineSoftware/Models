// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef TLSLIBRARIES_SAGTENSION_CATENARYCABLERELOADER_H_
#define TLSLIBRARIES_SAGTENSION_CATENARYCABLERELOADER_H_

#include <list>
#include <string>

#include "base/vector.h"
#include "sagtension/cable_state.h"
#include "sagtension/cable_strainer.h"
#include "sagtension/catenary_cable.h"
#include "transmissionline/catenary.h"

/// \par OVERVIEW
///
/// This class reloads a catenary cable to a specified state and unit weight.
///
/// \par UNLOADING
///
/// The catenary cable is unloaded to a fixed temperature. The unloaded
/// unstretched length is cached and used for reloading the catenary cable.
///
/// \par LOADING
///
/// Using the unloaded unstretched length, the horizontal tension of the
/// reloaded catenary cable is numerically solved. The horizontal tension
/// solution will have a catenary length and cable length (as predicted by the
/// cable strainer) that match.
///
/// The catenary tension varies along the curve. To interact with the cable
/// load-strain model, the catenary tension is converted to a constant
/// effective tension which produces the same elongation as the catenary
/// loading.
class CatenaryCableReloader {
 public:
  /// \brief Default constructor.
  CatenaryCableReloader();

  /// \brief Destructor.
  ~CatenaryCableReloader();

  /// \brief Gets the reloaded catenary cable.
  /// \return The reloaded catenary cable.
  CatenaryCable CatenaryCableReloaded() const;

  /// \brief Gets the length of the cable when unloaded and unstretched.
  /// \return The length of cable when unloaded and unstretched.
  double LengthUnloadedUnstretched() const;

  /// \brief Gets the horizontal tension of the reloaded catenary cable.
  /// \return The horizontal tension of the reloaded catenary cable.
  double TensionHorizontal() const;

  /// \brief Validates member variables.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages_error
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings,
                std::list<std::string>* messages_error) const;

  /// \brief Gets the catenary cable.
  /// \return The catenary cable.
  CatenaryCable catenary_cable() const;

  /// \brief Sets the catenary cable.
  /// \param[in] catenary_cable
  ///   The catenary cable.
  void set_catenary_cable(const CatenaryCable& catenary_cable);

  /// \brief Sets the reloaded state.
  /// \param[in] state_reloaded
  ///   The reloaded loaded state.
  void set_state_reloaded(const CableState& state_reloaded);

  /// \brief Sets the unit weight of the reloaded catenary.
  /// \param[in] weight_unit_reloaded
  ///   The unit weight of the reloaded catenary.
  void set_weight_unit_reloaded(const Vector3d& weight_unit_reloaded);

  /// \brief Gets the reloaded state.
  /// \return The reloaded state.
  CableState state_reloaded() const;

  /// \brief Gets the unit weight of the reloaded catenary.
  /// \return The unit weight of the reloaded catenary.
  Vector3d weight_unit_reloaded() const;

 private:
  /// \brief Initializes the reloaded catenary cable.
  /// \return A boolean indicating the success status.
  bool InitializeReloadedCatenaryCable() const;

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

  /// \brief Solves the reloaded catenary cable horizontal tension.
  /// \return The success status of solution.
  bool SolveReloadedCatenaryCableTension() const;

  /// \brief Updates cached member variables and modifies control variables if
  ///   update is required.
  /// \return A boolean indicating if class updates completed successfully.
  bool Update() const;

  /// \brief Updates the unloaded unstretched length of cable.
  /// \return The success status of the update.
  bool UpdateLengthUnloadedUnstretched() const;

  /// \brief Updates the catenary horizontal tension and load of the cable
  ///   strainer.
  /// \param[in] tension_horizontal
  ///   The horizontal tension for the catenary.
  /// \return The success status of the update.
  bool UpdatedReloadedCatenaryCableAndStrainer(
      const double& tension_horizontal) const;

  /// \var catenary_cable_
  ///   The reference catenary cable.
  CatenaryCable catenary_cable_;

  /// \var catenary_cable_reloaded_
  ///   The reloaded catenary cable.
  mutable CatenaryCable catenary_cable_reloaded_;

  /// \var is_updated_catenary_cable_reloaded_
  ///   An indicator that tells if the catenary cable has been reloaded.
  mutable bool is_updated_catenary_cable_reloaded_;

  /// \var is_updated_length_unloaded_unstretched_
  ///   An indicator that tells if the unloaded unstretched length is updated.
  mutable bool is_updated_length_unloaded_unstretched_;

  /// \var length_unloaded_unstretched_
  ///   The length of cable when unloaded and unstretched.
  mutable double length_unloaded_unstretched_;

  /// \var state_reloaded_
  ///   The state of the reloaded catenary cable.
  CableState state_reloaded_;

  /// \var state_unloaded_
  ///   The state of hte unloaded catenary cable.
  CableState state_unloaded_;

  /// \var strainer_
  ///   The strainer that uses cable elongation models to strain the cable from
  ///   an unloaded to a loaded state.
  mutable CableStrainer strainer_;

  /// \var weight_unit_reloaded_
  ///   The unit weight of the reloaded catenary.
  Vector3d weight_unit_reloaded_;
};

#endif // TLSLIBRARIES_SAGTENSION_CATENARYCABLERELOADER_H_
