// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/transmissionline/alignment.h"

AlignmentPoint::AlignmentPoint() {
  elevation = -999999;
  rotation = -999999;
  station = -999999;
}

AlignmentPoint::~AlignmentPoint() {
}

bool AlignmentPoint::Validate(const bool& is_included_warnings,
                              std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "ALIGNMENT POINT";

  // validates elevation
  if (elevation < 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid elevation";
      messages->push_back(message);
    }
  }

  // validates rotation
  if (360 < std::abs(rotation)) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid rotation";
      messages->push_back(message);
    }
  }

  // validates station
  if (station < 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid station";
      messages->push_back(message);
    }
  }

  // returns validation status
  return is_valid;
}


Alignment::Alignment() {
}

Alignment::~Alignment() {
}

int Alignment::AddPoint(const AlignmentPoint& point) {
  // searches list for the position to insert point
  auto iter = points_.begin();
  while (iter != points_.end()) {
    const AlignmentPoint& point_list = *iter;
    if (point.station < point_list.station) {
      break;
    } else {
      iter++;
    }
  }

  // inserts point into list
  points_.insert(iter, point);

  // gets index and returns
  return std::distance(points_.begin(), iter) - 1;
}

void Alignment::DeletePoint(const int& index) {
  // gets iterator and erases point
  auto iter = std::next(points_.cbegin(), index);
  points_.erase(iter);
}

int Alignment::ModifyPoint(const int& index, const AlignmentPoint& point) {
  // uses the delete/add methods to keep the point list sorted
  DeletePoint(index);
  return AddPoint(point);
}

bool Alignment::Validate(const bool& is_included_warnings,
                     std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "ALIGNMENT";

  // validates individual points and checks for ascending order
  const AlignmentPoint* point_prev = nullptr;
  for (auto iter = points_.cbegin(); iter != points_.cend(); iter++) {
    const AlignmentPoint& point = *iter;
    if (point.Validate(is_included_warnings, messages) == false) {
      is_valid = false;
    }

    if (point_prev != nullptr) {
      if (point.station < point_prev->station) {
        message.description = "Invalid point sorting";
      }
    }

    point_prev = &point;
  }

  return is_valid;
}

const std::list<AlignmentPoint>* Alignment::points() const {
  return &points_;
}
