// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef TRANSMISSIONLINE_CABLELOADCONSTRAINT_H_
#define TRANSMISSIONLINE_CABLELOADCONSTRAINT_H_

#include <list>
#include <string>

#include "include/CableLoadCase.h"

/// \par OVERVIEW
///
/// This enum contains types of cable load constraints.
enum class ConstraintLimitType {
  HORIZONTAL,
  HW,
  SUPPORT
};

/// \par OVERVIEW
///
/// This enum contains types of cable conditions.
enum class ConditionType {
  CREEP,
  INITIAL,
  LOAD
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
  /// \param is_included_warnings A flag that tightens the acceptable value
  ///        range.
  /// \param messages_error A list of detailed error messages. If this is
  ///        provided, any validation errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(bool is_included_warnings = true,
                std::list<std::string>* messages_error = nullptr) const;

  /// \var case_load The load case.
  CableLoadCase case_load;

  /// \var condition The state of the cable, which can determine whether the
  ///      cable is modeled as stretched, or unstretched.
  ConditionType condition;

  /// \var limit The limit that the cable cannot exceed.
  double limit;

  /// \var type_limit The type of constraint, which determines where and/or how
  ///      the constraint is defined.
  ConstraintLimitType type_limit;
};

#endif // TRANSMISSIONLINE_CABLELOADCONSTRAINT_H_
