// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/sagtension/catenary_cable_unloader.h"

#include "gtest/gtest.h"

#include "models/base/helper.h"
#include "test/factory.h"

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
    state.temperature = 60;
    state.type_polynomial =
        SagTensionCableComponent::PolynomialType::kLoadStrain;

    CableStretchState state_stretch;
    state_stretch.load = 0;
    state_stretch.temperature = 0;
    state_stretch.type_polynomial =
        SagTensionCableComponent::PolynomialType::kLoadStrain;

    model_reference_ = factory::BuildCableElongationModel(cable_);
    model_reference_->set_state(state);
    model_reference_->set_state_stretch(state_stretch);

    // builds dependency object - unloaded cable model
    state = CableState();
    state.temperature = 32;
    state.type_polynomial =
        SagTensionCableComponent::PolynomialType::kLoadStrain;

    state_stretch = CableStretchState();
    state_stretch.load = 0;
    state_stretch.temperature = 0;
    state_stretch.type_polynomial =
        SagTensionCableComponent::PolynomialType::kLoadStrain;

    model_unloaded_ = factory::BuildCableElongationModel(cable_);
    model_unloaded_->set_state(state);
    model_unloaded_->set_state_stretch(state_stretch);

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

  CableStretchState state_stretch = model_reference_->state_stretch();

  // unstretched catenary cable state
  value = c_.LengthUnloaded();
  EXPECT_EQ(1200.82, helper::Round(value, 2));

  // stretched catenary cable state
  state_stretch.load = 12179.2;
  model_reference_->set_state_stretch(state_stretch);
  c_.set_model_reference(model_reference_);
  value = c_.LengthUnloaded();
  EXPECT_EQ(1200.36, helper::Round(value, 2));
}

TEST_F(CatenaryCableUnloaderTest, Validate) {
  EXPECT_TRUE(c_.Validate(false, nullptr));
}
