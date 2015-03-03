// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef TRANSMISSIONLINE_CABLELOADCASE_H_
#define TRANSMISSIONLINE_CABLELOADCASE_H_

#include <list>
#include <string>

#include "include/base/vector.h"

/// \par OVERVIEW
///
/// This struct is a container for a cable load case.
struct CableLoadCase {
 public:
  /// \brief Default constructor.
  CableLoadCase();

  /// \brief Destructor.
  ~CableLoadCase();

  /// \brief Validates member variables.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages_error
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings = true,
                std::list<std::string>* messages_error = nullptr) const;

  /// \var load_unit
  ///   The load per unit length on the cable. The x component is transverse,
  ///   and the y component is vertical.
  Vector2d load_unit;

  /// \var temperature_cable
  ///   The temperature of the cable.
  double temperature_cable;
};

#endif  // TRANSMISSIONLINE_CABLELOADCASE_H_
