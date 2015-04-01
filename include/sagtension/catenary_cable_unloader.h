// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef TLSLIBRARIES_SAGTENSION_CATENARYCABLEUNLOADER_H_
#define TLSLIBRARIES_SAGTENSION_CATENARYCABLEUNLOADER_H_

#include "base/vector.h"
#include "sagtension/cable_strainer.h"
#include "transmissionline/catenary.h"

/// \par OVERVIEW
///
/// This class models a cable segment in a loaded state (in the shape of a
/// catenary) and strains it to an unloaded state.
///
/// \par LOADED STATE (CATENARY)
///
/// The catenary represents the cable in the loaded state.
///
/// The catenary tension varies along the curve. To interact with the cable
/// elongation model, the catenary tension is converted to a constant effective
/// tension which produces the same elongation as the catenary loading.
///
/// \par UNLOADED STATE
///
/// The cable segment is unloaded according to the specified parameters, and is
/// no longer modeled as a catenary.
class CatenaryCableUnloader {
 public:
  /// \brief Default constructor.
  CatenaryCableUnloader();

  /// \brief Destructor.
  ~CatenaryCableUnloader();

  /// \brief Gets the length of the cable when loaded.
  /// \return The length when loaded.
  double LengthLoaded() const;

  /// \brief Gets the length of the cable when unloaded.
  /// \return The length when unloaded.
  double LengthUnloaded() const;

  /// \brief Gets the strain transition, or strain difference, between loaded
  ///   and unloaded states.
  /// \return The strain transition from loaded to unloaded state.
  /// This value may be positive or negative, indicating whether the cable is
  /// shrinking (negative) or elongating (positive).
  double StrainTransitionLoad() const;

  /// \brief Gets the thermal based strain transition, or strain difference,
  ///   between loaded and unloaded states.
  /// \return The thermal based strain transition from loaded to unloaded state.
  /// This value may be positive or negative, indicating whether the cable is
  /// shrinking (negative) or elongating (positive).
  double StrainTransitionThermal() const;

  /// \brief Gets the load that the cable is stretched to.
  /// \return The load that the cable was stretched to.
  double load_stretch() const;

  /// \brief Sets the cable.
  /// \param[in] cable
  ///   The cable.
  void set_cable(const Cable& cable);

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

  /// \brief Sets the horizontal tension of the catenary.
  /// \param[in] tension_horizontal
  ///   The horizontal tension of the catenary.
  void set_tension_horizontal_catenary(const double& tension_horizontal);

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

  /// \brief Gets the horizontal tension of the catenary.
  /// \return The horizontal tension of the catenary.
  double tension_horizontal() const;

  /// \brief Gets the unit weight of the catenary.
  /// \return A copy of the unit weight of the catenary.
  Vector3d weight_unit_catenary() const;

 private:
  /// \brief Determines if class is updated.
  /// \return A boolean indicating if class is updated.
  bool IsUpdated() const;

  /// \brief Updates cached member variables and modifies control variables if
  ///   update is required.
  /// \return A boolean indicating if class updates completed successfully.
  bool Update() const;

  /// \todo This function needs renamed - it updates both length and load.
  /// \brief Updates the effective load of the cable, which is based on the
  ///   catenary tension.
  /// \return The success status of the update.
  bool UpdateLoadEffective() const;

  /// \var catenary_
  ///   The shape that models the cable loading.
  mutable Catenary3d catenary_;

  /// \var is_updated_load_
  ///   An indicator that tells if the effective load of the cable is updated.
  mutable bool is_updated_load_;

  /// \var strainer_cable_
  ///   The strainer that uses cable elongation models to strain the cable from
  ///   a loaded to an unloaded state.
  mutable CableStrainer strainer_cable_;
};

#endif // TLSLIBRARIES_SAGTENSION_CATENARYCABLEUNLOADER_H_
