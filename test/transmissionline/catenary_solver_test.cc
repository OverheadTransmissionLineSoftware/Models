// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/transmissionline/catenary_solver.h"

#include "gtest/gtest.h"

#include "models/base/helper.h"

class CatenarySolverTest : public ::testing::Test {
 protected:
  CatenarySolverTest() {
    // builds fixture object
    c_.set_position_target(0);
    c_.set_spacing_endpoints(Vector3d(1200, 0, 0));
    c_.set_type_target(CatenarySolver::TargetType::kConstant);
    c_.set_value_target(4000);
    c_.set_weight_unit(Vector3d(0, 0, 1));
  }

  ~CatenarySolverTest() {
  }

  // test object
  CatenarySolver c_;
};

TEST_F(CatenarySolverTest, Catenary) {
  // gets catenary and checks horizontal tension
  Catenary3d catenary = c_.Catenary();
  double value = catenary.tension_horizontal();
  EXPECT_EQ(4000.0, helper::Round(value, 1));
}

TEST_F(CatenarySolverTest, TensionHorizontal) {
  double value = -999999;

  // checks catenary constant type
  c_.set_position_target(-1);
  c_.set_type_target(CatenarySolver::TargetType::kConstant);
  c_.set_value_target(4000);
  value = c_.TensionHorizontal();
  EXPECT_EQ(4000.0, helper::Round(value, 1));

  // checks length type
  c_.set_position_target(-1);
  c_.set_type_target(CatenarySolver::TargetType::kLength);
  c_.set_value_target(1204.505065);
  value = c_.TensionHorizontal();
  EXPECT_EQ(4000.0, helper::Round(value, 1));

  // checks sag type
  c_.set_position_target(-1);
  c_.set_type_target(CatenarySolver::TargetType::kSag);
  c_.set_value_target(45.08438);
  value = c_.TensionHorizontal();
  EXPECT_EQ(4000.0, helper::Round(value, 1));

  // checks tension type
  c_.set_position_target(0);
  c_.set_type_target(CatenarySolver::TargetType::kTension);
  c_.set_value_target(4045.084438);
  value = c_.TensionHorizontal();
  EXPECT_EQ(4000.0, helper::Round(value, 1));
}

TEST_F(CatenarySolverTest, Validate) {
  EXPECT_TRUE(c_.Validate(true, nullptr));
}
