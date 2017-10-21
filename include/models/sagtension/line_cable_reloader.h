// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef OTLS_MODELS_SAGTENSION_LINECABLERELOADER_H_
#define OTLS_MODELS_SAGTENSION_LINECABLERELOADER_H_

#include <list>

#include "models/base/error_message.h"
#include "models/sagtension/line_cable_loader_base.h"
#include "models/transmissionline/catenary.h"

/// \par OVERVIEW
///
/// This class reloads a line cable to a specified weather case and condition.
/// The process is as follows:
///   - set up the line cable for sag-tension analysis (see LineCableLoaderBase)
///   - build a cable model for the reloaded state
///   - reload the constraint catenary to the reloaded state
///     (see CatenaryCableReloader)
class LineCableReloader : public LineCableLoaderBase {
 public:
  /// \brief Default constructor.
  LineCableReloader();

  /// \brief Destructor.
  ~LineCableReloader();

  /// \brief Gets a reloaded catenary.
  /// \return A catenary at the reloaded load case.
  Catenary3d CatenaryReloaded() const;

  /// \brief Gets the reloaded cable state.
  /// \return The reloaded cable state.
  CableState StateReloaded() const;

  /// \brief Gets the average load of the cable component.
  /// \param[in] type_component
  ///   The model component type.
  /// \return The average load of the cable component.
  double TensionAverageComponent(
      const CableElongationModel::ComponentType& type_component) const;

  /// \brief Gets the horizontal tension.
  /// \return The horizontal tension.
  double TensionHorizontal() const;

  /// \brief Gets the horizontal tension of the cable component.
  /// \param[in] type_component
  ///   The model component type.
  /// \return The horizontal tension of the cable component.
  double TensionHorizontalComponent(
      const CableElongationModel::ComponentType& type_component) const;

  /// \brief Validates member variables.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings = true,
                std::list<ErrorMessage>* messages = nullptr) const;

  /// \brief Gets the reloaded cable condition.
  /// \return The reloaded cable condition.
  CableConditionType condition_reloaded() const;

  /// \brief Sets the reloaded cable condition.
  /// \param[in] condition_reloaded
  ///   The reloaded cable condition.
  void set_condition_reloaded(const CableConditionType& condition_reloaded);

  /// \brief Sets the reloaded load case.
  /// \param[in] weathercase_reloaded
  ///   The reloaded weathercase.
  void set_weathercase_reloaded(const WeatherLoadCase* weathercase_reloaded);

  /// \brief Gets the reloaded load case.
  /// \return The reloaded load case.
  const WeatherLoadCase* weathercase_reloaded() const;

 private:
  /// \brief Determines if class is updated.
  /// \return A boolean indicating if class is updated.
  virtual bool IsUpdated() const;

  /// \brief Updates cached member variables and modifies control variables if
  ///   update is required.
  /// \return A boolean indicating if class updates completed successfully.
  virtual bool Update() const;

  /// \brief Updates the reloaded cable model.
  /// \return The success status of the update.
  bool UpdateReloadedCableModel() const;

  /// \brief Updates the reloaded catenary.
  /// \return The success status of the update.
  bool UpdateReloadedCatenary() const;

  /// \var catenary_reloaded_
  ///   The catenary that is reloaded.
  mutable Catenary3d catenary_reloaded_;

  /// \var condition_reloaded_
  ///   The condition of the cable when loaded at the query load case.
  CableConditionType condition_reloaded_;

  /// \var is_updated_catenary_reloaded_
  ///   An indicator that tells if the reloaded catenary is updated.
  mutable bool is_updated_catenary_reloaded_;

  /// \var is_updated_model_reloaded_
  ///   An indicator that tells if the reloaded cable model is updated.
  mutable bool is_updated_model_reloaded_;

  /// \var model_reloaded_
  ///   The cable model for the reloaded weathercase. This model may contain
  ///   stretch, depending on the reloaded condition.
  mutable CableElongationModel model_reloaded_;

  /// \var weathercase_reloaded_
  ///   The load case that the cable is being reloaded to.
  const WeatherLoadCase* weathercase_reloaded_;
};

#endif  // OTLS_MODELS_SAGTENSION_LINECABLERELOADER_H_
