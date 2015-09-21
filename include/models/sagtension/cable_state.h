// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef OTLS_MODELS_SAGTENSION_CABLESTATE_H_
#define OTLS_MODELS_SAGTENSION_CABLESTATE_H_

#include <list>
#include <string>

/// \par OVERVIEW
///
/// This struct contains the information for a cable state.
struct CableState {
 public:
  /// \brief Default constructor.
  CableState();

  /// \brief Destructor.
  ~CableState();

  /// \brief Validates member variables.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages_error
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings = true,
                std::list<std::string>* messages_error = nullptr) const;

  /// \var load_stretch
  ///   The load that the cable is stretched to.
  double load_stretch;

  /// \var temperature
  ///   The temperature of the cable.
  double temperature;

  /// \var temperature_stretch
  ///   The temperature that the cable was stretched at.
  double temperature_stretch;
};

#endif // OTLS_MODELS_SAGTENSION_CABLESTATE_H_
