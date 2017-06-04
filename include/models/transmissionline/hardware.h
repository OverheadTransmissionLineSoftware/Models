// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef OTLS_MODELS_TRANSMISSIONLINE_HARDWARE_H_
#define OTLS_MODELS_TRANSMISSIONLINE_HARDWARE_H_

#include <list>
#include <string>

#include "models/base/error_message.h"

/// \par OVERVIEW
///
/// This struct is a container for a transmission hardware assembly.
struct Hardware {
 public:
  /// \par OVERVIEW
  ///
  /// This enum contains types of hardware assemblies.
  enum class HardwareType {
    kNull,
    kDeadEnd,
    kSuspension,
  };

  /// \brief Default constructor.
  Hardware();

  /// \brief Destructor.
  ~Hardware();

  /// \brief Validates member variables.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings = true,
                std::list<ErrorMessage>* messages = nullptr) const;

  /// \var area_cross_section
  ///   The area of the cross section.
  double area_cross_section;

  /// \var length
  ///   The length.
  double length;

  /// \var name
  ///   The name.
  std::string name;

  /// \var type
  ///   The type of assembly.
  HardwareType type;

  /// \var weight
  ///   The weight.
  double weight;
};

#endif  // OTLS_MODELS_TRANSMISSIONLINE_HARDWARE_H_