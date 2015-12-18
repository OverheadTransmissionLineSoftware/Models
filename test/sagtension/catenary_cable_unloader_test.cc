// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/sagtension/catenary_cable_unloader.h"

#include "gtest/gtest.h"

#include "factory.h"
#include "models/base/helper.h"

class CatenaryCableUnloaderTest : public ::testing::Test {
 protected:
  CatenaryCableUnloaderTest() {
    cable_ = factory::BuildSagTensionCable();
    Vector3d spacing_endpoints(1200, 0, 0);

    CableState state;
    state.load_stretch = 0;
    state.temperature = 60;
    state.temperature_stretch = 0;
    state.type_polynomial =
        SagTensionCableComponent::PolynomialType::kLoadStrain;

    Vector3d weight_unit(0, 0, 1.094);

    catenarycable_ = new CatenaryCable();
    catenarycable_->set_cable(cable_);
    catenarycable_->set_spacing_endpoints(spacing_endpoints);
    catenarycable_->set_state(state);
    catenarycable_->set_tension_horizontal(6000);
    catenarycable_->set_weight_unit(weight_unit);

    // builds dependency object - unloaded state
    state_unloaded_ = new CableState();
    state_unloaded_->load_stretch = 0;
    state_unloaded_->temperature = 32;
    state_unloaded_->temperature_stretch = 0;
    state_unloaded_->type_polynomial =
        SagTensionCableComponent::PolynomialType::kLoadStrain;

    // builds fixture object
    c_.set_catenary_cable(catenarycable_);
    c_.set_state_unloaded(state_unloaded_);
  }

  ~CatenaryCableUnloaderTest() {
    factory::DestroySagTensionCable(cable_);
    delete catenarycable_;
    delete state_unloaded_;
  }

  // allocated dependency objects
  SagTensionCable* cable_;
  CatenaryCable* catenarycable_;
  CableState* state_unloaded_;

  // test object
  CatenaryCableUnloader c_;
};

TEST_F(CatenaryCableUnloaderTest, LengthUnloaded) {
  double value = -999999;

  CableState state = *catenarycable_->state();

  // unstretched catenary cable state
  value = c_.LengthUnloaded();
  EXPECT_EQ(1200.82, helper::Round(value, 2));

  // stretched catenary cable state
  state.load_stretch = 12179.2;
  catenarycable_->set_state(state);
  c_.set_catenary_cable(catenarycable_);
  value = c_.LengthUnloaded();
  EXPECT_EQ(1200.35, helper::Round(value, 2));
}

TEST_F(CatenaryCableUnloaderTest, Validate) {
  EXPECT_TRUE(c_.Validate(false, nullptr));
}
