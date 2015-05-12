// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "sagtension/cable_component_elongation_model.h"

#include <cmath>
#include <vector>

#include "gtest/gtest.h"

class CableComponentElongationModelTest : public ::testing::Test {
 protected:
  CableComponentElongationModelTest() {

    // builds dependency object - component
    const double area_physical = 0.7264;

    std::vector<double> coefficients_creep(5, 0);
    coefficients_creep.push_back(-544.8 * area_physical);
    coefficients_creep.push_back(21426.8 * area_physical);
    coefficients_creep.push_back(-18842.2 * area_physical);
    coefficients_creep.push_back(5495 * area_physical);
    coefficients_creep.push_back(0 * area_physical);

    std::vector<double> coefficients_loadstrain(5, 0);
    coefficients_loadstrain.push_back(-1213 * area_physical);
    coefficients_loadstrain.push_back(44308.1 * area_physical);
    coefficients_loadstrain.push_back(-14004.4 * area_physical);
    coefficients_loadstrain.push_back(-37618 * area_physical);
    coefficients_loadstrain.push_back(30676 * area_physical);

    CableComponent shell;
    shell.coefficient_expansion_linear_thermal = 0.0000128;
    shell.coefficients_polynomial_creep = coefficients_creep;
    shell.coefficients_polynomial_loadstrain = coefficients_loadstrain;
    shell.load_limit_polynomial_creep = 7535 * area_physical;
    shell.load_limit_polynomial_loadstrain = 20252 * area_physical;
    shell.modulus_compression_elastic_area = 1500 * area_physical * 100;
    shell.modulus_tension_elastic_area = 64000 * area_physical * 100;

    // builds fixture object
    c_.set_component_cable(shell);
    c_.set_load_stretch(5000);
    c_.set_temperature(70);
    c_.set_temperature_reference(70);
    c_.set_type_polynomial_active(CablePolynomialType::kLoadStrain);
  }

  CableComponentElongationModel c_;
};

TEST_F(CableComponentElongationModelTest, Load) {

  EXPECT_EQ(-211.1, std::round(c_.Load(-0.001)));

}
