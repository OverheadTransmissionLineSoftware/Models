// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "transmissionline/cable.h"

#include "gtest/gtest.h"

class CableComponentTest : public ::testing::Test {
 protected:
  CableComponentTest() {

    const double kAreaPhysical = 0.7264;

    // builds dependency object - creep coefficients
    std::vector<double> coefficients_creep;
    coefficients_creep.push_back(-544.8 * kAreaPhysical);
    coefficients_creep.push_back(21426.8 * kAreaPhysical);
    coefficients_creep.push_back(-18842.2 * kAreaPhysical);
    coefficients_creep.push_back(5495 * kAreaPhysical);
    coefficients_creep.push_back(0 * kAreaPhysical);

    // builds dependency object - loadstrain coefficients
    std::vector<double> coefficients_loadstrain;
    coefficients_loadstrain.push_back(-1213 * kAreaPhysical);
    coefficients_loadstrain.push_back(44308.1 * kAreaPhysical);
    coefficients_loadstrain.push_back(-14004.4 * kAreaPhysical);
    coefficients_loadstrain.push_back(-37618 * kAreaPhysical);
    coefficients_loadstrain.push_back(30676 * kAreaPhysical);

    // builds fixture object
    c_.coefficient_expansion_linear_thermal = 0.0000128;
    c_.coefficients_polynomial_creep = coefficients_creep;
    c_.coefficients_polynomial_loadstrain = coefficients_loadstrain;
    c_.modulus_compression_elastic_area = 0 * kAreaPhysical * 100;
    c_.modulus_tension_elastic_area = 64000 * kAreaPhysical * 100;
    c_.load_limit_polynomial_creep = 7535 * kAreaPhysical;
    c_.load_limit_polynomial_loadstrain = 16816 * kAreaPhysical;
  }

  CableComponent c_;
};

TEST_F(CableComponentTest, Validate) {

  EXPECT_TRUE(c_.Validate(true, nullptr));
}

class CableTest : public ::testing::Test {
 protected:
  CableTest() {

    const double kAreaPhysical = 0.7264;
    std::vector<double> coefficients_creep;
    std::vector<double> coefficients_loadstrain;

    // builds dependency object - core
    coefficients_creep.clear();
    coefficients_creep.push_back(47.1 * kAreaPhysical);
    coefficients_creep.push_back(36211.3 * kAreaPhysical);
    coefficients_creep.push_back(12201.4 * kAreaPhysical);
    coefficients_creep.push_back(-72392 * kAreaPhysical);
    coefficients_creep.push_back(46338 * kAreaPhysical);

    coefficients_loadstrain.clear();
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

    // builds dependency object - shell
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

    // builds fixture object
    c_.area_electrical = 795000;
    c_.area_physical = kAreaPhysical;
    c_.component_core = core;
    c_.component_shell = shell;
    c_.diameter = 1.108;
    c_.name = "DRAKE";
    c_.strength_rated = 31500;
    c_.temperature_properties_components = 70;
    c_.type_construction = "ASCR";
    c_.type_polynomial_active = CablePolynomialType::kLoadStrain;
    c_.weight_unit = 1.094;
  }

  Cable c_;
};

TEST_F(CableTest, Validate) {

  EXPECT_TRUE(c_.Validate(true, nullptr));
}
