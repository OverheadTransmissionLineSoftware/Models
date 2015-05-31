// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "sagtension/catenary_cable_reloader.h"

#include "base/convert_units.h"
#include "gtest/gtest.h"
class CatenaryCableReloaderTest : public ::testing::Test {
 protected:
  CatenaryCableReloaderTest() {

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

    // builds dependency object - reloaded state
    CableState state_reloaded;
    state_reloaded.load_stretch = 0;
    state_reloaded.temperature = 60;
    state_reloaded.temperature_stretch = 0;

    // builds dependency object - reloaded unit weight
    Vector3d weight_unit_reloaded(0, 0, 1.094);

    // builds fixture object
    c_.set_catenary_cable(catenary_cable);
    c_.set_state_reloaded(state_reloaded);
    c_.set_weight_unit_reloaded(weight_unit_reloaded);
  }

  CatenaryCableReloader c_;
};

TEST_F(CatenaryCableReloaderTest, LengthUnloadedUnstretched) {

  // unstretched original catenary cable
  EXPECT_EQ(1200.8179,
            supportfunctions::Round(c_.LengthUnloadedUnstretched(), 4));
}

TEST_F(CatenaryCableReloaderTest, CatenaryCableReloaded) {

  CatenaryCable catenary_cable;
  CableState state = c_.state_reloaded();
  Vector3d weight_unit = c_.weight_unit_reloaded();

  // nothing is modified - original and reloaded parameters are equal
  catenary_cable = c_.CatenaryCableReloaded();
  EXPECT_EQ(6000,
            supportfunctions::Round(catenary_cable.tension_horizontal(), 1));

  // tests temperature changes
  state.temperature = 0;
  c_.set_state_reloaded(state);
  catenary_cable = c_.CatenaryCableReloaded();
  EXPECT_EQ(6787.6,
            supportfunctions::Round(catenary_cable.tension_horizontal(), 1));

  state.temperature = 212;
  c_.set_state_reloaded(state);
  catenary_cable = c_.CatenaryCableReloaded();
  EXPECT_EQ(4702.2,
            supportfunctions::Round(catenary_cable.tension_horizontal(), 1));

  // changes temperature to zero and tests unit weight changes
  state.temperature = 0;
  c_.set_state_reloaded(state);

  weight_unit.set_y(2.072);
  weight_unit.set_z(3.729);
  c_.set_weight_unit_reloaded(weight_unit);
  catenary_cable = c_.CatenaryCableReloaded();
  EXPECT_EQ(17125.9,
            supportfunctions::Round(catenary_cable.tension_horizontal(), 1));

  weight_unit.set_y(1.405);
  weight_unit.set_z(2.099);
  c_.set_weight_unit_reloaded(weight_unit);
  catenary_cable = c_.CatenaryCableReloaded();
  EXPECT_EQ(12146.8,
            supportfunctions::Round(catenary_cable.tension_horizontal(), 1));

  // sets stretch load and tests all above cases again
  const double kLoadStretch =
      supportfunctions::Round(catenary_cable.TensionAverage(100), 1);
  EXPECT_EQ(12179, kLoadStretch);
  state.load_stretch = kLoadStretch;
  c_.set_state_reloaded(state);

  state.temperature = 60;
  c_.set_state_reloaded(state);
  weight_unit.set_y(0);
  weight_unit.set_z(1.094);
  c_.set_weight_unit_reloaded(weight_unit);
  catenary_cable = c_.CatenaryCableReloaded();
  EXPECT_EQ(5561.5,
            supportfunctions::Round(catenary_cable.tension_horizontal(), 1));

  state.temperature = 0;
  c_.set_state_reloaded(state);
  catenary_cable = c_.CatenaryCableReloaded();
  EXPECT_EQ(6320.1,
            supportfunctions::Round(catenary_cable.tension_horizontal(), 1));

  state.temperature = 212;
  c_.set_state_reloaded(state);
  catenary_cable = c_.CatenaryCableReloaded();
  EXPECT_EQ(4537.2,
            supportfunctions::Round(catenary_cable.tension_horizontal(), 1));

  state.temperature = 0;
  c_.set_state_reloaded(state);
  weight_unit.set_y(2.072);
  weight_unit.set_z(3.729);
  c_.set_weight_unit_reloaded(weight_unit);
  catenary_cable = c_.CatenaryCableReloaded();
  EXPECT_EQ(17126,
            supportfunctions::Round(catenary_cable.tension_horizontal(), 1));

  weight_unit.set_y(1.405);
  weight_unit.set_z(2.099);
  c_.set_weight_unit_reloaded(weight_unit);
  catenary_cable = c_.CatenaryCableReloaded();
  EXPECT_EQ(12147,
            supportfunctions::Round(catenary_cable.tension_horizontal(), 1));
}

TEST_F(CatenaryCableReloaderTest, Validate) {

  EXPECT_TRUE(c_.Validate(false, nullptr));
}
