// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef TRANSMISSIONLINE_CABLELOADCONSTRAINT_H_
#define TRANSMISSIONLINE_CABLELOADCONSTRAINT_H_

#include <list>
#include <string>

#include "include/TransmissionLine/cable_load_case.h"

/// \par OVERVIEW
///
/// This enum contains types of constraint limits.
enum class ConstraintLimitType {
  kHorizontalTension,
  kCatenaryConstant,
  kSupportTension
};

/// \par OVERVIEW
///
/// This enum contains types of cable conditions.
enum class ConditionType {
  kCreep,
  kInitial,
  kLoad
};

/// \par OVERVIEW
///
/// This struct is a container for a cable load constraint.
struct CableLoadConstraint {
 public:
  /// \brief Default constructor.
  CableLoadConstraint();

  /// \brief Destructor.
  ~CableLoadConstraint();

  /// \brief Validates member variables.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages_error
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings = true,
                std::list<std::string>* messages_error = nullptr) const;

  /// \var case_load
  ///   The load case.
  CableLoadCase case_load;

  /// \var condition
  ///   The state of the cable, which can determine whether the cable is
  ///   modeled as unstretched, or stretched.
  ConditionType condition;

  /// \var limit
  ///   The limit that the cable cannot exceed.
  double limit;

  /// \var type_limit
  ///   The type of constraint, which determines how the constraint is defined.
  ConstraintLimitType type_limit;
};

#endif // TRANSMISSIONLINE_CABLELOADCONSTRAINT_H_
