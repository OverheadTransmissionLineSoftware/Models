// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "factory.h"

#include "models/base/units.h"

namespace factory {

/// This function converts the units that are typically used to describe the
/// value into consistent units for the library classes.
Cable* BuildCable() {
  Cable* cable = new Cable();
  CableComponent component;
  const double kAreaPhysical = units::ConvertLength(
      0.7264, units::LengthConversionType::kInchesToFeet, 2);
  std::vector<double> coefficients_creep;
  std::vector<double> coefficients_loadstrain;
  double c0, c1, c2, c3, c4;

  // sets entire cable mechanical properties
  cable->area_physical = kAreaPhysical;
  cable->diameter = units::ConvertLength(
      1.108, units::LengthConversionType::kInchesToFeet);
  cable->name = "DRAKE";
  cable->strength_rated = 31500;
  cable->temperature_properties_components = 70;
  cable->type_construction = "ASCR";
  cable->weight_unit = 1.094;

  // sets entire cable thermal properties
  cable->absorptivity = 0.8;
  cable->emissivity = 0.8;

  Cable::ResistancePoint point;
  point.temperature = units::ConvertTemperature(
      77, units::TemperatureConversionType::kFahrenheitToCelsius);
  point.resistance = units::ConvertLength(
      0.1166, units::LengthConversionType::kMilesToFeet, 1, false);
  cable->resistances_ac.push_back(point);

  point.temperature = units::ConvertTemperature(
      167, units::TemperatureConversionType::kFahrenheitToCelsius);
  point.resistance = units::ConvertLength(
      0.1390, units::LengthConversionType::kMilesToFeet, 1, false);
  cable->resistances_ac.push_back(point);

  // builds core component
  // converts lb/in^2 to lbs
  coefficients_creep.clear();
  c0 = units::ConvertStress(47.1, units::StressConversionType::kPsiToPsf)
           * kAreaPhysical;
  c1 = units::ConvertStress(36211.3, units::StressConversionType::kPsiToPsf)
           * kAreaPhysical;
  c2 = units::ConvertStress(12201.4, units::StressConversionType::kPsiToPsf)
           * kAreaPhysical;
  c3 = units::ConvertStress(-72392, units::StressConversionType::kPsiToPsf)
           * kAreaPhysical;
  c4 = units::ConvertStress(46338, units::StressConversionType::kPsiToPsf)
           * kAreaPhysical;
  coefficients_creep.push_back(c0);
  coefficients_creep.push_back(c1);
  coefficients_creep.push_back(c2);
  coefficients_creep.push_back(c3);
  coefficients_creep.push_back(c4);

  coefficients_loadstrain.clear();
  c0 = units::ConvertStress(-69.3, units::StressConversionType::kPsiToPsf)
           * kAreaPhysical;
  c1 = units::ConvertStress(38629, units::StressConversionType::kPsiToPsf)
           * kAreaPhysical;
  c2 = units::ConvertStress(3998.1, units::StressConversionType::kPsiToPsf)
           * kAreaPhysical;
  c3 = units::ConvertStress(-45713, units::StressConversionType::kPsiToPsf)
           * kAreaPhysical;
  c4 = units::ConvertStress(27892, units::StressConversionType::kPsiToPsf)
           * kAreaPhysical;
  coefficients_loadstrain.push_back(c0);
  coefficients_loadstrain.push_back(c1);
  coefficients_loadstrain.push_back(c2);
  coefficients_loadstrain.push_back(c3);
  coefficients_loadstrain.push_back(c4);

  component.capacity_heat = units::ConvertTemperature(
      41.316,
      units::TemperatureConversionType::kFahrenheitToCelsius,
      1, false, false);
  component.coefficient_expansion_linear_thermal = 0.0000064;
  component.coefficients_polynomial_creep = coefficients_creep;
  component.coefficients_polynomial_loadstrain = coefficients_loadstrain;
  component.load_limit_polynomial_creep =
      units::ConvertStress(22406, units::StressConversionType::kPsiToPsf)
           * kAreaPhysical;
  component.load_limit_polynomial_loadstrain =
      units::ConvertStress(19154, units::StressConversionType::kPsiToPsf)
           * kAreaPhysical;
  component.modulus_compression_elastic_area =
      units::ConvertStress(0 * 100, units::StressConversionType::kPsiToPsf)
           * kAreaPhysical;
  component.modulus_tension_elastic_area =
      units::ConvertStress(37000 * 100, units::StressConversionType::kPsiToPsf)
           * kAreaPhysical;

  cable->component_core = component;

  // builds shell component
  // converts lb/in^2 to lbs
  coefficients_creep.clear();
  c0 = units::ConvertStress(-544.8, units::StressConversionType::kPsiToPsf)
           * kAreaPhysical;
  c1 = units::ConvertStress(21426.8, units::StressConversionType::kPsiToPsf)
           * kAreaPhysical;
  c2 = units::ConvertStress(-18842.2, units::StressConversionType::kPsiToPsf)
           * kAreaPhysical;
  c3 = units::ConvertStress(5495, units::StressConversionType::kPsiToPsf)
           * kAreaPhysical;
  c4 = units::ConvertStress(0, units::StressConversionType::kPsiToPsf)
           * kAreaPhysical;
  coefficients_creep.push_back(c0);
  coefficients_creep.push_back(c1);
  coefficients_creep.push_back(c2);
  coefficients_creep.push_back(c3);
  coefficients_creep.push_back(c4);

  coefficients_loadstrain.clear();
  c0 = units::ConvertStress(-1213, units::StressConversionType::kPsiToPsf)
           * kAreaPhysical;
  c1 = units::ConvertStress(44308.1, units::StressConversionType::kPsiToPsf)
           * kAreaPhysical;
  c2 = units::ConvertStress(-14004.4, units::StressConversionType::kPsiToPsf)
           * kAreaPhysical;
  c3 = units::ConvertStress(-37618, units::StressConversionType::kPsiToPsf)
           * kAreaPhysical;
  c4 = units::ConvertStress(30676, units::StressConversionType::kPsiToPsf)
           * kAreaPhysical;
  coefficients_loadstrain.push_back(c0);
  coefficients_loadstrain.push_back(c1);
  coefficients_loadstrain.push_back(c2);
  coefficients_loadstrain.push_back(c3);
  coefficients_loadstrain.push_back(c4);

  component.capacity_heat = units::ConvertTemperature(
      180.203,
      units::TemperatureConversionType::kFahrenheitToCelsius,
      1, false, false);
  component.coefficient_expansion_linear_thermal = 0.0000128;
  component.coefficients_polynomial_creep = coefficients_creep;
  component.coefficients_polynomial_loadstrain = coefficients_loadstrain;
  component.load_limit_polynomial_creep =
      units::ConvertStress(7535, units::StressConversionType::kPsiToPsf)
           * kAreaPhysical;
  component.load_limit_polynomial_loadstrain =
      units::ConvertStress(20252, units::StressConversionType::kPsiToPsf)
           * kAreaPhysical;
  component.modulus_compression_elastic_area =
      units::ConvertStress(1500 * 100, units::StressConversionType::kPsiToPsf)
           * kAreaPhysical;
  component.modulus_tension_elastic_area =
      units::ConvertStress(64000 * 100, units::StressConversionType::kPsiToPsf)
           * kAreaPhysical;

  cable->component_shell = component;

  return cable;
}

CableElongationModel* BuildCableElongationModel(const SagTensionCable* cable) {
  CableElongationModel* model = new CableElongationModel();

  CableState state;
  state.temperature = *cable->temperature_properties_components();
  state.type_polynomial = SagTensionCableComponent::PolynomialType::kLoadStrain;

  CableStretchState state_stretch;
  state_stretch.load = 0;
  state_stretch.temperature = 0;
  state_stretch.type_polynomial =
      SagTensionCableComponent::PolynomialType::kLoadStrain;

  model->set_cable(cable);
  model->set_state(state);
  model->set_state_stretch(state_stretch);

  return model;
}

Hardware* BuildHardware() {
  Hardware* hardware = new Hardware();
  hardware->name = "DeadEnd";
  hardware->area_cross_section = 1.0;
  hardware->length = 10;
  hardware->type = Hardware::HardwareType::kDeadEnd;
  hardware->weight = 100;

  return hardware;
}

LineCable* BuildLineCable() {
  LineStructure* line_structure = nullptr;
  WeatherLoadCase* weathercase = nullptr;

  // builds constraint
  weathercase = new WeatherLoadCase();
  weathercase->description = "0-0-60";
  weathercase->thickness_ice = 0;
  weathercase->density_ice = 0;
  weathercase->pressure_wind = 0;
  weathercase->temperature_cable = 60;

  CableConstraint constraint;
  constraint.case_weather = weathercase;
  constraint.condition = CableConditionType::kInitial;
  constraint.limit = 6000;
  constraint.type_limit = CableConstraint::LimitType::kHorizontalTension;

  LineCable* line_cable = new LineCable();
  line_cable->set_cable(factory::BuildCable());
  line_cable->set_constraint(constraint);
  line_cable->set_spacing_attachments_ruling_span(Vector3d(1200, 0, 0));

  // builds creep stretch weathercase
  weathercase = new WeatherLoadCase();
  weathercase->description = "0-0-60";
  weathercase->thickness_ice = 0;
  weathercase->density_ice = 0;
  weathercase->pressure_wind = 0;
  weathercase->temperature_cable = 60;
  line_cable->set_weathercase_stretch_creep(weathercase);

  // builds load stretch weathercase
  weathercase = new WeatherLoadCase();
  weathercase->description = "0.5-8-0";
  weathercase->thickness_ice = units::ConvertLength(
      0.5, units::LengthConversionType::kInchesToFeet);
  weathercase->density_ice = 57.3;
  weathercase->pressure_wind = 8;
  weathercase->temperature_cable = 00;
  line_cable->set_weathercase_stretch_load(weathercase);

  // builds connections
  LineCableConnection connection;
  line_structure = factory::BuildLineStructure();
  line_structure->set_station(0);
  connection.line_structure = line_structure;
  connection.index_attachment = 0;
  line_cable->AddConnection(connection);

  connection = LineCableConnection();
  line_structure = factory::BuildLineStructure();
  line_structure->set_station(1000);
  connection.line_structure = line_structure;
  connection.index_attachment = 0;
  line_cable->AddConnection(connection);

  return line_cable;
}

LineStructure* BuildLineStructure(const Structure* structure) {
  // builds structure if necessary
  if (structure == nullptr) {
    structure = factory::BuildStructure();
  }

  // sets line placement and orientation
  LineStructure* line_structure = new LineStructure();
  line_structure->set_height_adjustment(0);
  line_structure->set_offset(0);
  line_structure->set_rotation(0);
  line_structure->set_station(0);
  line_structure->set_structure(structure);

  // adds hardware
  line_structure->AttachHardware(0, factory::BuildHardware());
  line_structure->AttachHardware(2, factory::BuildHardware());

  return line_structure;
}

SagTensionCable* BuildSagTensionCable() {
  Cable* cable = BuildCable();
  SagTensionCable* cable_sagtension = new SagTensionCable();
  cable_sagtension->set_cable_base(cable);

  return cable_sagtension;
}

Structure* BuildStructure() {
  StructureAttachment attachment;

  Structure* structure = new Structure();
  structure->name = "POLE - EXAMPLE";
  structure->height = 100;

  // adds attachment points
  attachment = StructureAttachment();
  attachment.offset_longitudinal = -1;
  attachment.offset_transverse = 0;
  attachment.offset_vertical_top = 0;
  structure->attachments.push_back(attachment);

  attachment = StructureAttachment();
  attachment.offset_longitudinal = 0;
  attachment.offset_transverse = 0;
  attachment.offset_vertical_top = 0;
  structure->attachments.push_back(attachment);

  attachment = StructureAttachment();
  attachment.offset_longitudinal = 1;
  attachment.offset_transverse = 0;
  attachment.offset_vertical_top = 0;
  structure->attachments.push_back(attachment);

  return structure;
}

ThermalRatingCable* BuildThermalRatingCable() {
  Cable* cable = BuildCable();
  ThermalRatingCable* cable_thermalrating = new ThermalRatingCable();
  cable_thermalrating->set_cable_base(cable);

  return cable_thermalrating;
}

TransmissionLine* BuildTransmissionLine() {
  TransmissionLine* line = new TransmissionLine();
  line->set_origin(Point3d<double>(0, 0, 0));

  // adds alignment points
  AlignmentPoint point;

  point = AlignmentPoint();
  point.elevation = 0;
  point.rotation = 0;
  point.station = 0;
  line->AddAlignmentPoint(point);

  point = AlignmentPoint();
  point.elevation = 0;
  point.rotation = 90;
  point.station = 1000;
  line->AddAlignmentPoint(point);

  point = AlignmentPoint();
  point.elevation = 0;
  point.rotation = -90;
  point.station = 2000;
  line->AddAlignmentPoint(point);

  point = AlignmentPoint();
  point.elevation = 100;
  point.rotation = 90;
  point.station = 3000;
  line->AddAlignmentPoint(point);

  point = AlignmentPoint();
  point.elevation = 0;
  point.rotation = 0;
  point.station = 4000;
  line->AddAlignmentPoint(point);

  // adds line structures
  Hardware* hardware_deadend = factory::BuildHardware();
  hardware_deadend->type = Hardware::HardwareType::kDeadEnd;

  Hardware* hardware_suspension = factory::BuildHardware();
  hardware_suspension->type = Hardware::HardwareType::kSuspension;

  LineStructure line_structure;

  line_structure = *factory::BuildLineStructure();
  line_structure.set_station(0);
  line_structure.DetachHardware(0);
  line_structure.DetachHardware(1);
  line_structure.DetachHardware(2);
  line_structure.AttachHardware(2, hardware_deadend);
  line->AddLineStructure(line_structure);

  line_structure = *factory::BuildLineStructure();
  line_structure.set_station(1000);
  line_structure.DetachHardware(0);
  line_structure.DetachHardware(1);
  line_structure.DetachHardware(2);
  line_structure.AttachHardware(0, hardware_deadend);
  line_structure.AttachHardware(2, hardware_deadend);
  line->AddLineStructure(line_structure);

  line_structure = *factory::BuildLineStructure();
  line_structure.set_station(2000);
  line_structure.DetachHardware(0);
  line_structure.DetachHardware(1);
  line_structure.DetachHardware(2);
  line_structure.AttachHardware(0, hardware_deadend);
  line_structure.AttachHardware(2, hardware_deadend);
  line->AddLineStructure(line_structure);

  line_structure = *factory::BuildLineStructure();
  line_structure.set_station(3000);
  line_structure.DetachHardware(0);
  line_structure.DetachHardware(1);
  line_structure.DetachHardware(2);
  line_structure.AttachHardware(1, hardware_suspension);
  line->AddLineStructure(line_structure);

  line_structure = *factory::BuildLineStructure();
  line_structure.set_station(4000);
  line_structure.DetachHardware(0);
  line_structure.DetachHardware(1);
  line_structure.DetachHardware(2);
  line_structure.AttachHardware(0, hardware_deadend);
  line->AddLineStructure(line_structure);

  // adds line cables
  LineCable line_cable;
  LineCableConnection connection;
  const std::list<LineStructure>* line_structures = line->line_structures();

  line_cable = *factory::BuildLineCable();
  line_cable.ClearConnections();
  connection.line_structure = &(*std::next(line_structures->cbegin(), 0));
  connection.index_attachment = 2;
  line_cable.AddConnection(connection);
  connection.line_structure = &(*std::next(line_structures->cbegin(), 1));
  connection.index_attachment = 0;
  line_cable.AddConnection(connection);
  line->AddLineCable(line_cable);

  line_cable = *factory::BuildLineCable();
  line_cable.ClearConnections();
  connection.line_structure = &(*std::next(line_structures->cbegin(), 1));
  connection.index_attachment = 2;
  line_cable.AddConnection(connection);
  connection.line_structure = &(*std::next(line_structures->cbegin(), 2));
  connection.index_attachment = 0;
  line_cable.AddConnection(connection);
  line->AddLineCable(line_cable);

  line_cable = *factory::BuildLineCable();
  line_cable.ClearConnections();
  connection.line_structure = &(*std::next(line_structures->cbegin(), 2));
  connection.index_attachment = 2;
  line_cable.AddConnection(connection);
  connection.line_structure = &(*std::next(line_structures->cbegin(), 3));
  connection.index_attachment = 1;
  line_cable.AddConnection(connection);
  connection.line_structure = &(*std::next(line_structures->cbegin(), 4));
  connection.index_attachment = 0;
  line_cable.AddConnection(connection);
  line->AddLineCable(line_cable);

  return line;
}

void DestroyLineCable(LineCable* linecable) {
  delete linecable->cable();
  delete linecable->constraint().case_weather;
  delete linecable->weathercase_stretch_creep();
  delete linecable->weathercase_stretch_load();

  delete linecable;
}

void DestroyLineStructure(LineStructure* linestructure) {
  delete linestructure->structure();

  delete linestructure;
}

void DestroySagTensionCable(SagTensionCable* cable) {
  delete cable->cable_base();

  delete cable;
}

}  // namespace factory
