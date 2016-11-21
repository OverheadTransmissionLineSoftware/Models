// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef OTLS_MODELS_TEST_FACTORY_H_
#define OTLS_MODELS_TEST_FACTORY_H_

#include "models/sagtension/cable_elongation_model.h"
#include "models/sagtension/sag_tension_cable.h"
#include "models/transmissionline/cable.h"
#include "models/transmissionline/line_cable.h"

/// \par OVERVIEW
///
/// This namespace is for the testing object factory. Data structs that are
/// constant and re-used used frequently will have build methods here.
namespace factory {

  /// \brief This function builds a cable.
  /// \return A cable.
  Cable* BuildCable();

  /// \brief This function builds a cable elongation model.
  /// \return A cable elongation model.
  CableElongationModel* BuildCableElongationModel(const SagTensionCable* cable);

  /// \brief This function builds a line cable.
  /// \return A line cable.
  LineCable* BuildLineCable();

  /// \brief This function builds a sag-tension cable.
  SagTensionCable* BuildSagTensionCable();

  /// \brief Destroys the linecable and any data referenced to it.
  void DestroyLineCable(LineCable* linecable);

  /// \brief Destroys the sag-tension cable and any data referenced to it.
  void DestroySagTensionCable(SagTensionCable* cable);

} // namespace factory

#endif  // OTLS_MODELS_TEST_FACTORY_H_
