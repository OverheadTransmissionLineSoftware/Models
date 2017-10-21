// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef OTLS_MODELS_SAGTENSION_LINECABLEUNLOADER_H_
#define OTLS_MODELS_SAGTENSION_LINECABLEUNLOADER_H_

#include <list>

#include "models/base/error_message.h"
#include "models/sagtension/line_cable_loader_base.h"

/// \par OVERVIEW
///
/// This class unloads a line cable at a specific condition and temperature.
/// The process is as follows:
///   - set up the line cable for sag-tension analysis (see LineCableLoaderBase)
///   - build a cable model for the unloaded state
///   - unload the constraint catenary to the unloaded state
///     (see CatenaryCableUnloader)
class LineCableUnloader : public LineCableLoaderBase {
 public:
  /// \brief Default constructor.
  LineCableUnloader();

  /// \brief Destructor.
  ~LineCableUnloader();

  /// \brief Gets the unloaded length.
  /// \return The unloaded length.
  double LengthUnloaded() const;

  /// \brief Validates member variables.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings = true,
                std::list<ErrorMessage>* messages = nullptr) const;

  /// \brief Gets the unloaded cable condition.
  /// \return The unloaded cable condition.
  CableConditionType condition_unloaded() const;

  /// \brief Sets the unloaded cable condition.
  /// \param[in] condition_unloaded
  ///   The unloaded cable condition.
  void set_condition_unloaded(const CableConditionType& condition_unloaded);

  /// \brief Sets the attachment spacing.
  /// \param[in] spacing_attachments
  ///   The attachment spacing
  void set_spacing_attachments(const Vector3d& spacing_attachments);

  /// \brief Sets the unloaded temperature.
  /// \param[in] temperature_unloaded
  ///   The unloaded temperature.
  void set_temperature_unloaded(const double& temperature_unloaded);

  /// \brief Gets the attachment spacing.
  /// \return The attachment spacing.
  Vector3d spacing_attachments() const;

  /// \brief Gets the unloaded temperature.
  /// \return The unloaded temperature.
  double temperature_unloaded() const;

 private:
  /// \brief Determines if class is updated.
  /// \return A boolean indicating if class is updated.
  virtual bool IsUpdated() const;

  /// \brief Updates cached member variables and modifies control variables if
  ///   update is required.
  /// \return A boolean indicating if class updates completed successfully.
  virtual bool Update() const;

  /// \brief Updates the unloaded cable model.
  /// \return The success status of the update.
  bool UpdateUnloadedCableModel() const;

  /// \brief Updates the unloaded length.
  /// \return The success status of the update.
  bool UpdateUnloadedLength() const;

  /// \var condition_unloaded_
  ///   The condition of the cable when unloaded.
  CableConditionType condition_unloaded_;

  /// \var is_updated_length_unloaded_
  ///   An indicator that tells if the unloaded cable length is updated.
  mutable bool is_updated_length_unloaded_;

  /// \var is_updated_model_unloaded_
  ///   An indicator that tells if the unloaded cable model is updated.
  mutable bool is_updated_model_unloaded_;

  /// \var length_unloaded_
  ///   The unloaded cable length.
  mutable double length_unloaded_;

  /// \var model_unloaded_
  ///   The cable model for the unloaded state. This model may contain stretch,
  ///   depending on the unloaded condition.
  mutable CableElongationModel model_unloaded_;

  /// \var spacing_attachments_
  ///   The spacing of the span attachments. The coordinates follows the
  ///   Catenary3d coordinate system.
  Vector3d spacing_attachments_;

  /// \var temperature_unloaded_
  ///   The temperature that the cable is being unloaded to.
  double temperature_unloaded_;
};

#endif  // OTLS_MODELS_SAGTENSION_LINECABLEUNLOADER_H_
