// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef OTLS_MODELS_TRANSMISSIONLINE_TRANSMISSIONLINE_H_
#define OTLS_MODELS_TRANSMISSIONLINE_TRANSMISSIONLINE_H_

#include <list>

#include "models/base/point.h"
#include "models/base/vector.h"
#include "models/transmissionline/alignment.h"
#include "models/transmissionline/line_cable.h"
#include "models/transmissionline/line_structure.h"

/// \par OVERVIEW
///
/// This class is a transmission line. The line consists of an alignment,
/// line structures, and line cables.
///
/// \par COORDINATE SYSTEM
///
/// The transmission line is modeled in a 3D coordinate system, as follows:
///  - x axis = east-west
///  - y axis = south-north
///  = z axis = elevation
///
/// The origin coordinate defines the datum for the coordinate system.
///
/// \par ALIGNMENT
///
/// The alignment defines the centerline path of the transmission line. The
/// origin coordinate is used to map the turns/orientations to 3D coordinates.
///
/// \par LINE STRUCTURES
///
/// Line structures can be placed anywhere along the alignment. Coordinates for
/// each line structure are cached to help determine 3D coordinates for any
/// dependent items.
///
/// \par LINE CABLES
///
/// Line cables can be attached to the line structures. Line cables can only
/// exist on the transmission line if the connections are, and remain, valid.
/// If any of the terminating connections become invalid, the line cable is
/// removed. Only one line cable is allowed for each line structure attachment.
class TransmissionLine {
 public:
  /// \brief Default constructor.
  TransmissionLine();

  /// \brief Copy constructor.
  /// \param[in] that
  ///   The transmission line to copy from.
  /// This is needed to keep the references within the class valid when copying.
  TransmissionLine(const TransmissionLine& that);

  /// \brief Copy assignment operator.
  /// \param[in] that
  ///   The transmission line to copy from.
  /// \return A reference to the transmission line.
  /// This is needed to keep the references within the class valid when copying.
  TransmissionLine& operator=(const TransmissionLine& that);

  /// \brief Destructor.
  ~TransmissionLine();

  /// \brief Adds an alignment point.
  /// \param[in] point
  ///   The alignment point.
  /// \return The index of the alignment point. If adding the point to the
  ///    alignment fails, -1 will be returned.
  int AddAlignmentPoint(const AlignmentPoint& point);

  /// \brief Adds a line cable.
  /// \param[in] line_cable
  ///   The line cable.
  /// \return The index of the line cable. If adding the cable to the line
  ///   fails, -1 will be returned.
  int AddLineCable(const LineCable& line_cable);

  /// \brief Adds a line structure.
  /// \param[in] structure
  ///   The line structure.
  /// \return The index of the line structure. If adding the structure to the
  ///    alignment fails, -1 will be returned.
  int AddLineStructure(const LineStructure& structure);

  /// \brief Deletes an alignment point.
  /// \param[in] index
  ///   The index of the alignment point.
  /// \return If the alignment point is successfully deleted.
  bool DeleteAlignmentPoint(const int& index);

  /// \brief Deletes a line cable.
  /// \param[in] index
  ///   The index of the line cable.
  /// \return If the line cable is successfully deleted.
  bool DeleteLineCable(const int& index);

  /// \brief Deletes a line structure.
  /// \param[in] index
  ///   The index of the line structure.
  /// \return If the line structure is successfully deleted.
  bool DeleteLineStructure(const int& index);

  /// \brief Determines if the line structure attachment has a line cable
  ///   connection.
  /// \param[in] index_structure
  ///   The line structure index.
  /// \param[in] index_attachment
  ///   The attachment index.
  /// \return If the line structure attachment has a line cable connection.
  bool HasConnection(const int& index_structure,
                     const int& index_attachment) const;

  /// \brief Gets the line structure index.
  /// \param[in] line_structure
  ///   The line structure.
  /// \return The index of the line structure. If no line structure is found,
  ///   -1 is returned.
  int IndexLineStructure(const LineStructure* line_structure) const;

  /// \brief Modifies an alignment point.
  /// \param[in] index
  ///   The list index.
  /// \param[in] point
  ///   The alignment point.
  /// \return The index of the point after sorting. If modifying the point
  ///   fails, -1 will be returned.
  int ModifyAlignmentPoint(const int& index, const AlignmentPoint& point);

  /// \brief Modifies a line cable.
  /// \param[in] index
  ///   The list index.
  /// \param[in] line_cable
  ///   The line cable.
  /// \return The index of the line cable after sorting. If modifying the line
  ///   cable fails, -1 will be returned.
  int ModifyLineCable(const int& index, const LineCable& line_cable);

  /// \brief Modifies a line structure.
  /// \param[in] index
  ///   The list index.
  /// \param[in] structure
  ///   The line structure.
  /// \return If the line structure is successfully modified.
  /// Line structures cannot be moved back or ahead of the adjacent structures
  /// to it, so the index will remain constant.
  bool ModifyLineStructure(const int& index, const LineStructure& structure);

  /// \brief Gets the xyz coordinates for all alignment points.
  /// \return The xyz coordinates for all alignment points.
  const std::vector<Point3d>* PointsXyzAlignment() const;

  /// \brief Gets the xyz coordinates for all line structures on the alignment.
  /// \return The xyz coordinate for all line structure alignment points.
  const std::vector<Point3d>* PointsXyzLineStructures() const;

  /// \brief Gets the xyz point of the alignment path.
  /// \param[in] station
  ///   The position along the alignment.
  /// \return The xyz point of the alignment path.
  Point3d PointXyzAlignment(const double& station) const;

  /// \brief Validates member variables.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings = true,
                std::list<ErrorMessage>* messages = nullptr) const;

  /// \brief Gets the alignment.
  /// \return The alignment.
  const Alignment* alignment() const;

  /// \brief Gets the line cables.
  /// \return The line cables, sorted by starting structure and attachment
  ///   index.
  const std::list<LineCable>* line_cables() const;

  /// \brief Gets the line structures.
  /// \return The line structures, sorted by station.
  const std::list<LineStructure>* line_structures() const;

  /// \brief Gets the origin.
  /// \return The origin.
  Point3d origin() const;

  /// \brief Sets the xyz origin.
  /// \param[in] origin
  ///   The xyz origin point.
  void set_origin(const Point3d& origin);

 private:
  /// \brief Deletes all connections from the line structure.
  /// \param[in] index_structure
  ///   The line structure index.
  /// This function will modify the line cables to delete all connections to
  /// the structure.
  void DeleteConnectionsFromStructure(const int& index_structure);

  /// \brief Deletes invalid connections from the line structure.
  /// \param[in] index_structure
  ///   The line structure index.
  /// This function will modify the line cables to delete invalid connections to
  /// the structure.
  void DeleteInvalidConnectionsFromStructure(const int& index_structure);

  /// \brief Deletes invalid line cables.
  /// Invalid cables are commonly caused by deleted connections.
  void DeleteInvalidLineCables();

  /// \brief Clears invalid line structures.
  /// Line structures that are beyond the alignment bounds are also considered
  /// invalid.
  void DeleteInvalidLineStructures();

  /// \brief Determines if class is updated.
  /// \return A boolean indicating if class is updated.
  bool IsUpdated() const;

  /// \brief Determines if the line cable index is valid.
  /// \param[in] index
  ///   The line cable index.
  /// \return If the line cable index is valid.
  bool IsValidLineCableIndex(const int& index) const;

  /// \brief Determines if the line structure index is valid.
  /// \param[in] index
  ///   The line structure index.
  /// \return If the line structure index is valid.
  bool IsValidLineStructureIndex(const int& index) const;

  /// \brief Gets the xyz point of the alignment path.
  /// \param[in] station
  ///   The position along the alignment.
  /// \return The xyz point of the alignment path.
  Point3d PointXyzAlignmentFromStation(const double& station) const;

  /// \brief Solves for the xyz point along the alignment.
  /// \param[in] point_xyz
  ///   The xyz point to base from. Coincides with the alignment point.
  /// \param[in] distance_station
  ///   The station distance from the base xyz point.
  /// \param[in] distance_elevation
  ///   The elevation distance from the base xyz point. Positive indicates
  ///   upward, while negative is downward.
  /// \param[in] rotation_xy
  ///   The rotation to apply in the xy plane.
  /// \param[in,out] vector_xy
  ///   The orientation vector in the xy plane. This is used to convert the
  ///   station to xy coordinates and must be aligned with the previous
  ///   alignment path. This vector is rotated and scaled to determine the new
  ///   point.
  /// \return The xyz point along the alignment path.
  Point3d PointXyzAlignmentFromVector(const Point3d& point_xyz,
                                      const double& distance_station,
                                      const double& distance_elevation,
                                      const double& rotation_xy,
                                      Vector2d& vector_xy) const;

  /// \brief Updates cached member variables and modifies control variables if
  ///    update is required.
  /// \return A boolean indicating if class updates completed successfully.
  bool Update() const;

  /// \brief Updates the cached coordinate points for the alignment.
  /// \return A boolean indicating the success status of the update.
  bool UpdatePointsXyzAlignment() const;

  /// \brief Updates the cached coordinate points for line structures.
  /// \return A boolean indicating the success status of the update.
  bool UpdatePointsXyzLineStructures() const;

  /// \brief Gets an xy alignment segment orientation vector.
  /// \param[in] index
  ///   The alignment segment index.
  /// \return An xy unit vector that is parallel to the alignment segment.
  Vector2d VectorXyAlignmentSegment(const int& index) const;

  /// \var alignment_
  ///   The centerline path of the transmission line. The datum orientation for
  ///   the alignment (zero degree rotation) is along the x-axis.
  Alignment alignment_;

  /// \var is_updated_points_xyz_alignment_
  ///   An indicator that tells if the cached alignment xyz points are updated.
  mutable bool is_updated_points_xyz_alignment_;

  /// \var is_updated_points_xyz_structures_
  ///   An indicator that tells if the cached structure xyz points are updated.
  mutable bool is_updated_points_xyz_structures_;

  /// \var line_cables_
  ///   The line cables, sorted by starting structure and then attachment index.
  std::list<LineCable> line_cables_;

  /// \var line_structures_
  ///   The line structures, sorted by increasing station.
  std::list<LineStructure> line_structures_;

  /// \var origin_
  ///   The xyz origin point. This coincides with the first alignment point.
  Point3d origin_;

  /// \var points_xyz_alignment_
  ///   The xyz coordinates for the alignment points.
  mutable std::vector<Point3d> points_xyz_alignment_;

  /// \var points_xyz_structures_
  ///   The xyz coordinates for the structure position on the alignment.
  mutable std::vector<Point3d> points_xyz_structures_;
};

#endif  // OTLS_MODELS_TRANSMISSIONLINE_TRANSMISSIONLINE_H_
