// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/sagtension/catenary_cable_component_tension_solver.h"

#include "gtest/gtest.h"

#include "factory.h"
#include "models/base/helper.h"

class CatenaryCableComponentTensionSolverTest : public ::testing::Test {
 protected:
  CatenaryCableComponentTensionSolverTest() {

    Cable* cable = factory::BuildCable();

    Vector3d spacing_endpoints(1200, 0, 0);

    CableState state;
    state.load_stretch = 0;
    state.temperature = 60;
    state.temperature_stretch = 0;

    CatenaryCable* catenary_cable = new CatenaryCable();
    catenary_cable->set_cable(cable);
    catenary_cable->set_spacing_endpoints(spacing_endpoints);
    catenary_cable->set_state(state);
    catenary_cable->set_tension_horizontal(6000);
    catenary_cable->set_weight_unit(Vector3d(0, 0, 1.094));

    // builds fixture object
    c_.set_catenary_cable(catenary_cable);
  }

  CatenaryCableComponentTensionSolver c_;
};

TEST_F(CatenaryCableComponentTensionSolverTest, TensionAverageComponent) {

  EXPECT_EQ(3174.0,
            helper::Round(c_.TensionAverageComponent(
                CableElongationModel::ComponentType::kCore), 1));

  EXPECT_EQ(2838.4,
            helper::Round(c_.TensionAverageComponent(
                CableElongationModel::ComponentType::kShell), 1));

  EXPECT_EQ(6012.3,
            helper::Round(c_.TensionAverageComponent(
                CableElongationModel::ComponentType::kCombined), 1));
}

TEST_F(CatenaryCableComponentTensionSolverTest, TensionHorizontalComponent) {

  EXPECT_EQ(3167.5,
            helper::Round(c_.TensionHorizontalComponent(
                CableElongationModel::ComponentType::kCore), 1));

  EXPECT_EQ(2832.6,
            helper::Round(c_.TensionHorizontalComponent(
                CableElongationModel::ComponentType::kShell), 1));

  EXPECT_EQ(6000.1,
            helper::Round(c_.TensionHorizontalComponent(
                CableElongationModel::ComponentType::kCombined), 1));
}

TEST_F(CatenaryCableComponentTensionSolverTest, Validate) {
  EXPECT_TRUE(c_.Validate(false, nullptr));
}
