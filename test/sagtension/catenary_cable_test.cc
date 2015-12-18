// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/sagtension/catenary_cable.h"

#include "gtest/gtest.h"

#include "factory.h"

class CatenaryCableTest : public ::testing::Test {
 protected:
  CatenaryCableTest() {
    cable_ = factory::BuildSagTensionCable();

    // builds dependency object - endpoint spacing
    Vector3d spacing_endpoints(1200, 0, 0);

    // builds dpendency object - state
    CableState state;
    state.load_stretch = 0;
    state.temperature = 0;
    state.temperature_stretch = 0;
    state.type_polynomial =
        SagTensionCableComponent::PolynomialType::kLoadStrain;

    // builds dependency object - unit weight
    Vector3d weight_unit(0, 0, 1.094);

    // builds fixture object
    c_.set_cable(cable_);
    c_.set_spacing_endpoints(spacing_endpoints);
    c_.set_state(state);
    c_.set_tension_horizontal(6000);
    c_.set_weight_unit(weight_unit);
  }

  ~CatenaryCableTest() {
    factory::DestroySagTensionCable(cable_);
  }

  // allocated dependency object
  SagTensionCable* cable_;

  // test object
  CatenaryCable c_;
};

TEST_F(CatenaryCableTest, Validate) {
  EXPECT_TRUE(c_.Validate(false, nullptr));
}
