// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef TLSLIBRARIES_SAGTENSION_LINECABLESAGGER_H_
#define TLSLIBRARIES_SAGTENSION_LINECABLESAGGER_H_

#include <list>
#include <string>
#include <vector>

#include "transmissionline/catenary.h"
#include "transmissionline/line_cable.h"

/// \par OVERVIEW
///
/// This class solves for the constraint limit of (aka: sags) a line cable
/// which is limited by several design constraints.
///
/// \par LINE CABLE CONSTRAINT
///
/// Throughout the sagging process only the limit will be changed. The
/// constraint loading, condition, etc. remain the same.
///
/// \par DESIGN CONSTRAINTS
///
/// Design constraints are limits that the line cable must not exceed.
/// Typically, these constraints are defined under different loading conditions
/// than the line cable constraint.
///
/// \par CONTROLLING CONSTRAINT
///
/// The controlling constraint is the most limiting of the design constraints.
/// It alone will determine what the line cable constraint limit will be
/// modified to. This class solves for the controlling constraint and caches
/// the index corresponding to it.
///
/// \par ACTUAL DESIGN CONSTRAINT LOADING
///
/// Once the controlling constraint is identified and the line cable is sagged,
/// (the line cable constraint limit is updated) the design constraints can be
/// further analyzed to determine their impact on the sagging process.
/// Reloading the sagged line cable at each design constraint and comparing the
/// actual limit to the allowable limit shows how close the constraint was to
/// becoming the controlling constraint.
class LineCableSagger {
 public:
  /// \brief Default constructor.
  LineCableSagger();

  /// \brief Destructor.
  ~LineCableSagger();

  /// \brief Gets the percentage of allowable capacity of the design
  ///   constraint.
  /// \param[in] index
  ///   The design constraint index.
  /// \return The percentage of allowable capacity of the design constraint.
  double CapacityAllowable(const int& index) const;

  /// \brief Gets the actual catenary constant of the design constraint.
  /// \param[in] index
  ///   The design constraint index.
  /// \return The actual catenary constant of the design constraint.
  double CatenaryConstantActual(const int& index) const;

  /// \brief Gets the controlling constraint index.
  /// \return The controlling constraint index.
  int IndexConstraintControlling() const;

  /// \brief Gets the sagged line cable constraint limit.
  /// \return The sagged line cable constraint limit.
  double LimitConstraintSaggedLineCable() const;

  /// \brief Gets the actual horizontal tension of the design constraint.
  /// \param[in] index
  ///   The design constraint index.
  /// \return The actual horizontal tension of the design constraint.
  double TensionHorizontalActual(const int& index) const;

  /// \brief Gets the actual support tension of the design constraint.
  /// \param[in] index
  ///   The design constraint index.
  /// \return The actual support tension of the design constraint.
  double TensionSupportActual(const int& index) const;

  /// \brief Validates member variables.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages_error
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings = true,
                std::list<std::string>* messages_error = nullptr) const;

  /// \brief Gets the stretch load case.
  /// \return The stretch load case.
  WeatherLoadCase case_stretch() const;

  /// \brief Gets the design constraints.
  /// \return The design constraints.
  std::vector<CableConstraint> constraints_design() const;

  /// \brief Gets the line cable.
  /// \return The line cable.
  LineCable line_cable() const;

  /// \brief Sets the stretch load case.
  /// \param[in] case_stretch
  ///   The stretch load case.
  void set_case_stretch(const WeatherLoadCase case_stretch);

  /// \brief Sets the design constraints.
  /// \param[in] constraints_design
  ///   The design constraints.
  void set_constraints_design(
      const std::vector<CableConstraint> constraints_design);

  /// \brief Sets the line cable.
  /// \param[in] line_cable
  ///   The line cable.
  void set_line_cable(const LineCable& line_cable);

 private:
  /// \brief Determines if class is updated.
  /// \return A boolean indicating if class is updated.
  bool IsUpdated() const;

  /// \brief Updates cached member variables and modifies control variables if
  ///   update is required.
  /// \return A boolean indicating if class updates completed successfully.
  bool Update() const;

  /// \brief Updates the controlling constraint index.
  /// \return The success status of the update.
  bool UpdateControllingConstraintIndex() const;

  /// \brief Updates the line cable constraint limit.
  /// \return The success status of the update.
  bool UpdateLineCableConstraintLimit() const;

  /// \brief Updates the catenaries for the design constraints with the actual
  ///   loading.
  /// \return The success status of the update.
  bool UpdateCatenariesConstraintsActual() const;

  /// \var case_stretch_
  ///   The load case that defines the cable stretch.
  WeatherLoadCase case_stretch_;

  /// \var catenaries_constraints_actual_
  ///   The catenaries for the design constraints, which contain the actual
  ///   values at the constraint loading.
  mutable std::vector<Catenary3d> catenaries_constraints_actual_;

  /// \var constraints_design_
  ///   The constraints for the line cable.
  std::vector<CableConstraint> constraints_design_;

  /// \var index_constraint_controlling_
  ///   The controlling, or most limiting constraint for the line cable.
  mutable int index_constraint_controlling_;

  /// \var is_updated_linecable_constraint_limit_
  ///   An indicator that tells if the line cable constraint limit is updated.
  mutable bool is_updated_linecable_constraint_limit_;

  /// \var line_cable_
  ///   The line cable that is being reloaded.
  mutable LineCable line_cable_;
};

#endif  // TLSLIBRARIES_SAGTENSION_LINECABLESAGGER_H_
