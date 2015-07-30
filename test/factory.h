// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef TLSLIBRARIES_TEST_FACTORY_H_
#define TLSLIBRARIES_TEST_FACTORY_H_

#include "transmissionline/cable.h"
#include "transmissionline/line_cable.h"

/// \par OVERVIEW
///
/// This namespace is for the testing object factory. Data structs that are
/// constant and re-used used frequently will have build methods here.
namespace factory {

  /// \brief This function builds a cable.
  Cable BuildCable();

  /// \brief This function builds a cable component.
  CableComponent BuildCableComponent();

  /// \brief This function builds a line cable.
  /// \return A line cable.
  LineCable BuildLineCable();

} // namespace factory

#endif  // TLSLIBRARIES_TEST_FACTORY_H_
