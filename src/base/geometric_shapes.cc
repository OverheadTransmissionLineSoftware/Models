// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/base/geometric_shapes.h"

#include <cmath>

#include "models/base/units.h"

Cylinder::Cylinder() {
  diameter_ = -999999;
  length_ = -999999;
}

Cylinder::~Cylinder() {}

double Cylinder::AreaCrossSection() const {
  return (units::kPi / 4) * std::pow(diameter_, 2);
}

bool Cylinder::Validate(const bool& is_included_warnings,
                        std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "CYLINDER";

  // validates diameter
  if (diameter_ <= 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid diameter";
      messages->push_back(message);
    }
  }

  // validates length
  if (length_ <= 0) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid length";
      messages->push_back(message);
    }
  }

  return is_valid;
}

double Cylinder::Volume() const {
  return AreaCrossSection() * length_;
}

double Cylinder::diameter() const {
  return diameter_;
}

double Cylinder::length() const {
  return length_;
}

void Cylinder::set_diameter(const double& diameter) {
  diameter_ = diameter;
}

void Cylinder::set_length(const double& length) {
  length_ = length;
}
