// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/transmissionline/transmission_line.h"

TransmissionLine::TransmissionLine() {
  is_updated_points_xyz_alignment_ = false;
  is_updated_points_xyz_structures_ = false;
}

TransmissionLine::~TransmissionLine() {
}

int TransmissionLine::AddAlignmentPoint(const AlignmentPoint& point) {
  is_updated_points_xyz_alignment_ = false;
  return alignment_.AddPoint(point);
}

int TransmissionLine::AddLineStructure(const LineStructure& structure) {
  is_updated_points_xyz_structures_ = false;

  // checks station
  if (alignment_.IsValidStation(structure.station()) == false) {
    return -1;
  }

  // searches list for the position to insert structure
  auto iter = line_structures_.begin();
  while (iter != line_structures_.end()) {
    const LineStructure& list_structure = *iter;
    if (structure.station() < list_structure.station()) {
      // position is found
      break;
    } else if (structure.station() == list_structure.station()) {
      // exits due to duplicate station
      return -1;
    } else {
      iter++;
    }
  }

  // inserts structure into list
  line_structures_.insert(iter, structure);

  // gets index and returns
  return std::distance(line_structures_.begin(), iter) - 1;
}

bool TransmissionLine::DeleteAlignmentPoint(const int& index) {
  is_updated_points_xyz_alignment_ = false;
  return alignment_.DeletePoint(index);
}

bool TransmissionLine::DeleteLineStructure(const int& index) {
  is_updated_points_xyz_structures_ = false;

  // checks index
  if (IsValidLineStructureIndex(index) == false) {
    return false;
  }

  // gets iterator and erases structure
  auto iter = std::next(line_structures_.cbegin(), index);
  line_structures_.erase(iter);

  return true;
}

int TransmissionLine::ModifyAlignmentPoint(const int& index,
                                            const AlignmentPoint& point) {
  is_updated_points_xyz_alignment_ = false;
  return alignment_.ModifyPoint(index, point);
}

int TransmissionLine::ModifyLineStructure(const int& index,
                                          const LineStructure& structure) {
  is_updated_points_xyz_structures_ = false;

  // checks if index is valid
  if (IsValidLineStructureIndex(index) == false) {
    return -1;
  }

  // caches specified line structure in case something goes wrong
  LineStructure structure_cache = *std::next(line_structures_.cbegin(), index);
  if (DeleteLineStructure(index) == false) {
    return -1;
  }

  const int status = AddLineStructure(structure);
  if (status == -1) {
    // resets cached line structure
    AddLineStructure(structure_cache);
  }

  // successfully modified point
  return status;
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

const std::vector<Point3d>* TransmissionLine::PointsXyzLineStructures() const {
  // updates class if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return nullptr;
    }
  }

  return &points_xyz_structures_;
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

const std::list<LineStructure>* TransmissionLine::line_structures() const {
  return &line_structures_;
}

Point3d TransmissionLine::origin() const {
  return origin_;
}

void TransmissionLine::set_origin(const Point3d& origin) {
  origin_ = origin;
  is_updated_points_xyz_alignment_ = false;
  is_updated_points_xyz_structures_ = false;
}

bool TransmissionLine::IsUpdated() const {
  if ((is_updated_points_xyz_alignment_ == true)
      || (is_updated_points_xyz_structures_ == true)) {
    return true;
  } else {
    return false;
  }
}

bool TransmissionLine::IsValidLineStructureIndex(const int& index) const {
  const int kSize = line_structures_.size();
  if ((0 <= index) && (index < kSize)) {
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

  if (iter == alignment_.points()->cend()) {
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

  // updates the xyz points for the structures
  if (is_updated_points_xyz_structures_ == false) {
    is_updated_points_xyz_structures_ = UpdatePointsXyzLineStructures();
    if (is_updated_points_xyz_structures_ == false) {
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

bool TransmissionLine::UpdatePointsXyzLineStructures() const {
  // initializes
  points_xyz_structures_.clear();
  Vector2d vector(0, 0);

  // iterates over all of the line structures
  for (auto iter = line_structures_.cbegin(); iter != line_structures_.cend();
       iter++) {
    const LineStructure& line_structure = *iter;
    Point3d point = PointXyzAlignmentFromStation(line_structure.station());
    points_xyz_structures_.push_back(point);
  }

  return true;
}
