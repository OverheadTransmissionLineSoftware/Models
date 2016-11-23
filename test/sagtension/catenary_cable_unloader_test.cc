// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/sagtension/catenary_cable_unloader.h"

#include "gtest/gtest.h"

#include "factory.h"
#include "models/base/helper.h"

class CatenaryCableUnloaderTest : public ::testing::Test {
 protected:
  CatenaryCableUnloaderTest() {
    // builds dependency object - cable
    cable_ = factory::BuildSagTensionCable();

    // builds dependency object - catenary
    Vector3d spacing_endpoints(1200, 0, 0);
    Vector3d weight_unit(0, 0, 1.094);

    catenary_.set_spacing_endpoints(spacing_endpoints);
    catenary_.set_tension_horizontal(6000);
    catenary_.set_weight_unit(weight_unit);

    // builds dependency object - reference cable model
    CableState state;
    state.load_stretch = 0;
    state.temperature = 60;
    state.temperature_stretch = 0;
    state.type_polynomial =
        SagTensionCableComponent::PolynomialType::kLoadStrain;

    model_reference_ = factory::BuildCableElongationModel(cable_);
    model_reference_->set_state(state);

    // builds dependency object - unloaded cable model
    CableState state_unloaded;
    state_unloaded.load_stretch = 0;
    state_unloaded.temperature = 32;
    state_unloaded.temperature_stretch = 0;
    state_unloaded.type_polynomial =
        SagTensionCableComponent::PolynomialType::kLoadStrain;

    model_unloaded_ = factory::BuildCableElongationModel(cable_);
    model_unloaded_->set_state(state_unloaded);

    // builds fixture object
    c_.set_catenary(&catenary_);
    c_.set_model_reference(model_reference_);
    c_.set_model_unloaded(model_unloaded_);
  }

  ~CatenaryCableUnloaderTest() {
    factory::DestroySagTensionCable(cable_);
    delete model_reference_;
    delete model_unloaded_;
  }

  // allocated dependency objects
  SagTensionCable* cable_;
  Catenary3d catenary_;
  CableElongationModel* model_reference_;
  CableElongationModel* model_unloaded_;

  // test object
  CatenaryCableUnloader c_;
};

TEST_F(CatenaryCableUnloaderTest, LengthUnloaded) {
  double value = -999999;

  CableState state = model_reference_->state();

  // unstretched catenary cable state
  value = c_.LengthUnloaded();
  EXPECT_EQ(1200.82, helper::Round(value, 2));

  // stretched catenary cable state
  state.load_stretch = 12179.2;
  model_reference_->set_state(state);
  c_.set_model_reference(model_reference_);
  value = c_.LengthUnloaded();
  EXPECT_EQ(1200.35, helper::Round(value, 2));
}

TEST_F(CatenaryCableUnloaderTest, Validate) {
  EXPECT_TRUE(c_.Validate(false, nullptr));
}
