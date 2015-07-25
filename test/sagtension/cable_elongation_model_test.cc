// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "sagtension/cable_elongation_model.h"

#include "base/helper.h"
#include "gtest/gtest.h"

class CableElongationModelTest : public ::testing::Test {
 protected:
  CableElongationModelTest() {

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
    cable.type_polynomial_active = CableComponent::PolynomialType::kLoadStrain;
    cable.weight_unit = 1.094;

    // builds dependency object - state
    CableState state;
    state.load_stretch = 12000;
    state.temperature = 70;
    state.temperature_stretch = 0;

    // builds fixture object
    c_.set_cable(cable);
    c_.set_state(state);
  }

  CableElongationModel c_;
};

TEST_F(CableElongationModelTest, Load) {

  // core
  EXPECT_EQ(5433.5, helper::Round(
      c_.Load(CableElongationModel::ComponentType::kCore, 0.002), 1));

  // shell
  EXPECT_EQ(3754.9, helper::Round(
      c_.Load(CableElongationModel::ComponentType::kShell, 0.002), 1));

  // combined
  EXPECT_EQ(9188.4, helper::Round(
      c_.Load(CableElongationModel::ComponentType::kCombined, 0.002), 1));
}

TEST_F(CableElongationModelTest, Slope) {

  // core
  EXPECT_EQ(2687680, helper::Round(
      c_.Slope(CableElongationModel::ComponentType::kCore, 0.002), 0));

  // shell
  EXPECT_EQ(4648960, helper::Round(
      c_.Slope(CableElongationModel::ComponentType::kShell, 0.002), 0));

  // combined
  EXPECT_EQ(7336640, helper::Round(
      c_.Slope(CableElongationModel::ComponentType::kCombined, 0.002), 0));
}

TEST_F(CableElongationModelTest, Strain) {

  // core
  EXPECT_EQ(0.002, helper::Round(
      c_.Strain(CableElongationModel::ComponentType::kCore, 5433.5), 3));

  // shell
  EXPECT_EQ(0.002, helper::Round(
      c_.Strain(CableElongationModel::ComponentType::kShell, 3754.9), 3));

  // combined
  EXPECT_EQ(0.002, helper::Round(
      c_.Strain(CableElongationModel::ComponentType::kCombined, 9188.4), 3));
}

TEST_F(CableElongationModelTest, Validate) {

  EXPECT_TRUE(c_.Validate(false, nullptr));
}
