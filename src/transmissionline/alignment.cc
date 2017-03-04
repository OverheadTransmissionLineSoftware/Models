// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/transmissionline/alignment.h"

#include <cmath>

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
      // position is found
      break;
    } else if (point.station == point_list.station) {
      // exits due to duplicate station
      return -1;
    } else {
      iter++;
    }
  }

  // inserts point into list
  points_.insert(iter, point);

  // gets index and returns
  return std::distance(points_.begin(), iter) - 1;
}

bool Alignment::DeletePoint(const int& index) {
  // checks if index is valid
  if (IsValidPointIndex(index) == false) {
    return false;
  }

  // gets iterator and erases point
  auto iter = std::next(points_.cbegin(), index);
  points_.erase(iter);

  return true;
}

int Alignment::IndexSegment(const double& station) const {
  // checks station
  if (IsValidStation(station) == false) {
    return -1;
  }

  // checks for front point match
  if (station == points_.front().station) {
    return 0;
  }

  // searches points for a position
  auto iter = points_.cbegin();
  while (iter != points_.cend()) {
    const AlignmentPoint& point = *iter;
    if (station <= point.station) {
      break;
    } else {
      iter++;
    }
  }

  // returns index
  return std::distance(points_.cbegin(), iter) - 1;
}

bool Alignment::IsValidStation(const double& station) const {
  // gets the first and last alignment points
  const AlignmentPoint& point_front = points_.front();
  const AlignmentPoint& point_back = points_.back();

  // checks if the station value is within range
  if ((point_front.station <= station) && (station <= point_back.station)) {
    return true;
  } else {
    return false;
  }
}

int Alignment::ModifyPoint(const int& index, const AlignmentPoint& point) {
  // checks if index is valid
  if (IsValidPointIndex(index) == false) {
    return -1;
  }

  // caches specified point in case something goes wrong
  AlignmentPoint point_cache = *std::next(points_.cbegin(), index);
  if (DeletePoint(index) == false) {
    return -1;
  }

  const int status = AddPoint(point);
  if (status == -1) {
    // resets cached point
    AddPoint(point_cache);
  }

  // successfully modified point
  return status;
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

bool Alignment::IsValidPointIndex(const int& index) const {
  const int kSize = points_.size();
  if ((0 <= index) && (index < kSize)) {
    return true;
  } else {
    return false;
  }
}
