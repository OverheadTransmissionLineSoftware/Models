// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef MODELS_TRANSMISSIONLINE_STRUCTURE_H_
#define MODELS_TRANSMISSIONLINE_STRUCTURE_H_

#include <list>
#include <string>
#include <vector>

#include "models/base/error_message.h"

/// \par OVERVIEW
///
/// This struct is a container for a structure attachment.
struct StructureAttachment {
 public:
  /// \brief Default constructor.
  StructureAttachment();

  /// \brief Destructor.
  ~StructureAttachment();

  /// \brief Validates member variables.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings = true,
                std::list<ErrorMessage>* messages = nullptr) const;

  /// \var offset_longitudinal
  ///   The longitudinal offset from the center of the structure.
  double offset_longitudinal;

  /// \var offset_transverse
  ///   The transverse offset from the center of the structure.
  double offset_transverse;

  /// \var offset_vertical_top
  ///   The vertical offset from the top of the structure.
  double offset_vertical_top;
};

/// par OVERVIEW
///
/// This struct is a container for a transmission structure.
struct Structure {
 public:
  /// \brief Default constructor.
  Structure();

  /// \brief Destructor.
  ~Structure();

  /// \brief Validates member variables.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings = true,
                std::list<ErrorMessage>* messages = nullptr) const;

  /// \var attachments
  ///   The attachments.
  std::vector<StructureAttachment> attachments;

  /// \var height
  ///   The height.
  double height;

  /// \var name
  ///   The name.
  std::string name;
};

#endif  // MODELS_TRANSMISSIONLINE_STRUCTURE_H_
