// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/transmissionline/line_cable.h"

#include <cmath>

#include "models/transmissionline/cable_unit_load_solver.h"
#include "models/transmissionline/catenary_solver.h"
#include "models/transmissionline/line_structure.h"

LineCableConnection::LineCableConnection() {
  index_attachment = -1;
  line_structure = nullptr;
}

LineCable::LineCable() {
  cable_ = nullptr;
  weathercase_stretch_creep_ = nullptr;
  weathercase_stretch_load_ = nullptr;
}

LineCable::~LineCable() {
}

int LineCable::AddConnection(const LineCableConnection& connection) {
  if (connection.line_structure == nullptr) {
    return -1;
  }

  const LineStructure* line_structure = connection.line_structure;

  // searches list for the position to insert connection
  auto iter = connections_.begin();
  while (iter != connections_.end()) {
    const LineCableConnection& connection_list = *iter;
    const LineStructure* line_structure_list = connection_list.line_structure;
    if (line_structure->station() < line_structure_list->station()) {
      // position is found
      break;
    } else if (line_structure->station() == line_structure_list->station()) {
      // exits due to duplicate station
      return -1;
    } else {
      iter++;
    }
  }

  // inserts connection into list
  connections_.insert(iter, connection);

  // gets index and returns
  return std::distance(connections_.begin(), iter) - 1;
}

Catenary3d LineCable::CatenaryRulingSpan() const {
  // initializes
  Catenary3d catenary;
  catenary.set_spacing_endpoints(spacing_attachments_ruling_span_);

  // solves for the unit loads
  CableUnitLoadSolver solver_unit_loads;
  solver_unit_loads.set_diameter_cable(&cable_->diameter);
  solver_unit_loads.set_weight_unit_cable(&cable_->weight_unit);

  Vector3d load_unit = solver_unit_loads.UnitCableLoad(
      *constraint_.case_weather);
  catenary.set_weight_unit(load_unit);

  // solves for the horizontal tension
  CatenarySolver solver_tension;
  solver_tension.set_spacing_endpoints(catenary.spacing_endpoints());
  solver_tension.set_value_target(constraint_.limit);
  solver_tension.set_weight_unit(load_unit);

  if (constraint_.type_limit == CableConstraint::LimitType::kCatenaryConstant) {
    // sets up solver for an analysis target type
    solver_tension.set_type_target(CatenarySolver::TargetType::kConstant);
  } else if (constraint_.type_limit
      == CableConstraint::LimitType::kHorizontalTension) {
    // horizontal tension is already known
    catenary.set_tension_horizontal(constraint_.limit);
    return catenary;
  } else if (constraint_.type_limit == CableConstraint::LimitType::kLength) {
    // sets up the solver for a length target type
    solver_tension.set_type_target(CatenarySolver::TargetType::kLength);
  } else if (constraint_.type_limit == CableConstraint::LimitType::kSag) {
    // sets up the solver for a sag target type
    solver_tension.set_type_target(CatenarySolver::TargetType::kSag);
  } else if (constraint_.type_limit
      == CableConstraint::LimitType::kSupportTension) {
    // sets up the solver for a tension target type
    solver_tension.set_type_target(CatenarySolver::TargetType::kTension);

    if (spacing_attachments_ruling_span_.z() <= 0) {
      solver_tension.set_position_target(0);
    } else {
      solver_tension.set_position_target(1);
    }
  }

  catenary.set_tension_horizontal(solver_tension.TensionHorizontal());
  return catenary;
}

void LineCable::ClearConnections() {
  connections_.clear();
}

bool LineCable::DeleteConnection(const int& index) {
  // checks if index is valid
  if (IsValidConnectionIndex(index) == false) {
    return false;
  }

  // gets iterator and erases point
  auto iter = std::next(connections_.cbegin(), index);
  connections_.erase(iter);

  return true;
}

int LineCable::ModifyConnection(const int& index,
                                const LineCableConnection& connection) {
  // checks if index is valid
  if (IsValidConnectionIndex(index) == false) {
    return -1;
  }

  // caches specified connection in case something goes wrong
  LineCableConnection connection_cache = *std::next(connections_.cbegin(),
                                                    index);
  if (DeleteConnection(index) == false) {
    return -1;
  }

  const int status = AddConnection(connection);
  if (status == -1) {
    // resets cached connection
    AddConnection(connection_cache);
  }

  // successfully modified connection
  return status;
}

bool LineCable::Validate(const bool& is_included_warnings,
                         std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "LINE CABLE";

  // validates cable
  if (cable_ == nullptr) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid cable";
      messages->push_back(message);
    }
  } else {
    if (cable_->Validate(is_included_warnings, messages) == false) {
      is_valid = false;
    }
  }

  // validates constraint
  if (constraint_.Validate(is_included_warnings, messages) == false) {
    is_valid = false;
  }

  // validates spacing-attachments-rulingspan
  if (spacing_attachments_ruling_span_.x() <= 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid horizontal ruling span attachment spacing";
      messages->push_back(message);
    }
  }

  if (spacing_attachments_ruling_span_.y() != 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid transverse ruling span attachment spacing";
      messages->push_back(message);
    }
  }

  if (2000 < std::abs(spacing_attachments_ruling_span_.z())) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid vertical ruling span attachment spacing";
      messages->push_back(message);
    }
  }

  // validates weathercase-stretch-creep
  if (weathercase_stretch_creep_ == nullptr) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid creep stretch weathercase";
      messages->push_back(message);
    }
  } else {
    if (weathercase_stretch_creep_->Validate(is_included_warnings,
                                             messages) == false) {
      is_valid = false;
    }
  }

  // validates weathercase-stretch-load
  if (weathercase_stretch_load_ == nullptr) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid load stretch weathercase";
      messages->push_back(message);
    }
  } else {
    if (weathercase_stretch_load_->Validate(is_included_warnings,
                                            messages) == false) {
      is_valid = false;
    }
  }

  // returns if errors are present
  if (is_valid == false) {
    return is_valid;
  }

  // validates if catenary can be solved for with constraint and ruling
  // span geometry
  Catenary3d catenary = CatenaryRulingSpan();
  if (catenary.Validate(is_included_warnings, messages) == false) {
    is_valid = false;
  }

  // validates connections
  ValidateConnections(is_included_warnings, messages);

  // returns validation status
  return is_valid;
}

bool LineCable::ValidateConnections(const bool& is_included_warnings,
                                    std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "LINE CABLE";

  // validates all connections
  const int kSizeConnections = connections_.size();
  if (kSizeConnections < 2) {
    is_valid = false;
    message.description = "Not enough connections";
    if (messages != nullptr) {
      messages->push_back(message);
    }
  } else {
    for (auto iter = connections_.cbegin(); iter != connections_.cend();
         iter++) {
      const LineCableConnection* connection = &(*iter);
      const LineStructure* structure = connection->line_structure;
      const Hardware* hardware =
          structure->hardwares()->at(connection->index_attachment);
      if ((connection == &connections_.front())
          || (connection == &connections_.back())) {
        // checks for dead-end hardware at start and end connections
        if (hardware->type != Hardware::HardwareType::kDeadEnd) {
          message.description = "Terminal line cable connection does not have "
                                "dead-end type hardware";
          if (messages != nullptr) {
            messages->push_back(message);
          }
        }
      } else {
        // checks for suspension hardware at all middle connections
        if (hardware->type != Hardware::HardwareType::kSuspension) {
          message.description = "Interior line cable connection does not have "
                                "suspension type hardware";
          if (messages != nullptr) {
            messages->push_back(message);
          }
        }
      }
    }
  }

  return is_valid;
}

const Cable* LineCable::cable() const {
  return cable_;
}

const std::list<LineCableConnection>* LineCable::connections() const {
  return &connections_;
}

CableConstraint LineCable::constraint() const {
  return constraint_;
}

void LineCable::set_cable(const Cable* cable) {
  cable_ = cable;
}

void LineCable::set_constraint(const CableConstraint& constraint) {
  constraint_ = constraint;
}

void LineCable::set_spacing_attachments_ruling_span(
    const Vector3d& spacing_attachments_ruling_span) {
  spacing_attachments_ruling_span_ = spacing_attachments_ruling_span;
}

void LineCable::set_weathercase_stretch_creep(
    const WeatherLoadCase* weathercase) {
  weathercase_stretch_creep_ = weathercase;
}

void LineCable::set_weathercase_stretch_load(
    const WeatherLoadCase* weathercase) {
  weathercase_stretch_load_ = weathercase;
}

const Vector3d LineCable::spacing_attachments_ruling_span() const {
  return spacing_attachments_ruling_span_;
}

const WeatherLoadCase* LineCable::weathercase_stretch_creep() const {
  return weathercase_stretch_creep_;
}

const WeatherLoadCase* LineCable::weathercase_stretch_load() const {
  return weathercase_stretch_load_;
}

bool LineCable::IsValidConnectionIndex(const int& index) const {
  const int kSize = connections_.size();
  if ((0 <= index) && (index < kSize)) {
    return true;
  } else {
    return false;
  }
}
