// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef MODELS_TRANSMISSIONLINE_CABLE_CONSTRAINT_H_
#define MODELS_TRANSMISSIONLINE_CABLE_CONSTRAINT_H_

#include <list>
#include <string>

#include "models/base/error_message.h"
#include "models/transmissionline/weather_load_case.h"

/// \par OVERVIEW
///
/// This enum contains types of cable conditions.
enum class CableConditionType {
  kNull,
  kCreep,
  kInitial,
  kLoad
};

/// \par OVERVIEW
///
/// This struct is a container for a cable constraint.
struct CableConstraint {
 public:
  /// \par OVERVIEW
  ///
  /// This enum contains types of constraint limits.
  enum class LimitType {
    kNull,
    kHorizontalTension,
    kCatenaryConstant,
    kLength,
    kSag,
    kSupportTension
  };

  /// \brief Default constructor.
  CableConstraint();

  /// \brief Destructor.
  ~CableConstraint();

  /// \brief Validates member variables.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings = true,
                std::list<ErrorMessage>* messages = nullptr) const;

  /// \var case_weather
  ///   The weather case.
  const WeatherLoadCase* case_weather;

  /// \var condition
  ///   The state of the cable, which can determine whether the cable is
  ///   modeled as unstretched, or stretched.
  CableConditionType condition;

  /// \var limit
  ///   The limit that the cable cannot exceed.
  double limit;

  /// \var note
  ///   A generic note string (optional).
  std::string note;

  /// \var type_limit
  ///   The type of constraint, which determines how the constraint is defined.
  CableConstraint::LimitType type_limit;
};

#endif  // MODELS_TRANSMISSIONLINE_CABLE_CONSTRAINT_H_
