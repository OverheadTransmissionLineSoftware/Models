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
    SagTensionCable* cable = factory::BuildSagTensionCable();

    // builds dependency object - start state
    CableState* state_start = new CableState();
    state_start->load_stretch = 12000;
    state_start->temperature = 0;
    state_start->temperature_stretch = 0;
    state_start->type_polynomial =
        SagTensionCableComponent::PolynomialType::kLoadStrain;

    // builds dependency object - finish state
    CableState* state_finish = new CableState();
    state_finish->load_stretch = 12000;
    state_finish->temperature = 212;
    state_finish->temperature_stretch = 0;
    state_finish->type_polynomial =
        SagTensionCableComponent::PolynomialType::kLoadStrain;

    // builds fixture object
    c_.set_cable(cable);
    c_.set_length_start(1200);
    c_.set_load_finish(10000);
    c_.set_load_start(0);
    c_.set_state_finish(state_finish);
    c_.set_state_start(state_start);
  }
  CableStrainer c_;
};

TEST_F(CableStrainerTest, LengthFinish) {
  // checks loaded length
  EXPECT_TRUE(c_.length_start() < c_.LengthFinish());

  // switches unloaded and loaded states
  const double length1 = c_.length_start();
  const double length2 = c_.LengthFinish();
  const double load1 = c_.load_start();
  const double load2 = c_.load_finish();
  const CableState* state1 = c_.state_start();
  const CableState* state2 = c_.state_finish();

  c_.set_length_start(length2);
  c_.set_load_finish(load1);
  c_.set_load_start(load2);
  c_.set_state_start(state2);
  c_.set_state_finish(state1);

  // tests if original length is calculated
  EXPECT_EQ(length1, helper::Round(c_.LengthFinish(), 3));
}

TEST_F(CableStrainerTest, Validate) {
  EXPECT_TRUE(c_.Validate(false, nullptr));
}
