// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/transmissionline/cable_attachment_equilibrium_solver.h"

#include <cmath>

CableAttachmentEquilibriumSolver::CableAttachmentEquilibriumSolver() {
  angle_catenaries_ = -999999;
  angle_hardware_ = -999999;
  angle_hardware_equilibrium_ = -999999;
  catenary_ahead_ = nullptr;
  catenary_back_ = nullptr;
  direction_catenaries_ = AxisDirectionType::kPositive;

  is_updated_ = false;
}

CableAttachmentEquilibriumSolver::~CableAttachmentEquilibriumSolver() {
}

double CableAttachmentEquilibriumSolver::AngleEquilibrium() const {
  // updates class if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return angle_hardware_equilibrium_;
}

double CableAttachmentEquilibriumSolver::TensionImbalance() const {
  // updates class if necessary
  if (IsUpdated() == false) {
    if (Update() == false) {
      return -999999;
    }
  }

  return tension_imbalance_.Magnitude();
}

bool CableAttachmentEquilibriumSolver::Validate(
    const bool& is_included_warnings,
    std::list<ErrorMessage>* messages) const {
  // initializes
  bool is_valid = true;
  ErrorMessage message;
  message.title = "CABLE ATTACHMENT EQUILIBRIUM SOLVER";

  // validates angle-catenaries
  if ((angle_catenaries_ < 0) || (180 < angle_catenaries_)) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid angle between catenaries";
      messages->push_back(message);
    }
  }

  // validates angle-hardware
  if ((angle_hardware_ < 0) || (180 < angle_hardware_)) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid hardware angle";
      messages->push_back(message);
    }
  }

  // validates catenary-ahead
  if (catenary_ahead_ == nullptr) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid ahead catenary";
      messages->push_back(message);
    }
  } else {
    if (catenary_ahead_->Validate(is_included_warnings, messages) == false) {
      is_valid = false;
    }
  }

  // validates catenary-back
  if (catenary_back_ == nullptr) {
    is_valid = false;
    if (messages != nullptr) {
      message.description = "Invalid back catenary";
      messages->push_back(message);
    }
  } else {
    if (catenary_back_->Validate(is_included_warnings, messages) == false) {
      is_valid = false;
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

double CableAttachmentEquilibriumSolver::angle_catenaries() const {
  return angle_catenaries_;
}

double CableAttachmentEquilibriumSolver::angle_hardware() const {
  return angle_hardware_;
}

const Catenary3d* CableAttachmentEquilibriumSolver::catenary_ahead() const {
  return catenary_ahead_;
}

const Catenary3d* CableAttachmentEquilibriumSolver::catenary_back() const {
  return catenary_back_;
}

AxisDirectionType CableAttachmentEquilibriumSolver::direction_catenaries()
    const {
  return direction_catenaries_;
}

void CableAttachmentEquilibriumSolver::set_angle_catenaries(
    const double& angle) {
  angle_catenaries_ = angle;
  is_updated_ = false;
}

void CableAttachmentEquilibriumSolver::set_angle_hardware(
    const double& angle) {
  angle_hardware_ = angle;
  is_updated_ = false;
}

void CableAttachmentEquilibriumSolver::set_catenary_ahead(
    const Catenary3d* catenary) {
  catenary_ahead_ = catenary;
  is_updated_ = false;
}

void CableAttachmentEquilibriumSolver::set_catenary_back(
    const Catenary3d* catenary) {
  catenary_back_ = catenary;
  is_updated_ = false;
}

void CableAttachmentEquilibriumSolver::set_direction_catenaries(
    const AxisDirectionType& direction_catenaries) {
  direction_catenaries_ = direction_catenaries;
  is_updated_ = false;
}

bool CableAttachmentEquilibriumSolver::IsUpdated() const {
  if (is_updated_ == true) {
    return true;
  } else {
    return false;
  }
}

bool CableAttachmentEquilibriumSolver::Update() const {
  // initializes
  Vector3d tension(0, 0, 0);
  tension_cable_ = Vector3d(0, 0, 0);
  tension_hardware_ = Vector3d(0, 0, 0);
  tension_imbalance_ = Vector3d(0, 0, 0);

  // solves for the amount to rotate each catenary tension vector so it
  // aligns with the class coordinate system
  const double angle_rotate_xy = (180 - angle_catenaries_) / 2;

  // solves cable tension vector
  // gets each catenary tension vector and rotates to align with coordinate
  // system
  // sums the catenary tension vectors to get the cable tension vector
  tension = catenary_back_->Tension(1, AxisDirectionType::kNegative);
  if (direction_catenaries_ == AxisDirectionType::kPositive) {
    tension.Rotate(Plane2dType::kXy, angle_rotate_xy * -1);
  } else {
    tension.Rotate(Plane2dType::kXy, angle_rotate_xy);
  }

  tension_cable_.set_x(tension.x());
  tension_cable_.set_y(tension.y());
  tension_cable_.set_z(tension.z());

  tension = catenary_ahead_->Tension(0, AxisDirectionType::kPositive);
  if (direction_catenaries_ == AxisDirectionType::kPositive) {
    tension.Rotate(Plane2dType::kXy, angle_rotate_xy);
  } else {
    tension.Rotate(Plane2dType::kXy, angle_rotate_xy * -1);
  }

  tension_cable_.set_x(tension_cable_.x() + tension.x());
  tension_cable_.set_y(tension_cable_.y() + tension.y());
  tension_cable_.set_z(tension_cable_.z() + tension.z());

  // solves hardware tension vector by projecting cable tension vector onto
  // hardware angle unit vector, and then rotating to oppose cable vector
  const double tension_cable = tension_cable_.Magnitude();

  tension_hardware_.set_z(1);
  if (direction_catenaries_ == AxisDirectionType::kPositive) {
    tension_hardware_.Rotate(Plane2dType::kZy, angle_hardware_);
  } else {
    tension_hardware_.Rotate(Plane2dType::kZy, 360 - angle_hardware_);
  }
  tension_hardware_.set_x(tension_hardware_.x() * tension_cable);
  tension_hardware_.set_y(tension_hardware_.y() * tension_cable);
  tension_hardware_.set_z(tension_hardware_.z() * tension_cable);
  tension_hardware_.Rotate(Plane2dType::kZy, 180);

  // solves imbalance tension vector
  tension_imbalance_.set_x(-1 * (tension_hardware_.x() + tension_cable_.x()));
  tension_imbalance_.set_y(-1 * (tension_hardware_.y() + tension_cable_.y()));
  tension_imbalance_.set_z(-1 * (tension_hardware_.z() + tension_cable_.z()));

  // calculates the equilibrium angle using the cable tension vector
  if (direction_catenaries_ == AxisDirectionType::kPositive) {
    angle_hardware_equilibrium_ = tension_cable_.Angle(Plane2dType::kZy);
  } else {
    angle_hardware_equilibrium_ = 360 - tension_cable_.Angle(Plane2dType::kZy);
  }

  // updates class status
  is_updated_ = true;
  return is_updated_;
}
