// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/sagtension/cable_component_elongation_model.h"

#include "gtest/gtest.h"

#include "models/base/helper.h"
#include "test/factory.h"

class CableComponentElongationModelTest : public ::testing::Test {
 protected:
  CableComponentElongationModelTest() {
    // builds dependency object - component
    cable_ = factory::BuildSagTensionCable();
    const SagTensionCableComponent* component = cable_->component_shell();

    // builds dependency object - state
    state_.temperature = 70;
    state_.type_polynomial =
        SagTensionCableComponent::PolynomialType::kLoadStrain;

    // builds dependency object - stretch state
    state_stretch_.load = 5000;
    state_stretch_.temperature = 70;
    state_stretch_.type_polynomial =
        SagTensionCableComponent::PolynomialType::kLoadStrain;

    // builds fixture object
    c_.set_component_cable(component);
    c_.set_state(&state_);
    c_.set_state_stretch(&state_stretch_);
    c_.set_temperature_reference(cable_->temperature_properties_components());
  }

  ~CableComponentElongationModelTest() {
    factory::DestroySagTensionCable(cable_);
  }

  // allocated dependency objects
  SagTensionCable* cable_;
  CableState state_;
  CableStretchState state_stretch_;

  // test object
  CableComponentElongationModel c_;
};

TEST_F(CableComponentElongationModelTest, Load) {
  double value = -999999;

  // compressed region
  value = c_.Load(-0.0010);
  EXPECT_EQ(-211.1, helper::Round(value, 1));

  // stretched region
  value = c_.Load(0.0010);
  EXPECT_EQ(289.3, helper::Round(value, 1));
  value = c_.Load(0.0020);
  EXPECT_EQ(4938.2, helper::Round(value, 1));

  // polynomial region
  value = c_.Load(0.0030);
  EXPECT_EQ(7301.6, helper::Round(value, 1));
  value = c_.Load(0.0040);
  EXPECT_EQ(9187, helper::Round(value, 1));
  value = c_.Load(0.0050);
  EXPECT_EQ(10645.4, helper::Round(value, 1));

  // extrapolated region
  value = c_.Load(0.010);
  EXPECT_EQ(15851.8, helper::Round(value, 1));

  // adjusts to creep stretch state and repeats tests
  // this should stretch the component more than before
  state_stretch_.load = 1500;
  state_stretch_.type_polynomial =
      SagTensionCableComponent::PolynomialType::kCreep;
  c_.set_state_stretch(&state_stretch_);

  // compressed region
  value = c_.Load(-0.0010);
  EXPECT_EQ(-224.0, helper::Round(value, 1));

  // stretched region
  value = c_.Load(0.0010);
  EXPECT_EQ(-6.1, helper::Round(value, 1));
  value = c_.Load(0.0020);
  EXPECT_EQ(4390.0, helper::Round(value, 1));

  // polynomial region
  value = c_.Load(0.0030);
  EXPECT_EQ(7301.6, helper::Round(value, 1));
  value = c_.Load(0.0040);
  EXPECT_EQ(9187, helper::Round(value, 1));
  value = c_.Load(0.0050);
  EXPECT_EQ(10645.4, helper::Round(value, 1));

  // extrapolated region
  value = c_.Load(0.0100);
  EXPECT_EQ(15851.8, helper::Round(value, 1));
}

TEST_F(CableComponentElongationModelTest, PointsRegions) {
  std::vector<Point2d<double>> points = c_.PointsRegions();
  Point2d<double> p;

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
  EXPECT_EQ(1310911, helper::Round(value, 0));
}

TEST_F(CableComponentElongationModelTest, Strain) {
  double value = -999999;

  // compressed region
  value = c_.Strain(-211.1);
  EXPECT_EQ(-0.0010, helper::Round(value, 4));

  // stretched region
  value = c_.Strain(289.3);
  EXPECT_EQ(0.0010, helper::Round(value, 4));
  value = c_.Strain(4938.2);
  EXPECT_EQ(0.0020, helper::Round(value, 4));

  // polynomial region
  value = c_.Strain(7301.6);
  EXPECT_EQ(0.0030, helper::Round(value, 4));
  value = c_.Strain(9187.0);
  EXPECT_EQ(0.0040, helper::Round(value, 4));
  value = c_.Strain(10645.4);
  EXPECT_EQ(0.0050, helper::Round(value, 4));

  // extrapolated region
  value = c_.Strain(15851.8);
  EXPECT_EQ(0.0100, helper::Round(value, 4));

  // adjusts to creep stretch state and repeats tests
  // this should stretch the component more than before
  state_stretch_.load = 1500;
  state_stretch_.type_polynomial =
      SagTensionCableComponent::PolynomialType::kCreep;
  c_.set_state_stretch(&state_stretch_);

  // compressed region
  value = c_.Strain(-211.1);
  EXPECT_EQ(-0.0009, helper::Round(value, 4));

  // stretched region
  value = c_.Strain(289.3);
  EXPECT_EQ(0.0011, helper::Round(value, 4));
  value = c_.Strain(4938.2);
  EXPECT_EQ(0.0021, helper::Round(value, 4));

  // polynomial region
  value = c_.Strain(7301.6);
  EXPECT_EQ(0.0030, helper::Round(value, 4));
  value = c_.Strain(9187.0);
  EXPECT_EQ(0.0040, helper::Round(value, 4));
  value = c_.Strain(10645.4);
  EXPECT_EQ(0.0050, helper::Round(value, 4));

  // extrapolated region
  value = c_.Strain(15851.8);
  EXPECT_EQ(0.0100, helper::Round(value, 4));
}

TEST_F(CableComponentElongationModelTest, StrainThermal) {
  double value = -999999;

  // at reference temperature
  value = c_.StrainThermal();
  EXPECT_EQ(0, helper::Round(value, 7));

  // above reference temperature
  state_.temperature = 212;
  c_.set_state(&state_);
  value = c_.StrainThermal();
  EXPECT_EQ(0.0018176, helper::Round(value, 7));

  // below reference temperature
  state_.temperature = 0;
  c_.set_state(&state_);
  value = c_.StrainThermal();
  EXPECT_EQ(-0.000896, helper::Round(value, 7));
}

TEST_F(CableComponentElongationModelTest, Validate) {
  // when checking for warnings, it does not pass the polynomial slope
  // validation
  EXPECT_TRUE(c_.Validate(false, nullptr));
}
