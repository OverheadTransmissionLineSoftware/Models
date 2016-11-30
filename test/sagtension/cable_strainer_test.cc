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

    // builds dependency object - start cable model
    model_start_ = factory::BuildCableElongationModel(cable_);

    // builds dependency object - finish cable model
    CableState state;
    state.temperature = 212;
    state.type_polynomial =
        SagTensionCableComponent::PolynomialType::kLoadStrain;

    CableStretchState state_stretch;
    state_stretch.load = 12000;
    state_stretch.temperature = 0;
    state_stretch.type_polynomial =
        SagTensionCableComponent::PolynomialType::kLoadStrain;

    model_finish_ = factory::BuildCableElongationModel(cable_);
    model_finish_->set_state(state);
    model_finish_->set_state_stretch(state_stretch);

    // builds fixture object
    c_.set_length_start(1200);
    c_.set_load_finish(10000);
    c_.set_load_start(0);
    c_.set_model_finish(model_finish_);
    c_.set_model_start(model_start_);
  }

  ~CableStrainerTest() {
    factory::DestroySagTensionCable(cable_);
    delete model_finish_;
    delete model_start_;
  }

  // allocated dependency objects
  SagTensionCable* cable_;
  CableElongationModel* model_finish_;
  CableElongationModel* model_start_;

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
  c_.set_model_start(model_finish_);
  c_.set_model_finish(model_start_);

  // tests if original length is calculated
  EXPECT_EQ(length_start, helper::Round(c_.LengthFinish(), 3));
}

TEST_F(CableStrainerTest, Validate) {
  EXPECT_TRUE(c_.Validate(false, nullptr));
}
