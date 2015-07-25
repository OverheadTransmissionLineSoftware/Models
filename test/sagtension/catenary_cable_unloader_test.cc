// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "sagtension/catenary_cable_unloader.h"

#include "base/helper.h"
#include "gtest/gtest.h"

class CatenaryCableUnloaderTest : public ::testing::Test {
 protected:
  CatenaryCableUnloaderTest() {

    // builds dependency object - catenary cable
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
    shell.modulus_compression_elastic_area = 0 * kAreaPhysical * 100;
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

    Vector3d spacing_endpoints(1200, 0, 0);

    CableState state;
    state.load_stretch = 0;
    state.temperature = 60;
    state.temperature_stretch = 0;

    Vector3d weight_unit(0, 0, 1.094);

    CatenaryCable catenary_cable;
    catenary_cable.set_cable(cable);
    catenary_cable.set_spacing_endpoints(spacing_endpoints);
    catenary_cable.set_state(state);
    catenary_cable.set_tension_horizontal(6000);
    catenary_cable.set_weight_unit(weight_unit);

    // builds dependency object - unloaded state
    CableState state_unloaded;
    state_unloaded.load_stretch = 0;
    state_unloaded.temperature = 32;
    state_unloaded.temperature_stretch = 0;

    // builds fixture object
    c_.set_catenary_cable(catenary_cable);
    c_.set_state_unloaded(state_unloaded);
  }

  CatenaryCableUnloader c_;
};

TEST_F(CatenaryCableUnloaderTest, LengthUnloaded) {

  CatenaryCable catenary_cable = c_.catenary_cable();
  CableState state = catenary_cable.state();

  // unstretched catenary cable state
  EXPECT_EQ(1200.8179, helper::Round(c_.LengthUnloaded(), 4));

  // stretched catenary cable state
  state.load_stretch = 12179.2;
  catenary_cable.set_state(state);
  c_.set_catenary_cable(catenary_cable);
  EXPECT_EQ(1200.3543, helper::Round(c_.LengthUnloaded(), 4));
}

TEST_F(CatenaryCableUnloaderTest, Validate) {

  EXPECT_TRUE(c_.Validate(false, nullptr));
}
