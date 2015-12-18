// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/sagtension/cable_strainer.h"

#include "gtest/gtest.h"

#include "factory.h"
#include "models/base/helper.h"

class CableStrainerTest : public ::testing::Test {
 protected:
  CableStrainerTest() {
    // builds dependency object - cable
    cable_ = factory::BuildSagTensionCable();

    // builds dependency object - start state
    state_start_ = new CableState();
    state_start_->load_stretch = 12000;
    state_start_->temperature = 0;
    state_start_->temperature_stretch = 0;
    state_start_->type_polynomial =
        SagTensionCableComponent::PolynomialType::kLoadStrain;

    // builds dependency object - finish state
    state_finish_ = new CableState();
    state_finish_->load_stretch = 12000;
    state_finish_->temperature = 212;
    state_finish_->temperature_stretch = 0;
    state_finish_->type_polynomial =
        SagTensionCableComponent::PolynomialType::kLoadStrain;

    // builds fixture object
    c_.set_cable(cable_);
    c_.set_length_start(1200);
    c_.set_load_finish(10000);
    c_.set_load_start(0);
    c_.set_state_finish(state_finish_);
    c_.set_state_start(state_start_);
  }

  ~CableStrainerTest() {
    factory::DestroySagTensionCable(cable_);
    delete state_start_;
    delete state_finish_;
  }

  // allocated dependency objects
  SagTensionCable* cable_;
  CableState* state_start_;
  CableState* state_finish_;

  // test object
  CableStrainer c_;
};

TEST_F(CableStrainerTest, LengthFinish) {
  // checks loaded length
  EXPECT_TRUE(c_.length_start() < c_.LengthFinish());

  // switches unloaded and loaded states
  const double length_start = c_.length_start();
  const double length_finish = c_.LengthFinish();
  const double load_start = c_.load_start();
  const double load_finish = c_.load_finish();

  c_.set_length_start(length_finish);
  c_.set_load_start(load_finish);
  c_.set_load_finish(load_start);
  c_.set_state_start(state_finish_);
  c_.set_state_finish(state_start_);

  // tests if original length is calculated
  EXPECT_EQ(length_start, helper::Round(c_.LengthFinish(), 3));
}

TEST_F(CableStrainerTest, Validate) {
  EXPECT_TRUE(c_.Validate(false, nullptr));
}
