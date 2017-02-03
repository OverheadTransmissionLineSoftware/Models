// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef OTLS_MODELS_TRANSMISSIONLINE_TRANSMISSIONLINE_H_
#define OTLS_MODELS_TRANSMISSIONLINE_TRANSMISSIONLINE_H_

#include <list>
#include <vector>

#include "models/base/point.h"
#include "models/base/vector.h"
#include "models/transmissionline/alignment.h"

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
class TransmissionLine {
 public:
  /// \brief Default constructor.
  TransmissionLine();

  /// \brief Destructor.
  ~TransmissionLine();

  /// \brief Adds an alignment point.
  /// \param[in] point
  ///   The alignment point.
  /// \return The index of the alignment point.
  int AddAlignmentPoint(const AlignmentPoint& point);

  /// \brief Deletes an alignment point.
  /// \param[in] index
  ///   The index of the alignment point.
  void DeleteAlignmentPoint(const int& index);

  /// \brief Modifies an alignment point.
  /// \param[in] index
  ///   The list index.
  /// \param[in] point
  ///   The alignment point.
  /// \return The index of the point after sorting.
  int ModifyAlignmentPoint(const int& index, const AlignmentPoint& point);

  /// \brief Gets the xyz coordinates for all alignment points.
  /// \return The xyz coordinates for all alignment points.
  const std::vector<Point3d>* PointsXyzAlignment() const;

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

  /// \brief Gets the origin.
  /// \return The origin.
  Point3d origin() const;

  /// \brief Sets the xyz origin.
  /// \param[in] origin
  ///   The xyz origin point.
  void set_origin(const Point3d& origin);

 private:
  /// \brief Determines if class is updated.
  /// \return A boolean indicating if class is updated.
  bool IsUpdated() const;

  /// \brief Gets the xyz point along the alignment.
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
  Point3d PointXyzAlignment(const Point3d& point_xyz,
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

  /// \var alignment_
  ///   The centerline path of the transmission line. The datum orientation for
  ///   the alignment (zero degree rotation) is along the x-axis.
  Alignment alignment_;

  /// \var points_xyz_alignment_
  ///   The xyz coordinates for the alignment points.
  mutable std::vector<Point3d> points_xyz_alignment_;

  /// \var is_updated_points_xyz_alignment_
  ///   An indicator that tells if the cached alignment xyz points are updated.
  mutable bool is_updated_points_xyz_alignment_;

  /// \var origin_
  ///   The xyz origin point. This coincides with the first alignment point.
  Point3d origin_;
};

#endif  // OTLS_MODELS_TRANSMISSIONLINE_TRANSMISSIONLINE_H_