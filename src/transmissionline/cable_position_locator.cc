// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/transmissionline/cable_position_locator.h"

#include <cmath>

#include "models/base/units.h"
#include "models/transmissionline/cable_attachment_equilibrium_solver.h"

CablePositionLocator::CablePositionLocator() {
  direction_wind_ = AxisDirectionType::kNull;
  index_cable_ = -9999;
  line_ = nullptr;
  size_connections_ = -9999;
  tension_horizontal_ = -999999;
  weight_unit_ = nullptr;

  is_updated_ = false;
}

CablePositionLocator::~CablePositionLocator() {
}

std::list<Point3d> CablePositionLocator::PointsCable(const int& index_span,
                                                     const int& num) const {
  std::list<Point3d> points;

  // updates class if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return points;
    }
  }

  // determines if span index is valid
  if ((index_span < 0) || (size_connections_ - 1 < index_span)) {
    return points;
  }
  
  // gets cable attachment points
  const Point3d& point_back = points_cable_[index_span];
  const Point3d& point_ahead = points_cable_[index_span + 1];
  
  // creates a catenary
  Vector2d spacing_xy(point_ahead.x - point_back.x,
                      point_ahead.y - point_back.y);
  
  Vector3d spacing(spacing_xy.Magnitude(), 0, point_ahead.z - point_back.z);

  Catenary3d catenary;
  catenary.set_direction_transverse(direction_wind_);
  catenary.set_spacing_endpoints(spacing);
  catenary.set_tension_horizontal(tension_horizontal_);
  catenary.set_weight_unit(*weight_unit_);

  // calculates points
  const double increment = 1 / (static_cast<double>(num) - 1);
  double position_fraction = 0;
  const double kAngleXy = spacing_xy.Angle();
  for (int i = 0; i < num; i++) {
    // gets point from catenary
    Point3d point_catenary = catenary.Coordinate(position_fraction);

    // converts to global coordinate system
    Vector2d vector;
    vector.set_x(point_catenary.x);
    vector.set_y(point_catenary.y);
    vector.Rotate(kAngleXy);

    Point3d point_global = point_back;
    point_global.x += vector.x();
    point_global.y += vector.y();
    point_global.z += point_catenary.z;

    // adds to list
    points.push_back(point_global);

    // increments position fraction
    position_fraction += increment;
  }

  return points;
}

const std::vector<Point3d>* CablePositionLocator::PointsCableAttachment()
    const {
  // updates class if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return nullptr;
    }
  }

  return &points_cable_;
}

bool CablePositionLocator::Validate(const bool& is_included_warnings,
                                    std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "CABLE POSITION LOCATOR";

  // validates direction-wind
  if (direction_wind_ == AxisDirectionType::kNull) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid wind direction";
      messages->push_back(message);
    }
  }

  // validates index-cable
  if (index_cable_ < 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid line cable index";
      messages->push_back(message);
    }
  }

  // validates line
  if (line_ == nullptr) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid transmission line";
      messages->push_back(message);
    }
  } else {
    if (line_->Validate(is_included_warnings,
                        messages) == false) {
      is_valid = false;
    }
  }

  // validates tension-horizontal
  if (tension_horizontal_ <= 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid horizontal tension";
      messages->push_back(message);
    }
  }

  // validates weight-unit
  if (weight_unit_ == nullptr) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid unit weight";
      messages->push_back(message);
    }
  } else {
    if (weight_unit_->x() != 0) {
      is_valid = false;
      if (messages != nullptr) {
        message.description = "Invalid horizontal unit weight. It must "
                              "equal zero";
        messages->push_back(message);
      }
    }

    if (weight_unit_->y() < 0) {
      is_valid = false;
      if (messages != nullptr) {
        message.description = "Invalid transverse unit weight";
        messages->push_back(message);
      }
    }

    if (weight_unit_->z() <= 0) {
      is_valid = false;
      if (messages != nullptr) {
        message.description = "Invalid vertical unit weight";
        messages->push_back(message);
      }
    }
  }
  
  // validates update process
  if (Update() == false) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Error updating class.";
      messages->push_back(message);
    }
  }

  return is_valid;
}

AxisDirectionType CablePositionLocator::direction_wind() const {
  return direction_wind_;
}

int CablePositionLocator::index_line_cable() const {
  return index_cable_;
}

void CablePositionLocator::set_direction_wind(
    const AxisDirectionType& direction_wind) {
  direction_wind_ = direction_wind;
  is_updated_ = false;
}

void CablePositionLocator::set_index_line_cable(const int& index_line_cable) {
  index_cable_ = index_line_cable;
  is_updated_ = false;
}

void CablePositionLocator::set_tension_horizontal(
    const double& tension_horizontal) {
  tension_horizontal_ = tension_horizontal;
  is_updated_ = false;
}

void CablePositionLocator::set_transmission_line(const TransmissionLine* line) {
  line_ = line;
  is_updated_ = false;
}

void CablePositionLocator::set_weight_unit(const Vector3d* weight_unit) {
  weight_unit_ = weight_unit;
  is_updated_ = false;
}

double CablePositionLocator::tension_horizontal() const {
  return tension_horizontal_;
}

const TransmissionLine* CablePositionLocator::transmission_line() const {
  return line_;
}

const Vector3d* CablePositionLocator::weight_unit() const {
  return weight_unit_;
}

bool CablePositionLocator::InitializeConnectionData() const {
  // gets the line cable
  const std::list<LineCable>* line_cables = line_->line_cables();
  const int kSizeLineCables = line_cables->size();
  if ((index_cable_ < 0) || (kSizeLineCables <= index_cable_)) {
    return false;
  }

  const LineCable& line_cable = *std::next(line_cables->cbegin(), index_cable_);
  
  // adds data for every connection
  const std::list<LineCableConnection>* connections = line_cable.connections();
  int index = 0;
  for (auto iter = connections->cbegin(); iter != connections->cend(); iter++) {
    const LineCableConnection& connection = *iter;

    // gets structure and attachment index
    const LineStructure* line_structure = connection.line_structure;
    const int& index_structure = line_->IndexLineStructure(line_structure);
    const int& index_attachment = connection.index_attachment;

    // calculates structure attachment point and caches
    const Point3d& point = line_->PointXyzLineStructureAttachment(
        index_structure,
        connection.index_attachment);    
    if (point.x == -999999) {
      return false;
    } else {
      points_structure_[index] = point;
    }

    // gets hardware
    const Hardware* hardware =
        line_structure->hardwares()->at(index_attachment);    
    if (hardware == nullptr) {
      return false;
    } else {
      hardwares_[index] = hardware;
    }

    index++;
  }

  return true;
}

bool CablePositionLocator::InitializeContainers() const {
  // gets the line cable
  const std::list<LineCable>* line_cables = line_->line_cables();  
  const LineCable& line_cable = *std::next(line_cables->cbegin(), index_cable_);

  // caches the number of connections  
  size_connections_ = line_cable.connections()->size();
  
  // resizes containers to match connection count
  hardwares_.clear();
  hardwares_.resize(size_connections_, nullptr);

  points_cable_.clear();
  points_cable_.resize(size_connections_);

  points_hardware_.clear();
  points_hardware_.resize(size_connections_);

  points_structure_.clear();
  points_structure_.resize(size_connections_);

  return true;
}

bool CablePositionLocator::InitializePointsHardware() const {
  // creates hardware points for each connection
  for (int index = 0; index < size_connections_; index++) {
    // first and last connections have dead-end assemblies
    // the cable attachment has no movement relative to the structure attachment
    // skips these to avoid runtime errors
    if ((index == 0) || (index == size_connections_ - 1)) {
      SphericalPoint3d point(0, 0, 0);
      points_hardware_[index] = point;
      continue;
    }
    
    // gets structure attachment points for back, current, and ahead structure
    const Point3d& point_back = points_structure_[index - 1];
    const Point3d& point_current = points_structure_[index];
    const Point3d& point_ahead = points_structure_[index + 1];

    // creates unit vectors from current point to back and ahead points
    // adds the unit vectors together to get the bisect orientation
    Vector2d vector_back;
    vector_back.set_x(point_back.x - point_current.x);
    vector_back.set_y(point_back.y - point_current.y);
    vector_back.Scale(1 / vector_back.Magnitude());

    Vector2d vector_ahead;
    vector_ahead.set_x(point_ahead.x - point_current.x);
    vector_ahead.set_y(point_ahead.y - point_current.y);
    vector_ahead.Scale(1 / vector_ahead.Magnitude());

    Vector2d vector_combined;
    vector_combined.set_x(vector_back.x() + vector_ahead.x());
    vector_combined.set_y(vector_back.y() + vector_ahead.y());

    // gets the hardware assembly at the connection
    const Hardware* hardware = hardwares_[index];

    // creates the starting spherical coordinate that is oriented in the
    // vertical plane
    SphericalPoint3d point;
    point.radius = hardware->length;
    point.angle_x = vector_combined.Angle();
    point.angle_z = 180;

    points_hardware_[index] = point;
  }

  return true;
}

bool CablePositionLocator::IsUpdated() const {
  if (is_updated_ == true) {
    return true;
  } else {
    return false;
  }
}

bool CablePositionLocator::SolveCablePosition() const {
  // initializes hardware and cable positions
  InitializePointsHardware();
  UpdatePointsCable();

  // iterates until the tension imbalance is within range
  double tension_imbalance = -999999;
  int iter = 0;
  const double precision = 5;
  while((precision <= std::abs(tension_imbalance)) && (iter < 100)) {
    // updates hardware points
    // does a force balance at all points and gets the max tension imbalance
    tension_imbalance = UpdatePointsHardware();

    // updates the cable attachment points
    UpdatePointsCable();

    iter++;
  }

  // returns based on whether solution converged
  if (iter < 100) {
    return true;
  } else {
    return false;
  }
}

bool CablePositionLocator::Update() const {
  // updates catenary-cable-reloaded
  if (is_updated_ == false) {
    // resizes containers to match connection size
    is_updated_ = InitializeContainers();
    if (is_updated_ == false) {
      return false;
    }
    
    // intializes cached connection data
    is_updated_ = InitializeConnectionData();
    if (is_updated_ == false) {
      return false;
    }

    // solves for the cable position
    is_updated_ = SolveCablePosition();
    if (is_updated_ == false) {
      return false;
    }
  }

  // if it reaches this point, update was successful
  return true;
}

bool CablePositionLocator::UpdatePointsCable() const {
  // updates all cable attachment points
  for (int index = 0; index < size_connections_; index++) {
    const Point3d& point_structure = points_structure_[index];
    const SphericalPoint3d& point_hardware = points_hardware_[index];
    
    // converts hardware spherical point to cartesian point
    // adds to structure xyz point to solve
    const double angle_x_rad =
        units::ConvertAngle(point_hardware.angle_x,
                            units::AngleConversionType::kDegreesToRadians);

    const double angle_z_rad =
        units::ConvertAngle(point_hardware.angle_z,
                            units::AngleConversionType::kDegreesToRadians);

    Point3d point_cable;
    point_cable.x = point_hardware.radius * std::cos(angle_x_rad)
                    * std::sin(angle_z_rad);
    point_cable.y = point_hardware.radius * std::sin(angle_x_rad)
                    * std::sin(angle_z_rad);
    point_cable.z = point_hardware.radius * std::cos(angle_z_rad);

    point_cable.x += point_structure.x;
    point_cable.y += point_structure.y;
    point_cable.z += point_structure.z;

    points_cable_[index] = point_cable;
  }

  return true;
}

double CablePositionLocator::UpdatePointsHardware() const {
  double tension_imbalance_max = 0;

  Catenary3d catenary_back;
  catenary_back.set_direction_transverse(direction_wind_);
  catenary_back.set_tension_horizontal(tension_horizontal_);
  catenary_back.set_weight_unit(*weight_unit_);

  Catenary3d catenary_ahead;
  catenary_ahead.set_direction_transverse(direction_wind_);
  catenary_ahead.set_tension_horizontal(tension_horizontal_);
  catenary_ahead.set_weight_unit(*weight_unit_);

  Vector2d spacing_horizontal(0, 0);
  Vector3d spacing(0, 0, 0);

  CableAttachmentEquilibriumSolver solver_equilibrium;

  for (int index = 0; index < size_connections_; index++) {
    // first and last connections have dead-end assemblies
    // the cable attachment has no movement relative to the structure attachment
    // skips these to avoid runtime errors
    if ((index == 0) || (index == size_connections_ - 1)) {
      continue;
    }
    
    // gets cable attachment points for back, current, and ahead attachments
    const Point3d& point_back = points_cable_[index - 1];
    const Point3d& point_current = points_cable_[index];
    const Point3d& point_ahead = points_cable_[index + 1];

    // updates back catenary
    spacing_horizontal.set_x(point_current.x - point_back.x);
    spacing_horizontal.set_y(point_current.y - point_back.y);
    
    spacing.set_x(spacing_horizontal.Magnitude());
    spacing.set_z(point_current.z - point_back.z);

    catenary_back.set_spacing_endpoints(spacing);

    // updates ahead catenary
    spacing_horizontal.set_x(point_ahead.x - point_current.x);
    spacing_horizontal.set_y(point_ahead.y - point_current.y);
    
    spacing.set_x(spacing_horizontal.Magnitude());
    spacing.set_z(point_ahead.z - point_current.z);

    catenary_ahead.set_spacing_endpoints(spacing);

    // creates unit vectors from current point to back and ahead points
    // finds the difference between the angles
    Vector2d vector_back;
    vector_back.set_x(point_back.x - point_current.x);
    vector_back.set_y(point_back.y - point_current.y);
    const double angle_back = vector_back.Angle();

    Vector2d vector_ahead;
    vector_ahead.set_x(point_ahead.x - point_current.x);
    vector_ahead.set_y(point_ahead.y - point_current.y);
    vector_ahead.Scale(1 / vector_ahead.Magnitude());
    const double angle_ahead = vector_ahead.Angle();

    const double angle_catenaries = std::abs(angle_ahead - angle_back);

    // determines the direction of the line angle
    AxisDirectionType direction_line_angle;
    vector_back.Rotate(180);
    const double angle_diff_xy = std::abs(angle_ahead - angle_back);
    if (angle_diff_xy <= 180) {
      direction_line_angle = AxisDirectionType::kPositive;
    } else {
      direction_line_angle = AxisDirectionType::kNegative;
    }

    // gets the hardware point
    SphericalPoint3d& point_hardware = points_hardware_[index];

    // updates the equilibrium solver
    solver_equilibrium.set_angle_catenaries(angle_catenaries);
    solver_equilibrium.set_angle_hardware(point_hardware.angle_z);
    solver_equilibrium.set_catenary_back(&catenary_back);
    solver_equilibrium.set_catenary_ahead(&catenary_ahead);
    solver_equilibrium.set_direction_catenaries(direction_line_angle);

    // updates the hardware point z-axis angle
    point_hardware.angle_z = solver_equilibrium.AngleEquilibrium();

    // updates the max tension imbalance
    const double tension_imbalance = solver_equilibrium.TensionImbalance();
    if (tension_imbalance_max < tension_imbalance) {
      tension_imbalance_max = tension_imbalance;
    }
  }

  return tension_imbalance_max;
}
