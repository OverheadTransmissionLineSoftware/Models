// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/sagtension/sag_tension_cable.h"

#include "gtest/gtest.h"

#include "test/factory.h"

class SagTensionCableComponentTest : public ::testing::Test {
 protected:
  SagTensionCableComponentTest() {
    cable_ = factory::BuildCable();
    s_.set_component_base(&cable_->component_shell);
  }

  ~SagTensionCableComponentTest() {
    delete cable_;
  }

  // dependency object
  Cable* cable_;

  // fixture object
  SagTensionCableComponent s_;
};

TEST_F(SagTensionCableComponentTest, IsEnabled) {
  EXPECT_TRUE(s_.IsEnabled());

  // deletes creep polynomial coefficients
  cable_->component_shell.coefficients_polynomial_creep.clear();
  s_.set_component_base(&cable_->component_shell);

  EXPECT_FALSE(s_.IsEnabled());
}

TEST_F(SagTensionCableComponentTest, Validate) {
  EXPECT_TRUE(s_.Validate(true, nullptr));
}

class SagTensionCableTest : public ::testing::Test {
 protected:
  SagTensionCableTest() {
    cable_ = factory::BuildCable();
    s_.set_cable_base(cable_);
  }

  ~SagTensionCableTest() {
    delete cable_;
  }

  // dependency object
  Cable* cable_;

  // test object
  SagTensionCable s_;
};

TEST_F(SagTensionCableTest, IsEnabled) {
  // deletes core polynomial coefficients
  cable_->component_core.coefficients_polynomial_creep.clear();
  cable_->component_core.coefficients_polynomial_loadstrain.clear();
  s_.set_cable_base(cable_);

  // core
  EXPECT_FALSE(s_.IsEnabled(SagTensionCable::ComponentType::kCore));

  // shell
  EXPECT_TRUE(s_.IsEnabled(SagTensionCable::ComponentType::kShell));

  // combined
  EXPECT_TRUE(s_.IsEnabled(SagTensionCable::ComponentType::kCombined));
}

TEST_F(SagTensionCableTest, Validate) {
  EXPECT_TRUE(s_.Validate(true, nullptr));

  // deletes core polynomial coefficients
  cable_->component_core.coefficients_polynomial_creep.clear();
  cable_->component_core.coefficients_polynomial_loadstrain.clear();
  s_.set_cable_base(cable_);

  EXPECT_TRUE(s_.Validate(true, nullptr));
}
