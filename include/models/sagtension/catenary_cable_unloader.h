// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef OTLS_MODELS_SAGTENSION_CATENARYCABLEUNLOADER_H_
#define OTLS_MODELS_SAGTENSION_CATENARYCABLEUNLOADER_H_

#include <list>

#include "models/base/error_message.h"
#include "models/base/vector.h"
#include "models/sagtension/cable_state.h"
#include "models/sagtension/cable_strainer.h"
#include "models/sagtension/catenary_cable.h"
#include "models/transmissionline/catenary.h"

/// \par OVERVIEW
///
/// This class unloads a catenary cable.
///
/// \par LOADED STATE
///
/// The catenary catenary cable represents the cable in the loaded state.
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

  /// \brief Gets the catenary cable.
  /// \return The catenary cable.
  const CatenaryCable* catenary_cable() const;

  /// \brief Sets the catenary cable.
  /// \param[in] catenary_cable
  ///   The catenary cable.
  void set_catenary_cable(const CatenaryCable* catenary_cable);

  /// \brief Sets the unloaded state.
  /// \param[in] state_unloaded
  ///   The unloaded state.
  void set_state_unloaded(const CableState* state_unloaded);

  /// \brief Gets the unloaded state.
  /// \return The unloaded state.
  const CableState* state_unloaded() const;

 private:
  /// \brief Determines if class is updated.
  /// \return A boolean indicating if class is updated.
  bool IsUpdated() const;

  /// \brief Updates cached member variables and modifies control variables if
  ///   update is required.
  /// \return A boolean indicating if class updates completed successfully.
  bool Update() const;

  /// \brief Updates the strainer with the laoded state (catenary cable)
  ///   parameters.
  /// \return The success status of the update.
  bool UpdateStrainer() const;

  /// \var catenary_cable_
  ///   The catenary cable that is unloaded.
  const CatenaryCable* catenary_cable_;

  /// \var is_updated_strainer_
  ///   An indicator that tells if the cable strainer is updated.
  mutable bool is_updated_strainer_;

  /// \var strainer_
  ///   The strainer that uses cable elongation models to strain the cable from
  ///   a loaded to an unloaded state.
  mutable CableStrainer strainer_;
};

#endif // OTLS_MODELS_SAGTENSION_CATENARYCABLEUNLOADER_H_
