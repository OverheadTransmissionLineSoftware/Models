// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/transmissionline/transmission_line.h"

TransmissionLine::TransmissionLine() {
  is_updated_points_xyz_alignment_ = false;
}

TransmissionLine::~TransmissionLine() {
}

int TransmissionLine::AddAlignmentPoint(const AlignmentPoint& point) {
  is_updated_points_xyz_alignment_ = false;
  return alignment_.AddPoint(point);
}

bool TransmissionLine::DeleteAlignmentPoint(const int& index) {
  is_updated_points_xyz_alignment_ = false;
  return alignment_.DeletePoint(index);
}

int TransmissionLine::ModifyAlignmentPoint(const int& index,
                                            const AlignmentPoint& point) {
  is_updated_points_xyz_alignment_ = false;
  return alignment_.ModifyPoint(index, point);
}

const std::vector<Point3d>* TransmissionLine::PointsXyzAlignment() const {
  // updates class if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return nullptr;
    }
  }
  
  return &points_xyz_alignment_;
}

Point3d TransmissionLine::PointXyzAlignment(const double& station) const {
  // updates class if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return Point3d();
    }
  }

  return PointXyzAlignmentFromStation(station);
}

bool TransmissionLine::Validate(const bool& is_included_warnings,
                                std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "TRANSMISSION LINE";

  // validates alignment
  if (alignment_.Validate(is_included_warnings, messages) == false) {
    is_valid = false;
  }

  // validates update process
  if (Update() == false) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Error updating class";
      messages->push_back(message);
    }
  }

  // returns validation status
  return is_valid;
}

const Alignment* TransmissionLine::alignment() const {
  return &alignment_;
}

Point3d TransmissionLine::origin() const {
  return origin_;
}

void TransmissionLine::set_origin(const Point3d& origin) {
  origin_ = origin;
  is_updated_points_xyz_alignment_ = false;
}

bool TransmissionLine::IsUpdated() const {
  if (is_updated_points_xyz_alignment_ == true) {
    return true;
  } else {
    return false;
  }
}

Point3d TransmissionLine::PointXyzAlignmentFromStation(
    const double& station) const {
  // searches alignment boundary points for current alignment path
  std::list<AlignmentPoint>::const_iterator iter;
  for (iter = alignment_.points()->cbegin();
       iter != alignment_.points()->cend(); iter++) {
    const AlignmentPoint& point = *iter;
    if (station < point.station) {
      // alignment point exceeds station
      break;
    } else if (station == point.station) {
      // station coincides with existing alignment point
      const int index = std::distance(alignment_.points()->cbegin(), iter);
      return points_xyz_alignment_.at(index);
    }
  }

  if (iter == alignment_.points()->cend() == true) {
    return Point3d();
  }

  const AlignmentPoint& point_align_ahead = *iter;
  iter--;
  const AlignmentPoint& point_align_back = *iter;

  // calculates station and elevation distances
  const double distance_station = station - point_align_back.station;
  const double slope = (point_align_ahead.elevation - point_align_back.elevation)
                       / (point_align_ahead.station - point_align_back.station);
  const double distance_elevation = slope * distance_station;

  // gets the back xyz point
  const int index = std::distance(alignment_.points()->cbegin(), iter);
  const Point3d& point_xyz_back = points_xyz_alignment_.at(index);

  // creates an xy vector along the previous alignment path
  // if the back position is the start of the xyz coordinates, sets to the
  // default direction
  Vector2d vector;
  if (index == 0) {
    vector.set_x(1);
    vector.set_y(0);
  } else {
    const Point3d& point_xyz_back_back = points_xyz_alignment_.at(index -1);
    vector.set_x(point_xyz_back.x - point_xyz_back_back.x);
    vector.set_y(point_xyz_back.y - point_xyz_back_back.y);
  }

  // gets point
  return PointXyzAlignmentFromVector(point_xyz_back, distance_station,
                                     distance_elevation,
                                     point_align_back.rotation, vector);
}

Point3d TransmissionLine::PointXyzAlignmentFromVector(
    const Point3d& point_xyz, const double& distance_station,
    const double& distance_elevation, const double& rotation_xy,
    Vector2d& vector_xy) const {
  // rotates and scales xy vector to new position
  vector_xy.Rotate(rotation_xy);
  vector_xy.Scale(distance_station / vector_xy.Magnitude());
  
  // calculates new point
  Point3d point;
  point.x = point_xyz.x + vector_xy.x();
  point.y = point_xyz.y + vector_xy.y();
  point.z = point_xyz.z + distance_elevation;

  return point;
}

bool TransmissionLine::Update() const {
  // updates the xyz points for the alignment
  if (is_updated_points_xyz_alignment_ == false) {
    is_updated_points_xyz_alignment_ = UpdatePointsXyzAlignment();
    if (is_updated_points_xyz_alignment_ == false) {
      return false;
    }
  }

  // if it reaches this point, update was successful
  return true;
}

bool TransmissionLine::UpdatePointsXyzAlignment() const {
  // initializes
  points_xyz_alignment_.clear();
  const Point3d* point_xyz_prev = nullptr;
  const AlignmentPoint* point_align_prev = nullptr;
  Vector2d vector(0, 0);

  // iterates over all of the alignment points
  const std::list<AlignmentPoint>* points = alignment_.points();
  for (auto iter = points->cbegin(); iter != points->cend(); iter++) {
    const AlignmentPoint& point_align = *iter;

    if (iter == points->cbegin()) {
      // initializes for first point
      points_xyz_alignment_.push_back(origin_);
      vector.set_x(1);
    } else {
      // calculates station and elevation distances
      const double distance_station = point_align.station
                                      - point_align_prev->station;
      const double distance_elevation = point_align.elevation
                                        - point_align_prev->elevation;
      
      // gets point and caches
      Point3d point = PointXyzAlignmentFromVector(*point_xyz_prev,
                                                  distance_station,
                                                  distance_elevation,
                                                  point_align_prev->rotation,
                                                  vector);

      points_xyz_alignment_.push_back(point);
    }
   
    // updates previous points for next calculation
    point_align_prev = &point_align;
    point_xyz_prev = &points_xyz_alignment_.back();
  }

  return true;
}
