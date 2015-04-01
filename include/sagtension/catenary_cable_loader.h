// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef TLSLIBRARIES_SAGTENSION_CATENARYCABLELOADER_H_
#define TLSLIBRARIES_SAGTENSION_CATENARYCABLELOADER_H_

#include "base/vector.h"
#include "sagtension/cable_strainer.h"
#include "transmissionline/catenary.h"

/// \par OVERVIEW
///
/// This class models a cable segment in an unloaded state and strains it to a
/// loaded state in the shape of a catenary.
///
/// \par UNLOADED STATE
///
/// The unloaded cable segment defined according to the specified parameters,
/// and is not modeled as a catenary.
///
/// \par LOADED STATE (CATENARY)
///
/// The catenary models the cable in the loaded state.
///
/// The catenary tension varies along the curve. To interact with the cable
/// load-strain model, the catenary tension is converted to a constant
/// effective tension which produces the same elongation as the catenary
/// loading.
///
/// \par CABLE LOAD NUMERICAL SOLUTION
///
/// The cable load is not directly specified, and is solved for numerically.
/// Any modifications to the catenary or unloaded cable parameters will change
/// the cable load solution.
class CatenaryCableLoader {
 public:
  /// \brief Default constructor.
  CatenaryCableLoader();

  /// \brief Destructor.
  ~CatenaryCableLoader();

  /// \brief Gets the length of the cable when loaded.
  /// \return The length when loaded.
  double LengthLoaded() const;

  /// \brief Gets the average load of the cable.
  /// \return The average load of the cable.
  double LoadAverage() const;

  /// \brief Gets the load of the core component.
  /// \return The load of the core component.
  double LoadAverageCore() const;

  /// \brief Gets the load of the shell component.
  /// \return The load of the shell component.
  double LoadAverageShell() const;

  /// \brief Gets the strain transition, or strain difference, between unloaded
  ///   and loaded states.
  /// \return The strain transition from unloaded to loaded state.
  /// This value may be positive or negative, indicating whether the cable is
  /// shrinking (negative) or elongating (positive).
  double StrainTransitionLoad() const;

  /// \brief Gets the thermal based strain transition, or strain difference,
  ///   between unloaded and loaded states.
  /// \return The thermal based strain transition from unloaded to loaded state.
  /// This value may be positive or negative, indicating whether the cable is
  /// shrinking (negative) or elongating (positive).
  double StrainTransitionThermal() const;

  /// \brief Gets the horizontal catenary tension of the entire cable.
  /// \return The horizontal catenary tension of the entire cable.
  double TensionHorizontalCatenary() const;

  /// \brief Gets the horizontal catenary tension of the core component.
  /// \return The horizontal catenary tension of the core component.
  double TensionHorizontalCatenaryCore() const;

  /// \brief Gets the horizontal catenary tension of the shell component.
  /// \return The horizontal catenary tension of the shell component.
  double TensionHorizontalCatenaryShell() const;

  /// \brief Gets the length of the cable when unloaded.
  /// \return The length of cable when unloaded.
  double length_unloaded() const;

  /// \brief Gets the load that the cable is stretched to.
  /// \return The load that the cable was stretched to.
  double load_stretch() const;

  /// \brief Sets the cable.
  /// \param[in] cable
  ///   The cable.
  void set_cable(const Cable& cable);

  /// \brief Sets the length of the cable at the unloaded state.
  /// \param[in] length_unloaded
  ///   The length of the cable at the unloaded state.
  void set_length_unloaded(const double& length_unloaded);

  /// \brief Sets the load that the cable is stretched to.
  /// \param[in] load_stretch
  ///   The load that the cable was stretched to.
  void set_load_stretch(const double& load_stretch);

  /// \brief Sets the endpoint spacing of the catenary.
  /// \param[in] spacing_endpoints
  ///   The vector spacing between endpoints of the catenary.
  void set_spacing_endpoints_catenary(const Vector3d& spacing_endpoints);

  /// \brief Sets the state parameters at the loaded state.
  /// \param[in] state_loaded
  ///   The state parameters at the loaded start.
  void set_state_loaded(const CableStrainerState& state_loaded);

  /// \brief Sets the state parameters at the unloaded state.
  /// \param[in] state_unloaded
  ///   The state parameters at the unloaded state.
  void set_state_unloaded( const CableStrainerState& state_unloaded);

  /// \brief Sets the temperature that the cable was stretched at.
  /// \param[in] temperature_stretch
  ///   The temperature that the cable was stretched at.
  void set_temperature_stretch(const double& temperature_stretch);

  /// \brief Sets the unit weight of the catenary.
  /// \param[in] weight_unit
  ///   The unit weight vector of the catenary.
  void set_weight_unit_catenary(const Vector3d& weight_unit);

  /// \brief Gets the endpoint spacing vector of the catenary.
  /// \return A copy of the endpoint spacing vector of the catenary.
  Vector3d spacing_endpoints_catenary() const;

  /// \brief Gets the state parameters at the loaded state.
  /// \return The state parameters at the loaded state.
  CableStrainerState state_loaded() const;

  /// \brief Gets the state parameters at the unloaded state.
  /// \return The state parameters at the unloaded state.
  CableStrainerState state_unloaded() const;

  /// \brief Getst the temperature that the cable was stretched at.
  /// \return The temperature that the cable was stretched at.
  double temperature_stretch() const;

  /// \brief Gets the unit weight of the catenary.
  /// \return A copy of the unit weight of the catenary.
  Vector3d weight_unit_catenary() const;

 private:
  /// \brief Determines if class is updated.
  /// \return A boolean indicating if class is updated.
  bool IsUpdated() const;

  /// \todo This function needs renamed or better documented. It also updates
  ///   the load of the strainer class.
  /// \brief Gets the difference in length between the catenary curve and the
  /// loaded cable.
  /// \return The difference in length between the catenary curve and the loaded
  ///   cable.
  double LengthDifference(const double& tension_horizontal) const;

  /// \brief Updates cached member variables and modifies control variables if
  ///   update is required.
  /// \return A boolean indicating if class updates completed successfully.
  bool Update() const;

  /// \brief Updates the catenary horizontal tension.
  /// \return The success status of the update.
  bool UpdateTensionHorizontal() const;

  /// \var catenary_
  ///   The shape that models the cable loading.
  mutable Catenary3d catenary_;

  /// \var is_updated_catenary
  ///   An indicator that tells if the catenary, specifically the horizontal
  ///   tension has been updated.
  mutable bool is_updated_catenary_;

  /// \var
  mutable CableStrainer strainer_cable_;
};

#endif // TLSLIBRARIES_SAGTENSION_CATENARYCABLELOADER_H_
