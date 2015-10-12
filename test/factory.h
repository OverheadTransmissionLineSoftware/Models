// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef OTLS_MODELS_TEST_FACTORY_H_
#define OTLS_MODELS_TEST_FACTORY_H_

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

  /// \brief This function builds a line cable.
  /// \return A line cable.
  LineCable* BuildLineCable();

  /// \brief This function builds a sag-tension cable.
  SagTensionCable* BuildSagTensionCable();

} // namespace factory

#endif  // OTLS_MODELS_TEST_FACTORY_H_
