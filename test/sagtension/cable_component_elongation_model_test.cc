// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/sagtension/cable_component_elongation_model.h"

#include "gtest/gtest.h"

#include "factory.h"
#include "models/base/helper.h"

class CableComponentElongationModelTest : public ::testing::Test {
 protected:
  CableComponentElongationModelTest() {
    const SagTensionCable* cable = factory::BuildSagTensionCable();
    const SagTensionCableComponent* component = cable->component_shell();

    double* temperature = new double;
    *temperature = 70;
    SagTensionCableComponent::PolynomialType* type_polynomial =
        new SagTensionCableComponent::PolynomialType();
    *type_polynomial = SagTensionCableComponent::PolynomialType::kLoadStrain;

    // builds fixture object
    c_.set_component_cable(component);
    c_.set_load_stretch(5000);
    c_.set_temperature(temperature);
    c_.set_temperature_reference(cable->temperature_properties_components());
    c_.set_type_polynomial_active(type_polynomial);
  }

  CableComponentElongationModel c_;
};

TEST_F(CableComponentElongationModelTest, Load) {
  double value = -999999;

  // compressed region
  value = c_.Load(-0.001);
  EXPECT_EQ(-211.1, helper::Round(value, 1));

  // stretched region
  value = c_.Load(0.001);
  EXPECT_EQ(289.3, helper::Round(value, 1));
  value = c_.Load(0.002);
  EXPECT_EQ(4938.2, helper::Round(value, 1));

  // polynomial region
  value = c_.Load(0.003);
  EXPECT_EQ(7301.6, helper::Round(value, 1));
  value = c_.Load(0.004);
  EXPECT_EQ(9187, helper::Round(value, 1));
  value = c_.Load(0.005);
  EXPECT_EQ(10645.4, helper::Round(value, 1));

  // extrapolated region
  value = c_.Load(0.010);
  EXPECT_EQ(18756.6, helper::Round(value, 1));
}

TEST_F(CableComponentElongationModelTest, PointPolynomialEnd) {
  Point2d p = c_.PointPolynomialEnd();
  EXPECT_EQ(0.0091, helper::Round(p.x, 4));
  EXPECT_EQ(14711.1, helper::Round(p.y, 1));
}

TEST_F(CableComponentElongationModelTest, PointPolynomialStart) {
  Point2d p = c_.PointPolynomialStart();
  EXPECT_EQ(0.0020, helper::Round(p.x, 4));
  EXPECT_EQ(5000, helper::Round(p.y, 1));
}

TEST_F(CableComponentElongationModelTest, PointsRegions) {
  std::vector<Point2d> points = c_.PointsRegions();
  Point2d p;

  p = points.at(0);
  EXPECT_EQ(0.000938, helper::Round(p.x, 6));
  EXPECT_EQ(0, helper::Round(p.y, 1));

  p = points.at(1);
  EXPECT_EQ(0.0020, helper::Round(p.x, 4));
  EXPECT_EQ(5000, helper::Round(p.y, 1));

  p = points.at(2);
  EXPECT_EQ(0.0091, helper::Round(p.x, 4));
  EXPECT_EQ(14711.1, helper::Round(p.y, 1));
}

TEST_F(CableComponentElongationModelTest, PointUnloaded) {
  Point2d p = c_.PointUnloaded();
  EXPECT_EQ(0.000938, helper::Round(p.x, 6));
  EXPECT_EQ(0, helper::Round(p.y, 1));
}

TEST_F(CableComponentElongationModelTest, Slope) {
  double value = -999999;

  // compressed region
  value = c_.Slope(-0.001);
  EXPECT_EQ(108960, helper::Round(value, 0));

  // stretched region
  value = c_.Slope(0.001);
  EXPECT_EQ(4648960, helper::Round(value, 0));
  value = c_.Slope(0.002);
  EXPECT_EQ(4648960, helper::Round(value, 0));

  // polynomial region
  value = c_.Slope(0.003);
  EXPECT_EQ(2111035, helper::Round(value, 0));
  value = c_.Slope(0.004);
  EXPECT_EQ(1663528, helper::Round(value, 0));
  value = c_.Slope(0.005);
  EXPECT_EQ(1265984, helper::Round(value, 0));

  // extrapolated region
  value = c_.Slope(0.010);
  EXPECT_EQ(4648960, helper::Round(value, 0));
}

TEST_F(CableComponentElongationModelTest, Strain) {
  double value = -999999;

  // compressed region
  value = c_.Strain(-211.1);
  EXPECT_EQ(-0.001, helper::Round(value, 3));

  // stretched region
  value = c_.Strain(289.3);
  EXPECT_EQ(0.001, helper::Round(value, 3));
  value = c_.Strain(4938.2);
  EXPECT_EQ(0.002, helper::Round(value, 3));

  // polynomial region
  value = c_.Strain(7301.6);
  EXPECT_EQ(0.003, helper::Round(value, 3));
  value = c_.Strain(9187.0);
  EXPECT_EQ(0.004, helper::Round(value, 3));
  value = c_.Strain(10645.4);
  EXPECT_EQ(0.005, helper::Round(value, 3));

  // extrapolated region
  value = c_.Strain(18756.6);
  EXPECT_EQ(0.010, helper::Round(value, 3));
}

TEST_F(CableComponentElongationModelTest, StrainThermal) {
  double* temperature = new double;
  double value = -999999;

  // at reference temperature
  value = c_.StrainThermal();
  EXPECT_EQ(0, helper::Round(value, 7));

  // above reference temperature
  *temperature = 212;
  c_.set_temperature(temperature);
  value = c_.StrainThermal();
  EXPECT_EQ(0.0018176, helper::Round(value, 7));

  // below reference temperature
  *temperature = 0;
  c_.set_temperature(temperature);
  value = c_.StrainThermal();
  EXPECT_EQ(-0.000896, helper::Round(value, 7));
}

TEST_F(CableComponentElongationModelTest, Validate) {
  // when checking for warnings, it does not pass the polynomial slope
  // validation
  EXPECT_TRUE(c_.Validate(false, nullptr));
}
