// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/sagtension/catenary_cable_component_tension_solver.h"

CatenaryCableComponentTensionSolver::CatenaryCableComponentTensionSolver() {
  catenary_cable_ = nullptr;
}

CatenaryCableComponentTensionSolver::CatenaryCableComponentTensionSolver(
    const CatenaryCable* catenary_cable) {
  catenary_cable_ = catenary_cable;
}

CatenaryCableComponentTensionSolver::~CatenaryCableComponentTensionSolver() {
}

double CatenaryCableComponentTensionSolver::TensionAverageComponent(
    const CableElongationModel::ComponentType& type_component) const {

  const double strain_total =
      model_elongation_.Strain(CableElongationModel::ComponentType::kCombined,
                               catenary_cable_->TensionAverage());

  return model_elongation_.Load(type_component, strain_total);
}

/// The component horizontal tension cannot be directly solved for. It is
/// calculated by scaling the average component load by the ratio of the entire
/// cable horizontal tension and entire cable average load.
double CatenaryCableComponentTensionSolver::TensionHorizontalComponent(
    const CableElongationModel::ComponentType type_component) const {

  const double tension_average_component =
      TensionAverageComponent(type_component);

  // uses ratio of average tension and horizontal tension to scale value
  return tension_average_component
      * (catenary_cable_->tension_horizontal()
      / catenary_cable_->TensionAverage());
}

bool CatenaryCableComponentTensionSolver::Validate(
    const bool& is_included_warnings,
    std::list<std::string>* messages_error) const {

  bool is_valid = true;

  // validates catenary cable
  if (catenary_cable_ == nullptr) {
    is_valid = false;
    if (messages_error != nullptr) {
      messages_error->push_back("CATENEARY CABLE COMPONENT TENSION SOLVER - "
                                "Invalid catenary cable");
    }
  } else {
    if (catenary_cable_->Validate(is_included_warnings,
                                 messages_error) == false) {
      is_valid = false;
    }
  }

  // further validates
  if (is_valid == true) {

    // validates elongation model
    if (model_elongation_.Validate(is_included_warnings,
                                   messages_error) == false) {
      is_valid = false;
    }
  }

  return is_valid;
}

const CatenaryCable* CatenaryCableComponentTensionSolver::catenary_cable()
     const {
  return catenary_cable_;
}

void CatenaryCableComponentTensionSolver::set_catenary_cable(
    const CatenaryCable* catenary_cable) {

    catenary_cable_ = catenary_cable;

    // updates cable elongation model
    model_elongation_.set_cable(catenary_cable_->cable());
    model_elongation_.set_state(catenary_cable_->state());
}
