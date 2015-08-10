// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef TLSLIBRARIES_SAGTENSION_LINECABLETOCATENARYCABLECONVERTER_H_
#define TLSLIBRARIES_SAGTENSION_LINECABLETOCATENARYCABLECONVERTER_H_

#include <list>
#include <string>

#include "sagtension/catenary_cable.h"
#include "sagtension/catenary_cable_reloader.h"
#include "transmissionline/line_cable.h"
#include "transmissionline/weather_load_case.h"

/// \par OVERVIEW
///
/// This class solves for the state of a catenary cable that is used as a
/// constraint with a non-zero stretch load. Note that for non-constraint
/// catenary cables, the state is determined differently.
/// \see LineCableReloader
///
/// \par NUMERICAL SOLUTION
///
/// When the constraint catenary cable is stretched, it cannot be reloaded
/// because the catenary cable is not competely solved. An iterative solution
/// is used that guesses a stretch load, and then reloads the constraint
/// catenary cable at the stretch case until it matches the stretch load.
class LineCableToCatenaryCableConverter {
 public:
  /// \brief Default constructor.
  LineCableToCatenaryCableConverter();

  /// \brief Destructor.
  ~LineCableToCatenaryCableConverter();

  /// \brief Gets the catenary cable with the updated state.
  /// \return The catenary cable with the updated state.
  CatenaryCable GetCatenaryCable() const;

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
  const WeatherLoadCase* case_stretch() const;

  /// \brief Gets the line cable.
  /// \return The line cable.
  const LineCable* line_cable() const;

  /// \brief Sets the line cable.
  /// \param[in] line_cable
  ///   The line cable.
  void set_line_cable(const LineCable* line_cable);

  /// \brief Sets the stretch load case.
  /// \param[in] case_stretch
  ///   The stretch load case.
  void set_case_stretch(const WeatherLoadCase* case_stretch);

  /// \brief Sets the type of stretch.
  /// \param[in] type_stretch
  ///   The type of stretch.
  void set_type_stretch(const CableConditionType& type_stretch);

  /// \brief Gets the type of stretch.
  /// \return The type of stretch.
  CableConditionType type_stretch() const;

 private:
  /// \brief Gets the difference between the reloaded and defined stretch load.
  /// \param[in] load_stretch
  ///   The defined stretch load.
  /// \return The difference between the reloaded and defined stretch load.
  /// difference = reloaded - defined
  double LoadStretchDifference(const double& load_stretch) const;

  /// \brief Intializes the reloader.
  /// \return The success status of the update.
  bool InitializeReloader() const;

  /// \brief Determines if class is updated.
  /// \return A boolean indicating if class is updated.
  bool IsUpdated() const;

  /// \brief Solves the stretch load for the catenary cable state.
  /// \return The success status of converging on the stretch load.
  bool SolveStateLoadStretch() const;

  /// \brief Updates cached member variables and modifies control variables if
  ///   update is required.
  /// \return A boolean indicating if class updates completed successfully.
  bool Update() const;

  /// \brief Updates catenary cable.
  /// \return The success status of the update.
  bool UpdateCatenaryCable() const;

  /// \brief Updates the catenary cable state.
  bool UpdateCatenaryCableState(const double& load_stretch) const;

  /// \var case_stretch_
  ///   The cable load case that defines the stretch in the cable state.
  const WeatherLoadCase* case_stretch_;

  /// \var catenary_cable_
  ///   The catenary cable which contains the solved for state.
  mutable CatenaryCable catenary_cable_;

  /// \var is_updated_catenarycable_
  ///   An indicator that tells if the catenary cable is updated.
  mutable bool is_updated_catenarycable_;

  /// \var line_cable_
  ///   The line cable.
  const LineCable* line_cable_;

  /// \var reloader_
  ///   The catenary cable reloader.
  mutable CatenaryCableReloader reloader_;

  /// \var type_stretch_
  ///   The type of stretch that the cable is stretched to.
  CableConditionType type_stretch_;
};

#endif  // TLSLIBRARIES_SAGTENSION_LINECABLETOCATENARYCABLECONVERTER_H_
