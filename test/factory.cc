// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "../test/factory.h"

#include "base/units.h"

namespace factory {

/// This function converts the units that are typically used to describe the
/// value into consistent units for the library classes.
Cable BuildCable() {

  Cable cable;
  CableComponent component;
  const double kAreaPhysical = units::Convert(
      0.7264, units::ConversionType::kInchesToFeet, 2);
  std::vector<double> coefficients_creep;
  std::vector<double> coefficients_loadstrain;
  double c0, c1, c2, c3, c4;

  // builds core component
  // converts lb/in^2 to lbs
  coefficients_creep.clear();
  c0 = units::Convert(47.1, units::ConversionType::kInchesToFeet,
                      2, false) * kAreaPhysical;
  c1 = units::Convert(36211.3, units::ConversionType::kInchesToFeet,
                      2, false) * kAreaPhysical;
  c2 = units::Convert(12201.4, units::ConversionType::kInchesToFeet,
                      2, false) * kAreaPhysical;
  c3 = units::Convert(-72392, units::ConversionType::kInchesToFeet,
                      2, false) * kAreaPhysical;
  c4 = units::Convert(46338, units::ConversionType::kInchesToFeet,
                      2, false) * kAreaPhysical;
  coefficients_creep.push_back(c0);
  coefficients_creep.push_back(c1);
  coefficients_creep.push_back(c2);
  coefficients_creep.push_back(c3);
  coefficients_creep.push_back(c4);

  coefficients_loadstrain.clear();
  c0 = units::Convert(-69.3, units::ConversionType::kInchesToFeet,
                      2, false) * kAreaPhysical;
  c1 = units::Convert(38629, units::ConversionType::kInchesToFeet,
                      2, false) * kAreaPhysical;
  c2 = units::Convert(3998.1, units::ConversionType::kInchesToFeet,
                      2, false) * kAreaPhysical;
  c3 = units::Convert(-45713, units::ConversionType::kInchesToFeet,
                      2, false) * kAreaPhysical;
  c4 = units::Convert(27892, units::ConversionType::kInchesToFeet,
                      2, false) * kAreaPhysical;
  coefficients_loadstrain.push_back(c0);
  coefficients_loadstrain.push_back(c1);
  coefficients_loadstrain.push_back(c2);
  coefficients_loadstrain.push_back(c3);
  coefficients_loadstrain.push_back(c4);

  component.coefficient_expansion_linear_thermal = 0.0000064;
  component.coefficients_polynomial_creep = coefficients_creep;
  component.coefficients_polynomial_loadstrain = coefficients_loadstrain;
  component.load_limit_polynomial_creep =
      units::Convert(22406, units::ConversionType::kInchesToFeet,
                     2, false) * kAreaPhysical;
  component.load_limit_polynomial_loadstrain =
      units::Convert(19154, units::ConversionType::kInchesToFeet,
                     2, false) * kAreaPhysical;
  component.modulus_compression_elastic_area =
      units::Convert(0 * 100, units::ConversionType::kInchesToFeet,
                     2, false) * kAreaPhysical;
  component.modulus_tension_elastic_area =
      units::Convert(37000 * 100, units::ConversionType::kInchesToFeet,
                     2, false) * kAreaPhysical;

  cable.component_core = component;

  // builds shell component
  // converts lb/in^2 to lbs
  coefficients_creep.clear();
  c0 = units::Convert(-544.8, units::ConversionType::kInchesToFeet,
                      2, false) * kAreaPhysical;
  c1 = units::Convert(21426.8, units::ConversionType::kInchesToFeet,
                      2, false) * kAreaPhysical;
  c2 = units::Convert(-18842.2, units::ConversionType::kInchesToFeet,
                      2, false) * kAreaPhysical;
  c3 = units::Convert(5495, units::ConversionType::kInchesToFeet,
                      2, false) * kAreaPhysical;
  c4 = units::Convert(0, units::ConversionType::kInchesToFeet,
                      2, false) * kAreaPhysical;
  coefficients_creep.push_back(c0);
  coefficients_creep.push_back(c1);
  coefficients_creep.push_back(c2);
  coefficients_creep.push_back(c3);
  coefficients_creep.push_back(c4);

  coefficients_loadstrain.clear();
  c0 = units::Convert(-1213, units::ConversionType::kInchesToFeet,
                      2, false) * kAreaPhysical;
  c1 = units::Convert(44308.1, units::ConversionType::kInchesToFeet,
                      2, false) * kAreaPhysical;
  c2 = units::Convert(-14004.4, units::ConversionType::kInchesToFeet,
                      2, false) * kAreaPhysical;
  c3 = units::Convert(-37618, units::ConversionType::kInchesToFeet,
                      2, false) * kAreaPhysical;
  c4 = units::Convert(30676, units::ConversionType::kInchesToFeet,
                      2, false) * kAreaPhysical;
  coefficients_loadstrain.push_back(c0);
  coefficients_loadstrain.push_back(c1);
  coefficients_loadstrain.push_back(c2);
  coefficients_loadstrain.push_back(c3);
  coefficients_loadstrain.push_back(c4);

  component.coefficient_expansion_linear_thermal = 0.0000128;
  component.coefficients_polynomial_creep = coefficients_creep;
  component.coefficients_polynomial_loadstrain = coefficients_loadstrain;
  component.load_limit_polynomial_creep =
      units::Convert(7535, units::ConversionType::kInchesToFeet,
                     2, false) * kAreaPhysical;
  component.load_limit_polynomial_loadstrain =
      units::Convert(20252, units::ConversionType::kInchesToFeet,
                     2, false) * kAreaPhysical;
  component.modulus_compression_elastic_area =
      units::Convert(1500 * 100, units::ConversionType::kInchesToFeet,
                     2, false) * kAreaPhysical;
  component.modulus_tension_elastic_area =
      units::Convert(64000 * 100, units::ConversionType::kInchesToFeet,
                     2, false) * kAreaPhysical;

  cable.component_shell = component;

  // finishes building cable
  cable.area_electrical = 795000;
  cable.area_physical = kAreaPhysical;
  cable.diameter = units::Convert(1.108,
                                  units::ConversionType::kInchesToFeet);
  cable.name = "DRAKE";
  cable.strength_rated = 31500;
  cable.temperature_properties_components = 70;
  cable.type_construction = "ASCR";
  cable.type_polynomial_active = CableComponent::PolynomialType::kLoadStrain;
  cable.weight_unit = 1.094;

  return cable;
}

CableComponent BuildCableComponent() {

  CableComponent component;
  const double kAreaPhysical = units::Convert(
      0.7264, units::ConversionType::kInchesToFeet, 2);
  std::vector<double> coefficients_creep;
  std::vector<double> coefficients_loadstrain;
  double c0, c1, c2, c3, c4;

  coefficients_creep.clear();
  c0 = units::Convert(-544.8, units::ConversionType::kInchesToFeet,
                      2, false) * kAreaPhysical;
  c1 = units::Convert(21426.8, units::ConversionType::kInchesToFeet,
                      2, false) * kAreaPhysical;
  c2 = units::Convert(-18842.2, units::ConversionType::kInchesToFeet,
                      2, false) * kAreaPhysical;
  c3 = units::Convert(5495, units::ConversionType::kInchesToFeet,
                      2, false) * kAreaPhysical;
  c4 = units::Convert(0, units::ConversionType::kInchesToFeet,
                      2, false) * kAreaPhysical;
  coefficients_creep.push_back(c0);
  coefficients_creep.push_back(c1);
  coefficients_creep.push_back(c2);
  coefficients_creep.push_back(c3);
  coefficients_creep.push_back(c4);

  coefficients_loadstrain.clear();
  c0 = units::Convert(-1213, units::ConversionType::kInchesToFeet,
                      2, false) * kAreaPhysical;
  c1 = units::Convert(44308.1, units::ConversionType::kInchesToFeet,
                      2, false) * kAreaPhysical;
  c2 = units::Convert(-14004.4, units::ConversionType::kInchesToFeet,
                      2, false) * kAreaPhysical;
  c3 = units::Convert(-37618, units::ConversionType::kInchesToFeet,
                      2, false) * kAreaPhysical;
  c4 = units::Convert(30676, units::ConversionType::kInchesToFeet,
                      2, false) * kAreaPhysical;
  coefficients_loadstrain.push_back(c0);
  coefficients_loadstrain.push_back(c1);
  coefficients_loadstrain.push_back(c2);
  coefficients_loadstrain.push_back(c3);
  coefficients_loadstrain.push_back(c4);

  component.coefficient_expansion_linear_thermal = 0.0000128;
  component.coefficients_polynomial_creep = coefficients_creep;
  component.coefficients_polynomial_loadstrain = coefficients_loadstrain;
  component.load_limit_polynomial_creep =
      units::Convert(7535, units::ConversionType::kInchesToFeet,
                     2, false) * kAreaPhysical;
  component.load_limit_polynomial_loadstrain =
      units::Convert(20252, units::ConversionType::kInchesToFeet,
                     2, false) * kAreaPhysical;
  component.modulus_compression_elastic_area =
      units::Convert(1500 * 100, units::ConversionType::kInchesToFeet,
                     2, false) * kAreaPhysical;
  component.modulus_tension_elastic_area =
      units::Convert(64000 * 100, units::ConversionType::kInchesToFeet,
                     2, false) * kAreaPhysical;

  return component;
}

}  // namespace factory
