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
    cable_ = factory::BuildCable();
    cable_sagtension_ = new SagTensionCable();
    cable_sagtension_->set_cable_base(cable_);

    // builds dependency object - state
    state_ = new CableState();
    state_->load_stretch = 12000;
    state_->temperature = 70;
    state_->temperature_stretch = 0;
    state_->type_polynomial =
        SagTensionCableComponent::PolynomialType::kLoadStrain;

    // builds fixture object
    c_.set_cable(cable_sagtension_);
    c_.set_state(state_);
  }

  ~CableElongationModelTest() {
    factory::DestroySagTensionCable(cable_sagtension_);
    delete state_;
  }

  // allocated dependency objects
  Cable* cable_;
  SagTensionCable* cable_sagtension_;
  CableState* state_;

  // test object
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
  EXPECT_EQ(0.000026, helper::Round(value, 6));
  value = c_.Strain(CableElongationModel::ComponentType::kCombined, 9188.4);
  EXPECT_EQ(0.002, helper::Round(value, 3));

  // modifies core and shell compression modulus to zero
  // checks combined strain at zero load
  cable_->component_core.modulus_compression_elastic_area = 0;
  cable_->component_shell.modulus_compression_elastic_area = 0;
  c_.set_cable(cable_sagtension_);
  value = c_.Strain(CableElongationModel::ComponentType::kCombined, 0);
  EXPECT_EQ(-0.000022, helper::Round(value, 6));
}

TEST_F(CableElongationModelTest, Validate) {
  EXPECT_TRUE(c_.Validate(false, nullptr));
}
