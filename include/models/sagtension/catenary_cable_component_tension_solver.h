// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef OTLS_MODELS_SAGTENSION_CATENARYCABLECOMPONENTTENSIONSOLVER_H_
#define OTLS_MODELS_SAGTENSION_CATENARYCABLECOMPONENTTENSIONSOLVER_H_

#include <list>
#include <string>

#include "models/sagtension/cable_elongation_model.h"
#include "models/sagtension/catenary_cable.h"

/// \par OVERVIEW
///
/// This class solves for the tension in catenary cable components.
///
/// This is done by creating a CableElongationModel at the CatenaryCable
/// CableState.
class CatenaryCableComponentTensionSolver {
 public:
  /// \brief Default constructor.
  CatenaryCableComponentTensionSolver();

  /// \brief Alternate constructor.
  CatenaryCableComponentTensionSolver(const CatenaryCable* catenary_cable);

  /// \brief Destructor.
  ~CatenaryCableComponentTensionSolver();

  /// \brief Gets the average load of the cable component.
  /// \param[in] type_component
  ///   The model component type.
  /// \return The average load of the cable component.
  double TensionAverageComponent(
      const CableElongationModel::ComponentType& type_component) const;

  /// \brief Gets the horizontal tension of the cable component.
  /// \param[in] type_component
  ///   The model component type.
  /// \return The horizontal tension of the cable component.
  double TensionHorizontalComponent(
      const CableElongationModel::ComponentType type_component) const;

  /// \brief Validates member variables.
  /// \param[in] is_included_warnings
  ///   A flag that tightens the acceptable value range.
  /// \param[in,out] messages_error
  ///   A list of detailed error messages. If this is provided, any validation
  ///   errors will be appended to the list.
  /// \return A boolean value indicating status of member variables.
  bool Validate(const bool& is_included_warnings = true,
                std::list<std::string>* messages_error = nullptr) const;

  /// \brief Gets the catenary cable.
  /// \return The catenary cable.
  const CatenaryCable* catenary_cable() const;

  /// \brief Sets the catenary cable.
  /// \param[in] catenary_cable
  ///   The catenary cable.
  void set_catenary_cable(const CatenaryCable* catenary_cable);

 private:
  /// \var catenary_cable_
  ///   The reference catenary cable.
  const CatenaryCable* catenary_cable_;

  /// \var model_elongation_
  ///   The elongation model for the catenary cable.
  CableElongationModel model_elongation_;
};

#endif // OTLS_MODELS_SAGTENSION_CATENARYCABLECOMPONENTTENSIONSOLVER_H_
