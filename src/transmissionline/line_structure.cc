// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/transmissionline/line_structure.h"

#include <cmath>

LineStructure::LineStructure() {
  height_adjustment_ = -999999;
  offset_ = -999999;
  rotation_ = -999999;
  station_ = -999999;
  structure_ = nullptr;
}

LineStructure::~LineStructure() {
}

void LineStructure::AttachHardware(const int& index,
                                   const Hardware* hardware) {
  // checks for valid structure attachment index
  if (IsValidIndex(index) == false) {
    return;
  }

  // replaces the current hardware assembly
  hardwares_.at(index) = hardware;
}

void LineStructure::DetachHardware(const int& index) {
  // checks for valid structure attachment index
  if (IsValidIndex(index) == false) {
    return;
  }

  // deletes the line hardware
  hardwares_.at(index) = nullptr;
}

bool LineStructure::Validate(
    const bool& is_included_warnings,
    std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "LINE STRUCTURE";

  // validates assemblies
  for (auto iter = hardwares_.cbegin(); iter != hardwares_.cend(); iter++) {
    const Hardware* hardware = *iter;
    if (hardware != nullptr) {
      if (hardware->Validate(is_included_warnings, messages) == false) {
        is_valid = false;
      }
    }
  }

  // validates height-adjustment
  if (height_adjustment_ < 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid height adjustment";
      messages->push_back(message);
    }
  }

  // validates rotation
  if (360 < std::abs(rotation_)) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid rotation";
      messages->push_back(message);
    }
  }

  // validates station
  if (station_ < 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid station";
      messages->push_back(message);
    }
  }

  // validates structure
  if (structure_ == nullptr) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid structure";
      messages->push_back(message);
    }
  } else {
    if (structure_->Validate(is_included_warnings, messages) == false) {
      is_valid = false;
    }
  }

  return is_valid;
}

const std::vector<const Hardware*>* LineStructure::hardwares() const {
  return &hardwares_;
}

double LineStructure::height_adjustment() const {
  return height_adjustment_;
}

double LineStructure::offset() const {
  return offset_;
}

double LineStructure::rotation() const {
  return rotation_;
}

void LineStructure::set_height_adjustment(const double& height_adjustment) {
  height_adjustment_ = height_adjustment;
}

void LineStructure::set_offset(const double& offset) {
  offset_ = offset;
}

void LineStructure::set_rotation(const double& rotation) {
  rotation_ = rotation;
}

void LineStructure::set_station(const double& station) {
  station_ = station;
}

void LineStructure::set_structure(const Structure* structure) {
  structure_ = structure;

  if (structure_ == nullptr) {
    hardwares_.clear();
  } else {
    // resizes hardware vector to match new attachment vector
    // any additions to the vector are set to null pointers
    hardwares_.resize(structure->attachments.size(), nullptr);
  }
}

double LineStructure::station() const {
  return station_;
}

const Structure* LineStructure::structure() const {
  return structure_;
}

bool LineStructure::IsValidIndex(const int& index) const {
  if (structure_ == nullptr) {
    return false;
  }

  // checks for valid structure attachment index
  const int kSize = structure_->attachments.size();
  if ((0 <= index) && (index < kSize)) {
    return true;
  } else {
    return false;
  }
}
