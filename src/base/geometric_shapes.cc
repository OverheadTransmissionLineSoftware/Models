// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "include/base/geometric_shapes.h"

#include <cmath>

#include "include/base/convert_units.h"

Cylinder::Cylinder() {
  diameter_ = -999999;
  length_ = -999999;
}

Cylinder::~Cylinder() {}

double Cylinder::AreaCrossSection() const {
  return (convertunits::kPi / 4) * pow(diameter_, 2);
}

bool Cylinder::Validate(const bool& is_included_warnings,
                        std::list<std::string>* messages_error) const {
  bool is_valid = true;

  // validate diameter
  if (diameter_ <= 0
      || ((100 < diameter_) && (is_included_warnings == true))) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CYLINDER - Invalid diameter");
    }
  }

  // validate length
  if (length_ <= 0
      || ((120 < length_) && (is_included_warnings == true))) {

    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CYLINDER - Invalid length");
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
