// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/sagtension/cable_elongation_model.h"

#include "gtest/gtest.h"

#include "factory.h"
#include "models/base/helper.h"

class CableElongationModelTest : public ::testing::Test {
 protected:
  CableElongationModelTest() {
    // builds dependency object - cable
    SagTensionCable* cable = factory::BuildSagTensionCable();

    // builds dependency object - state
    CableState* state = new CableState();
    state->load_stretch = 12000;
    state->temperature = 70;
    state->temperature_stretch = 0;
    state->type_polynomial =
        SagTensionCableComponent::PolynomialType::kLoadStrain;

    // builds fixture object
    c_.set_cable(cable);
    c_.set_state(state);
  }

  CableElongationModel c_;
};

TEST_F(CableElongationModelTest, Load) {
  double value = -999999;

  // core
  value = c_.Load(CableElongationModel::ComponentType::kCore, 0.002);
  EXPECT_EQ(5433.5, helper::Round(value, 1));

  // shell
  value = c_.Load(CableElongationModel::ComponentType::kShell, 0.002);
  EXPECT_EQ(3754.9, helper::Round(value, 1));

  // combined
  value = c_.Load(CableElongationModel::ComponentType::kCombined, 0.002);
  EXPECT_EQ(9188.4, helper::Round(value, 1));
}

TEST_F(CableElongationModelTest, Slope) {
  double value = -999999;

  // core
  value = c_.Slope(CableElongationModel::ComponentType::kCore, 0.002);
  EXPECT_EQ(2687680, helper::Round(value, 0));

  // shell
  value = c_.Slope(CableElongationModel::ComponentType::kShell, 0.002);
  EXPECT_EQ(4648960, helper::Round(value, 0));

  // combined
  value = c_.Slope(CableElongationModel::ComponentType::kCombined, 0.002);
  EXPECT_EQ(7336640, helper::Round(value, 0));
}

TEST_F(CableElongationModelTest, Strain) {
  double value = -999999;

  // core
  value = c_.Strain(CableElongationModel::ComponentType::kCore, 5433.5);
  EXPECT_EQ(0.002, helper::Round(value, 3));

  // shell
  value = c_.Strain(CableElongationModel::ComponentType::kShell, 3754.9);
  EXPECT_EQ(0.002, helper::Round(value, 3));

  // combined
  value = c_.Strain(CableElongationModel::ComponentType::kCombined, 0);
  EXPECT_EQ(0.0001, helper::Round(value, 4));
  value = c_.Strain(CableElongationModel::ComponentType::kCombined, 9188.4);
  EXPECT_EQ(0.002, helper::Round(value, 3));
}

TEST_F(CableElongationModelTest, Validate) {
  EXPECT_TRUE(c_.Validate(false, nullptr));
}
