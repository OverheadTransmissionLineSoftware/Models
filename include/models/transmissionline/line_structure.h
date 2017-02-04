// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef OTLS_MODELS_TRANSMISSIONLINE_LINESTRUCTURE_H_
#define OTLS_MODELS_TRANSMISSIONLINE_LINESTRUCTURE_H_

#include <list>

#include "models/base/error_message.h"
#include "models/transmissionline/hardware.h"
#include "models/transmissionline/structure.h"

/// \par OVERVIEW
///
/// This class models a structure that is used in a transmission line.
class LineStructure {
 public:
  /// \brief Default constructor.
  LineStructure();

  /// \brief Destructor.
  ~LineStructure();

  /// \brief Attaches a hardware assembly to the structure.
  /// \param[in] index
  ///   The index of the attachment.
  /// \param[in] hardware
  ///   The hardware assembly.
  void AttachHardware(const int& index, const Hardware* hardware);

  /// \brief Detaches a hardware assembly from the structure.
  /// \param[in] index
  ///   The index of the attachment.
  void DetachHardware(const int& index);

  /// \brief Validates member variables.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings = true,
                std::list<ErrorMessage>* messages = nullptr) const;

  /// \brief Gets the hardware assemblies attached to the structure.
  /// \return The hardware assemblies attached to the structure.
  const std::vector<const Hardware*>* hardwares() const;

  /// \brief Gets the height adjustment.
  /// \return The height adjustment.
  double height_adjustment() const;

  /// \brief Gets the offset.
  /// \return The offset.
  double offset() const;

  /// \brief Gets the rotation.
  /// \return The rotation.
  double rotation() const;

  /// \brief Sets the height adjustment.
  /// \param[in] height_adjustment
  ///   The height adjustment from the alignment.
  void set_height_adjustment(const double& height_adjustment);

  /// \brief Sets the offset.
  /// \param[in] offset
  ///   The offset from the alignment.
  void set_offset(const double& offset);

  /// \brief Sets the rotation.
  /// \param[in] rotation
  ///   The rotation.
  void set_rotation(const double& rotation);

  /// \brief Sets the station.
  /// \param[in] station
  ///   The position along the alignment.
  void set_station(const double& station);

  /// \brief Sets the base structure.
  /// \param[in] structure
  ///   The base structure.
  void set_structure(const Structure* structure);

  /// \brief Gets the station.
  /// \return The station.
  double station() const;

  /// \brief Gets the base structure.
  /// \return The base structure.
  const Structure* structure() const;

 private:
  /// \brief Determines if the attachment index is valid.
  /// \param[in] index
  ///   The attachment index.
  /// \return If the index is valid.
  bool IsValidIndex(const int& index) const;

  /// \var hardwares_
  ///   The attached hardware assemblies. The index of this vector aligns with
  ///   the attachments vector in the base structure. If the structure
  ///   attachment point does not have an assembly attached, a nullptr is
  ///   assigned. This variable name is used as a plural for consistency with
  ///   other containers.
  std::vector<const Hardware*> hardwares_;

  /// \var height_adjustment_
  ///   The vertical height adjustment from the alignment center line.
  double height_adjustment_;

  /// \var offset_
  ///   The transverse offset from the alignment centerline.
  double offset_;

  /// \var rotation_
  ///   The angular (degree) rotation from the neutral structure position.
  double rotation_;

  /// \var station_
  ///   The longitudinal location along the transmission centerline.
  double station_;

  /// \var structure_
  ///   The base structure.
  const Structure* structure_;
};

#endif  // OTLS_MODELS_TRANSMISSIONLINE_LINESTRUCTURE_H_