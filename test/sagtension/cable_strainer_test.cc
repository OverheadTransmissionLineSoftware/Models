// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "sagtension/cable_strainer.h"

#include "base/convert_units.h"
#include "gtest/gtest.h"

class CableStrainerTest : public ::testing::Test {
 protected:
  CableStrainerTest() {

    // builds dependency object - cable
    const double kAreaPhysical = 0.7264;

    std::vector<double> coefficients_creep;
    std::vector<double> coefficients_loadstrain;

    coefficients_creep.push_back(47.1 * kAreaPhysical);
    coefficients_creep.push_back(36211.3 * kAreaPhysical);
    coefficients_creep.push_back(12201.4 * kAreaPhysical);
    coefficients_creep.push_back(-72392 * kAreaPhysical);
    coefficients_creep.push_back(46338 * kAreaPhysical);

    coefficients_loadstrain.push_back(-69.3 * kAreaPhysical);
    coefficients_loadstrain.push_back(38629 * kAreaPhysical);
    coefficients_loadstrain.push_back(3998.1 * kAreaPhysical);
    coefficients_loadstrain.push_back(-45713 * kAreaPhysical);
    coefficients_loadstrain.push_back(27892 * kAreaPhysical);

    CableComponent core;
    core.coefficient_expansion_linear_thermal = 0.0000064;
    core.coefficients_polynomial_creep = coefficients_creep;
    core.coefficients_polynomial_loadstrain = coefficients_loadstrain;
    core.load_limit_polynomial_creep = 22406 * kAreaPhysical;
    core.load_limit_polynomial_loadstrain = 19154 * kAreaPhysical;
    core.modulus_compression_elastic_area = 0 * kAreaPhysical * 100;
    core.modulus_tension_elastic_area = 37000 * kAreaPhysical * 100;

    coefficients_creep.clear();
    coefficients_creep.push_back(-544.8 * kAreaPhysical);
    coefficients_creep.push_back(21426.8 * kAreaPhysical);
    coefficients_creep.push_back(-18842.2 * kAreaPhysical);
    coefficients_creep.push_back(5495 * kAreaPhysical);
    coefficients_creep.push_back(0 * kAreaPhysical);

    coefficients_loadstrain.clear();
    coefficients_loadstrain.push_back(-1213 * kAreaPhysical);
    coefficients_loadstrain.push_back(44308.1 * kAreaPhysical);
    coefficients_loadstrain.push_back(-14004.4 * kAreaPhysical);
    coefficients_loadstrain.push_back(-37618 * kAreaPhysical);
    coefficients_loadstrain.push_back(30676 * kAreaPhysical);

    CableComponent shell;
    shell.coefficient_expansion_linear_thermal = 0.0000128;
    shell.coefficients_polynomial_creep = coefficients_creep;
    shell.coefficients_polynomial_loadstrain = coefficients_loadstrain;
    shell.load_limit_polynomial_creep = 7535 * kAreaPhysical;
    shell.load_limit_polynomial_loadstrain = 20252 * kAreaPhysical;
    shell.modulus_compression_elastic_area = 1500 * kAreaPhysical * 100;
    shell.modulus_tension_elastic_area = 64000 * kAreaPhysical * 100;

    Cable cable;
    cable.area_electrical = 795000;
    cable.area_physical = kAreaPhysical;
    cable.component_core = core;
    cable.component_shell = shell;
    cable.diameter = 1.108;
    cable.name = "ACSR Drake";
    cable.strength_rated = 31500;
    cable.temperature_properties_components = 70;
    cable.type_polynomial_active = CablePolynomialType::kLoadStrain;
    cable.weight_unit = 1.094;

    // builds dependency object - start state
    CableState state_start;
    state_start.load_stretch = 12000;
    state_start.temperature = 0;
    state_start.temperature_stretch = 0;

    // builds dependency object - finish state
    CableState state_finish;
    state_finish.load_stretch = 12000;
    state_finish.temperature = 212;
    state_finish.temperature_stretch = 0;

    // builds fixture object
    c_.set_cable(cable);
    c_.set_length_start(1200);
    c_.set_load_finish(10000);
    c_.set_load_start(0);
    c_.set_state_finish(state_finish);
    c_.set_state_start(state_start);
  }
  CableStrainer c_;
};

TEST_F(CableStrainerTest, LengthFinish) {

  // calculates the loaded length
  const double length = c_.LengthFinish();

  // switches unloaded and loaded states
  const CableState state1 = c_.state_start();
  const CableState state2 = c_.state_finish();

  c_.set_length_start(length);
  c_.set_load_finish(0);
  c_.set_load_start(10000);
  c_.set_state_start(state2);
  c_.set_state_finish(state1);

  // tests if original length is calculated
  EXPECT_EQ(1200.000, supportfunctions::Round(c_.LengthFinish(), 3));
}

TEST_F(CableStrainerTest, Validate) {

  EXPECT_EQ(true, c_.Validate(false, nullptr));
}
