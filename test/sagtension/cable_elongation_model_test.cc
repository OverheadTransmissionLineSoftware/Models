// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "sagtension/cable_elongation_model.h"

#include "gtest/gtest.h"

#include "base/helper.h"
#include "factory.h"

class CableElongationModelTest : public ::testing::Test {
 protected:
  CableElongationModelTest() {

    Cable cable = factory::BuildCable();

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
