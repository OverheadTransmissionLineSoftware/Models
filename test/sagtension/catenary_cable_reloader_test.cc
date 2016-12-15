// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/sagtension/catenary_cable_reloader.h"

#include "gtest/gtest.h"

#include "factory.h"
#include "models/base/helper.h"

class CatenaryCableReloaderTest : public ::testing::Test {
 protected:
  CatenaryCableReloaderTest() {
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

    // builds dependency object - reloaded cable model
    state = CableState();
    state.temperature = 60;
    state.type_polynomial =
        SagTensionCableComponent::PolynomialType::kLoadStrain;

    state_stretch = CableStretchState();
    state_stretch.load = 0;
    state_stretch.temperature = 0;
    state_stretch.type_polynomial =
        SagTensionCableComponent::PolynomialType::kLoadStrain;

    model_reloaded_ = factory::BuildCableElongationModel(cable_);
    model_reloaded_->set_state(state);
    model_reloaded_->set_state_stretch(state_stretch);

    // builds dependency object - reloaded unit weight
    weight_unit_reloaded_ = Vector3d(0, 0, 1.094);

    // builds fixture object
    c_.set_catenary(&catenary_);
    c_.set_model_reference(model_reference_);
    c_.set_model_reloaded(model_reloaded_);
    c_.set_weight_unit_reloaded(&weight_unit_reloaded_);
  }

  ~CatenaryCableReloaderTest() {
    factory::DestroySagTensionCable(cable_);
    delete model_reference_;
    delete model_reloaded_;
  }

  // allocated dependency objects
  SagTensionCable* cable_;
  Catenary3d catenary_;
  CableElongationModel* model_reference_;
  CableElongationModel* model_reloaded_;
  Vector3d weight_unit_reloaded_;

  // test object
  CatenaryCableReloader c_;
};

TEST_F(CatenaryCableReloaderTest, LengthUnloaded) {
  double value = -999999;

  // unstretched original catenary cable
  value = c_.LengthUnloaded();
  EXPECT_EQ(1201.04, helper::Round(value, 2));
}

TEST_F(CatenaryCableReloaderTest, CatenaryReloaded) {
  double value = -999999;

  Catenary3d catenary;
  CableState state = c_.model_reloaded()->state();
  CableStretchState state_stretch = c_.model_reloaded()->state_stretch();

  // nothing is modified - original and reloaded parameters are equal
  catenary = c_.CatenaryReloaded();
  value = catenary.tension_horizontal();
  EXPECT_EQ(6000, helper::Round(value, 0));

  // tests temperature changes
  state.temperature = 0;
  model_reloaded_->set_state(state);
  c_.set_model_reloaded(model_reloaded_);
  catenary = c_.CatenaryReloaded();
  value = catenary.tension_horizontal();
  EXPECT_EQ(6788, helper::Round(value, 0));

  state.temperature = 212;
  model_reloaded_->set_state(state);
  c_.set_model_reloaded(model_reloaded_);
  catenary = c_.CatenaryReloaded();
  value = catenary.tension_horizontal();
  EXPECT_EQ(4701, helper::Round(value, 0));

  // changes temperature to zero and tests unit weight changes
  state.temperature = 0;
  model_reloaded_->set_state(state);
  c_.set_model_reloaded(model_reloaded_);

  weight_unit_reloaded_.set_y(2.072);
  weight_unit_reloaded_.set_z(3.729);
  c_.set_weight_unit_reloaded(&weight_unit_reloaded_);
  catenary = c_.CatenaryReloaded();
  value = catenary.tension_horizontal();
  EXPECT_EQ(17123, helper::Round(value, 0));

  weight_unit_reloaded_.set_y(1.405);
  weight_unit_reloaded_.set_z(2.099);
  c_.set_weight_unit_reloaded(&weight_unit_reloaded_);
  catenary = c_.CatenaryReloaded();
  value = catenary.tension_horizontal();
  EXPECT_EQ(12146, helper::Round(value, 0));

  // sets stretch load and tests all above cases again
  const double kLoadStretch = catenary.TensionAverage();
  EXPECT_EQ(12178, helper::Round(kLoadStretch, 0));
  state_stretch.load = kLoadStretch;
  model_reloaded_->set_state_stretch(state_stretch);
  c_.set_model_reloaded(model_reloaded_);

  state.temperature = 60;
  model_reloaded_->set_state(state);
  c_.set_model_reloaded(model_reloaded_);
  weight_unit_reloaded_.set_y(0);
  weight_unit_reloaded_.set_z(1.094);
  c_.set_weight_unit_reloaded(&weight_unit_reloaded_);
  catenary = c_.CatenaryReloaded();
  value = catenary.tension_horizontal();
  EXPECT_EQ(5561, helper::Round(value, 0));

  state.temperature = 0;
  model_reloaded_->set_state(state);
  c_.set_model_reloaded(model_reloaded_);
  catenary = c_.CatenaryReloaded();
  value = catenary.tension_horizontal();
  EXPECT_EQ(6321, helper::Round(value, 0));

  state.temperature = 212;
  model_reloaded_->set_state(state);
  c_.set_model_reloaded(model_reloaded_);
  catenary = c_.CatenaryReloaded();
  value = catenary.tension_horizontal();
  EXPECT_EQ(4527, helper::Round(value, 0));

  state.temperature = 0;
  model_reloaded_->set_state(state);
  c_.set_model_reloaded(model_reloaded_);
  weight_unit_reloaded_.set_y(2.072);
  weight_unit_reloaded_.set_z(3.729);
  c_.set_weight_unit_reloaded(&weight_unit_reloaded_);
  catenary = c_.CatenaryReloaded();
  value = catenary.tension_horizontal();
  EXPECT_EQ(17123, helper::Round(value, 0));

  weight_unit_reloaded_.set_y(1.405);
  weight_unit_reloaded_.set_z(2.099);
  c_.set_weight_unit_reloaded(&weight_unit_reloaded_);
  catenary = c_.CatenaryReloaded();
  value = catenary.tension_horizontal();
  EXPECT_EQ(12146, helper::Round(value, 0));
}

TEST_F(CatenaryCableReloaderTest, Validate) {
  EXPECT_TRUE(c_.Validate(false, nullptr));
}
