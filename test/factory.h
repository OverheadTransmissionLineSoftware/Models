// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#ifndef OTLS_MODELS_TEST_FACTORY_H_
#define OTLS_MODELS_TEST_FACTORY_H_

#include "models/sagtension/cable_elongation_model.h"
#include "models/sagtension/sag_tension_cable.h"
#include "models/thermalrating/thermal_rating_cable.h"
#include "models/transmissionline/cable.h"
#include "models/transmissionline/hardware.h"
#include "models/transmissionline/line_cable.h"
#include "models/transmissionline/line_structure.h"
#include "models/transmissionline/structure.h"
#include "models/transmissionline/transmission_line.h"

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

  /// \brief This function builds a hardware assembly.
  /// \return A hardware assembly.
  Hardware* BuildHardware();

  /// \brief This function builds a line cable.
  /// \return A line cable.
  LineCable* BuildLineCable();

  /// \brief This function builds a line structure.
  /// \param[in] structure
  ///   The structure to be used in the line structure. If left as nullptr,
  ///    the factory will build its own structure.
  /// \return A line structure.
  LineStructure* BuildLineStructure(const Structure* structure = nullptr);

  /// \brief This function builds a sag-tension cable.
  SagTensionCable* BuildSagTensionCable();

  /// \brief This function builds a structure.
  /// \return A structure.
  Structure* BuildStructure();

  /// \brief This function builds a thermal rating cable.
  ThermalRatingCable* BuildThermalRatingCable();

  /// \brief This function builds a transmission line.
  /// \return A transmission line.
  TransmissionLine* BuildTransmissionLine();

  /// \brief Destroys the linecable and any data referenced by it.
  void DestroyLineCable(LineCable* linecable);

  /// \brief Destroys the line structure and any data referenced by it.
  void DestroyLineStructure(LineStructure* linestructure);

  /// \brief Destroys the sag-tension cable and any data referenced by it.
  void DestroySagTensionCable(SagTensionCable* cable);

} // namespace factory

#endif  // OTLS_MODELS_TEST_FACTORY_H_
