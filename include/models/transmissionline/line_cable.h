// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef MODELS_TRANSMISSIONLINE_LINE_CABLE_H_
#define MODELS_TRANSMISSIONLINE_LINE_CABLE_H_

#include <list>

#include "models/base/error_message.h"
#include "models/base/vector.h"
#include "models/transmissionline/cable.h"
#include "models/transmissionline/cable_constraint.h"
#include "models/transmissionline/catenary.h"

class LineStructure;

/// \par OVERVIEW
///
/// This struct contains line cable connection information.
struct LineCableConnection {
 public:
  /// \brief Constructor.
  LineCableConnection();

  /// \var index_attachment
  ///   The index of the line structure attachment.
  int index_attachment;

  /// \var line_structure
  ///   The line structure.
  const LineStructure* line_structure;
};


/// \par OVERVIEW
///
/// This struct models a transmission cable line section (multiple spans), where
/// the cable is strung from one terminal (dead end) attachment to another.
///
/// \par CONSTRAINT
///
/// This class is defined by the cable, and constraint that it is tensioned to.
class LineCable {
 public:
  /// \brief Default constructor.
  LineCable();

  /// \brief Destructor.
  ~LineCable();

  /// \brief Adds a line connection.
  /// \param[in] connection
  ///   The connection.
  /// \return The index of the connection after sorting. If adding the
  ///    connection to the line cable fails, -1 will be returned.
  int AddConnection(const LineCableConnection& connection);

  /// \brief Gets a catenary based on the constraint and ruling span geometry.
  /// \return A catenary based on the constraint and ruling span geometry.
  Catenary3d CatenaryRulingSpan() const;

  /// \brief Clears all connections.
  void ClearConnections();

  /// \brief Deletes a connection.
  /// \param[in] index
  ///   The connection index.
  /// \return If the alignment point is successfully deleted.
  bool DeleteConnection(const int& index);

  /// \brief Modifies the connection.
  /// \param[in] index
  ///   The connection index.
  /// \param[in] connection
  ///   The connection.
  /// \return The index of the connection after sorting. If modifying the
  ///   connection fails, -1 will be returned.
  int ModifyConnection(const int& index, const LineCableConnection& connection);

  /// \brief Validates member variables.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings = true,
                std::list<ErrorMessage>* messages = nullptr) const;

  /// \brief Validates the connections.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool ValidateConnections(const bool& is_included_warnings = true,
                           std::list<ErrorMessage>* messages = nullptr) const;

  /// \brief Gets the cable.
  /// \return The cable.
  const Cable* cable() const;

  /// \brief Gets the connections.
  /// \return The connections.
  const std::list<LineCableConnection>* connections() const;

  /// \brief Gets the constraint.
  /// \return The constraint.
  CableConstraint constraint() const;

  /// \brief Sets the cable.
  /// \param[in] cable
  ///   The cable.
  void set_cable(const Cable* cable);

  /// \brief Sets the constraint.
  /// \param[in] constraint
  ///   The constraint.
  void set_constraint(const CableConstraint& constraint);

  /// \brief Sets the ruling span attachment spacing.
  /// \param[in] spacing_attachments_ruling_span
  ///   The ruling span attachment spacing.
  void set_spacing_attachments_ruling_span(
      const Vector3d& spacing_attachments_ruling_span);

  /// \brief Sets the creep stretch weathercase.
  /// \param[in] weathercase
  ///   The creep stretch weathercase.
  void set_weathercase_stretch_creep(const WeatherLoadCase* weathercase);

  /// \brief Sets the load stretch weathercase.
  /// \param[in] weathercase
  ///   The load stretch weathercase.
  void set_weathercase_stretch_load(const WeatherLoadCase* weathercase);

  /// \brief Gets the ruling span attachment spacing.
  /// \return The ruling span attachment spacing.
  const Vector3d spacing_attachments_ruling_span() const;

  /// \brief Gets the creep stretch weathercase.
  /// \return The creep stretch weathercase.
  const WeatherLoadCase* weathercase_stretch_creep() const;

  /// \brief Gets the load stretch weathercase.
  /// \return The load stretch weathercase.
  const WeatherLoadCase* weathercase_stretch_load() const;

 private:
  /// \brief Determines if the connection index is valid.
  /// \param[in] index
  ///   The connection index.
  /// \return If the connection index is valid.
  bool IsValidConnectionIndex(const int& index) const;

  /// \var cable_
  ///   The cable.
  const Cable* cable_;

  /// \var connections_
  ///   The connections, sorted by line structure station.
  std::list<LineCableConnection> connections_;

  /// \var constraint_
  ///   The constraint the cable is tensioned to.
  CableConstraint constraint_;

  /// \var spacing_attachments_ruling_span_
  ///   The attachment spacing for the ruling span geometry.
  Vector3d spacing_attachments_ruling_span_;

  /// \var weathercase_stretch_creep_
  ///   The weathercase that defines the amount of non-elastic stretch due to
  ///   creep.
  const WeatherLoadCase* weathercase_stretch_creep_;

  /// \var weathercase_stretch_load_
  ///   The weathercase that defines the amount of non-elastic stretch due to
  ///   load.
  const WeatherLoadCase* weathercase_stretch_load_;
};

#endif  // MODELS_TRANSMISSIONLINE_LINE_CABLE_H_
