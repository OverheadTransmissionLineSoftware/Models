// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef MODELS_SAGTENSION_LINE_CABLE_LOADER_BASE_H_
#define MODELS_SAGTENSION_LINE_CABLE_LOADER_BASE_H_

#include <list>

#include "models/base/error_message.h"
#include "models/sagtension/cable_elongation_model.h"
#include "models/sagtension/sag_tension_cable.h"
#include "models/transmissionline/catenary.h"
#include "models/transmissionline/line_cable.h"

/// \par OVERVIEW
///
/// This class sets up a LineCable for a sag-tension analysis. The process is
/// as follows:
///   - solve for a catenary based on the line cable constraint
///   - solve cable model for the constraint
///   - solve cable models for the stretch conditions
///   - determine the stretch load for each condition
///
/// \par STRETCH
///
/// This class supports multiple forms of stretch, including:
///   - short term, heavy load stretch
///   - long term, creep stretch
///
/// \par LINE CABLE GEOMETRY
///
/// This class uses the ruling span geometry defined in the line cable.
class LineCableLoaderBase {
 public:
  /// \brief Default constructor.
  LineCableLoaderBase();

  /// \brief Destructor.
  ~LineCableLoaderBase();

  /// \brief Gets the stretch state for the creep condition.
  /// \return The stretch state for the creep condition.
  CableStretchState StretchStateCreep() const;

  /// \brief Gets the stretch state for the load condition.
  /// \return The stretch state for the load condition.
  CableStretchState StretchStateLoad() const;

  /// \brief Validates member variables.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings = true,
                std::list<ErrorMessage>* messages = nullptr) const;

  /// \brief Gets the line cable.
  /// \return The line cable.
  const LineCable* line_cable() const;

  /// \brief Sets the line cable.
  /// \param[in] line_cable
  ///   The line cable.
  void set_line_cable(const LineCable* line_cable);

 protected:
  /// \brief Initializes the models defined in the line cable.
  /// \return The success status of the update.
  /// This function initializes the cable models with the correct temperature
  /// and polynomial, but does not solve for any stretch.
  bool InitializeLineCableModels() const;

  /// \brief Determines if class is updated.
  /// \return A boolean indicating if class is updated.
  virtual bool IsUpdated() const;

  /// \brief Calculates the unit load of the catenary.
  /// \param[in] weathercase
  ///   The weather case.
  /// \return The unit loading of the cable.
  Vector3d UnitLoad(const WeatherLoadCase& weathercase) const;

  /// \brief Updates cached member variables and modifies control variables if
  ///   update is required.
  /// \return A boolean indicating if class updates completed successfully.
  virtual bool Update() const;

  /// \brief Updates the constraint cable model.
  /// \return The success status of the update.
  /// This function further updates the constraint cable model to include any
  /// stretch.
  bool UpdateConstraintCableModel() const;

  /// \brief Updates the constraint catenary.
  /// \return The success status of the update.
  bool UpdateConstraintCatenary() const;

  /// \brief Updates the stretch due to load and creep.
  /// \return The success status of the update.
  bool UpdateLoadStretch() const;

  /// \var cable_sagtension_
  ///   The cable, which is referenced for sag-tension methods.
  SagTensionCable cable_sagtension_;

  /// \var catenary_constraint_
  ///   The catenary for the line cable constraint.
  mutable Catenary3d catenary_constraint_;

  /// \var is_updated_catenary_constraint_
  ///   An indicator that tells if the constraint catenary cable is updated.
  mutable bool is_updated_catenary_constraint_;

  /// \var is_updated_stretch_
  ///   An indicator that tells if the stretch load is updated.
  mutable bool is_updated_stretch_;

  /// \var line_cable_
  ///   The line cable that is being reloaded.
  const LineCable* line_cable_;

  /// \var model_constraint_
  ///   The cable model for the constraint weathercase. This model may contain
  ///   stretch, depending on the constraint condition.
  mutable CableElongationModel model_constraint_;

  /// \var model_creep_
  ///   The cable model used for solving creep stretch. This model will always
  ///   contain zero stretch to ensure that only the polynomial is referenced.
  mutable CableElongationModel model_creep_;

  /// \var model_load_
  ///   The cable model used for solving heavy load stretch. This model will
  ///   always contain zero stretch to ensure that only the polynomial is
  ///   referenced.
  mutable CableElongationModel model_load_;

  /// \var state_stretch_creep_
  ///   The stretch state for the creep condition.
  mutable CableStretchState state_stretch_creep_;

  /// \var state_stretch_load_;
  ///   The stretch state for the load condition.
  mutable CableStretchState state_stretch_load_;
};

#endif  // MODELS_SAGTENSION_LINE_CABLE_LOADER_BASE_H_
