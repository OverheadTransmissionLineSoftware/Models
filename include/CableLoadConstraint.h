// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef TRANSMISSIONLINE_CABLELOADCONSTRAINT_H_
#define TRANSMISSIONLINE_CABLELOADCONSTRAINT_H_

#include <list>
#include <string>

#include "include/CableLoadCase.h"

/// \par CONSTRAINT LIMIT TYPE OVERVIEW
///
/// This enum contains types of cable load constraints.
enum class ConstraintLimitType {
  HORIZONTAL,
  HW,
  SUPPORT
};

/// \par CONDITION TYPE OVERVIEW
///
/// This enum contains types of cable conditions.
enum class ConditionType {
  CREEP,
  INITIAL,
  LOAD
};

/// \par CABLE LOAD CONSTRAINT OVERVIEW
///
/// This struct contains information for a cable load constraint.
struct CableLoadConstraint {
 public:
  /// \brief Default constructor.
  CableLoadConstraint();

  /// \brief Destructor.
  ~CableLoadConstraint();

  /// \brief Validates class data.
  /// \param is_included_warnings A flag that tightens the acceptable value
  ///        range.
  /// \param messages_error A list of detailed error messages. If this is
  ///        provided, any validation errors will be appended to the list.
  /// \return A boolean value indicating status of class data.
  bool Validate(bool is_included_warnings = true,
                std::list<std::string>* messages_error = nullptr) const;

  // member variables
  CableLoadCase       case_load;
  ConditionType       condition;
  double              limit;
  ConstraintLimitType type_limit;
};

#endif // TRANSMISSIONLINE_CABLELOADCONSTRAINT_H_
