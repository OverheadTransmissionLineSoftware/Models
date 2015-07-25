// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "sagtension/cable_component_elongation_model.h"

#include "gtest/gtest.h"

#include "base/helper.h"
#include "factory.h"

class CableComponentElongationModelTest : public ::testing::Test {
 protected:
  CableComponentElongationModelTest() {

    CableComponent component = factory::BuildCableComponent();

    // builds fixture object
    c_.set_component_cable(component);
    c_.set_load_stretch(5000);
    c_.set_temperature(70);
    c_.set_temperature_reference(70);
    c_.set_type_polynomial_active(CableComponent::PolynomialType::kLoadStrain);
  }

  CableComponentElongationModel c_;
};

TEST_F(CableComponentElongationModelTest, Load) {

  // compressed region
  EXPECT_EQ(-211.1, helper::Round(c_.Load(-0.001), 1));

  // stretched region
  EXPECT_EQ(289.3, helper::Round(c_.Load(0.001), 1));
  EXPECT_EQ(4938.2, helper::Round(c_.Load(0.002), 1));

  // polynomial region
  EXPECT_EQ(7301.6, helper::Round(c_.Load(0.003), 1));
  EXPECT_EQ(9187, helper::Round(c_.Load(0.004), 1));
  EXPECT_EQ(10645.4, helper::Round(c_.Load(0.005), 1));

  // extrapolated region
  EXPECT_EQ(18756.6, helper::Round(c_.Load(0.010), 1));
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

  // compressed region
  EXPECT_EQ(108960, helper::Round(c_.Slope(-0.001), 0));

  // stretched region
  EXPECT_EQ(4648960, helper::Round(c_.Slope(0.001), 0));
  EXPECT_EQ(4648960, helper::Round(c_.Slope(0.002), 0));

  // polynomial region
  EXPECT_EQ(2111035, helper::Round(c_.Slope(0.003), 0));
  EXPECT_EQ(1663528, helper::Round(c_.Slope(0.004), 0));
  EXPECT_EQ(1265984, helper::Round(c_.Slope(0.005), 0));

  // extrapolated region
  EXPECT_EQ(4648960, helper::Round(c_.Slope(0.010), 0));
}

TEST_F(CableComponentElongationModelTest, Strain) {

  // compressed region
  EXPECT_EQ(-0.001, helper::Round(c_.Strain(-211.1), 3));

  // stretched region
  EXPECT_EQ(0.001, helper::Round(c_.Strain(289.3), 3));
  EXPECT_EQ(0.002, helper::Round(c_.Strain(4938.2), 3));

  // polynomial region
  EXPECT_EQ(0.003, helper::Round(c_.Strain(7301.6), 3));
  EXPECT_EQ(0.004, helper::Round(c_.Strain(9187.0), 3));
  EXPECT_EQ(0.005, helper::Round(c_.Strain(10645.4), 3));

  // extrapolated region
  EXPECT_EQ(0.010, helper::Round(c_.Strain(18756.6), 3));
}

TEST_F(CableComponentElongationModelTest, StrainThermal) {

  // at reference temperature
  EXPECT_EQ(0, helper::Round(c_.StrainThermal(), 7));

  // above reference temperature
  c_.set_temperature(212);
  EXPECT_EQ(0.0018176, helper::Round(c_.StrainThermal(), 7));

  // below reference temperature
  c_.set_temperature(0);
  EXPECT_EQ(-0.000896, helper::Round(c_.StrainThermal(), 7));
}

TEST_F(CableComponentElongationModelTest, Validate) {

  // when checking for warnings, it does not pass the polynomial slope
  // validation
  EXPECT_TRUE(c_.Validate(false, nullptr));
}
