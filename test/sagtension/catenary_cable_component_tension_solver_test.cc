// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/sagtension/catenary_cable_component_tension_solver.h"

#include "gtest/gtest.h"

#include "factory.h"
#include "models/base/helper.h"

class CatenaryCableComponentTensionSolverTest : public ::testing::Test {
 protected:
  CatenaryCableComponentTensionSolverTest() {
    cable_ = factory::BuildSagTensionCable();

    Vector3d spacing_endpoints(1200, 0, 0);

    CableState state;
    state.load_stretch = 0;
    state.temperature = 60;
    state.temperature_stretch = 0;
    state.type_polynomial =
        SagTensionCableComponent::PolynomialType::kLoadStrain;

    catenarycable_ = new CatenaryCable();
    catenarycable_->set_cable(cable_);
    catenarycable_->set_spacing_endpoints(spacing_endpoints);
    catenarycable_->set_state(state);
    catenarycable_->set_tension_horizontal(6000);
    catenarycable_->set_weight_unit(Vector3d(0, 0, 1.094));

    // builds fixture object
    c_.set_catenary_cable(catenarycable_);
  }

  ~CatenaryCableComponentTensionSolverTest() {
    factory::DestroySagTensionCable(cable_);
    delete catenarycable_;
  }

  // allocated dependency objects
  SagTensionCable* cable_;
  CatenaryCable* catenarycable_;

  // test object
  CatenaryCableComponentTensionSolver c_;
};

TEST_F(CatenaryCableComponentTensionSolverTest, TensionAverageComponent) {
  double value = -999999;

  value = c_.TensionAverageComponent(
      CableElongationModel::ComponentType::kCore);
  EXPECT_EQ(3174.0, helper::Round(value, 1));

  value = c_.TensionAverageComponent(
      CableElongationModel::ComponentType::kShell);
  EXPECT_EQ(2838.4, helper::Round(value, 1));

  value = c_.TensionAverageComponent(
      CableElongationModel::ComponentType::kCombined);
  EXPECT_EQ(6012.3, helper::Round(value, 1));
}

TEST_F(CatenaryCableComponentTensionSolverTest, TensionHorizontalComponent) {
  double value = -999999;

  value = c_.TensionHorizontalComponent(
      CableElongationModel::ComponentType::kCore);
  EXPECT_EQ(3167.5, helper::Round(value, 1));

  value = c_.TensionHorizontalComponent(
      CableElongationModel::ComponentType::kShell);
  EXPECT_EQ(2832.6, helper::Round(value, 1));

  value = c_.TensionHorizontalComponent(
      CableElongationModel::ComponentType::kCombined);
  EXPECT_EQ(6000.1, helper::Round(value, 1));
}

TEST_F(CatenaryCableComponentTensionSolverTest, Validate) {
  EXPECT_TRUE(c_.Validate(false, nullptr));
}
