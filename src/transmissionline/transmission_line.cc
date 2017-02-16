// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/transmissionline/transmission_line.h"

TransmissionLine::TransmissionLine() {
  is_updated_points_xyz_alignment_ = false;
  is_updated_points_xyz_structures_ = false;
}

TransmissionLine::TransmissionLine(const TransmissionLine& that) {
  // this will call the copy assignment operator
  *this = that;
}

TransmissionLine& TransmissionLine::operator=(const TransmissionLine& that) {
  // transfers members as-is
  this->alignment_ = that.alignment_;
  this->is_updated_points_xyz_alignment_ =
      that.is_updated_points_xyz_alignment_;
  this->is_updated_points_xyz_structures_ =
      that.is_updated_points_xyz_structures_;
  this->line_cables_ = that.line_cables_;
  this->line_structures_ = that.line_structures_;
  this->origin_ = that.origin_;

  // adjusts the new line cable connections to reference the new line structures
  for (auto iter_cable = line_cables_.begin(); iter_cable != line_cables_.end();
       iter_cable++) {
    LineCable& line_cable = *iter_cable;
    const std::list<LineCableConnection>* connections =
        line_cable.connections();
    const int kSizeConnections = connections->size();
    for (int index_connection = 0; index_connection < kSizeConnections;
         index_connection++) {
      LineCableConnection connection = *std::next(connections->cbegin(),
                                                  index_connection);

      // gets the structure index, and switches connection reference
      const int index_structure =
          that.IndexLineStructure(connection.line_structure);
      const LineStructure* structure = &*(std::next(line_structures_.cbegin(),
                                                    index_structure));
      connection.line_structure = structure;

      // modifies connection in line cable
      line_cable.ModifyConnection(index_connection, connection);
    }
  }

  return *this;
}

TransmissionLine::~TransmissionLine() {
}

int TransmissionLine::AddAlignmentPoint(const AlignmentPoint& point) {
  is_updated_points_xyz_alignment_ = false;
  is_updated_points_xyz_structures_ = false;
  return alignment_.AddPoint(point);
}

int TransmissionLine::AddLineCable(const LineCable& line_cable) {
  // does preliminary check if linecable connections are valid
  if (line_cable.Validate(false, nullptr) == false) {
    return -1;
  }

  // checks all the line cable connections
  const std::list<LineCableConnection>* connections = line_cable.connections();
  for (auto iter = connections->cbegin(); iter != connections->cend(); iter++) {
    const LineCableConnection& connection = *iter;

    // checks that structure is owned by line
    const int index_structure = IndexLineStructure(connection.line_structure);
    if (index_structure == -1) {
      return -1;
    }

    // checks if the attachment is open
    if (HasConnection(index_structure, connection.index_attachment) == true) {
      return -1;
    }
  }

  // searches list for a position to insert line cable
  const LineCableConnection& connection = *connections->cbegin();
  const int index_structure = IndexLineStructure(connection.line_structure);
  const int index_attachment = connection.index_attachment;

  auto iter = line_cables_.cbegin();
  while (iter != line_cables_.cend()) {
    const LineCable& line_cable_list = *iter;
    const LineCableConnection& connection_list =
        *line_cable_list.connections()->cbegin();
    const int index_structure_list =
        IndexLineStructure(connection_list.line_structure);
    const int index_attachment_list = connection_list.index_attachment;

    // checks if list connection indexes are larger than new connection indexes
    if (index_structure <= index_structure_list) {
      if (index_attachment < index_attachment_list) {
        // position is found
        break;
      }
    }

    iter++;
  }

  // inserts into line cable container
  iter = line_cables_.insert(iter, line_cable);

  // gets index and returns
  return std::distance(line_cables_.cbegin(), iter);
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
  iter = line_structures_.insert(iter, structure);

  // gets index and returns
  return std::distance(line_structures_.begin(), iter);
}

bool TransmissionLine::DeleteAlignmentPoint(const int& index) {
  is_updated_points_xyz_alignment_ = false;
  is_updated_points_xyz_structures_ = false;

  // determines if point will affect valid station bounds
  bool is_boundary_point = false;
  const int kSize = alignment_.points()->size();
  if ((index == 0) || (index == kSize - 1)) {
    is_boundary_point = true;
  }

  const bool is_deleted = alignment_.DeletePoint(index);

  // removes invalid dependency objects
  if ((is_deleted == true) && (is_boundary_point == true)) {
    DeleteInvalidLineStructures();
    DeleteInvalidLineCables();
  }

  return is_deleted;
}

bool TransmissionLine::DeleteLineCable(const int& index) {
  // checks index
  if (IsValidLineCableIndex(index) == false) {
    return false;
  }

  // gets iterator
  auto iter = std::next(line_cables_.cbegin(), index);

  // erases from container
  line_cables_.erase(iter);

  return true;
}

bool TransmissionLine::DeleteLineStructure(const int& index) {
  is_updated_points_xyz_structures_ = false;

  // checks index
  if (IsValidLineStructureIndex(index) == false) {
    return false;
  }

  // deletes dependency objects
  DeleteConnectionsFromStructure(index);
  DeleteInvalidLineCables();

  // erases from list
  auto iter_list = std::next(line_structures_.cbegin(), index);
  line_structures_.erase(iter_list);

  return true;
}

bool TransmissionLine::HasConnection(const int& index_structure,
                                     const int& index_attachment) const {
  // gets line structure
  auto iter_structure = std::next(line_structures_.cbegin(), index_structure);
  const LineStructure* line_structure = &(*iter_structure);

  // searches line cables
  for (auto iter_cable = line_cables_.cbegin();
       iter_cable != line_cables_.cend(); iter_cable++) {
    const LineCable& line_cable = *iter_cable;
    const std::list<LineCableConnection>* connections =
        line_cable.connections();

    // searches connections
    for (auto iter_connection = connections->cbegin();
         iter_connection != connections->cend(); iter_connection++) {
      const LineCableConnection& connection = *iter_connection;
      if ((connection.line_structure == line_structure)
          && (connection.index_attachment == index_attachment)) {
        // a match is found, there is already a line cable with that connection
        return true;
      }
    }
  }

  return false;
}

int TransmissionLine::ModifyAlignmentPoint(const int& index,
                                           const AlignmentPoint& point) {
  is_updated_points_xyz_alignment_ = false;
  is_updated_points_xyz_structures_ = false;

  // determines if point will affect valid station bounds
  bool is_boundary_point = false;
  const int kSize = alignment_.points()->size();
  if ((index == 0) || (index == kSize - 1)) {
    is_boundary_point = true;
  }

  // modifies alignment
  const int index_new = alignment_.ModifyPoint(index, point);

  // removes invalid dependency objects
  if ((index_new != -1) && (is_boundary_point == true)) {
    DeleteInvalidLineStructures();
    DeleteInvalidLineCables();
  }

  return index_new;
}

int TransmissionLine::ModifyLineCable(const int& index,
                                      const LineCable& line_cable) {
  // checks index
  if (IsValidLineCableIndex(index) == false) {
    return -1;
  }

  // caches current line cable in case something goes wrong
  auto iter = std::next(line_cables_.begin(), index);
  const LineCable line_cable_cache = *iter;

  // deletes and then adds back
  bool status = DeleteLineCable(index);
  if (status == false) {
    return -1;
  }

  const int index_new = AddLineCable(line_cable);
  if (index_new == -1) {
    // adds back original line cable
    AddLineCable(line_cable_cache);
  }

  return index_new;
}

bool TransmissionLine::ModifyLineStructure(const int& index,
                                           const LineStructure& structure) {
  is_updated_points_xyz_structures_ = false;

  // checks if index is valid
  if (IsValidLineStructureIndex(index) == false) {
    return false;
  }

  // gets station from back structure
  double station_back = alignment_.points()->front().station - 1;
  const LineStructure* structure_back = nullptr;
  if (index != 0) {
    structure_back = &(*std::next(line_structures_.cbegin(), index - 1));
    station_back = structure_back->station();
  }

  // gets station from ahead structure
  double station_ahead = alignment_.points()->back().station + 1;
  const LineStructure* structure_ahead = nullptr;
  const int kSizeLineStructures = line_structures_.size();
  if (index != (kSizeLineStructures - 1)) {
    structure_ahead = &(*std::next(line_structures_.cbegin(), index + 1));
    station_ahead = structure_ahead->station();
  }

  // checks if structure is within station boundaries
  // moving position within the structure list is restricted to keep the
  // order line cable connections
  const double station = structure.station();
  if ((station <= station_back) || (station_ahead <= station)) {
    return false;
  }

  // updates the structure
  LineStructure& line_structure = *std::next(line_structures_.begin(), index);
  line_structure = structure;

  // updates invalid dependencies
  DeleteInvalidConnectionsFromStructure(index);
  DeleteInvalidLineCables();

  // successfully modified structure
  return true;
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

  // validates line structures
  for (auto iter = line_structures_.cbegin(); iter != line_structures_.cend();
       iter++) {
    const LineStructure& line_structure = *iter;
    if (line_structure.Validate(is_included_warnings, messages) == false) {
      is_valid = false;
    }
  }

  // validates line cables
  for (auto iter = line_cables_.cbegin(); iter != line_cables_.cend(); iter++) {
    const LineCable& line_cable = *iter;
    if (line_cable.Validate(is_included_warnings, messages) == false) {
      is_valid = false;
    }
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

const std::list<LineCable>* TransmissionLine::line_cables() const {
  return &line_cables_;
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

void TransmissionLine::DeleteConnectionsFromStructure(
    const int& index_structure) {
  auto iter = std::next(line_structures_.cbegin(), index_structure);
  const LineStructure* line_structure = &(*iter);

  // searches all line cables for reference to the structure
  for (auto iter = line_cables_.begin(); iter != line_cables_.end(); iter++) {
    LineCable& line_cable = *iter;

    // searches all line cable connections
    const std::list<LineCableConnection>* connections =
        line_cable.connections();
    auto iter_connection = connections->cbegin();
    while (iter_connection != connections->cend()) {
      const LineCableConnection& connection = *iter_connection;
      if (connection.line_structure == line_structure) {
        // a connection to the line structure is found, erases from line cable
        int index_connection = std::distance(connections->cbegin(),
                                             iter_connection);
        line_cable.DeleteConnection(index_connection);

        iter_connection = std::next(connections->cbegin(), index_connection);
      } else {
        // no connection to the line structure is found
        iter_connection++;
      }
    }
  }
}

void TransmissionLine::DeleteInvalidConnectionsFromStructure(
    const int& index_structure) {
  // gets line structure and attachment count
  auto iter_structure = std::next(line_structures_.cbegin(), index_structure);
  const LineStructure* line_structure = &(*iter_structure);
  const int kSizeAttachments = line_structure->structure()->attachments.size();

  // searches line cables
  for (auto iter_cable = line_cables_.begin();
       iter_cable != line_cables_.end(); iter_cable++) {
    LineCable& line_cable = *iter_cable;
    const std::list<LineCableConnection>* connections =
        line_cable.connections();

    // searches connections
    auto iter_connection = connections->cbegin();
    while (iter_connection != connections->cend()) {
      const LineCableConnection& connection = *iter_connection;
      if ((connection.line_structure == line_structure)
          && (kSizeAttachments <= connection.index_attachment)) {
        // connection is invalid, deletes from line cable
        int index_connection = std::distance(connections->cbegin(),
                                             iter_connection);
        line_cable.DeleteConnection(index_connection);
        iter_connection = std::next(connections->cbegin(), index_connection);
      } else {
        iter_connection++;
      }
    }
  }
}

void TransmissionLine::DeleteInvalidLineCables() {
  // clears line structures that are invalid
  // this is most likely due to a deleted start/end connection
  auto iter = line_cables_.cbegin();
  while (iter != line_cables_.cend()) {
    const LineCable& line_cable = *iter;
    if (line_cable.Validate(false, nullptr) == false) {
      iter = line_cables_.erase(iter);
    } else {
      iter++;
    }
  }
}

void TransmissionLine::DeleteInvalidLineStructures() {
  const double station_min = alignment_.points()->front().station;
  const double station_max = alignment_.points()->back().station;

  // clears line structures outside of alignment
  auto iter = line_structures_.cbegin();
  while (iter != line_structures_.cend()) {
    const LineStructure& line_structure = *iter;
    const double station = line_structure.station();
    if ((station < station_min) || (station_max < station)) {
      // clears any references to the structure and deletes
      const int index = std::distance(line_structures_.cbegin(), iter);
      DeleteConnectionsFromStructure(index);
      iter = line_structures_.erase(iter);
    } else {
      // line structure is within the boundaries, keep searching
      iter++;
    }
  }
}

int TransmissionLine::IndexLineStructure(
    const LineStructure* line_structure) const {
  // searches list for a memory address match
  auto iter = line_structures_.cbegin();
  while (iter != line_structures_.cend()) {
    const LineStructure* line_structure_list = &(*iter);
    if (line_structure == line_structure_list) {
      break;
    } else {
      iter++;
    }
  }

  // checks iterator and returns
  if (iter == line_structures_.cend()) {
    return -1;
  } else {
    return std::distance(line_structures_.cbegin(), iter);
  }
}

bool TransmissionLine::IsUpdated() const {
  if ((is_updated_points_xyz_alignment_ == true)
      || (is_updated_points_xyz_structures_ == true)) {
    return true;
  } else {
    return false;
  }
}

bool TransmissionLine::IsValidLineCableIndex(const int& index) const {
  const int kSize = line_cables_.size();
  if ((0 <= index) && (index < kSize)) {
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
