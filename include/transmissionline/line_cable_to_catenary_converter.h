// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef TLSLIBRARIES_TRANSMISSIONLINE_LINECABLETOCATENARYCONVERTER_H_
#define TLSLIBRARIES_TRANSMISSIONLINE_LINECABLETOCATENARYCONVERTER_H_

#include <list>
#include <string>

#include "transmissionline/catenary.h"
#include "transmissionline/line_cable.h"

/// \par OVERVIEW
///
/// This class converts a line cable to a catenary.
class LineCableToCatenaryConverter {
 public:
  /// \brief Default constructor.
  LineCableToCatenaryConverter();

  /// \brief Destructor.
  ~LineCableToCatenaryConverter();

  /// \brief Gets the catenary.
  Catenary3d Catenary() const;

  /// \brief Validates member variables.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages_error
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings = true,
                std::list<std::string>* messages_error = nullptr) const;

  /// \brief Gets the line cable.
  /// \return The line cable.
  LineCable line_cable() const;

  /// \brief Sets the line cable.
  /// \param[in] line_cable
  ///   The line cable.
  void set_line_cable(const LineCable& line_cable);

 private:
  /// \brief Determines if class is updated.
  /// \return A boolean indicating if class is updated.
  bool IsUpdated() const;

  /// \brief Updates cached member variables and modifies control variables if
  ///    update is required.
  /// \return A boolean indicating if class updates completed successfully.
  bool Update() const;

  /// \brief Solves and updates catenary horizontal tension by converting from
  ///    the catenary constant.
  /// \return A boolean indicating the success status of the update.
  bool SolveHorizontalTensionFromConstant() const;

  /// \brief Solves and updates catenary horizontal tension by converting from
  ///    support tension.
  /// \return A boolean indicating the success status of the update.
  bool SolveHorizontalTensionFromSupportTension() const;

  /// \brief Solves and updates the catenary unit weight.
  /// \return A boolean indicating the success status of the update.
  bool SolveWeightUnit() const;

  /// \brief Updates catenary tension and calculates new support tension.
  /// \param tension_horizontal The new horizontal tension for the catenary.
  /// \return The support tension correlating to the new horizontal tension of the
  ///    catenary.
  double UpdateCatenaryMaxTension(const double& tension_horizontal) const;

  /// \var catenary_
  ///   The catenary being solved for.
  mutable Catenary3d catenary_;

  /// \var is_updated_catenary_
  ///   An indicator that tells if the catenary is updated.
  mutable bool is_updated_catenary_;

  /// \var line_cable_
  ///   The line cable.
  LineCable line_cable_;
};

#endif // TLSLIBRARIES_TRANSMISSIONLINE_LINECABLETOCATENARYCONVERTER_H_
